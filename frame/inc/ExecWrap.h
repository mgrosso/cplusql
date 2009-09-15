// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExecWrap.h
// Author:    mgrosso 
// Created:   Wed May  7 02:10:13 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ExecWrap.h,v 1.2 2004/02/25 00:46:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EXECWRAP_H
#define EXECWRAP_H 1

#include <sys/types.h>
#include "ptr_strdup.h"

typedef struct ewproc {
    int in;
    int out;
    int err;
    int result;
    bool deleteme;
    ewproc()
    {
        in=-1;
        out=-1;
        err=-1;
        result=-1;
        deleteme=false;
    };
} ewproc_t ;

class ExecWrapPriv ;
class ExecWrap
{
    public:

        // fork_exec()
        //
        // does a fork() and child does exec() of execstr().  fdin,
        // fdout, and fderr will be hooked up (pipe(), then dup2()) to
        // stdout, stdin, and stderr of child if they are non-null.
        //
        // if any of inpyut, output, or error are non-null, files at
        // those names are read for input, or written for output
        // respectively.  if you pass an int * and a const char * for
        // one of the three standard streams, the int * will be used
        // and the const char * will be ignored.
        //
        // does not block caller indefinitely.
        //

        static pid_t    fork_exec( 
            const char *execstr,
            int *fdin=NULL, int *fdout=NULL, int *fderr=NULL,
            const char *input=NULL, const char *output=NULL, const char *error=NULL,
            char ** env_keys=NULL, char **env_vals=NULL, unsigned num_env_pair=0,
            const char *dir=NULL
        );

        // fork_exec_watch()
        //
        // does fork_exec(), then ensures that signal handler watches
        // for this pid. handles race condition where pid may exit
        // right away.
        //
        // returns pid of child, or else throws.
        //
        // arguments have the same meaning as they do for fork_exec().
        //
        // does not block caller indefinitely.
        //

        static pid_t      fork_exec_watch( 
            const char *execstr,
            int *fdin=NULL, int *fdout=NULL, int *fderr=NULL,
            const char *input=NULL, const char *output=NULL, const char *error=NULL,
            char ** env_keys=NULL, char **env_vals=NULL, unsigned num_env_pair=0,
            const char *dir=NULL
            );

        //fork_exec_wait()
        //
        // does a fork() and child does exec() of execstr().  passing
        // fd pointers for pipes would create hang or race so you cant.
        //
        // blocks caller indefinitely.
        //
        // returns exit status of child.
        //
        static int      fork_exec_wait ( 
            const char *execstr,
            const char *input=NULL, 
            const char *output=NULL, 
            const char *error=NULL ,
            char ** env_keys=NULL, char **env_vals=NULL, unsigned num_env_pair=0,
            const char *dir=NULL
        );

        //fork_exec_wait_or_panic()
        //
        // does a fork() and child does exec() of execstr(). 
        //
        // blocks caller indefinitely.
        //
        // throws a BaseException if exit status of child is non-zero.
        //
        // version with 4 args will redirect those args that are not
        // null to the named files.
        //
        static void     fork_exec_wait_or_panic ( 
            const char *execstr,
            const char *input=NULL, 
            const char *output=NULL, 
            const char *error=NULL ,
            char ** env_keys=NULL, char **env_vals=NULL, unsigned num_env_pair=0,
            const char *dir=NULL
        );

        //ensures that when we recive sigchild for this pid, the result
        //will be remembered and sockets will be shutdown if appropriate.
        //
        static void     watch_pid( pid_t p, ewproc_t *e );

        //-1 if pid is not exited yet. other return values are not trustworthy.
        //a good way to see if process has exited yet, and if it has you can
        //follow up with wait_for_exit_status without fear of blocking the app.
        //
        //this method does not hang.
        //
        static int      check_for_exit_status( pid_t p ); //-1 if not done yet.

        //returns exists status, which can be evaluated with status_* funcs
        //below.
        //
        //will hang until pid exits. use check_for_exit_status to see if the
        //process has exited allready.
        //
        static int      wait_for_exit_status( pid_t p );

        //free memory associated with watching for results of pid p.
        //
        static void     done_watching( pid_t p );
        
        //just like using backticks in perl, except it will throw an exception
        //by default if the command fails.
        static char_ptr eval( const char *execstr, bool panic_on_failure=true );

        // two methods for interpreting exit status.
        //
        // necessary to tell whether app did exit(9) or died from kill(9)
        // if app did exit(0), status will be zero. no special method for that.
        //
        static int     status_uncaught_signal( int status );
        static int     status_exit( int status );

    private:
        static int      get_exit_status( pid_t p, int flags );

        //private data with singleton like semantics is required to correctly
        //handle sigchld across multiple threads and 
        friend class ExecWrapPriv ;
        static ExecWrapPriv *p;
};


#endif /* EXECWRAP_H */
