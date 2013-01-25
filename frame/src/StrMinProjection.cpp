#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrMinProjection.cpp
// Author:    mgrosso 
// Created:   Mon Jul 17 12:44:04 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "StrMinProjection.h"
#include "ptr_strdup.h"
#include "AppLog.h"
#include "dbg.h"

class StrMinProjectionPriv 
{
    private:
    friend class StrMinProjection ;
    StrMinProjectionPriv( ExpressionPtr &e )
    :src_(e),first(true)
    {
        //noop
    };
    ExpressionPtr src_ ;
    char_ptr prev_winner_;
    bool first;
};

StrMinProjection::StrMinProjection(ExpressionPtr &e)
{
    d_=new StrMinProjectionPriv (e);
    set(ptr_strdup(""));
}

StrMinProjection::~StrMinProjection()
{
    delete d_;
}

int
StrMinProjection::compare(const char *lhs, const char *rhs)
{
    //if this function returns positive, it means rhs wins.
    return strcmp(lhs,rhs);
}

void
StrMinProjection::childNotify()
{
    d_->src_->notify();
    const char *lhs = getRaw();
    const char *rhs = d_->src_->getRaw();
    int cmp=compare(lhs,rhs);
    //APPLOG_DBG(" lhs=%s rhs=%s cmp=%i first=%i",lhs,rhs,cmp,(int)d_->first);
    if(d_->first || cmp > 0 )
    {
        set(d_->src_->getCharPtr());
    }
    if(d_->first)
    {
        d_->first=false;
    }
}

AbstractProjectionPtr
StrMinProjection::copy()
{
    AbstractProjectionPtr ap( new StrMinProjection( d_->src_ ));
    return ap;
}

ExpressionPtr & 
StrMinProjection::getOperand()
{
    return d_->src_;
}
