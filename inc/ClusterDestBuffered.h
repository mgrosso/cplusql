// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      
// Author:   
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestBuffered.h,v 1.1 2004/06/16 10:19:10 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CLUSTERDESTBUFFERED_H
#define CLUSTERDESTBUFFERED_H 1

#include "Dest.h"

class ClusterDestBufferedPriv ;

class ClusterDestBuffered : public Dest
{
    public:
    ClusterDestBuffered( int bufsize, pDest &d, bool newline );
    virtual ~ClusterDestBuffered();
    virtual void put(const char *s );
    virtual void put(const char *s, int slen );
    virtual void flush(void);

    private:
    void doput(const char *s, int slen );

    ClusterDestBufferedPriv *d_;

    //not implemented
    ClusterDestBuffered();
    ClusterDestBuffered(const ClusterDestBuffered &rhs);
    ClusterDestBuffered operator=(const ClusterDestBuffered &rhs);
};

#endif /* CLUSTERDESTBUFFERED_H */
