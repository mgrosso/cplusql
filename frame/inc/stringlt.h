// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      stringlt.h
// Author:    Matt Grosso
// Created:   Thu Nov 12 00:04:08 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: stringlt.h,v 1.2 2001/03/05 08:14:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PTRCCLT_H
#define PTRCCLT_H 1

// return true if lhs is lexically less than rhs.

class stringlt {
    public:
    bool operator()( const string &lhs, const string &rhs){
        return lhs < rhs ;
    }
};

#endif /* PTRCCLT_H */
