// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExecWrap.cpp
// Author:    matt Matthew Grosso
// Created:   Fri Nov 26 22:33:33 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ExecWrap.cpp,v 1.9 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BaseException.h"
#include "ptr.h"
#include "ptr_strdup.h"
#include "fwrap.h"
#include "FileByteSource.h"
#include "Slurp.h"
#include "Parse.h"
#include "ExecWrap.h"
#include "Exclusion.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <assert.h>

#include <vector>
#include <string.h>

class ExecWrapPriv 
{
    private:
    friend class ExecWrap;

    explicit ExecWrapPriv();

    //helpers
    bool  watch_for( pid_t p, ewproc_t *e);
    void  done_watching( pid_t p );
    void cleanup_pid( pid_t p, int result );
    static void sigchild_handler( int sig, siginfo_t *si, void *v );

    //data
    Mutex       m_;
    int         watched_count_;
    struct sigaction oldsiga ;
    ewproc_t    *wpids[ SHRT_MAX ];//32kx4=128k 

};

ExecWrapPriv *ExecWrap::p = new ExecWrapPriv();

ExecWrapPriv::ExecWrapPriv()
:watched_count_(0)
{
    memset(wpids, 0, sizeof( ewproc_t *)*SHRT_MAX);
}

bool  
ExecWrapPriv::watch_for( pid_t p, ewproc_t *e)
{
    wpids[p]=e;
    //ensure that signal handler is installed.
    MutexGuard m( m_);
    if( ++watched_count_ <= 1 )
    {
        struct sigaction siga ;
        memset( &siga, '\0', sizeof( struct sigaction ));
        memset( &oldsiga, '\0', sizeof( struct sigaction ));
        siga.sa_sigaction=ExecWrapPriv::sigchild_handler ;
        siga.sa_flags = SA_NOCLDSTOP | SA_SIGINFO ; 
        if( -1== sigaction( SIGCHLD, &siga, NULL ))
        {
            PANIC( "cant set sigaction for SIGCHLD" );
        }
    }
    //avoid race condition by verifying that child hasnt died allready
    int status;
    pid_t pr = waitpid( p, &status, WNOHANG );
    if( pr==p )
    {
        //died allready, possibly before signal handler was 
        //installed. if that were true then read could hang, which
        //is why we check for this.
        cleanup_pid( pr, status );
        return false;
    }
    if( -1 == pr )
    {
        PANICV( "ExecWrapPriv::watch_for(), waitpid(%i,,) failed", p );
    }
    return true;
}

void  
ExecWrapPriv::done_watching( pid_t pid )
{
    ewproc_t *e= wpids[ pid ] ;
    wpids[ pid ] = NULL;
    if(e)
    {
        if( --watched_count_ == 0 )
        {
            //TODO: restore signal handler.
        } 
        if( e->deleteme )
        {
            delete e;
        }
    }
};

void
ExecWrapPriv::cleanup_pid( pid_t p, int result )
{
    ewproc_t *e = wpids[ p ];
    if( ! e )
    {
        return ;
    }
    if( -1 != e->in )
    {
        shutdown( e->in, SHUT_RDWR );
    }
    if( -1 != e->out )
    {
        shutdown( e->out, SHUT_RDWR );
    }
    if( -1 != e->err )
    {
        shutdown( e->err, SHUT_RDWR );
    }
    e->result=result ;
}

void 
ExecWrapPriv::sigchild_handler( int sig, siginfo_t *si, void *v )
{
    //frankly unnecessary paranoid check, with honestly no good action
    //to take in case the os is really that hosed...
    if( ! si && si->si_signo==SIGCHLD )
    {
        assert(0);
        return ;
    }
    if( si->si_code != CLD_EXITED &&
        si->si_code != CLD_DUMPED &&
        si->si_code != CLD_KILLED
    ){
        return;
    }
    ExecWrap::p->cleanup_pid( si->si_pid, si->si_status );
}


