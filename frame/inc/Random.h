// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Random.h
// Author:    Matt Grosso
// Created:   Sat Jun  5 19:01:30 EDT 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Matt Grosso.  All Rights Reserved.
// 
// $Id: Random.h,v 1.1 1999/06/10 17:14:47 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef RANDOM_H
#define RANDOM_H 1

class Random {
    public:

    //returns int between 0 and RAND_MAX
    static int nextRaw();       

    //returns double between min and max
    static double nextValue( double min, double max );       

    //returns double between 0 and 1
    static double nextValue();  

    private:
    static int init_ ;
};

#endif /* RANDOM_H */
