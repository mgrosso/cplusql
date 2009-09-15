#ident "file_id $Id: XpathRelation.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      XpathRelation.cpp
// Author:    mgrosso 
// Created:   Thu Jan  6 02:10:38 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: XpathRelation.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#if FRAME_USES_XML

#include "XpathRelation.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "AppLog.h"
#include "CharExpression.h"


#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <pthread.h>
#include <strings.h>

#ifndef LIBXML_XPATH_ENABLED
#error  "you must have LIBXML_XPATH_ENABLED"
#endif

static pthread_once_t libxml_is_initialized = PTHREAD_ONCE_INIT;
typedef ptr< CharExpression > CharExpressionPtr ;
typedef ptr< vector< CharExpressionPtr > > CharExpressionList ;

class XpathRelationPriv
{
private:
    friend class XpathRelation ;
    ExpressionPtr           xmltext_ ; 
    RelationMetaPtr         meta_ ;
    char_ptr                query_;
    const xmlChar *         xquery_;
    CharExpressionList      cols_;
    xmlDocPtr               xmldoc_;
    xmlXPathContextPtr      xpathCtx_;
    xmlXPathObjectPtr       xpathObj_;
    size_t                  line_ ;

    XpathRelationPriv( 
        ExpressionPtr &xmltext, RelationMetaPtr &meta, const char *query);
    static  void    init();
    void    cleanup();
    void    notify( Listener *l );
};

XpathRelation::XpathRelation( 
    ExpressionPtr &xmltext, RelationMetaPtr &meta, const char *query)
:ProxyRelation(meta)
{
    d_=new XpathRelationPriv (xmltext, meta, query);
    for(size_t i=0; i< meta->getNumColumns(); ++i )
    {
        CharExpressionPtr cep(new CharExpression(""));
        ExpressionPtr ep(cep);
        d_->cols_->push_back(cep);
        addExpression(ep);
    }
}

XpathRelation::~XpathRelation()
{
    delete d_;
}

bool
XpathRelation::childNotify()
{
    //not used.
    return true;
}

bool
XpathRelation::childNotifyLast()
{
    return true;
}

void
XpathRelation::notify()
{
    d_->notify( n_ );
}

int
XpathRelation::lineNum()
{
    return d_->line_;
}

void
XpathRelationPriv::notify( Listener *l )
{
    //get the raw doc.
    xmltext_->notify();
    const char *text = xmltext_->getRaw();
    size_t textlen = strlen(text);
    APPLOG_DBG("parsing xmldoc of %lu bytes, text follows:\n\n%s\n\n",
        textlen, text );

    //parse
    xmldoc_ = xmlParseMemory(text, textlen );
    if( !xmldoc_ )
    {
        PANICV( "unable to parse doc." );
    }

    //run the query.
    xpathCtx_ = xmlXPathNewContext(xmldoc_);
    if(xpathCtx_ == NULL) 
    {
        cleanup();
        PANICV("Unable to create new xml Path context using this doc.");
    }
    xpathObj_ = xmlXPathEvalExpression(xquery_, xpathCtx_);
    if(xpathObj_ == NULL) 
    {
        cleanup();
        PANICV("Error: unable to evaluate xpath expression \"%s\"\n", 
            xquery_ );
    }

    //for each node found, set the column expression values and then 
    //notify our downstream children.
    if(! xpathObj_->nodesetval )
    {
        APPLOG_DBG("no rows found");
        return;
    }
    size_t size = xpathObj_->nodesetval->nodeNr ;
    for( size_t i = 0; i< size; ++i )
    {
        xmlNodePtr cur = xpathObj_->nodesetval->nodeTab[i];
        if(cur->type != XML_ELEMENT_NODE || cur->ns ) 
        {
            continue;
        }
        size_t colsfound = 0;
        size_t ncols = meta_->getNumColumns();
        for( size_t j=0; j < ncols; ++j )
        {
            const char *name = meta_->getColumnName(j);
            for( xmlNodePtr n=cur->children; n ; n=n->next )
            {
                if( !strcmp((const char *)n->name, name ))
                {
                     CharExpressionPtr &cep = cols_->at(j);
                     cep->set( (const char *)xmlNodeGetContent(n));
                     ++colsfound;
                     break;
                }
            }
            if( colsfound != j+1)
            {
                break;
            }
        }
        if( ncols == colsfound )
        {
            ++line_;
            l->notify();
        }
    }
    cleanup();
}

XpathRelationPriv::XpathRelationPriv( 
    ExpressionPtr &xmltext, RelationMetaPtr &meta, const char *query)
:xmltext_(xmltext), meta_(meta), query_(ptr_strdup(query)),
    xquery_((const xmlChar *)query_.get()),
    xmldoc_(NULL), xpathCtx_(NULL), xpathObj_(NULL),line_(0)
{
    cols_ = new vector< CharExpressionPtr > ;
    pthread_once( &libxml_is_initialized, init );
}

void    
XpathRelationPriv::init()
{
    xmlInitParser();
    LIBXML_TEST_VERSION ;
}

void    
XpathRelationPriv::cleanup()
{
    if( xpathObj_)
    {
        xmlXPathFreeObject(xpathObj_);
        xpathObj_ = NULL;
    }
    if( xpathCtx_)
    {
        xmlXPathFreeContext(xpathCtx_);
        xpathCtx_=NULL;
    }
    if(xmldoc_)
    {
        xmlFreeDoc(xmldoc_);
        xmldoc_=NULL;
    }
}

#else  /* FRAME_USES_XML */
#include "XpathRelation.h"
#include "BaseException.h"

XpathRelation::XpathRelation( 
    ExpressionPtr &xmltext, RelationMetaPtr &meta, const char *query)
:ProxyRelation(meta)
{
    PANICV("XpathRelation cant be used if you didnt compile with --enable-libxml2");
}

XpathRelation::~XpathRelation()
{
}

bool
XpathRelation::childNotify()
{
    PANICV("XpathRelation cant be used if you didnt compile with --enable-libxml2");
    return true;
}

bool
XpathRelation::childNotifyLast()
{
    PANICV("XpathRelation cant be used if you didnt compile with --enable-libxml2");
    return true;
}

void
XpathRelation::notify()
{
    PANICV("XpathRelation cant be used if you didnt compile with --enable-libxml2");
}

int
XpathRelation::lineNum()
{
    PANICV("XpathRelation cant be used if you didnt compile with --enable-libxml2");
    return 0;
}
#endif  /* FRAME_USES_XML */
