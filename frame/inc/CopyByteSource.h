
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CopyByteSource.h
// Author:    mgrosso 
// Created:   Fri Jul  2 22:19:46 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: CopyByteSource.h,v 1.1 2004/07/13 22:31:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef COPYBYTESOURCE_H
#define COPYBYTESOURCE_H 1

#include "frame_config.h"
#include "ptr.h"

class CopyByteSource
{
    public:
    virtual                     ~CopyByteSource();
    virtual bool                next( char * &buf, size_t &len )=0;

    protected:
    CopyByteSource();

    private:
    //not implemented
    CopyByteSource( const CopyByteSource &rhs );
    CopyByteSource &operator=( const CopyByteSource &rhs );
};

typedef ptr< CopyByteSource > CopyByteSourcePtr ;


#endif /* COPYBYTESOURCE_H */
