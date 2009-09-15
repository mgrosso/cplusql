#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogInfoExpression.cpp
// Author:    mgrosso 
// Created:   Tue Jul 18 04:09:51 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LogInfoExpression.h"
#include "AppLog.h"

class LogInfoExpressionPriv 
{
    private:
    ExpressionPtr operand_;
    friend class LogInfoExpression ;
    LogInfoExpressionPriv(ExpressionPtr &operand )
    :operand_(operand)
    {
    };
};

LogInfoExpression::LogInfoExpression(ExpressionPtr &operand )
{
    d_=new LogInfoExpressionPriv (operand);
}

LogInfoExpression::~LogInfoExpression()
{
    delete d_;
}


const char *
LogInfoExpression::getRaw()
{
    return d_->operand_->getRaw();
}

const char *
LogInfoExpression::getRaw() const
{
    Expression * e = const_cast<Expression *>(d_->operand_.get());
    return e->getRaw();
}

void
LogInfoExpression::notify() 
{
    d_->operand_->notify();
    APPLOG_INFO( d_->operand_->getRaw());
}




