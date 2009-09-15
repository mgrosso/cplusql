#ident  "file_id $Id: test-merge_join_relation.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-merge_join_relation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 20:44:41 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-merge_join_relation.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $
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
#include "MergeJoinRelation.h"
#include "Joint.h"


#define TESTFILE_LEFT  "test-merge_join_relation-left.txt" 
#define TESTFILE_RIGHT "test-merge_join_relation-right.txt" 
#define TESTFILE_OUT   "test-merge_join_relation-out.txt" 

int main( int argc, char **argv )
{
    try{
        {
            pDest pd= DestFactory::getBufferedFileDest( TESTFILE_LEFT, true );
            pd->put( "0|0" );
            pd->put( "0|1" );
            pd->put( "1|0" );
            pd->put( "1|1" );
            pd->put( "1|2" );
            pd->put( "2|2" );
            pd->put( "2|3" );
            pd->put( "3|3" );
            pd->put( "9|3" );

            pDest pdright= DestFactory::getBufferedFileDest( TESTFILE_RIGHT, true );
            pdright->put( "0|a" );
            pdright->put( "1|b" );
            pdright->put( "2|c" );
            pdright->put( "3|d" );
            pdright->put( "8|e" );
        }
        fprintf( stderr, "so far so good 0\n" );
        DelimitedSourcePtr leftsrc = SourceFactory::getDelimitedMultiSource( TESTFILE_LEFT, "|", 2 ); 
        DelimitedSourcePtr rightsrc = SourceFactory::getDelimitedMultiSource( TESTFILE_RIGHT, "|", 2 ); 

        RelationMetaPtr leftmeta( new RelationMeta());
        leftmeta->setRelationName( "left" );
        leftmeta->addColumn("col1" );
        leftmeta->addColumn("col2" );

        RelationMetaPtr rightmeta( new RelationMeta());
        rightmeta->setRelationName( "right" );
        rightmeta->addColumn("col1" );
        rightmeta->addColumn("col2" );

        RelationMetaPtr outmeta( new RelationMeta());
        outmeta->setRelationName( "joined" );
        outmeta->addColumn("l.col1" );
        outmeta->addColumn("l.col2" );
        outmeta->addColumn("r.col1" );
        outmeta->addColumn("r.col2" );

        fprintf( stderr, "so far so good 1\n" );

        ExpressionPtr leftjoincol( (Expression *)new ColumnExpression( leftsrc, 0 ));
        ExpressionPtr rightjoincol( (Expression *)new ColumnExpression( rightsrc, 0 ));

        ExpressionList derived ( new expression_list_t());
        ExpressionList filters ( new expression_list_t());

        RelationPtr leftrel( new SimpleRelation( leftsrc, leftmeta, derived, filters, false ));
        RelationPtr rightrel( new SimpleRelation( rightsrc, rightmeta, derived, filters, false ));

        RelationPtr join( (AbstractRelation *)new MergeJoinRelation( 
            leftrel, rightrel, outmeta, leftjoincol, rightjoincol, false ));

        pDest output= DestFactory::getBufferedFileDest( TESTFILE_OUT, true );
        output->put( join->getRelationHeader().get());

        Joint j( join, output );
        j.start();
        if( 0 != j.wait())
        {
            fprintf( stderr, "error [%s]\n", j.error_string().get());
        }else{
            fprintf( stderr, "so far still so good 2\n" );
        }
    }catch( BaseException &be )
    {
        fprintf( stderr, "caught BaseException in main: %s", 
            be.getMessage()
        );
        return 1;
    }
    return 0;
}