pid_t
ExecWrap::fork_exec( 
    const char *execstr, 
    int *fdin, int *fdout, int *fderr,
    const char *input, const char *output, const char *error,
    char ** env_keys, char **env_vals, unsigned num_env_pair,
    const char *dir
)
{
    //set up exec filename and args
    if( !execstr )
    {
        PANIC( "ExecWrap::fork_exec(): null arg!" );
    }
    char_ptr execstr_buf = ptr_strdup( execstr );
    vector<char*> v_execstr=Parse::parse(execstr_buf.get()," "); 
    size_t vlen = v_execstr.size();
    if( vlen==0 )
    {
        PANIC( "ExecWrap::ExecWrap(): no args" );
    }

    int *childs_input;
    int *childs_output;
    int *childs_err;
    if( fdin )
    {
        childs_input=new int[2];
        if( -1 == pipe( childs_input ))
        {
            PANIC_S( "cant make input pipe for new process", execstr );
        }
        *fdin=childs_input[1];
    }
    if( fdout )
    {
        childs_output=new int[2];
        if( -1 == pipe( childs_output ))
        {
            PANIC_S( "cant make output pipe for new process", execstr );
        }
        *fdout=childs_output[0];
    }
    if( fderr )
    {
        childs_err=new int[2];
        if( -1 == pipe( childs_err ))
        {
            PANIC_S( "cant make output pipe for new process", execstr );
        }
        *fderr=childs_err[0];
    }

    pid_t pid=fork();
    if( -1==pid )
    {
        PANIC_S( "ExecWrap::exec() cant fork", execstr );
    }
    else if( pid==0)
    {
        //we are child process.
        //
        //dont let exceptions propogate. this allows parent process exception
        //handlers to assume that they are still in parent.

        try{

            //set up input and output.
            if( fdin && ( -1==dup2( childs_input[0], 0 )))
            {
                PANIC("cant dup2 input pipe");
            } 
            else if( input && *input )
            {
                fwrap::open_read_dup2( input, 0 );
            }

            if( fdout && ( -1==dup2( childs_output[1], 1 )))
            {
                PANIC("cant dup2 output");
            }
            else if( output && *output )
            {
                fwrap::open_write_dup2( output, 1 );
            }

            if( fderr && ( -1==dup2( childs_err[1], 2 )))
            {
                PANIC("cant dup2 error");
            }
            else if( error && *error )
            {
                fwrap::open_write_dup2( error, 2 );
            }
            fcntl(0,F_SETFD, 0);
            fcntl(1,F_SETFD, 0);
            fcntl(2,F_SETFD, 0);

            if(dir && 0 != chdir( dir ))
            {
                PANICV("fork_exec_watch(): couldnt chdir(%s)", dir );
            }
            if( dir )
            {
                //this really needs to be part of the interface rather than tied
                //to the dir option.
                if( 0!= setpgid(0,0))
                {
                    PANICV("fork_exec_watch: unable to setpgid(0,0)");
                }
            }

            //its ok to leak exec_argv, seeing as how our stack/heap are being 
            //replaced momentarily with call to execvp
            char **exec_argv= new char * [vlen+1] ;
            exec_argv[vlen]=NULL;
            for( size_t i=0; i< vlen ; ++i )
            {
                exec_argv[i]=strdup_wnew(v_execstr[i] );
            }

            //temporary hack, something is setting this, and causing cplusql jobexec
            //to pass it on to rqsub.pl then to run_remote_command.sh which causes 
            // that script to try and stage_files back its error to a directory that
            // cant be reached from bidw.
            //
            // this can go away once the bi/bin/runsw/ code has been replaced in batch
            // with cplusql scripts.
            //
            if( NULL == getenv( "DONT_TRASH_ERROR_FILE_ENV" ))
            {
				// This is the same as unsetenv()
                (void) putenv( "ERROR_FILE" );
            }

            for( unsigned i=0; i < num_env_pair; ++i )
            {
				size_t bufSz = strlen(env_keys[i]) + strlen(env_vals[i]) + 2;
				char buf[bufSz];

				snprintf(buf, bufSz, "%s=%s", env_keys[i], env_vals[i]);

				char *bufCopy = strdup(buf);

                if(putenv(bufCopy) != 0)
                {
                    PANICV("fork_exec(): not enough env space for putenv()" );
                }
            }
            execvp( exec_argv[0], exec_argv );
            PANICV("fork_exec(): execvp failed." );
        }
        catch( exception &e )
        {
            fprintf(stderr, 
                "[%s] exception caught in child process pid=%i exception=%s\n",
                __FILE__, getpid(), e.what());
        }
        catch( ... )
        {
            fprintf(stderr, 
                "[%s] unknown exception caught in child process pid=%i\n",
                __FILE__, getpid());
        }
        exit(1);
    }
    //parent process
    return pid;
}

pid_t
ExecWrap::fork_exec_watch( 
    const char *execstr, 
    int *fdin, int *fdout, int *fderr,
    const char *input, const char *output, const char *error,
    char ** env_keys, char **env_vals, unsigned num_env_pair,
    const char *dir
)
{
    pid_t pid=fork_exec( 
            execstr, fdin, fdout, fderr, input, output, error,
            env_keys, env_vals, num_env_pair, dir 
            );
    ewproc_t    *e=new ewproc_t();
    if(fdin)
    {
        e->in=*fdin;
    }
    if( fdout )
    {
        e->out=*fdout;
    }
    if( fderr )
    {
        e->err=*fderr;
    }
    (void)p->watch_for( pid, e );
    return pid;
}

