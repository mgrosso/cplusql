#ident  "file_id $Id: test-str_split_relation.cpp,v 1.3 2004/01/05 20:06:24 mgrosso Exp $"
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
// $Id: test-str_split_relation.cpp,v 1.3 2004/01/05 20:06:24 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Dest.h"
#include "Source.h"
#include "SourceFactory.h"
#include "BaseException.h"
#include "RelationMeta.h"
#include "SimpleRelation.h"
#include "ColumnExpression.h"
#include "ColumnElementExpression.h"
#include "DelimitedColumn.h"
#include "JoinExpression.h"
#include "CRC48Expression.h"
#include "Listener.h"

#define TESTFILE_INPUT "test-simple_relation-input.txt" 
#define TESTFILE_OUTPUT "test-simple_relation-output.txt" 
#define TESTNAME "test-simple_relation"

int main( int argc, char **argv )
{
    try{
        {
            pDest pd= DestFactory::getBufferedFileDest( TESTFILE_INPUT, true );
            pd->put( "ac0-ag0-ci0-log_ci0-li0|0" );
            pd->put( "ac1-ag1-ci1-log_ci1-li1|1" );
            //when we hit the next right bracket pd will be destroyed ensuring that
            //the file is flushed.
        }
        fprintf( stderr, "so far so good 0\n" );
        DelimitedSourcePtr testfile = 
            SourceFactory::getDelimitedMultiSource( TESTFILE_INPUT, "|", 2 ); 

        RelationMetaPtr meta( new RelationMeta());
        meta->setRelationName( TESTNAME );
        meta->addColumn("crc" );
        //meta->addColumn("col1" );
        //meta->addColumn("col2" );
        meta->addColumn("ac" );
        meta->addColumn("ag" );
        meta->addColumn("ci" );
        meta->addColumn("log_ci" );
        meta->addColumn("li" );
        meta->addColumn("join" );

        fprintf( stderr, "so far so good 1\n" );

        ExpressionPtr col1( (Expression *)new ColumnExpression( testfile, 0 ));
        ExpressionPtr col2( (Expression *)new ColumnExpression( testfile, 1 ));

        DelimitedColumnPtr de(new DelimitedColumn( col1, "-", 5, false ));
        ExpressionPtr ac( (Expression *)new ColumnElementExpression( de, 0 ));
        ExpressionPtr ag( (Expression *)new ColumnElementExpression( de, 1 ));
        ExpressionPtr ci( (Expression *)new ColumnElementExpression( de, 2 ));
        ExpressionPtr log_ci( (Expression *)new ColumnElementExpression( de, 3 ));
        ExpressionPtr li( (Expression *)new ColumnElementExpression( de, 4 ));

        ExpressionList joined_list ( new expression_list_t());
        joined_list->push_back( li );
        joined_list->push_back( ci );
	
		ExpressionPtr jce ( (Expression *) new JoinExpression(joined_list, "***" ));

		ExpressionPtr crc ( (Expression *) new CRC48Expression(jce));

        ExpressionList derived ( new expression_list_t());
        derived->push_back( crc );
        derived->push_back( ac );
        derived->push_back( ag );
        derived->push_back( ci );
        derived->push_back( log_ci );
        derived->push_back( li );
        derived->push_back( jce );

        fprintf( stderr, "so far so good 2\n" );

        ExpressionList filters ( new expression_list_t((size_t)0));
        RelationPtr sr( new SimpleRelation(testfile, meta, derived, filters, true ));
		sr->registerListener(reinterpret_cast<ListenerPtr &>(de));
        pDest output= DestFactory::getBufferedFileDest( TESTFILE_OUTPUT, true );
        output->put(sr->getRelationHeader().get());
        fprintf( stderr, "so far so good 3\n" );
        while(sr->next())
        {
            output->put( sr->getCharPtr().get());
        }
        fprintf( stderr, "so far still so good 4\n" );
    }catch( BaseException &be )
    {
        fprintf( stderr, "caught BaseException in main: %s", 
            be.getMessage()
        );
        return 1;
    }
    fprintf( stderr, "so far still so good 5\n" );
    return 0;
}

