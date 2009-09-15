#ident "file_id $Id: RelationFactory.cpp,v 1.6 2004/06/02 21:13:11 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RelationFactory.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 20:25:07 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: RelationFactory.cpp,v 1.6 2004/06/02 21:13:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "RelationFactory.h"
#include "SimpleRelation.h"
#include "RelationMeta.h"
#include "NoCopyDelimitedSource.h"
#include "MultiByteSource.h"
#include "BaseException.h"
#include "Config.h"

RelationPtr
RelationFactory::getSimpleRelationPtr( const char* relation_name, 
							    RelationPtr & rp,
								ptr< char_ptr_list > & derivedColumnNames,
						        ExpressionList &derivedColumns,
						        ExpressionList &filters,
						        ConfigPtr & config
								)
{
	RelationMetaPtr meta(new RelationMeta());
	meta->setRelationName( relation_name );
	bool ignore_src_cols ( config->getBool(IGNORE_SOURCE_COLUMNS, "false") );
	if ( ! ignore_src_cols )
	{
		unsigned n = rp->getNumColumns();
		for( unsigned i = 0; i<n; ++i )
		{
			meta->addColumn( rp->getColumnName( i ));
		}
	}
	while( ! derivedColumnNames->empty())
	{
		meta->addColumn( derivedColumnNames->front().get());
		derivedColumnNames->pop_front();
	}

    //fprintf(stderr, "RelationFactory::getSimpleRelationPtr(), rp numcol %u\n", rp->getNumColumns());

	DelimitedSourcePtr rp_dsp(rp);

    //AbstractDelimitedSource *ads=dynamic_cast< AbstractDelimitedSource * > ( rp.get() );
    //AbstractNotifier        *an =dynamic_cast< AbstractNotifier        * > ( rp.get() );
    //Listener                *l  =dynamic_cast< Listener                * > ( rp.get() );
    //AbstractDelimitedSource *ads=rp.get() ;
    //AbstractNotifier        *an =rp.get() ;
    //Listener                *l  =rp.get() ;

    //fprintf(stderr, "RelationFactory::getSimpleRelationPtr(), rp %x rp_dsp %x ads %x an %x l %x\n", 
    //    (unsigned int ) rp.get(), (unsigned int ) rp_dsp.get(),
    //    (unsigned int ) ads, (unsigned int ) an, (unsigned int ) l
    //);
	return new SimpleRelation(rp_dsp, meta, derivedColumns, filters, ignore_src_cols );
}

/*
RelationPtr
RelationFactory::getSimpleRelationPtr( 
    const char* relation_name, 
    ptr< char_ptr_list > & filelist,
    ptr< char_ptr_list > & cols,
    const char * delimiter,
    unsigned bufsize
)
{

    ByteSourcePtr srcp( MultiByteSource::makeFromGlobs( *filelist ));

	DelimitedSourcePtr dsp ( 
        new NoCopyDelimitedSource(
            srcp,
            bufsize,
            cols->size(),
            *delimiter
        )
    );

	RelationMetaPtr meta(new RelationMeta());
	meta->setRelationName( relation_name );
	list< char_ptr >::iterator i;
	list< char_ptr >::iterator e=cols->end();
	for( i=cols->begin(); i!=e; ++i )
	{
	    meta->addColumn( (*i).get() );
	}
	ExpressionList empty( new expression_list_t());
	RelationPtr sr(new SimpleRelation( dsp, meta, empty, empty, false ));

    char_ptr rname = ptr_strdup(relation_name );
    return new NextNotifyJoint( sr, rname );
}
*/
