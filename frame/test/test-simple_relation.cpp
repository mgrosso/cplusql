#ident  "file_id $Id: test-simple_relation.cpp,v 1.3 2004/01/05 20:06:24 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-simple_relation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Nov 24 00:32:48 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-simple_relation.cpp,v 1.3 2004/01/05 20:06:24 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Dest.h"
#include "Source.h"
#include "SourceFactory.h"
#include "BaseException.h"
#include "RelationMeta.h"
#include "ColumnExpression.h"
#include "LiteralExpression.h"
#include "AdditionExpression.h"
#include "SubtractionExpression.h"
#include "MultiplicationExpression.h"
#include "LogicalAndExpression.h"
#include "LogicalOrExpression.h"
#include "EqualsExpression.h"
#include "StrEqualsExpression.h"
#include "NegationExpression.h"
#include "LessThanExpression.h"
#include "StrLessThanExpression.h"
#include "SimpleRelation.h"

#define TESTFILE_INPUT "test-simple_relation-input.txt" 
#define TESTFILE_OUTPUT "test-simple_relation-output.txt" 
#define TESTNAME "test-simple_relation"

int main( int argc, char **argv )
{
    try{
        {
            pDest pd= DestFactory::getBufferedFileDest( TESTFILE_INPUT, true );
            pd->put( "0|0" );
            pd->put( "0|1" );
            pd->put( "1|0" );
            pd->put( "1|1" );
            pd->put( "1|2" );
            pd->put( "2|2" );
            pd->put( "2|3" );
            pd->put( "3|3" );
            pd->put( "9|3" );
            //pd->put( "2000000000000|2000000000000" );
            //when we hit the next right bracket pd will be destroyed ensuring that
            //the file is flushed.
        }
        fprintf( stderr, "so far so good 0\n" );
        DelimitedSourcePtr testfile = 
            SourceFactory::getDelimitedMultiSource( TESTFILE_INPUT, "|", 2 ); 

        RelationMetaPtr meta( new RelationMeta());
        meta->setRelationName( TESTNAME );
        meta->addColumn("col1" );
        meta->addColumn("col2" );
        meta->addColumn("literal" );
        meta->addColumn("AND" );
        meta->addColumn("OR" );
        meta->addColumn("NOT col1" );
        meta->addColumn("NAND" );
        meta->addColumn("EQUALS" );
        meta->addColumn("NOT EQUALS" );
        meta->addColumn("PLUS" );
        meta->addColumn("MINUS" );
        meta->addColumn("MULTIPLY" );
        meta->addColumn("1lt2" );
        meta->addColumn("2lt1" );
        meta->addColumn("str1lt2" );
        meta->addColumn("str1eq2" );

        fprintf( stderr, "so far so good 1\n" );

        ExpressionPtr col1( (Expression *)new ColumnExpression( testfile, 0 ));
        ExpressionPtr col2( (Expression *)new ColumnExpression( testfile, 1 ));

        ExpressionPtr literal( (Expression *)new LiteralExpression( "fu" ));
        ExpressionPtr epand((Expression *)new LogicalAndExpression( col1, col2 ));
        ExpressionPtr epor((Expression *)new LogicalOrExpression( col1, col2 ));
        ExpressionPtr notcol1( (Expression *)new NegationExpression( col1 ));
        ExpressionPtr nand( (Expression *)new NegationExpression( epand ));
        ExpressionPtr equals( (Expression *)new EqualsExpression( col1, col2 ));
        ExpressionPtr notequals( (Expression *)new NegationExpression( equals ));

        ExpressionPtr plus( (Expression *)new AdditionExpression( col1, col2 ));
        ExpressionPtr minus( (Expression *)new SubtractionExpression( col1, col2 ));
        ExpressionPtr multiply( (Expression *)new MultiplicationExpression( col1, col2 ));
        ExpressionPtr lt12( (Expression *)new LessThanExpression( col1, col2 ));
        ExpressionPtr lt21( (Expression *)new LessThanExpression( col2, col1 ));
        ExpressionPtr strlt12( (Expression *)new StrLessThanExpression( col1, col2 ));
        ExpressionPtr streq12( (Expression *)new StrEqualsExpression( col1, col2 ));

        ExpressionList derived ( new expression_list_t());
        derived->push_back( literal );
        derived->push_back( epand );
        derived->push_back( epor );
        derived->push_back( notcol1 );
        derived->push_back( nand );
        derived->push_back( equals );
        derived->push_back( notequals );
        derived->push_back( plus );
        derived->push_back( minus );
        derived->push_back( multiply );
        derived->push_back( lt12 );
        derived->push_back( lt21 );
        derived->push_back( strlt12 );
        derived->push_back( streq12 );

        ExpressionList filters ( new expression_list_t((size_t)0));

        {
            SimpleRelation sr( testfile, meta, derived, filters, false );
            pDest output= DestFactory::getBufferedFileDest( TESTFILE_OUTPUT, true );
            output->put( sr.getRelationHeader().get());
            while(sr.next())
            {
                output->put( sr.getCharPtr().get());
            }
        }
        fprintf( stderr, "so far still so good 2\n" );
    }catch( BaseException &be )
    {
        fprintf( stderr, "caught BaseException in main: %s", 
            be.getMessage()
        );
        return 1;
    }
    return 0;
}

