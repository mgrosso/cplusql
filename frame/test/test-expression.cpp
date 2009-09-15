#ident  "file_id $Id: test-expression.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-expression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:11:15 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-expression.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "BaseException.h"
#include "Expression.h"
#include "LiteralExpression.h"
#include "AdditionExpression.h"
#include "SubtractionExpression.h"
#include "MultiplicationExpression.h"
#include "DivisionExpression.h"
#include "LogicalOrExpression.h"
#include "LogicalAndExpression.h"
#include "NegationExpression.h"
#include "EqualsExpression.h"

int
main( int argc, char **argv )
{

    const char *twos = "2";
    const char *ones = "1";
    const char *zeros = "0";
    try{
        ExpressionPtr two = new LiteralExpression(twos);
        ExpressionPtr four = new AdditionExpression(two,two);
        ExpressionPtr sixteen = new MultiplicationExpression(four,four);
        ExpressionPtr eight = new DivisionExpression(sixteen,two);
        ExpressionPtr six = new SubtractionExpression(eight,two);

        long int sixi = six->getLong();
        long long int sixli = six->getLongLong();
        double sixd = six->getDouble();
        bool sixb = six->getBool();
        printf("i %li, lli %lli, d %g, b %i\n", sixi, sixli, sixd, sixb );

        ExpressionPtr one = new LiteralExpression(ones);
        ExpressionPtr zero = new LiteralExpression(zeros);

        ExpressionPtr or_true1 = new LogicalOrExpression(one, one);
        ExpressionPtr or_true2 = new LogicalOrExpression(one, zero);
        ExpressionPtr or_true3 = new LogicalOrExpression(zero, one);
        ExpressionPtr or_false = new LogicalOrExpression(zero, zero);

        ExpressionPtr and_true = new LogicalAndExpression(one, one);
        ExpressionPtr and_false1 = new LogicalAndExpression(one, zero);
        ExpressionPtr and_false2 = new LogicalAndExpression(zero, one);
        ExpressionPtr and_false3 = new LogicalAndExpression(zero, zero);

        ExpressionPtr nand = new NegationExpression( and_true );
        ExpressionPtr nnand = new NegationExpression( nand );

        ExpressionPtr eq_false = new EqualsExpression( nand, nnand );
        ExpressionPtr eq_true = new EqualsExpression( nnand, nnand );

        printf("one %i, zero %i\n", one->getBool(), zero->getBool() );
        printf("or11 %i, or10 %i, or01 %i, or00 %i\n", 
            or_true1->getBool(), 
            or_true2->getBool(), 
            or_true3->getBool(), 
            or_false->getBool() 
        );
        printf("and11 %i, and10 %i, and01 %i, and00 %i\n", 
            and_true->getBool(), 
            and_false1->getBool(), 
            and_false2->getBool(), 
            and_false3->getBool() 
        );
        printf("nand %i, nnand %i\n", nand->getBool(), nnand->getBool());
        printf("eq_f %i, eq_t %i\n", eq_false->getBool(), eq_true->getBool());
        return 0;
    }catch( BaseException &be )
    {
        printf( "exception in main(): %s\n", be.getMessage());
    }
    return 1;
};
