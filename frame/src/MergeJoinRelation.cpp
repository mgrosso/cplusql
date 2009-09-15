#ident "file_id $Id: MergeJoinRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JoinRelation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec  5 00:48:53 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MergeJoinRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include <string>
#include "AbstractDelimitedSource.h"
#include "MergeJoinRelation.h"
#include "ProxyRelationPriv.h"
#include "BaseException.h"
#include "StrLessThanExpression.h"
#include "StrEqualsExpression.h"
#include "LessThanExpression.h"
#include "EqualsExpression.h"
#include "ColumnExpression.h"
#include "dbg.h"

class MergeJoinRelationPriv
{
    private:
    friend class MergeJoinRelation;
    RelationPtr         left_;
    RelationPtr         right_;
    ExpressionPtr       less_ ;
    ExpressionPtr       equals_ ;
    ExpressionList      all_ ;
    RelationMetaPtr     meta_;
    bool                done_;
    int                 linenum_;
};

MergeJoinRelation *
MergeJoinRelation::makeMergeJoinRelation(  
    RelationPtr left, RelationPtr right, const char *name,
    ExpressionPtr leftexp, ExpressionPtr rightexp,
    bool str
){
    RelationMetaPtr rmp( new RelationMeta());
    rmp->setRelationName( name );
    unsigned i;
    for( i=0; i< left->getNumColumns(); ++i )
    {
        rmp->addColumn( left->getColumnName( i ));
    }
    for( i=0; i< right->getNumColumns(); ++i )
    {
        //fprintf(stderr, 
        //    "MergeJoinRelation::make...(): name %s\n", 
        //    right->getRelationName()
        //);
        string s;
        try{
            rmp->addColumn( right->getColumnName( i ));
        }catch( BaseException &be )
        {
            s += right->getRelationName();
            s += "_" ;
            s += right->getColumnName( i );
        }
        if( ! s.empty())
        {
            rmp->addColumn( s.c_str());
        }
    }
    return new MergeJoinRelation( left, right, rmp, leftexp, rightexp, str);
}

MergeJoinRelation::MergeJoinRelation(  
    RelationPtr left, RelationPtr right, RelationMetaPtr meta,
    ExpressionPtr leftexp, ExpressionPtr rightexp,
    bool str
)
:ProxyRelation(meta)
{
    d_=new MergeJoinRelationPriv();
    if(!d_)
    {
        THROW_NOMEM;
    }
    d_->left_=left;
    d_->right_=right;
    if( str )
    {
        d_->less_ = (Expression *) new StrLessThanExpression( leftexp, rightexp );
        d_->equals_ = (Expression *) new StrEqualsExpression( leftexp, rightexp );
    }else
    {
        d_->less_ = (Expression *) new LessThanExpression( leftexp, rightexp );
        d_->equals_ = (Expression *) new EqualsExpression( leftexp, rightexp );
    }
    for( size_t il=0; il< d_->left_->getNumColumns(); ++il )
    {
        ExpressionPtr el( 
            (Expression *) new ColumnExpression( 
                ( const ptr< AbstractDelimitedSource > &)d_->left_,
                il
            )
        );
        addExpression(el);
    }
    for( size_t ir=0; ir< d_->right_->getNumColumns(); ++ir )
    {
        ExpressionPtr er(
            (Expression *)new ColumnExpression( 
                ( const ptr< AbstractDelimitedSource > &)d_->right_,
                ir
            )
        );
        addExpression(er);
    }
    d_->linenum_=0;
    d_->done_=false;
}

MergeJoinRelation::~MergeJoinRelation()
{
    delete d_;
}


bool        
MergeJoinRelation::childNotify()
{
    /*

    //FDBG_SSI( "next(): name", getRelationName(), d_->linenum_);
    if( d_->done_==true)
    {
        return false;
    }
    d_->done_=true;
    if(d_->linenum_==0)
    {
        if(! d_->right_->next())
        {
            return false;
        }
    }
    //FDBG_SSI( "next(): past right next", getRelationName(), d_->linenum_);
    if(! d_->left_->next())
    {
        return false;
    }
    //FDBG_SSI( "next(): past left next", getRelationName(), d_->linenum_);
    while( ! d_->equals_->getBool())
    {
        if( d_->less_->getBool())
        {
            if( ! d_->left_->next() )
            {
                //FDBG_SSI( "next(): finished left", getRelationName(), d_->linenum_);
                return false;
            }
            //FDBG_SSI( "next(): advanced left", getRelationName(), d_->linenum_);
        }else
        {
            if( ! d_->right_->next())
            {
                //FDBG_SSI( "next(): finished right", getRelationName(), d_->linenum_);
                return false;
            }
            //FDBG_SSI( "next(): advanced right", getRelationName(), d_->linenum_);
        }
    }
    //FDBG_SSI( "next(): bottom", getRelationName(), d_->linenum_);
    ++ d_->linenum_;
    d_->done_=false;
    */
    return true;
}

int         
MergeJoinRelation::lineNum()
{
    return d_->linenum_;
}

