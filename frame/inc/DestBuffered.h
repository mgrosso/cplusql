// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestBuffered.h
// Author:    mgrosso 
// Created:   Sun Jul  7 23:42:21 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestBuffered.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DESTBUFFERED_H
#define DESTBUFFERED_H 1

#include "Dest.h"

class DestBufferedPriv ;

class DestBuffered : public Dest
{
    public:
    DestBuffered( int bufsize, pDest &d, bool newline );
    virtual ~DestBuffered();
    virtual void put(const char *s );
    virtual void put(const char *s, int slen );
    virtual void flush(void);

    private:
    void doput(const char *s, int slen );

    DestBufferedPriv *d_;

    //not implemented
    DestBuffered();
    DestBuffered(const DestBuffered &rhs);
    DestBuffered operator=(const DestBuffered &rhs);
};

#endif /* DESTBUFFERED_H */
