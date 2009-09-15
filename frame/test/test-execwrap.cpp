#ident "$Id: test-execwrap.cpp,v 1.3 2004/09/22 17:59:26 mgrosso Exp $" 
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-execwrap.cpp
// Author:    mgrosso 
// Created:   Wed May  7 19:19:52 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: test-execwrap.cpp,v 1.3 2004/09/22 17:59:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <sys/types.h>
#include <signal.h>
#include <assert.h>
    
#include <stdio.h>
#include <string.h>
#include "BaseException.h"
#include "ExecWrap.h"
#include "Slurp.h"


#define MUSTTHROWTRY  \
        didthrow=false; \
        try{

#define MUSTTHROWCATCH        \
        } catch( exception &e ) {\
            didthrow=true;\
            fprintf(stderr,\
                "exception caught in main, as expected: %s\n", \
            e.what());\
        }\
        if(!didthrow)\
        {\
            PANICV("Expected to catch exception, but didnt.");\
        }

int main( int argc, char **argv )
{
    try{
        bool didthrow;
        int  result ;

        ExecWrap::fork_exec_wait_or_panic( "/bin/sleep 1" );

        MUSTTHROWTRY
            ExecWrap::fork_exec_wait_or_panic( "/does/not/exist" );
        MUSTTHROWCATCH
        MUSTTHROWTRY
            ExecWrap::fork_exec_wait_or_panic( "/bin/false" );
        MUSTTHROWCATCH
        MUSTTHROWTRY
            (void)ExecWrap::eval( "/bin/false" );
        MUSTTHROWCATCH

        pid_t p=ExecWrap::fork_exec_watch( "/bin/sleep 100" );

        result= ExecWrap::check_for_exit_status( p );
        fprintf(stderr,"check_for_exit_status() of sleep 100 was %i\n", result );
        assert( -1 == result );
        if(0!=kill(p,9))
        {
            PANICV("kill(%i,9) failed.",p);
        }
        result= ExecWrap::check_for_exit_status( p );
        result = ExecWrap::status_uncaught_signal( result );
        if( 9 != result )
        {
            fprintf(stderr,"check_for_exit_status() of sleep killed w/9 was %i\n", result );
        }
        result= ExecWrap::wait_for_exit_status( p );
        result = ExecWrap::status_uncaught_signal( result );
        fprintf(stderr,"wait_for_exit_status() of sleep killed w/9 was %i\n", result );
        assert( 9== result );

        result = ExecWrap::fork_exec_wait( "/bin/true" );
        assert( 0 == result );

        result = ExecWrap::fork_exec_wait( "/bin/false" );
        result = ExecWrap::status_exit( result );
        assert( 1 == result );

        char_ptr slurpself = Slurp::slurpFile( __FILE__ );

        char catfile[128];
        catfile[0]='\0';
        strcat( catfile, "cat ");
        strncat( catfile, __FILE__, 128 - 5 );

        fprintf(stderr,"about to eval() %s ... ", catfile );
        char_ptr catself=ExecWrap::eval( catfile , true);
        result = strcmp( catself.get(), slurpself.get());
        fprintf(stderr,"result was %i\n", result );
        if( catself.get() )
        {
            fprintf(stderr,"strlen catself.get() %i\n", strlen(catself.get()));
        }
        if( slurpself.get() )
        {
            fprintf(stderr,"strlen slurpself.get() %i\n", strlen(slurpself.get()));
        }
        assert( result == 0);

        const char *copyselffile = "test-execwrap.test.out" ;
        fprintf(stderr,"about to fork_exec_wait_or_panic() cat from __FILE__ to %s\n", copyselffile);
        ExecWrap::fork_exec_wait_or_panic( "cat", __FILE__, copyselffile, NULL );
        fprintf(stderr,"about to slurp() %s\n", copyselffile );
        char_ptr slurpcopyself = Slurp::slurpFile( copyselffile );

        if( slurpcopyself.get() )
        {
            fprintf(stderr,"strlen catself.get() %i\n", strlen(slurpcopyself.get()));
        }
        if( slurpself.get() )
        {
            fprintf(stderr,"strlen slurpself.get() %i\n", strlen(slurpself.get()));
        }
        if( slurpcopyself.get() && slurpself.get() )
        {
            result = strcmp( slurpcopyself.get(), slurpself.get());
        }
        else
        {
            fprintf(stderr,"null get()\n" );
            result = 1 ;
        }
        assert( result == 0);

        return 0;
    }catch( exception &e )
    {
        fprintf(stderr,"exception caught in main: %s\n", e.what());
    }
    return 1;
}



/*
class testexecwrap_writer
{
    public:
    static void wr( int fd, void *buf, size_t count)
    {
        if( -1==write( fd, buf, count ))
        {
            PANIC("writer cant write.");
        }
    };
    static void *run(void *f )
    {
        int fd= * static_cast<int *>(f);
        write( f, "01\n", 3 );
        write( f, "02\n", 3 );
        write( f, "03\n", 3 );
        write( f, "04\n", 3 );
        write( f, "05\n", 3 );
        sleep(10);
        write( f, "06\n", 3 );
        return NULL;
    }
};

class testexecwrap_reader
{
    public:
    static void *run(void *ignore )
    {
        char c;
        while(EOF!=(c=fgetc(stdin)))
        {
            if( EOF==putchar( c ))
            {
                PANIC("reader cant write");
            }
        }
        return NULL;
    }
};
*/

#if 0
int main ( int argc, char **argv )
{
    try {
        //test 1
        fprintf(stderr, "test 1, echo hello world... " );
        ExecWrap::run_or_panic( "/bin/echo hello world" );

        //test 2
        bool didfail=false;
        try {
            fprintf(stderr, "test 2, should fail: %u false ... ", getpid());
            //ExecWrap::run_or_panic( "nosuchcommand" );
            ExecWrap::run_or_panic( "/bin/false" );
        }catch( BaseException &b2 )
        {
            fprintf( stderr, "good! we expected it to fail %s\n", b2.getMessage());
            didfail=true;
        }
        if(!didfail)
        {
            PANIC("test 2, nosuchcommand should not suceed!");
        }

        //test 3
        fprintf(stderr, "test 3, cat /etc/passwd... " );
        char_ptr out = ExecWrap::eval( "cat /etc/passwd", true );
        fprintf(stderr, "(%s) test suceeded\n", out.get() );

        //test 4
        fprintf(stderr, "test 4, cat /etc/resolv.conf ... " );
        char_ptr out2 = ExecWrap::eval( "cat /etc/passwd", true );
        fprintf(stderr, "(%s) test suceeded\n", out2.get() );

        //test 5
        didfail=false;
        fprintf(stderr, "test 5, should fail: cat /does/not/exist ... " );
        try{
            char_ptr out3 = ExecWrap::eval( "cat /does/not/exist", true );
        }
        catch( BaseException &b3 )
        {
            fprintf( stderr, "good! we expected it to fail %s\n", b3.getMessage());
            didfail=true;
        }
        if(!didfail)
        {
            PANIC("test 5, cat /does/not/exist should not suceed!");
        }

    }catch( BaseException &b )
    {
        fprintf( stderr, "exception caught in main: %s\n", b.getMessage());
        exit(1);
    }

    fprintf(stderr, "all tests complete\n" );
    exit(0);
}
#endif /* 0 */

