#ident "file_id $Id: DirTree.cpp,v 1.2 2004/06/02 21:14:04 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirTree.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov 29 18:22:48 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirTree.cpp,v 1.2 2004/06/02 21:14:04 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

//frame_config.h must come after dirent.h here in order to provide S_ISLNK
//on systems that dont have it (like win32 + mingw)
#include "frame_config.h"

#include "DirOpen.h"
#include "DirTree.h"
#include "DirStack.h"
#include "BaseException.h"


class DirTreePriv 
{
    private:
    friend class DirTree ;

    char root_[PATH_MAX+1];
    DirAction  *action_;
    bool        do_hidden_;
    bool        ignore_errors_;

    DirTreePriv(const char *root )
    {
        memset(root_,'\0',PATH_MAX );
        strncpy( root_, root, PATH_MAX );
    };
    void traverse( const char *dirname )
    {
        //
        //for each file in dirname
        //  filepath=dirname+/+file
        //  if dir
        //      traverse( filepath )
        //  else
        //      do action( file, dirname )
        //

        DIRptr d = DirOpen::open( dirname );
        if( d.get() == NULL )
        {
            PANIC_S("cant opendir()", dirname );
        }
        struct dirent *de ;
        struct stat st;

        for( de=readdir(d.get()); de!=NULL; de=readdir(d.get()))
        {
            if( de->d_name[0]== '.' && 
                (
                    strlen( de->d_name )==1 ||
                    de->d_name[1]== '.'
                )
              ){
                continue;
            }
            if( de->d_name[0]== '.' && !do_hidden_ )
            {
                continue;
            }

            char path[PATH_MAX+1];
            memset(path, '\0', PATH_MAX+1);
            strncpy( path, dirname, PATH_MAX );
            strncat( path, PATH_SEP, PATH_MAX - strlen(path));
            strncat( path, de->d_name, PATH_MAX - strlen(path));

            if( 0!= stat( path, &st ))
            {
                if( ignore_errors_ )
                {
                    continue;
                }
                PANIC_S("cant stat ", path );
            }
            if( S_ISREG( st.st_mode ))
            {
                action_->callback( path, dirname, false, (unsigned)st.st_size);
            }
            else if( S_ISDIR( st.st_mode ) && !S_ISLNK( st.st_mode ))
            {
                traverse( path );
            }
        }
    };
};


void
DirTree::traverse(DirAction *action, bool do_hidden, bool ignore_errors )
{
    DirStack ds( d_->root_);
    d_->action_=action;
    d_->do_hidden_=do_hidden;
    d_->ignore_errors_=ignore_errors;
    d_->traverse( "." );
}

DirTree::DirTree( const char *root )
{
    d_=new DirTreePriv (root);
}

DirTree::~DirTree()
{
    delete d_;
}

#ifdef DIRTREE_MAIN
#include <stdio.h>

class DirTreeTestAction : public DirAction 
{
    virtual void callback( 
            const char *filename, const char *path, 
            bool is_dir, unsigned size 
    ){
        fprintf(stderr, "DirTreeTestAction: is_dir=%i, size=%u, file=%s path=%s\n",
            is_dir, size, filename, path
        );
    };
};

int main( int argc, char **argv )
{
    if( argc !=2 )
    {
        fprintf(stderr, "DirTree test: %s <dir>\n", argv[0] );
        exit(1);
    }
    try{
        DirTree dt(argv[1]);
        DirTreeTestAction action;
        dt.traverse( &action, false, false );
    }
    catch( exception &e )
    {
        fprintf(stderr, "DirTree test: exception: %s\n", e.what());
        exit(1);
    }
    catch( ... )
    {
        fprintf(stderr, "DirTree test: unknown exception\n" );
        exit(1);
    }
    exit(0);
};

#endif /* DIRTREE_MAIN */

