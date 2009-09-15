#ident "file_id $Id: NodeCntl.cpp,v 1.7 2004/06/02 22:45:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NodeCntl.cpp
// Author:    mgrosso 
// Created:   Sun Apr  4 21:19:11 EST 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: NodeCntl.cpp,v 1.7 2004/06/02 22:45:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "NodeCntl.h"
#include "ptr_strdup.h"
#include "AppLog.h"
#include "DestFile.h"
#include "BaseException.h"

class NodeCntlPriv 
{
    private:
    friend class NodeCntl ;

    char_ptr dir_;
    char_ptr suffix_;
    int nonhost_len;

    NodeCntlPriv( const char *dir, const char *suffix );
    void getpath( char *buf, int buflen, const char *host  ) const ;
    int  getlen( const char *host  ) const ;
};

NodeCntlPriv::NodeCntlPriv( const char *dir, const char *suffix )
:dir_(ptr_strdup(dir)), suffix_( ptr_strdup(suffix))
{
    nonhost_len = 
        strlen(dir) +
        1 + // for trailing /
        strlen(suffix) + 
        1 ; //for trailing null
    
    struct stat buf;        
    if( 
        (stat(dir, &buf) != 0) && 
        ( 0 != mkdir(dir, 0755)))
    {
        PANICV("Could not stat() or mkdir() %s", dir);
    }
}

void
NodeCntlPriv::getpath( char *buf, int buflen, const char *host ) const
{
    snprintf( buf, buflen-1, "%s/%s%s", dir_.get(), host, suffix_.get() );
    buf[buflen-1]='\0';
}

int
NodeCntlPriv::getlen( const char *host ) const
{
    return strlen(host)+nonhost_len+1 ;
}

NodeCntl::NodeCntl(  const char *dir, const char *suffix )
{
    d_=new NodeCntlPriv ( dir, suffix );
}

NodeCntl::~NodeCntl()
{
    delete d_;
}


void
NodeCntl::setOOS(  const char *host, const char *jobid, const char *command ) const
{
    int len = d_->getlen(host) ;
    char oosPath[  len ];
    d_->getpath( oosPath, len, host );

    APPLOG_DBG("setting oos %s", oosPath ); 
    pDest destFile = new DestFile(oosPath);// this new file will close upon return
    Log ooslog ( destFile );
    char buf[FRAMELOG_MAX_MESSAGE];
    AppLog::instance()->print( buf, __FILE__, __LINE__, __func__, Log::log_warn, 
        "jobid=%s command=%s", jobid, command );
    ooslog.write( Log::log_warn, buf );
    ooslog.shutdownFlush();
}

void
NodeCntl::setOK(  const char *host ) const
{
    int len = d_->getlen(host) ;
    char oosPath[  len ];
    d_->getpath( oosPath, len, host );

    APPLOG_DBG("unlinking oos file %s", oosPath ); 
    if( 0 != unlink( oosPath ))
    {
        if(errno != ENOENT )
        {
            APPLOG_ALARM( "unable to unlink oos file %s, errno=%i. please investigate.", oosPath, errno ); 
        }
    }
}

bool
NodeCntl::isOK(  const char *host ) const
{
    int len = d_->getlen(host) ;
    char oosPath[  len ];
    d_->getpath( oosPath, len, host );

    APPLOG_DBG("checking oos %s", oosPath ); 
    struct stat buf;        
    if(stat(oosPath, &buf) == 0)  
    {
        return false ;
    }
    return true;
}

