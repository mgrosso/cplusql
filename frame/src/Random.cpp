#ident "file_id $Id: Random.cpp,v 1.2 2004/01/05 20:21:30 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Random.cpp
// Author:    Matt Grosso
// Created:   Sat Jun  5 19:08:44 EDT 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Matt Grosso.  All Rights Reserved.
// 
// $Id: Random.cpp,v 1.2 2004/01/05 20:21:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Random.h"
#include "stdlib.h"
#include "time.h"

int Random::init_ = 0;

int
Random::nextRaw(){
    if(!init_){
        srand(time(NULL));
        init_=1;
    }
    int gb=rand();
    srand((unsigned int)gb/43);
    return gb;
}

double
Random::nextValue(){
    double x = nextRaw();
    return x / (double)RAND_MAX ;
}

double
Random::nextValue( double min, double max ){
    return min + nextValue() * ( max - min );
}

