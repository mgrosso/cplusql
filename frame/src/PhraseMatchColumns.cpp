#ident "file_id $Id: PhraseMatchColumns.cpp,v 1.2 2005/02/01 01:24:29 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PhraseMatchColumns.cpp
// Author:    mgrosso 
// Created:   Tue Jan 11 20:01:13 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: PhraseMatchColumns.cpp,v 1.2 2005/02/01 01:24:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "PhraseMatchColumns.h"
#include <map>
#include <vector>
#include <list>
#include "hashFuncs.h"
#include "ptr_strdup.h"
#include "Parse.h"
#include "ByteBuf.h"
#include "Convert.h"
#include "BaseException.h"
#include "AppLog.h"

typedef char *                          word_t;
typedef vector< word_t >                phrase_t;
typedef vector< word_t >                document_t ;
typedef multimap< word_t, unsigned  >   document_map_t ;
typedef list< phrase_t * >              phrase_list_t;
typedef map< word_t, phrase_list_t, cc_less_than > phrase_map_t;

class PhraseMatchColumnsPriv 
{
    friend class PhraseMatchColumns ;
    private:
    ptr< list< char_ptr > >         matchlist_ ;
    ExpressionPtr                   operand_ ;
    list< ptr< phrase_t > >         holder_phrases_;          
    phrase_map_t                    phrases_ ;
    ByteBuf                         docbuf_;
    ByteBuf                         resultbuf_;
    int                             matches_;
    char                            intbuf_[64];
    char_ptr                        input_delim_;
    char_ptr                        output_delim_;


    //ctor tasks:
    //- build phrase_map_t
    PhraseMatchColumnsPriv(
        ptr< list< char_ptr > > &matchlist, 
        ExpressionPtr &operand,
        const char *input_delim,
        const char *output_delim
    ):matchlist_(matchlist),
        operand_(operand), 
        matches_(0),
        input_delim_(ptr_strdup(input_delim)),
        output_delim_(ptr_strdup(output_delim))
    {
        list< char_ptr >::iterator i, e;
        for( i=matchlist->begin(), e=matchlist->end(); i!=e; ++i )
        {
            char_ptr &raw_phrase = (*i);
            ptr< phrase_t > phrase_words =
                Parse::parse_ptr(raw_phrase.get(), input_delim_.get(), false );
            holder_phrases_.push_back( phrase_words );

            list< phrase_t * > &p = phrases_[ *phrase_words->begin() ];
            phrase_t * pp = phrase_words.get();
            p.push_back( pp );
        }
        docbuf_.reset("");
        resultbuf_.reset("");
    };

    //findMatches tasks:
    //1- split raw text into word boundaries, build document_t
    //2- walk down document_t, counting and appending matches
    void findMatches( const char *s )
    {
        docbuf_.reset( s );
        resultbuf_.reset();
        matches_ = 0;
        ptr<document_t> pdoc = Parse::parse_ptr(
            docbuf_.buf(), input_delim_.get(), false);

        //APPLOG_DBG("searching through %i words", pdoc->size());
        //Word, LastWord
        size_t w, lw;
        for( w=0, lw = pdoc->size(); w!=lw; ++w )
        {
            char *word = pdoc->at(w);
            //APPLOG_DBG("searching phrase map: %s", word );
            phrase_map_t::iterator pm = phrases_.find(word);
            phrase_map_t::iterator pme = phrases_.end();
            if( pm==pme )
            {
                //APPLOG_DBG("not found in phrase map: %s", word );
                continue;
            }
            //list of all phrases beginning with "word"
            phrase_list_t &phraselist =  (*pm).second ;
            //APPLOG_DBG("found phrase list [%s] in phrase map with length: %lu", (*pm).first, phraselist.size());
            phrase_list_t::iterator a, ae;
            for( a=phraselist.begin(), ae=phraselist.end(); a!=ae; ++a )
            {
                phrase_t *p= *a ;
                if( p->size() == 1 )
                {
                    ++matches_;
                    if( *resultbuf_.get())
                    {
                        resultbuf_.append( ',' );
                    }
                    resultbuf_.appendNoNull( word );
                    //APPLOG_DBG("single word phrase [%s], matches=%i", (*pm).first, matches_ );
                    continue;
                }
                //Doc Index, Phrase Index, Phrase End
                size_t di, pi, pe;
                bool allmatched=true;
                //APPLOG_DBG("testing %i word phrase ", p->size());
                for(di=w+1, pi=1, pe=p->size(); pi!=pe && di!=lw; ++di, ++pi)
                {
                    //APPLOG_DBG("doc word %i/%i:%s, phrase word %i/%i:%s", 
                    //    di, lw, pdoc->at(di), pi, pe, p->at(pi));
                    if( strcmp( pdoc->at(di), p->at(pi)))
                    {
                        allmatched=false;
                        break;
                    }
                }
                if( allmatched )
                {
                    resultbuf_.appendNoNull( word );
                    resultbuf_.appendNoNull( output_delim_.get());
                    ++matches_;
                    //APPLOG_DBG("matched, %i", matches_);
                }
                else
                {
                    //APPLOG_DBG("nomatch, %i", matches_);
                }
            }
        }
        resultbuf_.append('\0');
        Convert::IntToString( intbuf_, matches_ );
        //APPLOG_DBG("matched %i : %s", matches_, resultbuf_.get());
    };
};

PhraseMatchColumns::PhraseMatchColumns(
        ptr< list< char_ptr > > &matchlist, 
        ExpressionPtr &operand,
        const char *input_delim,
        const char *output_delim
)
{
    d_=new PhraseMatchColumnsPriv( 
        matchlist, operand, input_delim, output_delim  );
}

PhraseMatchColumns::~PhraseMatchColumns()
{
    delete d_;
}

int
PhraseMatchColumns::getMatchCount()
{
    return d_->matches_;
}

const char *
PhraseMatchColumns::getMatches()
{
    return d_->resultbuf_.get();
}

void
PhraseMatchColumns::findMatches( const char *haystack )
{
    d_->findMatches(haystack);
}

const char *
PhraseMatchColumns::getField(unsigned field_index)
{
    switch(field_index  )
    {
        case eMATCH_COUNT:      return d_->intbuf_;
        case eMATCHES:          return d_->resultbuf_.get();
    }
    PANICV("only 0,1 columns defined, index %i requested", field_index);
    return NULL;
}

void
PhraseMatchColumns::notify()
{
    d_->operand_->notify();
    d_->findMatches( d_->operand_->getRaw());
}

