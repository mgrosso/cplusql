#ident "file_id $Id: CommandExpression.cpp,v 1.2 2005/01/24 19:30:37 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CommandExpression.cpp
// Author:    mgrosso 
// Created:   Sun Jan  2 03:21:40 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: CommandExpression.cpp,v 1.2 2005/01/24 19:30:37 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CommandExpression.h"
#include "ExecWrap.h"
#include "ptr_strdup.h"
#include "AppLog.h"

class CommandExpressionPriv 
{
    private:
    friend class CommandExpression ;

    char_ptr            output_;
    ExpressionPtr       operand_ ;
    bool                panic_on_failure_;

    CommandExpressionPriv(
        ExpressionPtr &operand, bool panic_on_failure )
    :operand_(operand),panic_on_failure_( panic_on_failure)
    {
        ;//noop
    };
};

CommandExpression::CommandExpression( 
    ExpressionPtr &operand, bool panic_on_failure )
{
    d_=new CommandExpressionPriv ( operand, panic_on_failure );
    d_->output_ = ptr_strdup_wnull( "" );
}

CommandExpression::~CommandExpression()
{
    delete d_;
}

const char *        
CommandExpression::getRaw()
{
    return d_->output_.get();
}

const char *        
CommandExpression::getRaw() const 
{
    return d_->output_.get();
}

void 
CommandExpression::notify() 
{
    //APPLOG_DBG( "." );
    d_->operand_->notify();
    const char *command = d_->operand_->getRaw();
    APPLOG_DBG( "evaluating command(%s)", command );
    d_->output_ = ExecWrap::eval( command, d_->panic_on_failure_ );
    if( ! d_->output_.get())
    {
        d_->output_ = ptr_strdup_wnull( "" );
    }
    APPLOG_DBG( "completed evaluating command(%s)", command );
    //APPLOG_DBG( "output=%s", d_->output_.get());
}

