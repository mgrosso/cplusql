// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RelationFactory.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 19:28:48 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: RelationFactory.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef RELATIONFACTORY_H
#define RELATIONFACTORY_H 1

#include "Expression.h"
#include "AbstractRelation.h"
#include "Config.h"


class RelationFactory {
    public:

        static RelationPtr getSimpleRelationPtr( 
								const char * relation_name,
								RelationPtr &rp,
						        ptr< char_ptr_list> &derivedColumnNames,
						        ExpressionList &derivedColumns,
						        ExpressionList &filters,
						        ConfigPtr & config
							    );

        /*
		static NextNotifyJoint *getSimpleRelationPtr(
								const char* relation_name,
								ptr< char_ptr_list > & filelist,
								ptr< char_ptr_list > & cols,
							    const char * delimiter,
                                unsigned bufsize
							    );
        */
    private:
};

#define IGNORE_SOURCE_COLUMNS "IGNORE_SOURCE_COLUMNS"

#endif /* RELATIONFACTORY_H */