int 
ExecWrap::fork_exec_wait( const char *execstr,
    const char *input, const char *output, const char *error,
    char ** env_keys, char **env_vals, unsigned num_env_pair,
    const char *dir
)
{
    pid_t pid=fork_exec_watch( 
            execstr, NULL, NULL, NULL, input, output, error,
            env_keys, env_vals, num_env_pair, dir
            );
    int status = wait_for_exit_status( pid );
    p->done_watching(pid);
    return status;
}

void 
ExecWrap::fork_exec_wait_or_panic( const char *execstr,
    const char *input, const char *output, const char *error,  
    char ** env_keys, char **env_vals, unsigned num_env_pair,
    const char *dir
)
{
    int gb=ExecWrap::fork_exec_wait( 
            execstr, input, output, error,
            env_keys, env_vals, num_env_pair, dir
            );
    if( gb )
    {
        PANIC_I( execstr, gb );
    }
}

void 
ExecWrap::watch_pid( pid_t pid, ewproc_t *e )
{
    if(!e)
    {
        e=new ewproc_t();
        e->deleteme=true;
    }
    if( ! p->watch_for( pid, e ))
    {
        PANIC( "process died even before signal handler could be installed");
    }
}

int 
ExecWrap::check_for_exit_status( pid_t pid )
{
    return get_exit_status( pid, WNOHANG );
}

int 
ExecWrap::wait_for_exit_status( pid_t pid )
{
    int result = get_exit_status( pid, 0 );
    if( result == -1 )
    {
        PANIC_I("problem getting exit status of pid.", pid );
    }
    return result;
}

void 
ExecWrap::done_watching( pid_t pid )
{
    p->done_watching( pid );
}

char_ptr 
ExecWrap::eval( const char *execstr, bool panic_on_failure )
{
    static const char *outtempl="eval-out-XXXXXX";
    static const char *errtempl="eval-err-XXXXXX";
    char_ptr errfilename ;
    char_ptr outfilename ;

    close( fwrap::mkstemp( outtempl, outfilename ));
    close( fwrap::mkstemp( errtempl, errfilename ));

    int status = ExecWrap::fork_exec_wait( 
        execstr, NULL, outfilename.get(), errfilename.get() 
    ); 
    int execerrno=errno;

    char_ptr giveback( Slurp::slurpFile( outfilename.get() ));
    if( status && panic_on_failure )
    {
        char_ptr error( Slurp::slurpFile( errfilename.get() ));
        unlink( errfilename.get() );
        errno = execerrno ;
        PANICV( "ExecWrap::exec() status[%i] cmd[%s] error[%s] stdout[%s]", 
            status, execstr, error.get(), giveback.get());
    }
    unlink( errfilename.get());
    unlink( outfilename.get() );
    return giveback;
}

int 
ExecWrap::get_exit_status( pid_t pid, int flags )
{
    int status;
    int eintr_count;
    int ret;
    for(  
        eintr_count=10 ;
        eintr_count && 
            ( -1==( ret=waitpid( pid, &status, flags ))) && 
            errno == EINTR ;
        -- eintr_count
    )
    {
        ;//noop
    }
    if(!eintr_count ) // trouble w/wait_pid.
    {
        return -1;
    }
    if(ret == 0 ) //not dead yet.
    {
        return -1 ;
    }
    if(ret == -1 )//probably pid died allready, and handler caught it.
    {
        ewproc_t *e = p->wpids[ pid ];
        if( e )
        {
            //-1 if handler did not catch. real result otherwise.
            return e->result ;
        }
        return -1;
    }
    //waitpid suceeded 
    fprintf( stderr, "status %x, WIFEXITED %i, WEXITSTATUS %i\n", status, WIFEXITED(status), WEXITSTATUS(status));
    return status ;
}

int 
ExecWrap::status_uncaught_signal( int s )
{
    int ret= ( s & 0x00ff ) ;
    return ret;
}

int 
ExecWrap::status_exit( int s )
{
    int ret= ( s & 0xff00 ) >> 8 ;
    return ret;
}

//notes:
#if 0
//TODO: get test for BROKEN_WMACROS into a configure script as it wasnt working
//for my lintel redhat 8.0 box, but my alternative hack may have portability 
//problems.
//      
//#ifndef BROKEN_WMACROS
//    int ret=WIFEXITED(status);
//    if( ret )
//    {
//        return 0;
//    }
//    return WEXITSTATUS( status );
//#else
    int ret= ( s & 0xff00 ) >> 8 ;
    //fprintf(stderr, "run: %u %s, %x %x\n", getpid(), execstr, status, ret );
    return ret;
//#endif
#endif
