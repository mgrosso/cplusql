#ident "file_id $Id: cplusql_api.cpp,v 1.2 2005/10/19 08:44:27 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      cplusql.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Feb  6 03:37:07 EST 2003 on circe.looksmart.net
// Project:
// Purpose:
//
// Copyright (c) 2003 LookSmart.
//
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: cplusql_api.cpp,v 1.2 2005/10/19 08:44:27 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdlib.h>

#include "frame_config.h"

#include "BaseException.h"
#include "Config.h"
#include "ConfigDump.h"
#include "Convert.h"
#include "Dest.h"
#include "DestFactory.h"
#include "DestThreadedBuffer.h"
#include "AbstractDestFactory.h"

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "ClusterDestFactory.h"
#include "ClusterDestFile.h"
#include "clusterioC.h"
#include "JobRunnable.h"
#include "SharedServiceMain.h"
#include "ClusterByteSource.h"
#endif
#endif


#include "DestHashFiles.h"
#include "HashHelperShim.h"
#include "RelationMeta.h"
#include "RelationFactory.h"
#include "SimpleRelation.h"
#include "MultiByteSource.h"
#include "PgCopyByteSource.h"
#include "NoCopyDelimitedSource.h"
#if 0
//temporary
#include "MergeJoinRelation.h"
#endif
#include "PreSortedProjection.h"
#include "AbstractJoint.h"
#include "NextNotifyJoint.h"
#include "DestJoint.h"
#include "MapJoint.h"
#include "ParsedColumns.h"
#include "DelimitedColumn.h"
#include "RegexColumns.h"
#include "MultiRegexColumns.h"


#include "CharExpression.h"
#include "EqualsExpression.h"
#include "StrEqualsExpression.h"
#include "LessThanExpression.h"
#include "GreaterThanExpression.h"
#include "HashExpression.h"
#include "CRC48Expression.h"
#include "MD5Expression.h"
#include "CeilExpression.h"
#include "FloorExpression.h"
#include "ColumnExpression.h"
#include "JoinExpression.h"
#include "IfThenExpression.h"
#include "AbortExpression.h"
#include "AdditionExpression.h"
#include "SubtractionExpression.h"
#include "MultiplicationExpression.h"
#include "DivisionExpression.h"
#include "ModuloExpression.h"
#include "LogicalAndExpression.h"
#include "LogicalOrExpression.h"
#include "NegationExpression.h"
#include "ColumnElementExpression.h"
#include "MapValueExpression.h"
#include "MapExistsExpression.h"
#include "LineNumberExpression.h"
#include "RandomExpression.h"
#include "RegexMatchExpression.h"
#include "StrStrExpression.h"
#include "StrLenExpression.h"
#include "StrPlusExpression.h"
#include "StrLessThanExpression.h"
#include "StrGreaterThanExpression.h"
#include "TranslateExpression.h"
#include "LiteralExpression.h"
#include "LnExpression.h"
#include "Log2Expression.h"
#include "Log10Expression.h"
#include "LongDoubleFormatExpression.h"
#include "LongLongFormatExpression.h"
#include "NormalizeExpression.h"
#include "MatchCountExpression.h"
#include "BitwiseAndExpression.h"
#include "BitwiseOrExpression.h"
#include "PowExpression.h"
#include "PgCopyListener.h"
#include "IconvExpression.h"
#include "CommandExpression.h"
#include "ChompExpression.h"
#include "WarnExpression.h"
#include "LogInfoExpression.h"
#include "DMLExpression.h"
#include "SprintfExpression.h"
#include "SQLEscapeExpression.h"
#include "NanosleepExpression.h"
#include "ExpressionRelation.h"
#include "PgListenExpression.h"
#include "RepeatDBRelation.h"
#include "SQLValueExpression.h"
#include "CoalesceExpression.h"
#include "StrGreatestExpression.h"
#include "StrLeastExpression.h"
#include "PhraseMatchColumns.h"
#include "SubStrExpression.h"
#include "ByteStripExpression.h"
#include "FactorialExpression.h"
#include "SequenceExpression.h"

#include "SumProjection.h"
#include "AvgProjection.h"
#include "MinProjection.h"
#include "MaxProjection.h"
#include "StrMinProjection.h"
#include "StrMaxProjection.h"
#include "FirstProjection.h"
#include "LastProjection.h"
#include "CountProjection.h"
#include "CountChangesProjection.h"
#include "CountDistinctProjection.h"

#include "db/DBConnectionFactory.h"
#include "db/DBDelimitedSource.h"
#include "SQLRunnable.h"
#include "PostgresMakeTable.h"
#include "AbstractKeyValMap.h"
#include "AbstractKeyValSetMap.h"
#include "ExpressionStringMap.h"
#include "ADSMapFactory.h"
#include "db/DBMapFactory.h"
#include "HashConnections.h"
#include "HashedDSMap.h"
#include "ADSProxyMap.h"
#include "ExecWrap.h"
#include "AsyncRunnable.h"
#include "ThreadRunnable.h"
#include "ForkExecRunnable.h"
#include "Parse.h"
#include "ThreadedByteSource.h"
#include "DestNotifier.h"
#include "BytePusher.h"
#include "StringArray.h"
#include "StringArrayRelation.h"
#include "StringArrayOrderedRelation.h"
#include "StringArrayListener.h"
#include "Index.h"
#include "IndexBuilder.h"
#include "MemoryJoinRelation.h"
#include "AppLog.h"
#include "fwrap.h"
#include "twrap.h"
#include "DelayStubDelimitedSource.h"
#include "QueryList.h"
#include "XpathRelation.h"

#include "CplusqlContext.h"
#include "CplusqlExceptionHandling.h"
#include "cplusql_api.h"
#include "cplusql_grammar.h"
#include "keys.h"

#include "dbg.h"

/*
 * note that every api in this file must not throw. all exceptions must
 * be caught by this file because this c++ code is exported as a c api 
 * which is called from the yacc and lexx (ok, bison/flex) files. any exceptions
 * which try to cross the c/c++ barrier will cause a core file rather than a
 * useful error message. The following cplusql_catch() functions are used to 
 * handle those exceptions in a consistent fashion.
 */

/*
 * helper functions that should not be called from c/yacc, only c++
 * 
 * for helping resolve the namespace server.
 * 
 * If you need a namespace server, pass the possibly null name
 * and config to cplsuql_namespace_helper. The response will
 * not be null, although the function may throw.
 */

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
clusterio::NameSpaceService_ptr
cplusql_namespace_helper(const char *name, ConfigPtr &cfg );

void
cplusql_add_error(
    const char *templ, const char *object_string,
    char_ptr &error, CORBA::Exception &ce);

clusterio::NameSpaceService_ptr
cplusql_namespace_resolve(
    const char *nsname, const char *object_string, char_ptr &error );

#endif
#endif

DestFactoryPtr
cplusql_destfactory_helper( ConfigPtr &cfg );

void
cplusql_connect_dest( const char *relname, pDest &pd, ConfigPtr &cfg );

void
cplusql_relation_helper(
    char_ptr &name,
    const char *delimiter,
    ptr< list < char_ptr > > &cols,
    ByteSourcePtr &src,
    ConfigPtr &cfg);

ByteSourcePtr
cplusql_bytesource_helper( ptr<list<char_ptr> > &files, ConfigPtr &cfg);

void
cplusql_event_helper( char_ptr &name, AbstractDelimitedSourceNextPtr &dsp, ConfigPtr &cfg);

void
cplusql_parsed_helper( ptr< list< char_ptr > > &lcp, ParsedColumnsPtr &dcp);

/*
 * cplusql_new_line() is called by lexer code whenever a newline is encountered.
 */
static int cplusql_line_ = 1 ;
void
cplusql_new_line()
{
    ++cplusql_line_;
}

unsigned
cplusql_get_line()
{
    return cplusql_line_;
}

/*
 * cplusql_lex_push/pop_state() are called by lexer code.
 * 
 */
static list< int > * lexer_state_ = NULL ;
void
cplusql_lex_push_state( int state )
{
    try {
        if( lexer_state_ == NULL )
        {
            //new can throw.
            lexer_state_ = new list< int >();
        }
        lexer_state_->push_front( state );
    }
    CPLUSQL_CATCHALL();
}

int 
cplusql_lex_pop_state()
{
    try {
        if( ! lexer_state_ || lexer_state_->size()==0)
        {
            PANICV( "popping lexer state stack but nothting there.");
        }
        int ret = lexer_state_->front();
        lexer_state_->pop_front();
        return ret;
    }
    CPLUSQL_CATCHALL();
    // not reached.
    return 0;
}

cplusql_id_t 
cplusql_foreach( char *varname, cplusql_id_t values, char *template_code)
{
    try{
        if( ! values )
        {
            return NULL;
        }
        if( !varname || !template_code )
        {
            PANICV("null arg");
        }
        char_ptr pvarname(varname);
        char_ptr ptemplate(template_code);
        ptr< list<char_ptr> > pvalues=
            reinterpret_cast<list<char_ptr>*>(values);
        
        size_t keysize = strlen(varname)+ 2 * strlen("__") ;
        char key[keysize+1];
        strcpy( key, "__" );
        strcat( key, varname );
        strcat( key, "__" );
        
        while( ! pvalues->empty())
        {
            const char *value = (pvalues->back()).get();
            if(!value)
            {
                continue;
            }
            APPLOG_DBG("ptr_replace_all(%s,%s,%s)",
                template_code, key, value);
            char_ptr pushback = ptr_replace_all(template_code, key, value);
            APPLOG_DBG("ptr_replace_all returned(%s)", pushback.get());
            pushback = ptr_replace_all(pushback.get(), "\\\"","\"");
            APPLOG_DBG("ptr_replace_all unescape(%s)", pushback.get());

            APPLOG_DBG("lex_pushback(%s)", pushback.get());
            push_lex_buffer(pushback.get());
            pvalues->pop_back();
        }
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

//for lexer
void
cplusql_puke( const char *s )
{
    try {
        PANICV( s );
    }
    CPLUSQL_CATCHALL();
}

//for lexer
char * 
cplusql_string( const char *s )
{
    try {
        if( !s )
        {
            PANICV( "cplusql_string: null string" );
        }
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_string: %s\n", s);
        }
        return strdup_wnew( s );
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

//for lexer
void
cplusql_free_string( char *s )
{
    try {
        if( !s )
        {
            PANICV( "cplusql_free_string: null arg" );
        }
        delete [] s;
    }
    CPLUSQL_CATCHALL();
}

const char *
cplusql_getvar( const char *s )
{
    try {
        if( !s )
        {
            PANICV( "null arg" );
        }
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_getvar: %s\n", s);
        }
        return CplusqlContext::instance().getDefaultConfig()->findString(s);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached
}

const char *
cplusql_getvar_wdflt( const char *s, const char *dflt )
{
    try {
        if( !s )
        {
            PANICV( "null arg" );
        }
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_getvar_wdflt: key %s dflt %s\n", 
                    s, dflt);
        }
        return CplusqlContext::instance().getDefaultConfig()->getString(s,dflt);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached
}

char *
cplusql_eval( const char *s )
{
    try {
            if( !s )
            {
                PANICV( "null string" );
            }
            if( get_yacdbg())
            {
                fprintf(stderr, "cplusql_eval: %s\n", s);
            }
            char_ptr output=ExecWrap::eval( s );
            Parse::chop_newline( output.get());
            //now we leak the string, which is used by cplusql.l lex 
            //generated code in some fashion.  
            return cplusql_string( output.get());
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached
}

cplusql_id_t
cplusql_exec( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
            PANICV("null arg");
        }
        ExpressionPtr expp(reinterpret_cast<Expression *>(exp));
        expp->notify();
        const char *s = expp->getRaw(); 
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_exec: %s\n", s);
        }
        char_ptr ignored=ExecWrap::eval(s);
    }
    CPLUSQL_CATCHALL();
    return NULL; 
}

cplusql_id_t
cplusql_command_string_set(cplusql_id_t exp )
{
    try {
        if( !exp )
        {
            PANICV("null arg");
        }
        ExpressionPtr expp(reinterpret_cast<Expression *>(exp));
        expp->notify();
        const char *s = expp->getRaw(); 
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_command_string_set: %s\n", s);
        }
        char_ptr results=ExecWrap::eval(s);
        list < char_ptr > * lcp = new list< char_ptr> ();
        vector<char *> parsed_lines = Parse::parse( results.get(), "\n" );
        vector<char *>::iterator i = parsed_lines.begin();
        vector<char *>::iterator e = parsed_lines.end();
        for( ; i!=e; ++i )
        {
            lcp->push_back( ptr_strdup( *i ));
        }
        return static_cast<cplusql_id_t>(lcp) ;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached
}

