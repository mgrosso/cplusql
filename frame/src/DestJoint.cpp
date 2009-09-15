#ident "file_id $Id: DestJoint.cpp,v 1.7 2005/02/01 01:26:01 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestJoint.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:24:09 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DestJoint.cpp,v 1.7 2005/02/01 01:26:01 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "DestJoint.h"
#include "DestFile.h"
#include "BaseException.h"
#include "dbg.h"


class DestJointPriv
{
    private:
    friend class DestJoint;
    RelationPtr         relp_;
    pDest               destp_;
    char_ptr            delimiterp_; 
    char_ptr            record_separatorp_; 
    unsigned            column_count_;

    DestJointPriv( 
        RelationPtr &rel, pDest &dest, 
        const char *delimiter, const char *record_separator
    )
    :   relp_(rel),
        destp_(dest),
        delimiterp_( ptr_strdup_wnull(delimiter)), 
        record_separatorp_(ptr_strdup_wnull( record_separator )),
        column_count_(rel->getNumColumns())
    {
        ;//noop
    };
    void writeMeta( Dest &pd )
    {
        pd.put( "#" );
        for( unsigned int c=0; c< column_count_; ++c )
        {
            pd.put( relp_->getColumnName(c));
            if( c+1 < column_count_ )
            {
                pd.put( delimiterp_.get());
            }
        }
        pd.put( record_separatorp_.get());
    };
};

DestJoint::DestJoint( 
    RelationPtr &rel, pDest &dest, const char *delimiter, const char *record_separator, const char *metafile, bool first_row_header )
:Listener()
{
    jp_=new DestJointPriv( rel, dest, delimiter, record_separator );
    if( first_row_header )
    {
        jp_->writeMeta( *dest );
    }
    if( metafile )
    {
        DestFile df ( metafile );
        jp_->writeMeta( df );
    }
    rel->registerListener( this );
}

DestJoint::~DestJoint()
{
    //fprintf(stderr, "DestJoint::~DestJoint()\n");
    jp_->destp_->flush();
    delete jp_;
}

void
DestJoint::notify()
{
    for( unsigned int c=0; c< jp_->column_count_; ++c )
    {
        jp_->destp_->put( jp_->relp_->getRaw(c));
        if( c+1 < jp_->column_count_ )
        {
            jp_->destp_->put( jp_->delimiterp_.get());
        }
    }
    jp_->destp_->put( jp_->record_separatorp_.get());
    FDBG_SS( "DestJoint::notify()", jp_->relp_->getRelationName());
}

