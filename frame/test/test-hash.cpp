#ident  "file_id $Id: test-hash.cpp,v 1.3 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-hash.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Tue Oct 29 02:16:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-hash.cpp,v 1.3 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "Source.h"
#include "stdio.h"
#include "SourceFactory.h"
#include "BaseException.h"
#include "DestHashFiles.h"
#include "DestHashHelperShim.h"
#include "SimpleRelation.h"
#include "MultiplexRelation.h"
#include "ColumnExpression.h"
#include "HashExpression.h"
#include "IfThenExpression.h"
#include "LessThanExpression.h"
#include "LiteralExpression.h"
#include "Joint.h"

int main( int argc, char **argv )
{
    try{
        DelimitedSourcePtr sp = 
            SourceFactory::getDelimitedMultiSource( "/etc/passwd", ":", 7 );
        //mgrosso:x:500:500:Matthew Grosso:/home/mgrosso:/bin/bash
        RelationMetaPtr meta(new RelationMeta());
        meta->addColumn("user");
        meta->addColumn("x");
        meta->addColumn("uid");
        meta->addColumn("gid");
        meta->addColumn("name");
        meta->addColumn("home");
        meta->addColumn("shell");

        RelationMetaPtr meta2(new RelationMeta());
        meta2->addColumn("user");
        meta2->addColumn("x");
        meta2->addColumn("uid");
        meta2->addColumn("gid");
        meta2->addColumn("name");
        meta2->addColumn("home");
        meta2->addColumn("shell");
        meta2->addColumn("uidcol");
        meta2->addColumn("100");
        meta2->addColumn("lt");
        meta2->addColumn("one");
        meta2->addColumn("zero");
        meta2->addColumn("ifltonezero");
        //meta2->addColumn("hasher");

        ExpressionList empty( new expression_list_t());

        const unsigned int buckets = 11;
        ExpressionPtr usercol( (Expression *)new ColumnExpression( sp, 0 ));
        ExpressionPtr hasher( (Expression *)new HashExpression( usercol, buckets ));
        ptr< DestHashHelper > helper( 
            new DestHashHelperShim( hasher, buckets )
        );

        ExpressionPtr uidcol((Expression *)new ColumnExpression(sp,2));
        ExpressionPtr hundred((Expression *)new LiteralExpression( "100"));
        ExpressionPtr zero((Expression *)new LiteralExpression( "0"));
        ExpressionPtr one((Expression *)new LiteralExpression( "1"));
        ExpressionPtr lt((Expression *)new LessThanExpression( uidcol, hundred ));
        ExpressionPtr ifthen((Expression *)new IfThenExpression( lt,one,zero ));
        //ExpressionPtr hasher2((Expression *)new HashExpression( ifthen, 2 ));
        ptr< DestHashHelper > helper2( 
            new DestHashHelperShim( ifthen, 2 )
        );

        RelationPtr sr(new SimpleRelation( sp, meta, empty, empty, false ));
        RelationPtr rwrap( (AbstractRelation *)new MultiplexRelation( sr, 2 ));

        RelationPtr sr2(
            new SimpleRelation( 
                    ( DelimitedSourcePtr &)rwrap, meta, empty, 
                    empty, false 
            )
        );

        ExpressionList dbgexp( new expression_list_t());
        dbgexp->push_back( uidcol );
        dbgexp->push_back( hundred );
        dbgexp->push_back( lt );
        dbgexp->push_back( one );
        dbgexp->push_back( zero );
        dbgexp->push_back( ifthen );
        //dbgexp->push_back( hasher2 );

        RelationPtr sr3(
            new SimpleRelation( 
                    ( DelimitedSourcePtr &)rwrap, meta2, dbgexp, 
                    empty, false 
            )
        );

        pDest pd(
            new DestHashFiles( 
                helper, "test-hashfile-output%i.asc", 8192, true 
            )
        );
        pDest pd2(
            new DestHashFiles( 
                helper2, "test-hashfile-output-alt-%i.asc", 8192, true 
            )
        );
        pDest pd3(
            DestFactory::getBufferedFileDest( 
                "test-hashfile-output-dbg.asc", true 
            )
        );
        Joint j( rwrap, pd );
        Joint j2( sr2, pd2 );
        Joint j3( sr3, pd3 );
        j.start();
        j2.start();
        j3.start();
        if( 0 != j.wait())
        {
            PANIC_S( "problem executing joint.", j.error_string().get());
        }
        if( 0 != j2.wait())
        {
            PANIC_S( "problem executing joint 2.", j2.error_string().get());
        }
        if( 0 != j3.wait())
        {
            PANIC_S( "problem executing joint 3.", j3.error_string().get());
        }
    }catch( BaseException &be )
    {
        fprintf( stderr, "caught BaseException in main: %s\n", be.getMessage());
        return 1;
    }catch( exception &e )
    {
        fprintf( stderr, "caught exception in main: %s\n", e.what());
        return 1;
    }catch( ... )
    {
        fprintf( stderr, "caught unknown exception in main.\n" );
        return 1;
    }
    fprintf( stderr, "happy happy joy joy\n" );
    return 0;
};


