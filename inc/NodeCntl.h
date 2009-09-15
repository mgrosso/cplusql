
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      inc/NodeCntl.h
// Author:    mgrosso 
// Created:   Sun Apr  4 21:19:20 EST 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: NodeCntl.h,v 1.4 2004/04/19 20:30:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NODECNTL_H
#define NODECNTL_H 1
    
#include "ptr.h"

class NodeCntlPriv ;
class NodeCntl
{
    public:
    NodeCntl( const char *dir, const char *suffix );
    ~NodeCntl();

    void    setOOS( const char *host, const char *jobid, const char *command ) const;
    void    setOK( const char *host ) const;
    bool    isOK( const char *host ) const;

    private:
    NodeCntlPriv *d_;
    //not implemented
    NodeCntl( const NodeCntl &rhs );
    NodeCntl &operator=( const NodeCntl &rhs );
};

typedef ptr< NodeCntl > NodeCntlPtr ;


#endif /* NODECNTL_H */
