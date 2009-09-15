#ident "file_id $Id: ProjectionRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProjectionRelation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec 21 21:47:52 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ProjectionRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <map>
#include "ptr.h"
#include "ptr_strdup.h"
#include "ProjectionRelation.h"
#include "Parse.h"
#include "CharExpression.h"
#include "BaseException.h"
#include "hashFuncs.h"


typedef ptr< vector< AbstractProjectionPtr > >      MetricList ;
typedef map< const char *, MetricList, cc_less_than >             AggregateMapT ;
typedef ptr< vector< char * > >                     charvec_t ;

class ProjectionRelationPriv 
{
    private:
    friend class ProjectionRelation ;

    RelationPtr                         src_;
    RelationMetaPtr                     meta_;
    ExpressionList                      projection_ ;
    expression_list_t                   outval_ ;
    MetricList                          basemetrics_ ;
    bool                                init_ ;
    AggregateMapT                       agmap_ ;
    char_ptr_list                       projectiondata_ ;
    AggregateMapT::iterator             rowi_ ;
    AggregateMapT::iterator             end_ ;
    MetricList                          row_ ;
    charvec_t                           values_ ;
    int                                 linenum_;
    ProjectionRelationPriv( 
        RelationPtr src, 
        RelationMetaPtr meta,
        ExpressionList projection, 
        MetricList metrics 
    )
    :src_(src),meta_(meta),projection_(projection),basemetrics_(metrics),
        init_(false),linenum_(0)
    {
        ;//noop
    };
    //helper
    inline 
    const ExpressionPtr &at( size_t index ) const 
    {
        return (const ptr< Expression, ptr_default_deleter<Expression> > &)
            *(outval_.begin()+index) ;
    };
    inline 
    ExpressionPtr &at( size_t index )
    {
        return (ptr< Expression, ptr_default_deleter<Expression> > &)
            *(outval_.begin()+index) ;
    };
};

ProjectionRelation::ProjectionRelation(
    RelationPtr         src,
    RelationMetaPtr     meta,
    ExpressionList      projection,
    MetricList          metrics
)
{
    d_=new ProjectionRelationPriv( src, meta, projection, metrics );
    if( !d_ )
    {
        THROW_NOMEM;
    }
}

ProjectionRelation::~ProjectionRelation()
{
    delete d_;
}

bool
ProjectionRelation::childNotify()
{
    char_ptr_list vals;
    unsigned sz=d_->projection_->size();
    for( unsigned i=0; i<sz; ++i )
    {
        vals.push_back( 
            (
                *(d_->projection_->begin()+i)
            ) -> getCharPtr()
        );
    }
    char_ptr rowstr( ptr_strdup_ptr_list( vals, "|", false ));
    const char *rowstrs = rowstr.get();
    d_->projectiondata_.push_back( rowstr );
    d_->end_=d_->agmap_.end();
    if( d_->end_ == ( d_->rowi_ = d_->agmap_.find( rowstrs )))
    {
        vector< AbstractProjectionPtr > *al=
            new vector< AbstractProjectionPtr >();
        if( !al )
        {
            THROW_NOMEM;
        }
        MetricList newleaf( al );
        for( unsigned j=0; j<d_->basemetrics_->size(); ++j )
        {
            al->push_back(
                (
                    *(d_->basemetrics_->begin()+j)
                )->copy()
            );
        }
        d_->agmap_.insert( AggregateMapT::value_type( rowstrs, newleaf ));
        d_->rowi_ = d_->agmap_.find( rowstrs );
    }
    MetricList &thisleaf = (*d_->rowi_).second ;
    for( unsigned k=0; k<thisleaf->size(); ++k )
    {
        (*(thisleaf->begin()+k))->loadRow();
    }
    return false;
}

bool
ProjectionRelation::childNotifyLast()
{
    unsigned prjvalsz = d_->projection_->size();
    unsigned metricsz = d_->basemetrics_->size();
    unsigned m;
    for( m=0; m< prjvalsz ; ++m )
    {
        d_->outval_.push_back( 
            (ptr< Expression, ptr_default_deleter<Expression> > &)
                ptr< CharExpression >( new CharExpression(NULL))
        );
    }
    //these next few are just placeholders for now, until later in next()
    for( m=0; m< metricsz ; ++m )
    {
        d_->outval_.push_back( 
            (ptr< Expression, ptr_default_deleter<Expression> > &)
            ptr< CharExpression >( new CharExpression(NULL))
        );
    }

    d_->rowi_   = d_->agmap_.begin();
    d_->end_    = d_->agmap_.end();

    for( ; d_->rowi_ != d_->end_ ; ++ d_->rowi_ )
    {
        //its ok to destructively parse the row now, because the map will 
        //not change at this point and we allready have an iterator. this
        //does mean that the map is no longer usable for lookups, but then 
        //we no longer need it for that.
        d_->values_= Parse::parse_ptr( (char *)(*d_->rowi_).first, "|", false );

        //now setup outval_ with all the right expression ptr
        vector< char * > *vc =d_->values_.get();
        unsigned projectsz =    d_->projection_->size();
        unsigned metricsz =     d_->basemetrics_->size();
        unsigned m;
        for( m=0; m<projectsz ; ++m )
        {
            ((ptr<CharExpression> &)(d_->outval_[m]))->set( (*vc)[m] );
        }
        MetricList &vep=(*d_->rowi_).second ;
        vector< AbstractProjectionPtr >  *ve=vep.get();
        vector< AbstractProjectionPtr >::iterator appb=ve->begin();
        for( m=0; m<metricsz ; ++m )
        {
            d_->outval_[m+projectsz] = 
                (ptr< Expression, ptr_default_deleter<Expression> > &) (*(appb+m));
        }

        //finally
        ++ d_->linenum_;
        n_->notify();
    }
    return false;
};

int
ProjectionRelation::lineNum()
{
    return d_->linenum_ ;
};

char_ptr    
ProjectionRelation::getCharPtr()
{
    char_ptr_list outl;//
    for( unsigned i=0 ; i< d_->meta_->getNumColumns(); ++i )
    {
        outl.push_back( getCharPtr( i ));
    }
    return ptr_strdup_ptr_list( outl, "|", false );
}

const char *
ProjectionRelation::getRaw( size_t index ) 
{
    //(*(d_->outval_->begin()+index))->getRaw();
    return d_->at( index )->getRaw();
}

char_ptr
ProjectionRelation::getCharPtr( size_t index ) const 
{
    return d_->at( index )->getCharPtr();
}

bool
ProjectionRelation::getBool( size_t index ) const 
{
    return d_->at( index )->getBool();
}

long int
ProjectionRelation::getLong( size_t index ) const 
{
    return d_->at( index )->getLong();
}

double
ProjectionRelation::getDouble( size_t index ) const 
{
    return d_->at( index )->getDouble();
}

long long int
ProjectionRelation::getLongLong( size_t index ) const 
{
    return d_->at( index )->getLongLong();
}

long double
ProjectionRelation::getLongDouble( size_t index ) const 
{
    return getLongDouble( index );
}

const char *
ProjectionRelation::getRelationName() const
{
    return d_->meta_->getRelationName();
}

const char *
ProjectionRelation::getColumnName( size_t i ) const
{
    return d_->meta_->getColumnName( i );
}

size_t
ProjectionRelation::getNumColumns() const
{
    return d_->meta_->getNumColumns();
}

size_t
ProjectionRelation::getColumnIndex( const char *col_name ) const
{
    return d_->meta_->getColumnIndex( col_name );
}

char_ptr
ProjectionRelation::getRelationHeader() const
{
    return d_->meta_->getRelationHeader();
}