cplusql_id_t 
cplusql_string_set( cplusql_id_t set, char *add )
{
    try {
        list < char_ptr > * lcp = reinterpret_cast< list< char_ptr> * >(set);
        lcp->push_back( ptr_strdup_take( (char *)add));
        return set;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_new_string_set( char * newstr )
{
    try {
        list < char_ptr > * lcp = new list< char_ptr> ();
        if( newstr == 0 )
        {
            return (cplusql_id_t)lcp ;
        }
        return cplusql_string_set( (cplusql_id_t)lcp, newstr );
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_range_string_set( char *from, char *to )
{
    return cplusql_range_step_string_set( from, to, cplusql_string("1"));
}

cplusql_id_t 
cplusql_range_step_string_set( char *from, char *to, char *step )
{
    try {
        char_ptr fromp(from);
        char_ptr top( to );
        char_ptr stepp( step );

        list < char_ptr > * lcp = new list< char_ptr> ();
        long int ibase = Convert::convertToLong( from );
        long int itop = Convert::convertToLong( to );
        long int istep = Convert::convertToLong( step );
        
        for( long int i = ibase; i <= itop; i += istep )
        {
            lcp->push_back( ptr_strdup_long( i ));
        }
        return static_cast<cplusql_id_t>(lcp) ;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_echo( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
            PANICV("null arg");
        }
        ExpressionPtr expp(reinterpret_cast<Expression *>(exp));
        expp->notify();
        fprintf(stderr, "%s\n", expp->getRaw());
    }
    CPLUSQL_CATCHALL();
    return NULL; 
}

cplusql_id_t 
cplusql_add_config( char * name, cplusql_id_t cfg )
{
    try {
        if( !name || !cfg )
        {
            PANICV("cplusql_named_config: null arg");
        }
        ConfigPtr cp(static_cast< Config * >(cfg));
        char_ptr  namep( name );
        CplusqlContext::instance().addConfig( name, cp );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_empty_config()
{
    try {
        return static_cast<cplusql_id_t>(new Config());
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_start_config( char *k, char *v )
{
    try {
        if( !k || !v )
        {
            PANICV("cplusql_start_config(): null arg");
        }
        char_ptr kp( k);
        char_ptr vp( v);
        Config *c = new Config( *(CplusqlContext::instance().getDefaultConfig()));
        c->addString( k, v, true );
        return static_cast<cplusql_id_t>(c);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_addto_config( cplusql_id_t cfg, char *k, char *v )
{
    try {
        if(!cfg || !k || !v )
        {
            PANICV("null arg");
        }
        char_ptr kp( k);
        char_ptr vp( v);
        (static_cast<Config *>(cfg))->addString( k, v, true );
        return cfg;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_copy_named_cfg( char *name )
{
    try {
        if(!name)
        {
            PANICV("null arg");
        }
        char_ptr namep( name );
        ConfigPtr &from = CplusqlContext::instance().getConfig( name );
        return static_cast<cplusql_id_t>(new Config(*from));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_merge_cfg( cplusql_id_t lhs, cplusql_id_t rhs )
{
    try {
        if(!lhs || !rhs )
        {
            PANICV("null arg");
        }
        Config *l=static_cast<Config *>(lhs);
        ConfigPtr rp( static_cast<Config *>(rhs));
        l->slurp( *rp, true );
        return lhs;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_default_cfg()
{
    try {
        return static_cast<cplusql_id_t>(
            new Config(
                *(CplusqlContext::instance().getDefaultConfig())
            )
        );
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_addto_default_cfg( cplusql_id_t addme )
{
    try {
        ConfigPtr c( static_cast<Config *>(addme));
        ConfigPtr &dflt = CplusqlContext::instance().getDefaultConfig();
        dflt->slurp( *c, true );
    }
    CPLUSQL_CATCHALL();
    return NULL; 
}

int 
cplusql_init_default_cfg( int *argc, char **argv )
{
    try {
        return static_cast<int>(
            CplusqlContext::instance().initConfig( argc, argv )
        );
    }
    CPLUSQL_CATCHALL();
    return 0; //not reached 
}

cplusql_id_t 
cplusql_cfg_file( char *filename )
{
    try {
        if( !filename || !*filename )
        {
            PANICV("null arg");
        }
        char_ptr filenamep(filename);
        Config *c = new Config(filename);
        return static_cast<cplusql_id_t>(c);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}


//for grammar
cplusql_id_t 
cplusql_expression( cplusql_id_t i )
{
    return i;
}

cplusql_id_t
cplusql_column_exp_str( char * relation, char * column )
{
    try {
        if( !relation || !column )
        {
            PANICV( "cplusql_column_exp_str(): null arg");
        }
        char_ptr rp(relation);
        char_ptr cp(column);
        RelationPtr &r = CplusqlContext::instance().getRelation( relation );
        unsigned index = r->getColumnIndex( column );
        Expression *ce = new ColumnExpression( r, index );
        return static_cast<cplusql_id_t>(ce);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_column_exp_str_dflt_rel( char * column )
{
    try {
        if( !column )
        {
            PANICV( "cplusql_column_exp_str(): null arg");
        }
        RelationPtr &r = CplusqlContext::instance().getCurrentSrcRelation();
        unsigned index = r->getColumnIndex( column );
        Expression *ce = new ColumnExpression( r, index );
        return static_cast<cplusql_id_t>(ce);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_literal_exp( char *str )
{
    try {
        //leak the string. its ok. how many literals can there be in
        //a source file? 
        //
        //char_ptr p(str);
        Expression *ln = new CharExpression( (const char *)str );
        return static_cast<cplusql_id_t>(ln);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_binary_exp( cplusql_id_t lhs, int opr, cplusql_id_t rhs )
{
    try {
        ExpressionPtr left((Expression *)lhs);
        ExpressionPtr right((Expression *)rhs);
        Expression *gb=NULL;
        if( !rhs && ( opr == LN || opr == LOG2 || opr == LOG10 ))
        {
            ExpressionPtr errmsg( new CharExpression(
                "math operand out of range"));
            right = new AbortExpression( errmsg );
        }
        switch(opr)
        {
            case AND: gb = new LogicalAndExpression(left,right); break ;
            case OR: gb = new LogicalOrExpression(left,right); break ;
            case EQUALS: gb = new EqualsExpression(left,right); break ;
            case NOTEQUAL: {
                ExpressionPtr inner = new EqualsExpression(left,right); 
                gb = new NegationExpression(inner);
                break;
            }
            case LT: gb = new LessThanExpression(left,right); break ;
            case GT: gb = new GreaterThanExpression(left,right); break ;
            case MULTIPLY: gb = new MultiplicationExpression(left,right); break ;
            case DIVIDE: gb = new DivisionExpression(left,right); break ;
            case MODULO: gb = new ModuloExpression(left,right); break ;
            case PLUS: gb = new AdditionExpression(left,right); break ;
            case MINUS: gb = new SubtractionExpression(left,right); break;
            case STR_EQUALS: gb = new StrEqualsExpression(left,right); break;
            case STR_PLUS: gb = new StrPlusExpression(left,right); break;
            case STR_LT: gb = new StrLessThanExpression(left,right); break;
            case STR_GT: gb = new StrGreaterThanExpression(left,right); break;
            case LN: gb = new LnExpression(left,right); break ;
            case LOG2: gb = new Log2Expression(left,right); break ;
            case LOG10: gb = new Log10Expression(left,right); break ;
            case BITAND: gb = new BitwiseAndExpression(left,right); break ;
            case BITOR: gb = new BitwiseOrExpression(left,right); break ;
            case POW: {
                ExpressionPtr zero( new LiteralExpression( "0"));
                gb = new PowExpression(left,right,zero); 
                break ;
            }
            default:
                PANICV("unknown binary operator %i", opr);
        }
        return gb;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_not( cplusql_id_t rhs )
{
    try {
        if(!rhs)
        {
            PANICV("null arg");
        }
        ExpressionPtr right((Expression *)rhs);
        Expression *ne= new NegationExpression(right);
        return ne;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_ftrans_exp( cplusql_id_t operand, int (*ftrans)(int) )
{
    try {
        if(!operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr op(static_cast<Expression *>(operand));
        Expression *te = new TranslateExpression( op, ftrans );
        return te;
    }
    CPLUSQL_CATCHALL()
    return NULL;//not reached 
}


cplusql_id_t 
cplusql_ldformat( char *format, cplusql_id_t operand )
{
    try {
        if(!format || ! operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr op(static_cast<Expression *>(operand));
        char_ptr pformat(format);
        Expression *ldfe = new LongDoubleFormatExpression( format, op);
        return ldfe;
    }
    CPLUSQL_CATCHALL()
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_llformat( char *format, cplusql_id_t operand )
{
    try {
        if(!format || ! operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr op(static_cast<Expression *>(operand));
        char_ptr pformat(format);
        Expression *ldfe = new LongLongFormatExpression( format, op);
        return ldfe;
    }
    CPLUSQL_CATCHALL()
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_ll( cplusql_id_t operand)
{
    try {
        if(! operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr op(static_cast<Expression *>(operand));
        Expression *ldfe = new LongLongFormatExpression( op);
        return ldfe;
    }
    CPLUSQL_CATCHALL()
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_regex_match_exp( 
    char *pattern, cplusql_id_t rhs, int case_insensitive )
{
    try {
        if(!pattern || !rhs )
        {
            PANICV("null arg");
        }
        char_ptr p(pattern);
        ExpressionPtr op(static_cast<Expression *>(rhs));
        Expression *re = new RegexMatchExpression(
            op, pattern, case_insensitive );
        return re;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_dml_exp( 
    char *dbconn,
    char *numrows,
    cplusql_id_t beginsql,
    cplusql_id_t sql,
    cplusql_id_t commitsql
)
{
    try {
        if( ! dbconn || ! numrows || ! beginsql || ! sql || ! commitsql)
        {
            PANICV("null arg");
        }
        char_ptr dbnamep(dbconn);
        char_ptr numrowsp(numrows);
        ExpressionList elbegin(
            reinterpret_cast<expression_list_t *>(beginsql));
        ExpressionList elsql(
            reinterpret_cast<expression_list_t *>(sql));
        ExpressionList elcommit(
            reinterpret_cast<expression_list_t *>(commitsql));
        size_t rows_per_commit = Convert::convertToUnsigned(numrows);
        DBConnectionPtr& conn = 
            CplusqlContext::instance().getDBConnection( dbconn );

        Expression *e = new DMLExpression( 
            conn, elbegin, elsql, elcommit, rows_per_commit );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_sprintf_exp( char *format, cplusql_id_t values )
{
    try {
        if( ! format || ! values )
        {
            PANICV("null arg");
        }
        char_ptr formatp(format);
        ExpressionList elvalues(
            reinterpret_cast<expression_list_t *>(values));
        
        Expression *e = new SprintfExpression( format, elvalues);
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_sqlescape_exp( cplusql_id_t operand )
{
    try {
        if( ! operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr op(static_cast<Expression *>(operand));
        Expression *e = new SQLEscapeExpression( op );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_strstr_exp(
    cplusql_id_t haystack,
	cplusql_id_t needle
)
{
    try{
        if(!haystack || !needle )
        {
            PANICV("null arg");
        }
        ExpressionPtr haystackp(static_cast<Expression *>(haystack));
        ExpressionPtr needlep(static_cast<Expression *>(needle));
        Expression *e = new StrStrExpression( haystackp, needlep );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_substr_exp(
    cplusql_id_t operand,
	cplusql_id_t offset,
	cplusql_id_t length
)
{
    try{
        if(!operand || !offset || !length)
        {
            PANICV("null arg");
        }
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        ExpressionPtr offsetp(static_cast<Expression *>(offset));
        ExpressionPtr lengthp(static_cast<Expression *>(length));
        Expression *e = new SubStrExpression( operandp, offsetp, lengthp);
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_cntmatch_exp(
    cplusql_id_t matchlist,
	cplusql_id_t operand
)
{
    try{
        if(!matchlist || !operand )
        {
            PANICV("null arg");
        }
        ptr< list < char_ptr > > lcp (
            reinterpret_cast< list< char_ptr> * >(matchlist));
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        Expression *e =  new MatchCountExpression( lcp, operandp );
        return e ;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_length_exp( cplusql_id_t operand)
{
    try{
        if(!operand )
        {
            PANICV("null arg");
        }
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        Expression *e = new StrLenExpression( operandp );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_normalize_exp( cplusql_id_t operand,
    cplusql_id_t stop_prefixes, 
    cplusql_id_t stop_words,
    int skip )
{
    try{
        if(!operand || !stop_prefixes || !stop_words )
        {
            PANICV("null arg");
        }
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        ptr< list< char_ptr > > prefixes ( 
            static_cast< list< char_ptr > * >(stop_prefixes));
        ptr< list< char_ptr > > stopwords ( 
            static_cast< list< char_ptr > * >(stop_words));
        Expression *e =
            new NormalizeExpression( operandp, prefixes, stopwords, skip);
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_bytestrip_exp( 
    cplusql_id_t operand, 
    char *squeeze,
    char *skip,
    char *from,
    char *to
)
{
    try{
        if(!operand )
        {
            PANICV("null arg");
        }
        //char_ptr psqueeze( squeeze ? squeeze : ptr_strdup_wnull(""));
        //char_ptr pskip( skip ? skip : ptr_strdup_wnull(""));
        //char_ptr pfrom( from ? from : ptr_strdup_wnull(""));
        //char_ptr pto( to ? to : ptr_strdup_wnull(""));
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        Expression *e = new ByteStripExpression(
            //operandp, psqueeze.get(), pskip.get(), pfrom.get(), pto.get());
            operandp, squeeze, skip, from, to );
        APPLOG_DBG("new ByteStripExpression %lx", reinterpret_cast<size_t>(e) );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t
cplusql_iconv_exp( 
    cplusql_id_t operand, 
    char *to,
    char *from
)
{
    try{
        if(!operand || !to || !from )
        {
            PANICV("null arg");
        }
        ExpressionPtr operandp(static_cast<Expression *>(operand));
        Expression *e = new IconvExpression( operandp, to, from );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

cplusql_id_t 
cplusql_parsed_columns_exp( 
    cplusql_id_t exp_to_parse, 
    int strict,
    char *delim,
    cplusql_id_t column_names 
)
{
    try{
        if(!exp_to_parse || !column_names || !delim )
        {
            PANIC("cplusql_parsed_columns, null arg");
        }
        ExpressionPtr e(static_cast<Expression *>(exp_to_parse));
        ptr< list< char_ptr > > lcp ( static_cast< list< char_ptr > * >(column_names));
        ParsedColumnsPtr dcp(
            new DelimitedColumn( 
                e, 
                delim,
                lcp->size(), 
                strict ? true : false
            )
        );
        cplusql_parsed_helper( lcp, dcp );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t 
cplusql_parsed_columns_regex( 
    cplusql_id_t expression, 
    char *pattern,
    cplusql_id_t config, 
    cplusql_id_t column_names 
)
{
    try{
        if(!expression || !pattern || !config || !column_names )
        {
            PANIC("cplusql_parsed_columns, null arg");
        }
        ConfigPtr cfg( static_cast<Config *>( config ));
        ExpressionPtr e(static_cast<Expression *>(expression));
        ptr< list< char_ptr > > lcp ( static_cast< list< char_ptr > * >(column_names));
        RegexColumns *rc = new RegexColumns( 
                e, 
                pattern,
                lcp->size(),
                cfg->getBool( "ICASE", "0" ),
                cfg->getBool( "POSIX_NEWLINES", "0" )
            );
        ParsedColumnsPtr pcp(rc);
        cplusql_parsed_helper( lcp, pcp );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t 
cplusql_parsed_columns_regexlist( 
    cplusql_id_t expression, 
    cplusql_id_t patterns, 
    cplusql_id_t config, 
    cplusql_id_t column_names 
)
{
    try{
        if(!expression || !patterns || !config || !column_names )
        {
            PANICV("cplusql_parsed_columns, null arg");
        }
        ExpressionPtr e(static_cast<Expression *>(expression));
        ConfigPtr cfg( static_cast<Config *>( config ));
        ptr<list<char_ptr> >lcp(static_cast<list<char_ptr>*>(column_names));
        ptr< list<char_ptr> >p=reinterpret_cast<list<char_ptr>*>(patterns);
        ParsedColumnsPtr dcp(
            new MultiRegexColumns( e, p, lcp->size(),
                cfg->getBool( "ICASE", "0" ),
                cfg->getBool( "POSIX_NEWLINES", "0" )
            )
        );
        cplusql_parsed_helper( lcp, dcp );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t
cplusql_parsed_matchlist(
    cplusql_id_t matchlist,
	cplusql_id_t operand,
    cplusql_id_t column_names
)
{
    try{
        if(!matchlist || !operand || !column_names )
        {
            PANICV("null arg");
        }
        ptr< list < char_ptr > > phraselist (
            reinterpret_cast< list< char_ptr> * >(matchlist));
        ptr<list<char_ptr> >lcp(static_cast<list<char_ptr>*>(column_names));

        ExpressionPtr operandp(static_cast<Expression *>(operand));
        ParsedColumnsPtr dcp(
            new PhraseMatchColumns( phraselist, operandp, " ", "," ));
        cplusql_parsed_helper( lcp, dcp );
    }
    CPLUSQL_CATCHALL();
    return NULL;//not reached 
}

void
cplusql_parsed_helper( 
    ptr< list< char_ptr > > &lcp, 
    ParsedColumnsPtr &dcp
)
{
    CplusqlContext &cplusql = CplusqlContext::instance();
    ListenerPtr lp( dcp );
    cplusql.getCurrentSrcRelation()->registerListener(lp);
    for( unsigned i=0; ! lcp->empty(); ++i )
    {
        ExpressionPtr cep( new ColumnElementExpression( dcp, i ));
        cplusql.pushUserColumn( lcp->front(), cep);
        lcp->pop_front();
    }
}

cplusql_id_t
cplusql_add_expression( cplusql_id_t list, cplusql_id_t exp )
{
    try{
        if( !exp )
        {
            PANICV("cplusql_add_exp_to_list, null arg");
        }
        ExpressionPtr expression(reinterpret_cast<Expression *>(exp));
        expression_list_t * elist = reinterpret_cast<expression_list_t *>(list);
        elist->push_back(expression);
        return reinterpret_cast<cplusql_id_t>(list);
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t
cplusql_new_expression_list( cplusql_id_t exp )
    {
    try{
        if( !exp )
        {
            PANICV("cplusql_add_exp_to_list, null arg");
        }
        expression_list_t * list = new expression_list_t();
        return cplusql_add_expression(reinterpret_cast<cplusql_id_t>(list), exp);
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t
cplusql_join_columns( cplusql_id_t list, char *delim)
{
    try{
        if(!delim )
        {
            PANICV("cplusql_join_columns, null arg");
        }
        ExpressionList elist(reinterpret_cast<expression_list_t *>(list));
        return reinterpret_cast<cplusql_id_t>(dynamic_cast< Expression *> ( new JoinExpression(elist, delim)));
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t 
cplusql_user_column_def( char *name )
{
    try {
        if( ! name )
        {
            PANICV("null name for user column");
        }
        char_ptr pname( name );
		cplusql_id_t exp = cplusql_column_exp_str_dflt_rel( name );
        ExpressionPtr expression((Expression *)exp);
        CplusqlContext::instance().pushUserColumn( pname, expression );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t 
cplusql_user_column( char *name, cplusql_id_t exp )
{
    try {
        if( ! name || !exp )
        {
            PANICV("null name for user column");
        }
        char_ptr pname( name );
        ExpressionPtr expression((Expression *)exp);
        CplusqlContext::instance().pushUserColumn( pname, expression );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t 
cplusql_named_metric( char *name, cplusql_id_t metric )
{
    try {
        if( ! name || !metric )
        {
            PANICV("cplusql_named_metric: null arg");
        }
        char_ptr pname( name );
        AbstractProjectionPtr pmetric(reinterpret_cast< AbstractProjection * >(metric));
        CplusqlContext::instance().pushMetric( pname, pmetric );
        return NULL;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t 
cplusql_hash_exp( cplusql_id_t exp, char * numbuckets )
{
    try {
        if( !exp || !numbuckets )
        {
            PANICV( "cplusql_hash_exp(): null args");
        }
        char_ptr numbucketsp( numbuckets);
        ExpressionPtr hasher((Expression *)exp);
        unsigned buckets = Convert::convertToUnsigned( numbuckets );
        Expression *giveback = new HashExpression( hasher,buckets);
        return giveback;
    }
    CPLUSQL_CATCHALL();
    //not reached 
    return NULL;
}

cplusql_id_t
cplusql_crc48_exp( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
                PANICV( "cplusql_crc48_exp(): null args");
        }
        ExpressionPtr crc48(reinterpret_cast<Expression *>(exp));
        Expression * giveback = new CRC48Expression( crc48 );
        return giveback;
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t
cplusql_md5_exp( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
                PANICV( "cplusql_md5_exp(): null args");
        }
        ExpressionPtr operand(reinterpret_cast<Expression *>(exp));
        Expression * giveback = new MD5Expression( operand );
        return giveback;
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t
cplusql_ceil_exp( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
                PANICV( "cplusql_ceil_exp(): null args");
        }
        ExpressionPtr ceilpt(reinterpret_cast<Expression *>(exp));
        Expression * giveback = new CeilExpression( ceilpt );
        return giveback;
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t
cplusql_floor_exp( cplusql_id_t exp )
{
    try {
        if( !exp )
        {
                PANICV( "cplusql_floor_exp(): null args");
        }
        ExpressionPtr floorpt(reinterpret_cast<Expression *>(exp));
        Expression * giveback = new FloorExpression( floorpt );
        return giveback;
    }
    CPLUSQL_CATCHALL();
    //not reached
    return NULL;
}

cplusql_id_t 
cplusql_ifthen_exp( cplusql_id_t conditional, cplusql_id_t texp, cplusql_id_t fexp )
{
    try {
        if( !conditional || !texp || !fexp )
        {
            PANICV( "cplusql_ifthen_exp(): null args");
        }
        ExpressionPtr maybe((Expression *)conditional);
        ExpressionPtr etrue((Expression *)texp);
        ExpressionPtr efalse((Expression *)fexp);

        Expression * ifthen = new IfThenExpression( maybe, etrue, efalse );
        return ifthen ;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_dest_hash( 
    char * source, char * filetemplate, cplusql_id_t hashexp, 
    cplusql_id_t config
)
{
    try {
        if( !source || !filetemplate || !hashexp ) 
        {
            PANICV( "cplusql_dest_hash: null arg");
        }
        //char_ptr sourcep(source);
        //char_ptr filetemplatep(filetemplate);

        ConfigPtr cfg( static_cast<Config *>( config ));

        DestFactoryPtr pdfact = cplusql_destfactory_helper( cfg );

        // create a pointer to the hash expression, and a listener wrapper
        ExpressionPtr ehash( static_cast< Expression *>( hashexp ));
        ListenerPtr lp_ehash( ehash );

        // register the listener wrapper
        RelationPtr &rp = CplusqlContext::instance().getRelation( source );
        rp->registerListener( lp_ehash );

        ptr< HashHelper> phelp( new HashHelperShim ( ehash ));
        pDest pd ( new DestHashFiles( phelp, pdfact, filetemplate, cfg ));

        cplusql_connect_dest( source, pd, cfg );

        return 0;  // why not null?
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_abort( cplusql_id_t expression )
{
    try{
        if( !expression )
        {
            PANICV("cplusql_abort: null arg");
        }
        ExpressionPtr e( reinterpret_cast<Expression *>( expression ));
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< Expression *> (new AbortExpression( e )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_warn_exp( cplusql_id_t expression )
{
    try{
        if( !expression )
        {
            PANICV("null arg");
        }
        ExpressionPtr e( reinterpret_cast<Expression *>( expression ));
        WarnExpression *we = new WarnExpression( e );
        Expression *ee = we;
        cplusql_id_t giveback = reinterpret_cast< cplusql_id_t>(ee);
        APPLOG_DBG("we=%lx, e=%lx, giveback=%lx", we,ee,giveback);
        return giveback;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_info_exp( cplusql_id_t expression )
{
    try{
        if( !expression )
        {
            PANICV("null arg");
        }
        ExpressionPtr e( reinterpret_cast<Expression *>( expression ));
        LogInfoExpression *we = new LogInfoExpression( e );
        Expression *ee = we;
        cplusql_id_t giveback = reinterpret_cast< cplusql_id_t>(ee);
        APPLOG_DBG("we=%lx, e=%lx, giveback=%lx", we,ee,giveback);
        return giveback;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_line_number( char *source )
{
    try{
        if( !source )
        {
            PANICV("null arg");
        }
        char_ptr sourcep(source);
        RelationPtr &rp = CplusqlContext::instance().getRelation( source );
        return new LineNumberExpression( rp );
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_random( cplusql_id_t min, cplusql_id_t max )
{
    try{
        if( !min || !max )
        {
            PANICV("null arg");
        }
        ExpressionPtr emin( static_cast< Expression *>( min));
        ExpressionPtr emax( static_cast< Expression *>( max));
        return new RandomExpression( emin, emax );
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_dest_file( char * source, char * filename, cplusql_id_t config )
{
    try {
        if( !source || !filename ) 
        { 
            PANICV( "null arg"); 
        }
        char_ptr sourcep(source);
        char_ptr filenamep(filename);

        ConfigPtr cfg( static_cast< Config *>(config));
        CplusqlContext &cplusql = CplusqlContext::instance();



        if( ! cplusql.haveRelation( source ))
        {
            //byte streams should not break writes on newlines.
            (void) cfg->addString( DEST_RECORD_SEP, "", true );
            (void) cfg->getInt( DEST_BUFSIZE, SRC_BUFSIZE_DFLT );//using SRC_ on purpose.

            //const char *dlm = cfg->getString(DEST_RECORD_SEP);
            //APPLOG_DBG("delim=%x", static_cast<int>(*dlm));
        }

        // create a buffered destination
        DestFactoryPtr pdfact = cplusql_destfactory_helper( cfg );
        pDest f =  pdfact->getFileDest( filename );
        pDest pd = DestThreadedBuffer::factory(f, cfg);
        cplusql_connect_dest( source, pd, cfg );
        return NULL;
    }
    CPLUSQL_CATCHALL()
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_dest_new_table( char * source, char * dbconn, cplusql_id_t config )
{
    try {
#if ! FRAME_USES_POSTGRES 
        PANICV("support for postgres table destinations not compiled in");
#else
        if( !source || !dbconn )
        {
            PANICV("null arg");
        }
        char_ptr sourcep(source);
        char_ptr dbconnp(dbconn);
        ConfigPtr cfg( static_cast< Config *>(config));
    
        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &rp = cplusql.getRelation( source );
        DBConnectionPtr& conn = cplusql.getDBConnection( dbconn );
    
        ListenerPtr lp ( 
            new PostgresMakeTable( 
                rp, conn, cfg,
                cfg->findString( "POSTLOAD_SQL" ),
                cfg->findString( "COLUMN_TYPES" ),
                cfg->findString( "TABLE_NAME" )));
        cplusql.addShim( lp );
#endif
        return NULL;
    }
    CPLUSQL_CATCHALL()
    return NULL; //not reached 
}

cplusql_id_t 
cplusql_dest_table( char * source, char * dbconn, cplusql_id_t config )
{
    try {
#if ! FRAME_USES_POSTGRES 
        PANICV("support for postgres table destinations not compiled in");
#else
        if( !source || !dbconn )
        {
            PANICV("null arg");
        }
        char_ptr sourcep(source);
        char_ptr dbconnp(dbconn);
        ConfigPtr cfg( static_cast< Config *>(config));
    
        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &rp = cplusql.getRelation( source );
        DBConnectionPtr& conn = cplusql.getDBConnection( dbconn );
    
        ListenerPtr lp ( 
            new PgCopyListener( 
                rp, conn, cfg,
                cfg->getString( "TABLE_NAME" ),
                cfg->getString( "DELIM", "|" ),
                cfg->findString( "PRELOAD_SQL" ),
                cfg->findString( "POSTLOAD_SQL" )
            )
        );
        cplusql.addShim( lp );
#endif
        return NULL;
    }
    CPLUSQL_CATCHALL()
    return NULL; //not reached 
}

char *
cplusql_src_ads_map ( char * name )
{
    CplusqlContext::instance().setCurrentSrcADSMap( name );
    return name;
}

char *
cplusql_src_relation ( char * name )
{
    CplusqlContext::instance().setCurrentSrcRelation( name );
    return name;
}

cplusql_id_t
cplusql_hashed_table(
    char * name,
    char * filetemplate,
    char * querytemplate,
    char * buckets,
	cplusql_id_t columns,
    cplusql_id_t config
)
{
    try {
        if(!name || !filetemplate || !querytemplate || !config)
        {
            PANICV("cplusql_hashed_table(): null arg");
        }
        char_ptr namep(name);
        char_ptr filetemplatep(filetemplate);
        char_ptr querytemplatep(querytemplate);
        ConfigPtr cfg( static_cast<Config *>(config ));

        ptr< list < char_ptr > > cols  ( static_cast< list< char_ptr> * > (columns));
        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name );
        list< char_ptr >::iterator i;
        list< char_ptr >::iterator e=cols->end();
        for( i=cols->begin(); i!=e; ++i )
        {
            meta->addColumn( (*i).get() );
        }

        unsigned num_buckets = Convert::convertToUnsigned( buckets );
        ConnectionFactoryPtr conn_fact(new DBConnectionFactory());
        HashConnectionsPtr conns( new HashConnections(conn_fact));
        for(unsigned i=0; i < num_buckets; ++i)
        {
            char file[FILENAME_MAX];
            memset(file, '\0', FILENAME_MAX);
            char_ptr bucket = ptr_strdup_long(i);
            snprintf(file, FILENAME_MAX, filetemplate, bucket.get());
            AuthPtr auth(new Auth(cfg));
            auth->slurp(file,true);
            conns->addAuth(auth, bucket.get());
        }
        ADSMapFactoryPtr fact(new DBMapFactory(meta, querytemplate));
        AbstractDelimitedSourceMapPtr dsm(
            new HashedDSMap (meta, conns, fact, num_buckets)
        );
        CplusqlContext::instance().addADSMap(namep.get(), dsm);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
};

cplusql_id_t
cplusql_db_conn(
    char *name,
    char *user,
    char *pass,
    char *url,
    cplusql_id_t config
)
{
    try {
        if( !name || !user || !pass || !url || !config)
        {
            PANICV("cplusql_db_conn(): null arg");
        }
        ConfigPtr cfg( static_cast<Config*>(config) );
        AuthPtr auth( new Auth( cfg ) );
        auth->setUser(user);
        auth->setPassword(pass);
        auth->setUrl(url);

        // DBConnectionFactory::POSTGRES_CONNECTION = 1;
        // DBConnectionFactory::ORACLE_CONNECTION = 0;
        int conntype = cfg->getInt( "DB_TYPE", "1" );

        DBConnectionPtr conn(
            DBConnectionFactory::createConnection( 
                user, pass, url, conntype ));
        CplusqlContext::instance().addDBConnection( name, conn );
    }
    CPLUSQL_CATCHALL()
    return NULL; //not reached 
}

cplusql_id_t
cplusql_sql(
    char * dbconnection,
    char * sql,
    cplusql_id_t config
)
{
    try {
        if(!dbconnection || !sql || !*dbconnection || !*sql )
        {
            PANICV("null arg to cplusql_sql, dbconnection=%s, sql=%s",
                dbconnection, sql );
        }
        CplusqlContext &cplusql = CplusqlContext::instance();
        DBConnectionPtr& conn = cplusql.getDBConnection( dbconnection );
        Runnable *r = new SQLRunnable( conn, sql );
        return reinterpret_cast< cplusql_id_t >( r );
    }
    CPLUSQL_CATCHALL()
    return NULL; //not reached 
}

cplusql_id_t
cplusql_run_sql(
    char * dbconnection,
    char * sql,
    cplusql_id_t config
)
{
    try {
        cplusql_id_t sqlr = cplusql_sql( dbconnection, sql, config );
        if(!sqlr )
        {
            PANICV("cplusql_sql returned null to cplusql_run_sql");
        }
        ConfigPtr cfg( static_cast<Config*>(config) );
        RunnablePtr r = reinterpret_cast< Runnable * >( sqlr );
        try{
            r->run();
        }catch( exception &e )
        {
            if( cfg->getBool( "IGNORE_FAILURE", "0" ))
            {
                APPLOG_INFO( "ignoring sql failure, sql[%s] error[%s]", 
                    sql, e.what());
            }
            else
            {
                throw;
            }
        }
    }
    CPLUSQL_CATCHALL()
    return NULL; 
}


cplusql_id_t
cplusql_add_sql(
    cplusql_id_t sqlr
)
{
    try {
        if(!sqlr )
        {
            PANICV("null arg to cplusql_run_sql");
        }
        RunnablePtr rp = reinterpret_cast< Runnable * >( sqlr );
        CplusqlContext::instance().addRunnable( rp );
    }
    CPLUSQL_CATCHALL()
    return NULL; 
}


cplusql_id_t
cplusql_relation_db(
    char *name,
    char *conn_name,
    char *query,
    cplusql_id_t columns,
    cplusql_id_t config
)
{
    try {
        if( !name || !conn_name || !query ||!config)
        {
            PANICV("cplusql_relation_db(): null arg");
        }
        char_ptr namep(name );
        ptr< list < char_ptr > > cols  ( static_cast< list< char_ptr> * > (columns));
        CplusqlContext &cplusql = CplusqlContext::instance();
        DBConnectionPtr& conn(cplusql.getDBConnection( conn_name ));
        ConfigPtr cfg( static_cast< Config * > (config));

        AbstractDelimitedSourceNextPtr dsp = new DBDelimitedSource(
            conn,
            query
        );
        DelimitedSourcePtr dsp_dsp ( dsp );

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name );
        list< char_ptr >::iterator i;
        list< char_ptr >::iterator e=cols->end();
        for( i=cols->begin(); i!=e; ++i )
        {
            meta->addColumn( (*i).get() );
        }
        ExpressionList empty( new expression_list_t());
        //extra () prevents bogus parse of sr as function.
        RelationPtr sr((
            new SimpleRelation( dsp_dsp , meta, empty, empty, false 
            )
        ));
        ListenerPtr lp( sr );
        NextNotifyJoint *jjn = new NextNotifyJoint( dsp, namep,
            cfg->getInt(CPLUSQL_SKIP, CPLUSQL_SKIP_DFLT ),
            cfg->getInt(CPLUSQL_LIMIT, CPLUSQL_LIMIT_DFLT )
            );
        RunnablePtr ar= jjn ;
        jjn->registerListener( lp );

        CplusqlContext::instance().addRelation( name, sr );
        CplusqlContext::instance().addRunnable( ar );
    }
    CPLUSQL_CATCHALL()
    return NULL; 
}

ByteSourcePtr
cplusql_bytesource_helper( ptr<list<char_ptr> > &files, ConfigPtr &cfg)
{
    const char *ns = cfg->getString( CPLUSQL_NAMESPACE, NSS_USE_FILE_SYSTEM);
    if ( strlen(ns) == 0 || !strcmp(ns, NSS_USE_FILE_SYSTEM) )
    {
        return MultiByteSource::makeFromGlobs( *files );
    }
    if( files->size() > 1 )
    {
        PANICV("list of patterns not implemented yet for clusterio streams");
    }
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
    const char *prefix = files->begin()->get();
    const char *pattern = cfg->getString( "PATTERN", "." );

    clusterio::NameSpaceService_ptr nss = 
        cplusql_namespace_helper( ns, cfg );

    MultiByteSource *mbs = new MultiByteSource() ;
    ByteSourcePtr srcp (mbs);

    clusterio::open_info_seq* open_infos =
        nss->find_open_seq( prefix, pattern,
                      cfg->getInt( NSS_REPLICA_COUNT, "2" ),
                      clusterio::o_rdonly, 
                      SharedServiceMain::instance()->myHostname());

    // next traverse the open_infos, create a ClusterByteSource
    // from each and add it to the MultiByteSource
    int length = open_infos->length();
    for( int i=0; i<length; i++ )
    {
        clusterio::open_info openInfo = (*open_infos)[i];
        ByteSourcePtr bsp( new ClusterByteSource( nss, openInfo ));
        mbs->add( bsp );
    }
    return srcp ;
#endif
#endif
    PANICV("to use clusterio you must compile with --enable-distributed");
    return MultiByteSource::makeFromGlobs( *files );
}

void
cplusql_byte_source_install( 
    ByteSourcePtr &bsp,
    const char *name,
    ConfigPtr &cfg
)
{
    CopyByteSourcePtr cbsp( 
        new ThreadedByteSource( bsp, 
            cfg->getInt( SRC_BUFFERS, SRC_BUFFERS_DFLT ),
            cfg->getInt( SRC_BUFSIZE, SRC_BUFSIZE_DFLT )));

    ptr< DestNotifier > pdestn ( new DestNotifier());

    pDest pd ( pdestn );
    RunnablePtr rp ( new BytePusher( cbsp, pd ));

    CplusqlContext::instance().addRunnable( rp );
    CplusqlContext::instance().addDestNotifier( name, pdestn );
}

cplusql_id_t
cplusql_byte_source( 
    char *name,
    cplusql_id_t filenames,
    cplusql_id_t config
)
{
    try{
        if( !name || !filenames || ! config )
        {
            PANICV("null arg");
        }
        char_ptr namep( name );
        ptr<list<char_ptr> >files ( 
            static_cast<list<char_ptr>*>(filenames));
        ConfigPtr cfg( static_cast< Config * > (config));

        ByteSourcePtr bsp = cplusql_bytesource_helper( files, cfg );
        cplusql_byte_source_install( bsp, name, cfg );
    }
    CPLUSQL_CATCHALL();
    return NULL; 
}


cplusql_id_t
cplusql_relation_fileset(
    char *name,
    cplusql_id_t filenames,
    char *delimiter,
    cplusql_id_t columns,
    cplusql_id_t config
)
{
    try {
        if( !name || !filenames || !columns )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        ptr<list<char_ptr> >files ( 
            static_cast<list<char_ptr>*>(filenames));
        ptr<list<char_ptr> > cols  
            ( static_cast<list<char_ptr>*>(columns));
        ConfigPtr cfg( static_cast< Config * > (config));

        ByteSourcePtr srcp = cplusql_bytesource_helper( files, cfg );
        cplusql_relation_helper( namep, delimiter, cols, srcp, cfg);
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_relation_pgcopy(
    char * name,
    char * dbconn,
    char * table,
    cplusql_id_t columns,
    cplusql_id_t config
)
{
#if ! FRAME_USES_POSTGRES 
    PANICV("null arg");
#else
    try {
        if( !name || !dbconn || !table || !columns || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        char_ptr dbconnp(dbconn );
        char_ptr tablep(table );
        ptr<list<char_ptr> > cols  
            ( static_cast<list<char_ptr>*>(columns));
        ConfigPtr cfg( static_cast< Config * > (config));

        CplusqlContext &cplusql = CplusqlContext::instance();
        DBConnectionPtr conn(cplusql.getDBConnection( dbconn ));
        ByteSourcePtr srcp ( new PgCopyByteSource( conn, table ));

        cplusql_relation_helper( namep, "|", cols, srcp, cfg);
    }
    CPLUSQL_CATCHALL();
#endif
    return NULL;
}

cplusql_id_t
cplusql_relation_pgcopy_short(
    char * name,
    char * dbconn,
    char * table,
    cplusql_id_t config
)
{
#if ! FRAME_USES_POSTGRES 
    PANICV("null arg");
#else
    try {
        if( !name || !dbconn || !table || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        char_ptr dbconnp(dbconn );
        char_ptr tablep(table );
        ConfigPtr cfg( static_cast< Config * > (config));
        CplusqlContext &cplusql = CplusqlContext::instance();
        DBConnectionPtr conn(cplusql.getDBConnection( dbconn ));

        char_ptr q = ptr_strdup(
            "select attname from pg_attribute where attrelid = ( select oid from pg_class where relname = '" );
        q = q + table ;
        q = q + "' ) and attnum > 0 and attisdropped = false order by attnum";

        ptr<list<char_ptr> > cols = QueryList::makeList(conn, q.get());

        ByteSourcePtr srcp ( new PgCopyByteSource( conn, table ));
        cplusql_relation_helper( namep, "|", cols, srcp, cfg);
    }
    CPLUSQL_CATCHALL();
#endif
    return NULL;
}

cplusql_id_t
cplusql_event_stream_nanosleep(
    char * name,
    char * secs,
    char * nsecs,
    cplusql_id_t config
)
{
    try {
        if( !name || !secs || !nsecs || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        char_ptr secsp(secs );
        char_ptr nsecsp(nsecs );
        ConfigPtr cfg( static_cast< Config * > (config));

        time_t seconds = Convert::convertToLong( secs );
        long nanosecs = Convert::convertToLong( nsecs );
        AbstractDelimitedSourceNextPtr dsp =
            new DelayStubDelimitedSource( seconds, nanosecs );

        cplusql_event_helper( namep, dsp, cfg );
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_nanosleep_exp(
    char * secs,
    char * nsecs
)
{
    try {
        if( !secs || !nsecs )
        {
            PANICV("null arg");
        }
        char_ptr secsp(secs );
        char_ptr nsecsp(nsecs );

        time_t seconds = Convert::convertToLong( secs );
        long nanosecs = Convert::convertToLong( nsecs );
        Expression *e = 
            new NanosleepExpression( seconds, nanosecs );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_pglisten_exp(
                char * db_name,
				char * event_name
)
{
#if ! FRAME_USES_POSTGRES 
        PANICV("support for postgres table destinations not compiled in");
#else
    try {
        if( !db_name || !event_name )
        {
            PANICV("null arg");
        }
        char_ptr db_namep(db_name );
        char_ptr event_namep(event_name );
        Expression *e = new PgListenExpression( 
            CplusqlContext::instance().getDBConnection( db_name ),
            event_name 
        );
        return e;
    }
    CPLUSQL_CATCHALL();
#endif
    return NULL;
}

cplusql_id_t
cplusql_sqlvalue_exp(
                char * db_name,
				cplusql_id_t sql
)
{
    try {
        if( !db_name || !sql )
        {
            PANICV("null arg");
        }
        char_ptr db_namep(db_name );
        ExpressionPtr sqlp(reinterpret_cast<Expression *>(sql));
        Expression *e = new SQLValueExpression( 
            CplusqlContext::instance().getDBConnection( db_name ), sqlp, 0 );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_coalesce_exp( 
				cplusql_id_t expargs
)
{
    try {
        if( !expargs )
        {
            PANICV("null arg");
        }
        ExpressionList pexpargs(
            reinterpret_cast<expression_list_t *>(expargs));
        Expression *e = new CoalesceExpression( pexpargs );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_strgreatest_exp( 
				cplusql_id_t expargs
)
{
    try {
        if( !expargs )
        {
            PANICV("null arg");
        }
        ExpressionList pexpargs(
            reinterpret_cast<expression_list_t *>(expargs));
        Expression *e = new StrGreatestExpression( pexpargs );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_strleast_exp( 
				cplusql_id_t expargs
)
{
    try {
        if( !expargs )
        {
            PANICV("null arg");
        }
        ExpressionList pexpargs(
            reinterpret_cast<expression_list_t *>(expargs));
        Expression *e = new StrLeastExpression( pexpargs );
        return e;
    }
    CPLUSQL_CATCHALL();
    return NULL;
}



cplusql_id_t
cplusql_relation_exp(
                char * name,
				cplusql_id_t usercolumnset,
				cplusql_id_t whiletrue,
				cplusql_id_t config
)
{
    try {
        if( !name || !whiletrue || !config)
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        ExpressionPtr whilst(reinterpret_cast<Expression *>(whiletrue));
        ConfigPtr cfg( static_cast< Config * > (config));

        CplusqlContext &cplusql = CplusqlContext::instance();
        ptr< char_ptr_list > usercolnames (cplusql.getUserColumnNames());
        ExpressionList user_columns(cplusql.getUserColumns());

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name);
        meta->addColumns( *usercolnames.get());

        DelimitedSourcePtr dsp
            = new ExpressionRelation( user_columns, meta, whilst);

        RelationPtr rp( dsp );
        cplusql.addRelation( name, rp );

        AbstractDelimitedSourceNextPtr dsnp(dsp);
        ListenerPtr lp(dsp);
        NextNotifyJoint *jjn = new NextNotifyJoint( 
            dsnp, namep,
            cfg->getInt(CPLUSQL_SKIP, CPLUSQL_SKIP_DFLT ),
            cfg->getInt(CPLUSQL_LIMIT, CPLUSQL_LIMIT_DFLT )
            );

        RunnablePtr ar = jjn ;
        jjn->registerListener( lp );
        CplusqlContext::instance().addRunnable( ar );

    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_relation_xmlquery(
                char * name,
				char * event_source,
				char * xpath_query,
				cplusql_id_t columns,
				cplusql_id_t xml,
				cplusql_id_t config
)
{
    try {
        if( !name || !event_source || !xpath_query || 
                !columns || !xml || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        char_ptr event_sourcep(event_source );
        char_ptr xpath_queryp(xpath_query );
        ptr<list<char_ptr> > cols  
            ( static_cast<list<char_ptr>*>(columns));
        ExpressionPtr xmlp(reinterpret_cast<Expression *>(xml));
        ConfigPtr cfg( static_cast< Config * > (config));

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name);
        meta->addColumns( *cols.get());

        RelationPtr rp = new XpathRelation( xmlp, meta, xpath_query );
        ListenerPtr lp( rp );
        CplusqlContext &cplusql = CplusqlContext::instance() ;
        cplusql.addRelation( name, rp );
        (cplusql.getRelation( event_source ))->registerListener( lp );
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_relation_dbrepeat( 
                char * name,
				char * event_source,
				char * db_name,
				cplusql_id_t query_exp,
				cplusql_id_t columns
){
    try {
        if( !name || !event_source || !db_name || 
                !query_exp || !columns )
        {
            PANICV("null arg");
        }
        char_ptr namep(name );
        char_ptr event_sourcep(event_source );
        char_ptr db_namep(db_name );

        ExpressionPtr queryp(reinterpret_cast<Expression *>(query_exp));
        ptr<list<char_ptr> > cols  
            ( static_cast<list<char_ptr>*>(columns));

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name);
        meta->addColumns( *cols.get());

        CplusqlContext &cplusql = CplusqlContext::instance() ;
        DBConnectionPtr conn(cplusql.getDBConnection( db_name ));

        RelationPtr rp = new RepeatDBRelation( conn, queryp, meta );
        ListenerPtr lp( rp );
        cplusql.addRelation( name, rp );
        (cplusql.getRelation( event_source ))->registerListener( lp );
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

void
cplusql_relation_helper(
    char_ptr &name,
    const char *delimiter,
    ptr< list < char_ptr > > &cols,
    ByteSourcePtr &src,
    ConfigPtr &cfg)
{
    const char *delim; 
    //
    // we allow "" to fall through the if/else below, rather assigning default
    // if !(*delimiter).  This is because setting "" as a delimiter to
    // NoCopyDelimitedSource is meaningful; it allows for one column streams to
    // contain any character.
    //
    if( ! delimiter )
    {
        delim= cfg->getString(CPLUSQL_DELIMITER, CPLUSQL_DELIMITER_DFLT );
    }
    else
    {
        delim=delimiter;
    }
    unsigned int read_buf_size=cfg->getInt(
        CPLUSQL_READ_BUFSIZE,CPLUSQL_READ_BUFSIZE_DFLT);
    unsigned int max_badparse_rows = cfg->getInt(
        CPLUSQL_MAX_BAD_PARSE,CPLUSQL_MAX_BAD_PARSE_DFLT );


    AbstractDelimitedSourceNextPtr dsp = new NoCopyDelimitedSource(
        src,
        read_buf_size,
        cols->size(),
        *delim,
        max_badparse_rows
    );
    DelimitedSourcePtr dsp_dsp ( dsp );

    RelationMetaPtr meta(new RelationMeta());
    meta->setRelationName( name.get());
    meta->addColumns( *cols.get());
    ExpressionList empty( new expression_list_t());
    //extra () prevents bogus parse of sr as function.
    RelationPtr sr((
        new SimpleRelation( dsp_dsp , meta, empty, empty, false 
        )
    ));

    ListenerPtr lp( sr );
    NextNotifyJoint *jjn = new NextNotifyJoint( dsp, name,
            cfg->getInt(CPLUSQL_SKIP, CPLUSQL_SKIP_DFLT ),
            cfg->getInt(CPLUSQL_LIMIT, CPLUSQL_LIMIT_DFLT )
            );
    RunnablePtr ar = jjn ;
    jjn->registerListener( lp);

    CplusqlContext::instance().addRelation( name.get(), sr );
    CplusqlContext::instance().addRunnable( ar );

}

void
cplusql_event_helper( char_ptr &name, AbstractDelimitedSourceNextPtr &dsp,
    ConfigPtr &cfg )
{
    DelimitedSourcePtr dsp_dsp ( dsp );

    RelationMetaPtr meta(new RelationMeta());
    meta->setRelationName( name.get());
    ExpressionList empty( new expression_list_t());
    //extra () prevents bogus parse of sr as function.
    RelationPtr sr((
        new SimpleRelation( dsp_dsp , meta, empty, empty, false )));

    ListenerPtr lp( sr );
    NextNotifyJoint *jjn = new NextNotifyJoint( dsp, name,
            cfg->getInt(CPLUSQL_SKIP, CPLUSQL_SKIP_DFLT ),
            cfg->getInt(CPLUSQL_LIMIT, CPLUSQL_LIMIT_DFLT )
            );
    RunnablePtr ar = jjn ;
    jjn->registerListener( lp);

    CplusqlContext::instance().addRelation( name.get(), sr );
    CplusqlContext::instance().addRunnable( ar );
}


cplusql_id_t
cplusql_relation_pattern(
    char *name,
    char *prefix,
    char *pattern,
    char *delimiter,
    cplusql_id_t columns,
    cplusql_id_t config
)
{
    try {
        if( !name || !prefix || !pattern || !columns )
        {
            PANICV("cplusql_relation_pattern(): null arg");
        }
        char_ptr namep(name );
        char_ptr previxp(prefix );
        char_ptr patternp(pattern );
        ptr< list < char_ptr > > cols ( 
            static_cast< list< char_ptr> * > (columns));
        ConfigPtr cfg( static_cast< Config * > (config));

        const char *nameSpace = 
            cfg->getString( CPLUSQL_NAMESPACE, NSS_USE_FILE_SYSTEM);

        ByteSourcePtr srcp;
        if ( strlen(nameSpace) == 0 || !strcmp(nameSpace, NSS_USE_FILE_SYSTEM) )
        {
            PANICV("not implemented, yet");
        }
        else
        {
#define DOPANIC 1
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#undef DOPANIC
            clusterio::NameSpaceService_ptr nss = 
                cplusql_namespace_helper( nameSpace, cfg );

            srcp = new MultiByteSource() ;

            clusterio::open_info_seq* open_infos =
                nss->find_open_seq(prefix,
                                   pattern, 
                                   cfg->getInt( NSS_REPLICA_COUNT, "2" ),
                                   clusterio::o_rdonly, 
                                   SharedServiceMain::instance()->myHostname());

            // next traverse the open_infos, create a ClusterByteSource
            // from each and add it to the MultiByteSource
            int length = open_infos->length();
            for( int i=0; i<length; i++ )
            {
                clusterio::open_info openInfo = (*open_infos)[i];
                ByteSourcePtr bsp( new ClusterByteSource( nss, openInfo ));
                ((MultiByteSource*)srcp.get())->add( bsp );
            }
#endif
#endif
#ifdef DOPANIC
            PANICV("In order to use cplusqlio you must compile with --enable-distributed");
#endif
        }
        cplusql_relation_helper( namep, delimiter, cols, srcp, cfg);
        return 0 ;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_command(
    char *name,
    char *command,
    cplusql_id_t config
)
{
    try {
        if( !name || !command || !config )
        {
            PANICV("cplusql_command(): null arg");
        }
        char_ptr namep( name );
        char_ptr commandp( command );
        ConfigPtr cfg( static_cast< Config * > (config));

        char_ptr input_file;
        char_ptr output_file;
        char_ptr error_file;

        if( cfg->findString( "INPUT_FILE"))
        {
            input_file=ptr_strdup(cfg->getString( "INPUT_FILE"));
        }
        if( cfg->findString( "OUTPUT_FILE" ))
        {
            output_file=ptr_strdup(cfg->getString( "OUTPUT_FILE"));
        }
        else
        {
            output_file = namep + ".out";
        }
        if( cfg->findString( "ERROR_FILE" ))
        {
            error_file=ptr_strdup(cfg->getString( "ERROR_FILE"));
        }
        else
        {
            error_file = namep + ".err";
        }
        if( get_yacdbg())
        {
            fprintf(stderr, "cplusql_command: %s in=%s, out=%s, err=%s\n", 
                commandp.get(), 
                input_file.get(),
                output_file.get(),
                error_file.get()
                );
        }


        AsyncRunnablePtr ar =new ForkExecRunnable( 
                commandp.get(), 
                input_file.get(),
                output_file.get(),
                error_file.get()
                );

        CplusqlContext::instance().addRunnable( ar );

        return 0 ;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_jobq(char *name, cplusql_id_t config)
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if ((name == (char *) 0) || (config == (char *) 0))
        {
            PANICV("cplusql_jobq(): null arg");
        }

        char_ptr namep (name);
        ConfigPtr cfg(static_cast< Config * > (config));
        
        // see if there's an ior file accessible
        const char *handle = cfg->findString(JQS_IOR_URI);
        if (handle == (const char *) 0)
        {
            // else use url file which must be there
            handle = cfg->getString(JOBQ_URL);
        }

        JobQueue::JobQueueService_ptr jobQ = NULL;
        while (jobQ == NULL)
        {
            CORBA::Object_ptr obj = NULL;
            try
            {
                APPLOG_DBG("%s: Wanting to resolve %s", name, handle);
                obj = SharedServiceMain::instance()->resolve(handle);
            }
            catch (CORBA::Exception &ce) {
                APPLOG_INFO("cplusql_jobq(): caught CORBA::Exception [%s][%s]",
                             ce._rep_id(), ce._name() );
                obj = NULL;
            }

            if (obj == (CORBA::Object_ptr) 0)
            {
                if (strcmp(handle, cfg->getString(JOBQ_URL)))
                {
                    // the first attempt wasn't with url so next try that
                    APPLOG_INFO("Could not create object from string[%s]. Next trying with URL", 
                                handle);
                    handle = cfg->getString(JOBQ_URL);
                }
                else
                {
                    PANICV("Could not create object from string[%s]",
                           handle);
                }
            }
            else
            {
                APPLOG_DBG("doing narrow()...");
                jobQ = JobQueue::JobQueueService::_narrow( obj );
                if (jobQ == NULL)
                {
                    if (strcmp(handle, cfg->getString(JOBQ_URL)))
                    {
                        APPLOG_INFO("Unable to narrow to JobQueueService. Next trying with URL");
                        handle = cfg->getString(JOBQ_URL);
                    }
                    else
                    {
                        PANICV("Unable to narrow to JobQueueService.");
                    }
                }
            }
        }
        JobQRunnablePtr jq = new JobQRunnable( jobQ );
        CplusqlContext::instance().addJobQ(name, jq);
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use job queueing you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0;
}


cplusql_id_t
cplusql_job(char *jobname, char *jobqname, char *command, cplusql_id_t config)
{
    APPLOG_DBG("creating job %s: %s", jobname, command);

    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if (    (jobname == (char *) 0) || 
                (jobqname == (char *) 0) || 
                (command == (char *) 0) || 
                (config == (cplusql_id_t) 0))
        {
            PANICV("cplusql_job(): null arg");
        }

        char_ptr namep ( jobname );
        char_ptr qp ( jobqname );
        char_ptr cmdp ( command );
        ConfigPtr cfg( static_cast< Config * > (config));

        (CplusqlContext::instance().getJobQ(jobqname))->addJob(jobname,command,cfg);
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use job queueing you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_ping(char *name )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        ConfigPtr &cfg = CplusqlContext::instance().getDefaultConfig();
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->ping();
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_reconfigure(char *name, cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        ConfigPtr cfg(static_cast< Config * > (config));
        Service::NVPairList *nvp = NVPairUtil::convert( *cfg.get() );
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->reconfigure(*nvp);
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_touch(
    char *name, 
    char *newfile, 
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !newfile )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr newfilep (newfile);
        ConfigPtr cfg(static_cast< Config * > (config));
        short copies = cfg->getInt( "COPIES", "0" );
        const char *preferred_host = cfg->getString( "PREFERRED_HOST", "" );
        bool exclusive = cfg->getBool( "EXCLUSIVE", "0" );
        int flags = clusterio::o_wronly + clusterio::o_creat +
            ( exclusive ? clusterio::o_excl : 0 );
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        clusterio::open_info *oi = 
            nss->open( newfile, copies, flags, preferred_host );

        char_ptr nodenames = ptr_strdup( "{" );
        unsigned int e= oi->servants.length(); 
        for( unsigned int i= 0; i< e; ++i )
        {
            nodenames = nodenames + oi->servants[i].hostname.in() ;
            nodenames = nodenames + ",";
        }
        if( e )
        {
            *(nodenames.get()+strlen(nodenames.get())-1) = '}';
        }else
        {
            nodenames = nodenames + "}";
        }
        APPLOG_DBG( "touched %s id=%llu dbg=%s flags=%u nodes=%s",
            name, oi->handle.id, oi->handle.debug_info.in(), oi->handle.flags, 
            nodenames.get());
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_mv(
    char *name, 
    char *from, 
    char *to, 
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !from || !to )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr fromp (from);
        char_ptr top (to);
        ConfigPtr cfg(static_cast< Config * > (config));
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->mv( from, to );
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_mvclobber(
    char *name, 
    char *from, 
    char *to, 
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !from || !to )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr fromp (from);
        char_ptr top (to);
        ConfigPtr cfg(static_cast< Config * > (config));
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->mvclobber( from, to );
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_rm(
    char *name, 
    char *file, 
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !file )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr filep (file);
        ConfigPtr cfg(static_cast< Config * > (config));
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->rm( file );
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_findrm(
    char *name,
    char *path,
    char *pattern,
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !path || !pattern )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr pathp (path);
        char_ptr patternp (pattern);
        ConfigPtr cfg(static_cast< Config * > (config));
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        nss->find_rm( path, pattern );
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_namespace_find(
    char *name,
    char *path,
    char *pattern,
    cplusql_id_t config )
{
    try {
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        if( !name || !config || !path || !pattern )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr pathp (path);
        char_ptr patternp (pattern);
        ConfigPtr cfg(static_cast< Config * > (config));
        clusterio::NameSpaceService_ptr nss = 
            cplusql_namespace_helper( name, cfg );
        ::clusterio::FileNameStatusList *names=nss->find(path,pattern);
        if(!names)
        {
            PANICV("nss->find() returned NULL");
        }

        time_t last_time_name_changed =0;
        time_t time_name_created =0; 

        StringArrayPtr lstext = new StringArray();

        for(size_t i=0; i< names->length(); ++i )
        {
            time_name_created = (*names)[i].time_name_created;
            last_time_name_changed = (*names)[i].last_time_name_changed;

            char_ptr created = twrap::getctime_ptr(&time_name_created);
            char_ptr changed =twrap::getctime_ptr(&last_time_name_changed);

            charvec hostnames ;
            size_t e = (*names)[i].servants.length();
            for( size_t j=0; j < e ; ++j )
            {
                hostnames.push_back((*names)[i].servants[j].hostname.in());
            }
            char_ptr commahosts = ptr_strdup_list( hostnames, ",", false);
            char_ptr numhosts = ptr_strdup_long( e );

            //fprintf(stdout, "%s|%s|%s|%lu|%s\n",
            //    (*names)[i].filename.in(),
            //    created.get(), changed.get(), e, commahosts.get());

            lstext->copy( (*names)[i].filename.in());
            lstext->take( created.release());
            lstext->take( changed.release());
            lstext->take( numhosts.release()); 
            lstext->take( commahosts.release());

        }
        if( 0 == names->length() )
        {
            if( cfg->getBool( "ABORT_IF_FILE_NOT_FOUND", "1" ))
            {
                PANICV( "no files found: path=%s pattern=%s\n", 
                    path, pattern );
            }
        }
        RelationMetaPtr rm=new RelationMeta();
        rm->setRelationName(__func__);
        rm->addColumn("filename");
        rm->addColumn("created");
        rm->addColumn("name changed");
        rm->addColumn("numhosts");
        rm->addColumn("hosts");

        //we have to create child class ptr to create base1 and base2 ptr
        //since we cant create ptr< base1 > from ptr< base2 > and vica 
        //versa.
        ptr< StringArrayRelation > sa= new StringArrayRelation( rm, lstext );
        RelationPtr rlp ( sa );
        RunnablePtr rnp ( sa );
        CplusqlContext::instance().addRelation( __func__, rlp );
        CplusqlContext::instance().addRunnable( rnp );
        APPLOG_DBG(".");
        if( cfg->getBool( "FIND_IS_STDOUT", "1" ))
        {
            //default header to true.
            APPLOG_DBG(".");
            cfg->getBool( "FIRST_ROW_HEADER", "1" );
            pDest pd = new DestFile("-");
            cplusql_connect_dest( __func__, pd, cfg );
        }
        return (cplusql_id_t) 0;
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL()
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_persist( char *to, char *from )
{
    try{
        if (! to || ! from )
        {
            PANICV("null arg");
        }
        char_ptr to_p(to);
        char_ptr from_p(from);
        APPLOG_DBG( "to=%s from=%s", to, from );

        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &rp = cplusql.getRelation( from );

        StringArrayPtr sa = new StringArray();
        ListenerPtr lp = new StringArrayListener( sa, rp );
        cplusql.addShim( lp );

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( to);
        for( size_t i= 0; i < rp->getNumColumns(); ++i )
        {
            meta->addColumn( rp->getColumnName( i ));
        }
        cplusql.addArray( to, sa, meta );
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0;
}

cplusql_id_t 
cplusql_relation_indexscan( 
    char *name, char *from, char *index, cplusql_id_t config
)
{
    try{
        if(!name || !from || !index || !config )
        {
            PANICV("null arg");
        }
        char_ptr name_p(name);
        char_ptr from_p(from);
        char_ptr index_p(index);
        APPLOG_DBG( "name=%s from=%s index=%s", name, from, index );

        CplusqlContext &cplusql = CplusqlContext::instance();

        pair_array_meta_t &p = cplusql.getArray( from );
        RelationMetaPtr meta( new RelationMeta());
        meta->setRelationName( name );
        for( size_t i = 0; i < p.second->getNumColumns(); ++i )
        {
            meta->addColumn( p.second->getColumnName( i ));
        }
        IndexPtr &indxp = cplusql.getIndex( index );
        ptr< StringArrayOrderedRelation > psa ( 
            new StringArrayOrderedRelation ( meta, p.first, indxp ));

        RelationPtr relp( psa );
        RunnablePtr runp( psa );

        cplusql.addRelation( name, relp );
        cplusql.addRunnable( runp );
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0;
}

cplusql_id_t
cplusql_index(
    char *name,
    char *from,
    cplusql_id_t key,
    cplusql_id_t where,
    cplusql_id_t config
)
{
    try{
        if(!name || !from || !key || !config )
        {
            PANICV("null arg");
        }
        char_ptr namep (name);
        char_ptr fromp (from);
        ExpressionPtr keyp(reinterpret_cast<Expression *>(key));
        ExpressionPtr wherep(reinterpret_cast<Expression *>(where));
        if( ! where )
        {
            wherep = new LiteralExpression( "1" );
        }
        ConfigPtr cfg(static_cast< Config * > (config));

        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &frel = cplusql.getRelation( from );

        IndexPtr ind ( new Index());
        cplusql.addIndex( name, ind );
        ListenerPtr lp ( new IndexBuilder ( frel, ind, keyp, wherep));
        cplusql.addShim( lp );
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0 ;
}

cplusql_id_t
cplusql_namespace(char *name, cplusql_id_t config)
{
    try {
        if ((name == (char *) 0) || (config == (char *) 0))
        {
            PANICV("null arg");
        }

        char_ptr namep (name);
        ConfigPtr cfg(static_cast< Config * > (config));
        
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
        (void)cplusql_namespace_helper( name, cfg );
#endif
#endif
        PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    }
    CPLUSQL_CATCHALL();
    return (cplusql_id_t) 0 ;
}


#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
clusterio::NameSpaceService_ptr
cplusql_namespace_helper(const char *name, ConfigPtr &cfg )
{
    char_ptr errors = ptr_strdup( "" );
    if(!name )
    {
        name=CPLUSQL_NAMESPACE_DFLT ;
    }
    //
    // first see if we have this namespace server cached allready.
    // then try ior, url, and default url  in that order.
    //
    clusterio::NameSpaceService_ptr nss =
        CplusqlContext::instance().getNameSpace( name );
    if ( nss != NULL )
    {
        return nss;
    }
    nss=cplusql_namespace_resolve(name,cfg->findString(NSS_IOR_URI),errors);
    if( nss != NULL )
    {
        return nss;
    }
    nss=cplusql_namespace_resolve(name,cfg->findString(NSS_URL),errors);
    if( nss != NULL )
    {
        return nss;
    }

    const char * templ = "corbaname::%s:%s#%s";
    const char * nshost = cfg->findString(NS_HOST);
    const char * nsport = cfg->findString(NS_PORT);
    if( !nshost || !nsport )
    {
        PANICV( "In order to use clusterio, you must define %s or %s or both %s and %s", 
            NSS_IOR_URI, NSS_URL, NS_HOST, NS_PORT );
    }
    size_t urlsz = 
        strlen(templ) +
        strlen(nshost)+
        strlen(nsport)+
        strlen(NSS_NAME)+ 1;
    char urlbuf[ urlsz ];
    sprintf( urlbuf, templ, nshost, nsport, NSS_NAME );
    nss=cplusql_namespace_resolve(name,urlbuf,errors);
    if( nss != NULL )
    {
        return nss;
    }
    PANICV( "could not resolve nss using ior, url, or default url: %s",
        errors.get());
    //not reached
    return NULL;
}
#endif
#endif

void
cplusql_connect_dest( const char *relname, pDest &pd, ConfigPtr &cfg )
{
    CplusqlContext &cplusql = CplusqlContext::instance();
    if( cplusql.haveRelation( relname ))
    {
        APPLOG_DBG("connecting relation %s to a dest", relname );
        ListenerPtr lp (
            new DestJoint( 
                cplusql.getRelation( relname )
                , pd
                , cfg->getString( "OUTPUT_DELIMITER", "|" )
                ,cfg->getString( DEST_RECORD_SEP, DEST_RECORD_SEP_DFLT )
                ,cfg->findString( "WRITE_HEADER" )
                ,cfg->getBool( "FIRST_ROW_HEADER", "0" )
            )
        );
        //const char *dlm = cfg->getString(DEST_RECORD_SEP);
        //APPLOG_DBG("delim=%x", static_cast<int>(*dlm));
        cplusql.addShim( lp );
    }
    else
    {
        //const char *dlm = cfg->getString(DEST_RECORD_SEP);
        //APPLOG_DBG("delim=%x", static_cast<int>(*dlm));
        //APPLOG_DBG("connecting bytestream %s to a file", relname );
        ptr< DestNotifier > &destn = cplusql.getDestNotifier( relname );
        destn->registerListener( pd );
    }
}


DestFactoryPtr
cplusql_destfactory_helper( ConfigPtr &cfg )
{
    //
    // if nameSpace is empty or set to filesystem, create filesystem-based
    // dest factory, otherwise create cluster-based dest factory
    //
    const char *ns = cfg->getString( CPLUSQL_NAMESPACE, NSS_USE_FILE_SYSTEM );
    DestFactoryPtr giveback;
    if ( strlen( ns ) == 0 || !strcmp( ns, NSS_USE_FILE_SYSTEM ))
    {
        giveback = new DestFactory(
            cfg->getBool(   CPLUSQL_DEST_CLOSE_ON_DELETE, "true"),
            cfg->getBool(   CPLUSQL_DEST_O_EXCL, "false"),
            cfg->getBool(   DEST_HASH_ONEFD, "false") ?  false :
                cfg->getBool(   CPLUSQL_DEST_O_TRUNC, "true")
        );   // oldSk00l factory 
        return giveback;
    }
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
    const char *writer_id_str = cfg->getString( "WRITERID", "" );
    clusterio::NameSpaceService_ptr nss =
        cplusql_namespace_helper( ns, cfg );

    // prepare writer id for factory consumtion
    clusterio::writer_id writer_id(
        strlen( writer_id_str ),
        reinterpret_cast< CORBA::Octet * >(
            CORBA::string_dup( writer_id_str )
        ),
        0  // no release
    );
    giveback = new ClusterDestFactory( nss, writer_id );
    return giveback;
#endif
#endif
    PANICV("In order to use cplusqlio you must compile with --enable-distributed");
    return giveback;
}

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
void
cplusql_add_error(
    const char *templ,
    const char *object_string,
    char_ptr &error, 
    CORBA::Exception &ce) 
{
    size_t sz = strlen( ce._rep_id()) + strlen(ce._name()) + 
        strlen(templ) + strlen(object_string) + 1;
    char errbuf[sz];
    sprintf( errbuf, templ, object_string, ce._rep_id(), ce._name());
    error = error + errbuf;
}


clusterio::NameSpaceService_ptr
cplusql_namespace_resolve(
    const char *nsname, 
    const char *object_string, 
    char_ptr &error )
{
    if( ! object_string )
    {
        return NULL;
    }

    clusterio::NameSpaceService_ptr nss = NULL;
    CORBA::Object_ptr obj = NULL;
    try {
        obj = SharedServiceMain::instance()->resolve(object_string);
    }
    catch (CORBA::Exception &ce) 
    {
        cplusql_add_error("resolve(%s) caught CORBA::Exception [%s][%s]",
            object_string, error, ce);
        return NULL;
    }
    if( ! obj )
    {
        error = error + "resolve returned null. ";
        return NULL;
    }
    try {
        nss = clusterio::NameSpaceService::_narrow( obj );
    }
    catch (CORBA::Exception &ce) 
    {
        cplusql_add_error("narrow(%s) caught CORBA::Exception [%s][%s]",
            object_string, error, ce);
        return NULL;
    }
    if (nss == NULL)
    {
        error = error + "resolve returned null. ";
    }
    else
    {
        CplusqlContext::instance().addNameSpace(nsname, nss);
    }
    return nss;
}
#endif
#endif


cplusql_id_t
cplusql_relation_mergejoin(
    char *name,
    char *left,
    cplusql_id_t leftexp,
    char *right,
    cplusql_id_t rightexp,
    cplusql_id_t where,
    cplusql_id_t config
)
{
    try {
//        if( 
//            NULL == name || NULL == left || NULL == leftexp || 
//                            NULL == right || NULL == rightexp 
//        ){
//            PANIC( "cplusql_relation_mergejoin: null input"); 
//        }
//        char_ptr namep(name);
//        char_ptr leftp(left);
//        char_ptr rightp(right);
//        ExpressionPtr eleft( (Expression *)leftexp);
//        ExpressionPtr eright( (Expression *)rightexp);

        //CplusqlContext &context=CplusqlContext::instance();
        //RelationPtr & leftr = context.getRelation( left );
        //RelationPtr & rightr = context.getRelation( right );
//temporary
        //extra () prevents bogus parse of j as function.
//        RelationPtr j ((
//            MergeJoinRelation::makeMergeJoinRelation(
//                leftr, rightr, name, eleft, eright, true
//            )
//        ));
//        (void)CplusqlContext::instance().addRelation( (const char *)name, j );
        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}


cplusql_id_t 
cplusql_relation_memjoin( 
    char *name,
    int outer,
    char *left,
    cplusql_id_t leftexp,
    char *right,
    char *right_index,
    cplusql_id_t where,
    cplusql_id_t config
)
{
    try {
        if( !name|| !left|| !leftexp|| !right|| !right_index|| !config ){
            PANICV( "cplusql_relation_memjoin: null input"); 
        }
        char_ptr namep(name);
        char_ptr leftp(left);
        char_ptr rightp(right);
        char_ptr right_indexp(right_index);
        ExpressionPtr eleft( (Expression *)leftexp);
        ExpressionPtr wherep( (Expression *)where);
        ConfigPtr cfg( static_cast<Config *>(config ));

        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &leftrel = cplusql.getRelation( left );
        pair_array_meta_t p = cplusql.getArray( right );
        IndexPtr &i = cplusql.getIndex( right_index );
        RelationPtr rp ( 
            MemoryJoinRelation::make( 
                leftrel,
                p.first,
                p.second,
                eleft,
                i,
                outer,
                name
                ));
        cplusql.addRelation( name, rp );
        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_relation_from(
    char *name,
    char *from,
    cplusql_id_t parsedcolumns, //not used... previously pushed onto Cplusql context
    cplusql_id_t usercolumns, //not used... previously pushed onto Cplusql context
    cplusql_id_t where,
    cplusql_id_t config
)
{
    try {
        if( !name || !from )
        {
            PANICV( "cplusql_relation_from: null input"); 
        }
        char_ptr namep(name);
        char_ptr fromp(from);
        ConfigPtr cfg( static_cast<Config *>(config ));
        ExpressionPtr wherep( static_cast<Expression *>(where));

        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &rp = cplusql.getRelation( from );
        ExpressionList where_clause( new expression_list_t());
        if( where )
        {
            where_clause->push_back( wherep );
        }
        ptr< char_ptr_list > usercolnames (cplusql.getUserColumnNames());
        ExpressionList user_columns(cplusql.getUserColumns());
        //fprintf( stderr, "cplusql_relation_from: name %s, from %s, usercolnames sz %lu, user_columns %lu, where_clause sz %lu, cfg %lx, rp numcol %lu\n",
         //   name,
          //  from,
           // static_cast< long unsigned int >( usercolnames->size()),
            //static_cast< long unsigned int >( user_columns->size()),
            //static_cast< long unsigned int >( where_clause->size()),
            //reinterpret_cast< long unsigned int >( cfg.get()),
            //static_cast< long unsigned int >( rp->getNumColumns())
        //);

        RelationPtr sr(
            RelationFactory::getSimpleRelationPtr(
                name,
                rp, 
                usercolnames,
                user_columns,
                where_clause,
                cfg
            )
        );
        (void)cplusql.addRelation( (const char *)name, sr );
        usercolnames->clear();

        ListenerPtr lp ( sr );
        rp->registerListener( lp );

        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_relation_presorted(
    char *name,
    char *from,
    cplusql_id_t projection_columns, 
    cplusql_id_t metrics, //not used... previously pushed onto Cplusql context
    cplusql_id_t where,   //nullable
    cplusql_id_t config
)
{
    try {
        if( !name || !from || !projection_columns || !config )
        {
            PANICV( "cplusql_relation_from: null input"); 
        }
        //get ptr around all of our raw yacc/c pointers
        char_ptr namep(name);
        char_ptr fromp(from);
        ptr<list<char_ptr> > groupby(static_cast<list< char_ptr>*>(projection_columns));
        ExpressionPtr wherep( static_cast<Expression *>(where));
        ConfigPtr cfg( static_cast<Config *>(config ));

        CplusqlContext &cplusql = CplusqlContext::instance();
        DelimitedSourcePtr dsp(cplusql.getRelation( from ));
        RelationPtr rp (dsp);

        //TODO, when supported add where_clause to relation ctor
        ExpressionList where_clause( new expression_list_t());
        if( where )
        {
            where_clause->push_back( wherep );
        }

        RelationMetaPtr meta(new RelationMeta());
        meta->setRelationName( name );

        //add groupby 
        FDBG_SI( "cplusql_relation_presorted(), top groupby", groupby->size());
        ExpressionList elist( new expression_list_t());
        while( ! groupby->empty())
        {
            //FDBG_SS( "cplusql_relation_presorted(), groupby", groupby->front().get());
            unsigned ci=rp->getColumnIndex( groupby->front().get());
            ExpressionPtr e( new ColumnExpression( dsp, ci));
            elist->push_back( e );
            meta->addColumn( groupby->front().get());
            groupby->pop_front();
        }

        //add metrics 
        ptr< char_ptr_list > metriccolnames (cplusql.getMetricNames());
        MetricList           pmetrics(cplusql.getMetrics());
        FDBG_SI( "cplusql_relation_presorted(), top metrics", metriccolnames->size());
        while( ! metriccolnames->empty())
        {
            //FDBG_SS( "cplusql_relation_presorted(), metrics", metriccolnames->front().get());
            if( pmetrics->empty())
            {
                PANICV( "cplusql_relation_presorted: no metric for colnames");
            }
            meta->addColumn( metriccolnames->front().get());

            metriccolnames->pop_front();
        }

        RelationPtr sr( new PreSortedProjection( dsp, meta, elist, pmetrics));
        cplusql.addRelation( (const char *)name, sr );
        ListenerPtr lp ( sr );
        rp->registerListener( lp );
        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

void
cplusql_yyparse()
{
    try{
        callback_yyparse();
    }
    CPLUSQL_CATCHALL();
}

void
cplusql_sync()
{
    try{
        if( !CplusqlContext::instance().run())
        {
            exit(1);
        }
    }
    CPLUSQL_CATCHALL();
}


void
cplusql_run()
{
    cplusql_sync();
}

cplusql_id_t
cplusql_map_from_relation(
    char *name,
    char *from,
    cplusql_id_t key_expression,
    cplusql_id_t val_expression
)
{
    try {
        if( !name || !from || !key_expression || !val_expression)
        {
            PANICV( "cplusql_map_from_relation: null input"); 
        }
        char_ptr namep(name);
        char_ptr fromp(from);
        ExpressionPtr ek(  reinterpret_cast<Expression *>(key_expression));
        ExpressionPtr ev( reinterpret_cast<Expression *>(val_expression));

        CplusqlContext &cplusql = CplusqlContext::instance();
        RelationPtr &rp = cplusql.getRelation( from );
        AbstractKeyValSetMapPtr kvsm( new ExpressionStringMap( ));
        ListenerPtr jp( new MapJoint( rp, kvsm, ek, ev ));
        rp->registerListener( jp );
        AbstractKeyValMapPtr kvm( kvsm );
        cplusql.addMap( name,kvm);
        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_from_config(
    char *name,
    cplusql_id_t config
)
{
    try {
        if( !name || !config )
        {
            PANICV( "cplusql_map_from_config: null input"); 
        }
        char_ptr namep(name);
        ConfigPtr cfg(static_cast<Config *>(config));

        ExpressionStringMap * esm = new ExpressionStringMap();
        AbstractKeyValMapPtr kvm( esm );
        pConfigDumpT pcd = ConfigDump::dump( *cfg );
        ConfigDumpT::iterator i=pcd->begin();
        ConfigDumpT::iterator e=pcd->end();
        for( ; i!=e; ++i )
        {
            esm->set( (*i).first.c_str(), (*i).second.c_str());
        }
        CplusqlContext::instance().addMap( name, kvm );

        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_from_ads_map(
    char *name,
    char *from,
    char *col
)
{
    try {
        if( !name || !from || !col)
        {
            PANICV( "cplusql_map_from_ads_map: null input"); 
        }
        char_ptr namep(name);

        CplusqlContext &cplusql = CplusqlContext::instance();
        AbstractDelimitedSourceMapPtr &ads(cplusql.getADSMap( from ));
		AbstractKeyValMapPtr map( new ADSProxyMap(ads, ads->getColumnIndex(col)));
        cplusql.addMap( name, map );
        return 0;
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_value_l(
    char *name,
    cplusql_id_t list,
    cplusql_id_t dflt
)
{
    try {
        if( !name || !list || !dflt )
        {
            PANICV( "cplusql_map_value: null input"); 
        }
        char_ptr namep(name);
		ExpressionList elist(reinterpret_cast<expression_list_t *>(list));
        ExpressionPtr dfltp( reinterpret_cast< Expression *>( dflt ));

        AbstractKeyValMapPtr &esmp(CplusqlContext::instance().getMap( name ));
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< Expression *> (new MapValueExpression( esmp, elist, dfltp )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_value(
    char *name,
    cplusql_id_t key,
    cplusql_id_t dflt
)
{
    try {
        if( !name || !key || !dflt )
        {
            PANICV( "cplusql_map_value: null input"); 
        }
        char_ptr namep(name);
        ExpressionPtr keyp( reinterpret_cast< Expression *>( key ));
        ExpressionPtr dfltp( reinterpret_cast< Expression *>( dflt ));

        AbstractKeyValMapPtr &esmp(CplusqlContext::instance().getMap( name ));
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< Expression *> (new MapValueExpression( esmp, keyp, dfltp )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_exists_l(
    char *name,
    cplusql_id_t list
)
{
    try {
        if( !name || !list )
        {
            PANICV( "cplusql_map_exists: null input"); 
        }
        char_ptr namep(name);
		ExpressionList elist(reinterpret_cast<expression_list_t *>(list));

        AbstractKeyValMapPtr &esmp(CplusqlContext::instance().getMap( name ));
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< Expression *> ( new MapExistsExpression( esmp, elist )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_map_exists(
    char *name,
    cplusql_id_t key
)
{
    try {
        if( !name || !key )
        {
            PANICV( "cplusql_map_exists: null input"); 
        }
        char_ptr namep(name);
        ExpressionPtr keyp( reinterpret_cast< Expression *>( key ));

        AbstractKeyValMapPtr &esmp(CplusqlContext::instance().getMap( name ));
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< Expression *> ( new MapExistsExpression( esmp, keyp )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_unary_metric(int metric, cplusql_id_t argptr )
{
    Expression *gb=NULL;
    try{
        if( !argptr )
        {
            PANICV( "cplusql_unary_metric: null input"); 
        }
        ExpressionPtr arg( reinterpret_cast< Expression *>( argptr ));
        switch(metric)
        {
            case INTMIN: gb = new MinProjection(arg); break ;
            case INTMAX: gb = new MaxProjection(arg); break ;
            case STRMIN: gb = new StrMinProjection(arg); break ;
            case STRMAX: gb = new StrMaxProjection(arg); break ;
            case FIRST: gb = new FirstProjection(arg); break ;
            case LAST: gb = new LastProjection(arg); break ;
            case AVG: gb = new AvgProjection(arg); break ;
            case COUNT_DISTINCT: gb=new CountDistinctProjection(arg);break;
            case COUNT_CHANGES: gb=new CountChangesProjection(arg);break;
            case SUM: gb=new SumProjection(arg);break;
            default: PANICV("unknown unary metric %i", metric);
        }
        return reinterpret_cast<cplusql_id_t>( 
            dynamic_cast< AbstractProjection *> ( gb ));
    }
    CPLUSQL_CATCHALL();
    return NULL;
}

cplusql_id_t
cplusql_metric_count()
{
    try {
        return reinterpret_cast<cplusql_id_t>( dynamic_cast< AbstractProjection *> ( new CountProjection()));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_chomp_exp( cplusql_id_t operand )
{
    try {
        if( !operand )
        {
            PANICV( "null input");
        }
        ExpressionPtr eop( reinterpret_cast< Expression *>( operand ));
        return reinterpret_cast<cplusql_id_t>(
            dynamic_cast< Expression *> ( new ChompExpression( eop)));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_command_exp( 
    cplusql_id_t operand, 
    cplusql_id_t config
)
{
    try {
        if( !operand || !config )
        {
            PANICV( "null input"); 
        }
        ExpressionPtr eop( reinterpret_cast< Expression *>( operand ));
        ConfigPtr cfg(static_cast<Config *>(config));
        return reinterpret_cast<cplusql_id_t>(
            dynamic_cast< Expression *> ( 
                new CommandExpression(
                    eop, cfg->getBool(CPLUSQL_CMD_PANIC_ON_FAILURE,"1")
                    )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_factorial_exp( cplusql_id_t operand )
{
    try {
        if( !operand )
        {
            PANICV( "null input"); 
        }
        ExpressionPtr eop( reinterpret_cast< Expression *>( operand ));
        return reinterpret_cast<cplusql_id_t>(
            dynamic_cast< Expression *> ( 
                new FactorialExpression( eop )));
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

cplusql_id_t
cplusql_sequence_exp( 
        cplusql_id_t start_value, 
        cplusql_id_t increment, 
        cplusql_id_t reset_expression
        )
{
    try {
        if( !start_value || !increment || !reset_expression)
        {
            PANICV( "null input"); 
        }
        ExpressionPtr estart(reinterpret_cast< Expression *>(start_value ));
        ExpressionPtr eincr(reinterpret_cast< Expression *>(increment ));
        ExpressionPtr ereset(reinterpret_cast< Expression *>(reset_expression ));
        Expression *giveback = new SequenceExpression(estart,eincr,ereset );
        return reinterpret_cast<cplusql_id_t>(giveback);
    }
    CPLUSQL_CATCHALL();
    return NULL; //not reached 
}

