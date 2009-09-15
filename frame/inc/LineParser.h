// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LineParser.h
// Author:    Matt Grosso
// Created:   Fri Nov 13 03:01:47 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: LineParser.h,v 1.1.1.1 1999/06/05 16:30:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LINEPARSER_H
#define LINEPARSER_H 1

class LineParser {
    LineParser( int sepchar );
    ~LineParser();
    void parse( char *date, vector<char *> &results );
};

#endif /* LINEPARSER_H */
