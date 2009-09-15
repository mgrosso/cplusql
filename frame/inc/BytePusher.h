
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BytePusher.h
// Author:    mgrosso 
// Created:   Sat Jul  3 10:25:56 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: BytePusher.h,v 1.1 2004/07/13 22:28:38 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BYTEPUSHER_H
#define BYTEPUSHER_H 1

#include "Runnable.h"
#include "CopyByteSource.h"
#include "Dest.h"

class BytePusherPriv ;
class BytePusher : public Runnable
{
    public:
    BytePusher( CopyByteSourcePtr &source, pDest &sink );
    virtual ~BytePusher();
    virtual void run(void);

    private:
    BytePusherPriv *d_;
    //not implemented
    BytePusher();
    BytePusher( const BytePusher &rhs );
    BytePusher &operator=( const BytePusher &rhs );
};



#endif /* BYTEPUSHER_H */
