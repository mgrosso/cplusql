// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Slurp.h
// Author:    Matt Grosso
// Created:   Fri Nov 13 03:08:43 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Slurp.h,v 1.1.1.1 1999/06/05 16:30:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SLURP_H
#define SLURP_H 1

class Slurp {
    public:
    static char *slurpFile( const char *filename );
    static char *slurpFile( int fd, bool close_after );
    static char *slurpSocket( int fd, bool close_after );
};

#endif /* SLURP_H */
