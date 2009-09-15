#ident "file_id $Id: MultiRegexColumns.cpp,v 1.3 2005/02/06 00:19:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiRegexColumns.cpp
// Author:    mgrosso 
// Created:   Tue Jun  8 22:22:05 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: MultiRegexColumns.cpp,v 1.3 2005/02/06 00:19:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MultiRegexColumns.h"
#include "RegexColumns.h"
#include "Convert.h"

class MultiRegexColumnsPriv 
{
    private:
    friend class MultiRegexColumns ;

    list< ptr< RegexColumns > >     rc_;
    vector< char_ptr >              colstrs_ ;
    size_t                          whichcol_ ;
    RegexColumns                   *p_;
};

MultiRegexColumns::MultiRegexColumns( ExpressionPtr & source,
                      ptr< char_ptr_list > &patterns,
                      size_t fields,
                      bool case_insensitive,
                      bool posix_newlines
                      )
{
    d_=new MultiRegexColumnsPriv ();
    char_ptr_list::iterator i = patterns->begin();
    char_ptr_list::iterator e= patterns->end();
    long j=0;
    for( ; i!=e; ++i, ++j )
    {
        ptr< RegexColumns > prc( 
            new RegexColumns( 
                source, i->get(), fields, case_insensitive, posix_newlines ));
        d_->rc_.push_back( prc );
        d_->colstrs_.push_back( ptr_strdup_long( j ));
        //fprintf(stderr, 
        //    "ctor pattern sz=%u, colstr sz=%u rc sz=%u, prc %u\n",
        //    patterns->size(), d_->colstrs_.size(), d_->rc_.size(), 
        //    reinterpret_cast<unsigned int>(prc.get()));
    }
    d_->colstrs_.push_back( ptr_strdup_long( j ));
    //fprintf(stderr, "final: ctor pattern sz=%u, colstr sz=%u rc sz=%u\n",
    //    patterns->size(), d_->colstrs_.size(), d_->rc_.size());
    d_->p_=NULL;
    d_->whichcol_ = 0;
}

MultiRegexColumns::~MultiRegexColumns()
{
    delete d_;
}

const char *
MultiRegexColumns::getField(unsigned field_index)
{
    if( ! field_index )
    {
        return d_->colstrs_[ d_->whichcol_ ].get() ;
    }
    return d_->p_->getField( field_index );
}

void
MultiRegexColumns::notify()
{
    list< ptr< RegexColumns > >::iterator i;
    list< ptr< RegexColumns > >::iterator e;
    size_t j=1;
    d_->whichcol_=0;
    for( i=d_->rc_.begin(), e=d_->rc_.end(); i!=e; ++i, ++j )
    {
        (*i)->notify();
        d_->p_ = i->get();
        //fprintf(stderr, "whichcol_ %i j %i p %u \n", 
        //    d_->whichcol_, j, reinterpret_cast< unsigned int>(d_->p_));
        if( Convert::convertToBool( (*i)->getField(0)))
        {
            d_->whichcol_=j;
            return;
        }
    }
    //fprintf(stderr, "no match whichcol_ %i j %i p %u\n", 
    //    d_->whichcol_, j, reinterpret_cast< unsigned int>(d_->p_));
}





