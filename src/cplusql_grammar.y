%{ 
/*
** ----------------------------------------------------------------------
** ----------------------------------------------------------------------
**
** File:      url.y
** Author:    mgrosso Matthew Grosso
** Created:   Sat Jan 19 15:20:09 EST 2002 on circe
** Project:   
** Purpose:   
** 
** Copyright (c) 2002 LookSmart. 
** 
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
** $Id: cplusql_grammar.y,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
** ----------------------------------------------------------------------
** ----------------------------------------------------------------------
*/ 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "cplusql_api.h"

/*
#define YYERROR_VERBOSE 1
*/
static int yacdbg=0;

/*
    yyerror and yylex defined here to avoid warnings, since bison
    as of version 1.875d (Sat Oct  1 22:24:45 PDT 2005)
    does not define them.
*/
int yyerror(char *s);
int yylex(void);

void 
set_yacdbg(int newdbg)
{
    yacdbg=newdbg;
};

int 
get_yacdbg()
{
    return yacdbg ;
};

void yacfdbg( const char *s )
{
    if( yacdbg )
    {
        fprintf(stderr,"yac %i: %s \n", cplusql_get_line(), s );
    }
};

%}

%union {
    char * idc;
    void * idp;
    int    id ;
}

%left IF
%left AND OR
%left NOT
%left STR_EQUALS STR_LT STR_GT EQUALS NOTEQUAL LT GT
%left BITOR
%left BITAND
%left STR_PLUS PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%left POW
%token <id> LPAREN RPAREN LCURLY RCURLY SEMICOLON COMMA DLOPEN CREATE STREAM USING FILES COLUMNS FILTERS MERGEJOIN OF TO WHERE USERCOLUMN DEST FROM HASH CRC48 MD5 CEIL FLOOR WITH BUCKETS FILENAME TEMPLATE COLUMN EQUALS NOTEQUAL STR_EQUALS STRCMP LT GT IF THEN ELSE DOT ELIPSES DELIMITER MULTIPLY DIVIDE PLUS MINUS MODULO AND OR NOT CONFIGURATION INHERIT CONFIGFILE PARSE INTO STRICT SYNC MAP MAPVALUE MAPEXISTS ABORT PRESORTED GROUPBY INTMIN INTMAX STRMIN STRMAX FIRST LAST AVG COUNT SUM METRICS COUNT_DISTINCT COUNT_CHANGES JOIN RANDOM LINENUMBER DBCONNECTION XML QUERY HASHED_TABLE LIST COMMAND CHOMP JOB JOBQ STEP NAMESPACE STRMATCH STRCASEMATCH PATTERN PATH TOUCH MV RM FINDRM NSFIND PING RECONFIGURE TOLOWER TOUPPER TABLE NEW MVCLOBBER BYTE PERSIST MERGE IN STR_GT STR_LT  MEMORY INDEX OUTER SORT FOREACH DEBUGECHO EXEC LN LOG2 LOG10 LLFORMAT LDFORMAT STRSTR LENGTH STR_PLUS SKIPNORMALIZE NORMALIZE BYTESTRIP CNTMATCH PHRASEMATCH BITAND BITOR POW ICONV NANOSLEEP EVENT PGLISTEN WARN DML SQLESCAPE SPRINTF SQLVALUE COALESCE WHILE SUBSTR FACTORIAL SEQUENCE STRGREATEST STRLEAST INFO
%token <idc> NUMBER QUOTED_STRING NAME
%type <idc> configvalue
%type <id> strict outer_clause unary_metric zeroary_metric
/*
%type <idp> statement, stream, usercolumnset, usercolumn, fileset, columnset, where_clause, quotedlist, quoted_string_set, dest_statement, relation, expression, expressionlist, quotednamelist, unquoted_name_set, configpairs, config_statement, configuration, config_object, config_clause, parsedcolumns parsedcolumnset synchronize_statement, map_statement metricset, named_metric, metric, delimiter, db_conn_statement
*/
%type <idp> statement stream_statement usercolumnset usercolumn fileset columnset where_clause quotedlist quoted_string_set dest_statement relation expression expressionlist quotednamelist unquoted_name_set configpairs config_statement configuration config_object config_clause parsedcolumns parsedcolumnset synchronize_statement map_statement metricset named_metric metric delimiter db_conn_statement hashed_table_statement ads_map command_statement job_statement jobq_statement namespace_statement namespace_command_statement bytestream_statement statement_semicolon index_statement foreach_statement echo_statement exec_statement sql_command_statement sql_exec_statement ifthenelse metric_expression

/*
%nonassoc EQUALS LT
*/

%%


cplusqlfile:
    statement_semicolon
    {
        yacfdbg("cplusqlfile: statement semicolon");
    }
    | cplusqlfile statement_semicolon
    {
        yacfdbg("cplusqlfile:   cplusqlfile statement semicolon");
    }
    ;

statement_semicolon: 
    foreach_statement |
    statement SEMICOLON ;

statement: 
    bytestream_statement |
    stream_statement |
    index_statement |
    dest_statement |
    config_statement | 
    synchronize_statement |
    map_statement |
    db_conn_statement |
    hashed_table_statement |
    jobq_statement |
    job_statement |
    namespace_statement |
    namespace_command_statement |
    echo_statement |
    command_statement |
    exec_statement |
    sql_command_statement |
    sql_exec_statement
    ;

foreach_statement:
    FOREACH NAME IN quotedlist QUOTED_STRING SEMICOLON
    {
        yacfdbg("foreach_statement: ");
        fprintf(stderr, "foreach: var %s template %s\n", $2, $5 );
        cplusql_foreach( $2, $4, $5 );
        $$ = NULL;
    }
    ;

echo_statement:
    DEBUGECHO expression
    {
        yacfdbg("echo_statement: ");
        cplusql_echo( $2 );
        $$ = NULL;
    }
    ;

exec_statement:
    EXEC expression
    {
        yacfdbg("eval_statement: ");
        cplusql_exec( $2 );
        $$ = NULL;
    }
    ;

command_statement:
    CREATE COMMAND NAME USING QUOTED_STRING config_clause
    {
        $$ = cplusql_command( $3, $5, $6 );
        yacfdbg("created command");
    }
    ;

sql_exec_statement:
    EXEC QUERY USING NAME QUOTED_STRING config_clause
    {
        yacfdbg("exec sql");
        cplusql_run_sql( $4, $5, $6 );
    }
    ;

sql_command_statement:
    CREATE QUERY USING NAME QUOTED_STRING config_clause
    {
        yacfdbg("command sql");
        cplusql_add_sql( cplusql_sql( $4, $5, $6 ));
    }
    ;

synchronize_statement:
    SYNC 
    {
        yacfdbg("sync_statement");
        cplusql_sync();
    }
    ;

jobq_statement:
	CREATE JOBQ NAME config_clause
	{
		$$ = cplusql_jobq( $3, $4 );
        yacfdbg("created jobq");
	}
	;

job_statement:
	CREATE JOB NAME USING NAME COMMAND QUOTED_STRING config_clause
	{
		$$ = cplusql_job( $3, $5, $7, $8 );
        yacfdbg("created job");
	}
	;

namespace_statement:
	CREATE NAMESPACE NAME config_clause
	{
		$$ = cplusql_namespace( $3, $4 );
        yacfdbg("created namespace");
	}
	;

namespace_command_statement:
	NAMESPACE NAME TOUCH QUOTED_STRING config_clause
	{
        yacfdbg("touch");
        $$ = cplusql_namespace_touch( $2, $4, $5 );
	}
    |
	NAMESPACE NAME PING 
	{
        yacfdbg("ping");
        $$ = cplusql_namespace_ping( $2 );
	}
    |
	NAMESPACE NAME RECONFIGURE config_clause
	{
        yacfdbg("reconfigure");
        $$ = cplusql_namespace_reconfigure( $2, $4 );
    }
    |
	NAMESPACE NAME MV QUOTED_STRING QUOTED_STRING config_clause
	{
        yacfdbg("mv");
        $$ = cplusql_namespace_mv( $2, $4, $5, $6 );
	}
    |
	NAMESPACE NAME MVCLOBBER QUOTED_STRING QUOTED_STRING config_clause
	{
        yacfdbg("mv");
        $$ = cplusql_namespace_mvclobber( $2, $4, $5, $6 );
	}
    |
	NAMESPACE NAME RM QUOTED_STRING config_clause
	{
        yacfdbg("rm");
        $$ = cplusql_namespace_rm( $2, $4, $5 );
	}
    |
	NAMESPACE NAME FINDRM 
        PATH QUOTED_STRING PATTERN QUOTED_STRING config_clause
	{
        $$ = cplusql_namespace_findrm( $2, $5, $7, $8 );
        yacfdbg("findrm");
	}
    |
	NAMESPACE NAME NSFIND
        PATH QUOTED_STRING PATTERN QUOTED_STRING config_clause
	{
        $$ = cplusql_namespace_find( $2, $5, $7, $8 );
        yacfdbg("find");
	}
    ;

map_statement:
    CREATE MAP NAME USING relation FROM expression TO expression
    {
        cplusql_map_from_relation( $3, $5, $7, $9 );
        yacfdbg("create map from relation");
    }
    |
    CREATE MAP NAME USING CONFIGURATION configuration
    {
        cplusql_map_from_config( $3, $6 );
        yacfdbg("create map from config");
    }
    |
    CREATE MAP NAME USING ads_map TO NAME
    {
        cplusql_map_from_ads_map( $3, $5, $7 );
        yacfdbg("create map from ads map");
    }
    ;

db_conn_statement:
    CREATE DBCONNECTION NAME LPAREN QUOTED_STRING COMMA QUOTED_STRING COMMA QUOTED_STRING RPAREN config_clause
    {
        yacfdbg("db_conn:");
        $$ = cplusql_db_conn( $3, $5, $7, $9, $11 );
    }
    ;

hashed_table_statement:
    CREATE HASHED_TABLE NAME TEMPLATE QUOTED_STRING QUERY QUOTED_STRING BUCKETS NUMBER columnset config_clause
    {
        yacfdbg("hashed_table:");
        $$ = cplusql_hashed_table( $3, $5, $7, $9, $10, $11 );
    }
    ;

config_statement:
    CREATE CONFIGURATION NAME configuration 
    {
        yacfdbg("config: CREATE configuration...");
        $$=cplusql_add_config( $3, $4 );
    }
    |
    configpairs
    {
        $$ = cplusql_addto_default_cfg( $1 );
    }
    |
    config_object
    {
        $$ = cplusql_addto_default_cfg( $1 );
    }
    ;

config_clause:
    { $$=cplusql_default_cfg();}
    |
    configuration
    {
        yacfdbg("config_clause:");
        $$=$1;
    }
    ;

config_object:
    LPAREN configpairs RPAREN
    {
        yacfdbg("config_object: configpairs:");
        $$=$2;
    }
    |
    CONFIGFILE QUOTED_STRING
    {
        yacfdbg("config_object: configfile QUOTED_STRING:");
        yacfdbg((const char *) $2);
        $$=cplusql_cfg_file( $2 );
    }
    |
    INHERIT NAME {
        yacfdbg("config_object: INHERIT");
        yacfdbg((const char *) $2);
        $$=cplusql_copy_named_cfg( $2 ); }
    ;

configuration:
    config_object
    {
        yacfdbg("configuration: config_object:");
        $$=$1;
    }
    | configuration config_object   { $$=cplusql_merge_cfg( $1, $2 ); }  
    ;

configpairs:
    { $$=cplusql_empty_config(); } |
    NAME EQUALS configvalue         { $$=cplusql_start_config( $1, $3 ); }
    | configpairs COMMA NAME EQUALS configvalue 
    {
        $$=cplusql_addto_config( $1, $3, $5 );
    }
    ;

configvalue:
    QUOTED_STRING   { $$=$1 ; }
    | NUMBER        { $$=$1 ; }
    ;

bytestream_statement:
    CREATE BYTE STREAM NAME USING fileset config_clause
    {
        yacfdbg("stream_statement: files");
        $$ = cplusql_byte_source( $4, $6, $7 );
    }
    ;

stream_statement:
    PERSIST NAME FROM NAME
    {
        yacfdbg("stream_statement: persist ");
        $$ = cplusql_persist( $2, $4 );
    }
    |
    CREATE STREAM NAME USING fileset 
    delimiter columnset config_clause
    {
        yacfdbg("stream_statement: files");
        $$ = cplusql_relation_fileset( $3, $5, $6, $7, $8 );
    }
    |
    CREATE STREAM NAME USING PATH QUOTED_STRING PATTERN QUOTED_STRING
    delimiter columnset config_clause
    {
        yacfdbg("stream_statement: files");
        $$ = cplusql_relation_pattern( $3, $6, $8, $9, $10, $11 );
    }
    |
    CREATE STREAM NAME USING MERGE JOIN 
    OF relation expression TO relation expression where_clause config_clause
    {
        yacfdbg("stream_statement: mergejoin");
        //TODO use where_clause, $12
        $$ = cplusql_relation_mergejoin( $3, $8, $9, $11, $12, $13, $14 );
    }
    |
    CREATE STREAM NAME USING outer_clause JOIN 
    OF relation expression TO relation INDEX NAME where_clause config_clause
    {
        yacfdbg("stream_statement: mergejoin");
        $$ = cplusql_relation_memjoin( $3, $5, $8, $9, $11, $13, $14, $15 );
    }
    |
    CREATE STREAM NAME USING relation 
    usercolumnset parsedcolumns where_clause config_clause
    {
        yacfdbg("stream_statement: from stream");
        $$ = cplusql_relation_from( $3, $5, $6, $7, $8, $9 );
    }
    |
    CREATE STREAM NAME USING relation PRESORTED GROUPBY 
    columnset metricset where_clause config_clause
    {
        yacfdbg("stream_statement: presorted groupby");
        $$ = cplusql_relation_presorted( $3, $5, $8, $9, $10, $11 );
    }
    |
    CREATE STREAM NAME USING DBCONNECTION NAME QUERY QUOTED_STRING
    columnset config_clause
    {
        yacfdbg("stream_statement: db");
        $$ = cplusql_relation_db( $3, $6, $8, $9, $10 );
    }
    |
    CREATE STREAM NAME USING NAME INDEX NAME config_clause
    {
        yacfdbg("stream_statement: indexscan");
        $$ = cplusql_relation_indexscan( $3, $5, $7, $8 );
    }
    |
    CREATE STREAM NAME USING MAP NAME config_clause
    {
        fprintf(stderr, "create stream from map not implemented\n" );
        $$ = NULL;//
    }
    |
    CREATE STREAM NAME USING DBCONNECTION NAME TABLE QUOTED_STRING
    columnset config_clause
    {
        yacfdbg("stream_statement: db");
        $$ = cplusql_relation_pgcopy( $3, $6, $8, $9, $10 );
    }
    |
    CREATE STREAM NAME USING DBCONNECTION NAME TABLE QUOTED_STRING
    config_clause
    {
        yacfdbg("stream_statement: db short");
        $$ = cplusql_relation_pgcopy_short( $3, $6, $8, $9 );
    }
    |
    CREATE STREAM NAME USING relation 
        XML QUERY QUOTED_STRING columnset 
        FROM expression config_clause
    {
        yacfdbg("xml stream");
        $$ = cplusql_relation_xmlquery( $3, $5, $8, $9, $11, $12 );
    }
    | CREATE STREAM NAME usercolumnset WHILE expression config_clause
    {
        yacfdbg("expression stream");
        $$ = cplusql_relation_exp( $3, $4, $6, $7 );
    }
    |
    CREATE STREAM NAME USING relation
        DBCONNECTION NAME QUERY expression columnset 
    {
        yacfdbg("dbrepeat stream");
        $$ = cplusql_relation_dbrepeat( $3, $5, $7, $9, $10);
    }
    ;

outer_clause:
    {
        yacfdbg("outer_clause: false");
        $$ = 0 ;
    }
    |
    OUTER
    {
        yacfdbg("outer_clause: true");
        $$ = 1 ;
    }
    ;


index_statement:
    CREATE INDEX NAME USING relation SORT USING expression where_clause config_clause
    {
        $$ = cplusql_index( $3, $5, $8, $9, $10 );
    }
    ;

ads_map:
    NAME
    {
        yacfdbg("relation: NAME");
        yacfdbg((const char *) $1 );
        $$=cplusql_src_ads_map( $1 );
    }
    ;

relation:
    NAME
    {
        yacfdbg("relation: NAME");
        yacfdbg((const char *) $1 );
        $$=cplusql_src_relation( $1 );
    }
;
/*
columnsets:
    { $$=NULL; }
    |
    columnsets parsedcolumnset
    |
    columnsets usercolumnset
    ;
*/

parsedcolumns:
    { $$=NULL; } |
    parsedcolumns parsedcolumnset 
    ;

parsedcolumnset:
    PARSE expression strict delimiter INTO columnset 
    {
        yacfdbg("parse expression using delimiter");
        $$=cplusql_parsed_columns_exp( $2, $3, $4, $6 ); 
    }
    |
    PARSE expression PATTERN QUOTED_STRING config_clause INTO columnset 
    {
        yacfdbg("parse expression using pattern");
        $$=cplusql_parsed_columns_regex( $2, $4, $5, $7 ); 
    }
    |
    PARSE expression PATTERN quotedlist config_clause INTO columnset 
    {
        yacfdbg("parse expression using pattern");
        $$=cplusql_parsed_columns_regexlist( $2, $4, $5, $7 ); 
    }
    |
    PARSE expression USING PHRASEMATCH quotedlist INTO columnset
    {
        yacfdbg("cntmatch expression using list into name, name");
        $$=cplusql_parsed_matchlist( $5, $2, $7);
    }
    ;

strict: { $$=0; } | STRICT { $$=1;} ;

delimiter:
    { 
        //result gets leaked by the way
        $$=0;
    } 
    | 
    DELIMITER QUOTED_STRING 
    {
        $$=$2;
    }
    ;

usercolumnset:
    { 
        yacfdbg("usercolumnset: empty");
        $$=NULL;
    }
    |
    USERCOLUMN usercolumn
    {
        yacfdbg("usercolumnset: usercolumn");
        $$=NULL;
    }
    |
    usercolumnset COMMA usercolumn
    { 
        yacfdbg("usercolumnset: COMMA usercolumn");
        $$=NULL;
    }
    ;

usercolumn:    
    NAME expression
    {
        $$=cplusql_user_column( $1, $2 );
    }
    |
    LPAREN NAME RPAREN
    {
        $$=cplusql_user_column_def( $2 );
    }
    ;

metricset:
    {
        yacfdbg("metricset: empty");
        $$=NULL;
    }
    |
    METRICS named_metric 
    {
        yacfdbg("metricset: metrics");
        $$=NULL;
    }
    |
    metricset COMMA named_metric
    {
        yacfdbg("metricset: COMMA named_metric");
        $$=NULL;
    }
    ;

named_metric:    
    NAME metric { $$=cplusql_named_metric( $1, $2 ); }
    ;


fileset:    FILES quotedlist 
    { 
        yacfdbg("fileset: ");
        $$ = $2 ;
    }
    ;

columnset:
    COLUMNS quotednamelist 
    {
        yacfdbg("columnset: ");
        $$ = $2 ;
    }
    ;

where_clause:
    {
        yacfdbg("empty where_clause: ");
        $$ = NULL ;
    }
    |
    WHERE expression
    {
        yacfdbg("where_clause: ");
        $$ = $2;
    }
    ;

quotedlist: 
    LPAREN quoted_string_set RPAREN 
    {
        yacfdbg("quotedlist: ");
        $$ = $2 ;
    }
    |
    LPAREN RPAREN
    {
        yacfdbg("quotedlist: ");
        $$ = cplusql_new_string_set(0);
    }
    |
    LPAREN NUMBER ELIPSES NUMBER RPAREN 
    { 
        yacfdbg("quotedlist: number ... number ");
        $$ = cplusql_range_string_set($2, $4);
    } 
    | LPAREN NUMBER ELIPSES NUMBER STEP NUMBER RPAREN 
    {
        yacfdbg("quotedlist: number ... number step number");
        $$ = cplusql_range_step_string_set($2, $4, $6 );
    }
    |  COMMAND expression
    {
        yacfdbg("quotedlist: COMMAND expression");
        $$ = cplusql_command_string_set($2 );
    }
    ;

quoted_string_set:
    quoted_string_set COMMA QUOTED_STRING
    {
        yacfdbg("quoted_string_set: quoted_string_set, QUOTED_STRING");
        yacfdbg((const char *) $3 );
        $$ = cplusql_string_set( $1, $3 );
    }
    |
    QUOTED_STRING
    {
        yacfdbg("quoted_string_set: QUOTED_STRING");
        yacfdbg((const char *) $1 );
        $$ = cplusql_new_string_set( $1 );
    }
    ;

quotednamelist: 
    LPAREN unquoted_name_set RPAREN 
    {
        yacfdbg("quotednamelist: ");
        $$ = $2 ;
    }
    |
    LPAREN RPAREN
    {
        yacfdbg("quotednamelist: empty");
        $$ = cplusql_new_string_set(0);
    }
    ;

unquoted_name_set:
    unquoted_name_set COMMA NAME
    {
        yacfdbg("unquoted_name_set: unquoted_name_set, NAME");
        yacfdbg((const char *) $3 );
        $$ = cplusql_string_set( $1, $3 );
    }
    |
    NAME
    {
        yacfdbg("unquoted_name_set: NAME");
        yacfdbg((const char *) $1 );
        $$ = cplusql_new_string_set( $1 );
    }
    ;

dest_statement:
    CREATE DEST FROM relation TEMPLATE QUOTED_STRING expression config_clause
    {
        yacfdbg("dest_statement: hashed files");
        $$= cplusql_dest_hash( $4, $6, $7, $8 );
    }
    |
    CREATE DEST FROM NAME FILENAME QUOTED_STRING config_clause
    {
        yacfdbg("dest_statement: file");
        $$= cplusql_dest_file( $4, $6, $7 );
    }
    |
    CREATE DEST FROM NAME TO TABLE USING NAME config_clause
    {
        yacfdbg("dest_statement: table");
        $$= cplusql_dest_table( $4, $8, $9 );
    }
    |
    CREATE DEST FROM NAME TO NEW TABLE USING NAME config_clause
    {
        yacfdbg("dest_statement: new table");
        $$= cplusql_dest_new_table( $4, $9, $10 );
    }
    ;

expressionlist:
    expression
    {
        yacfdbg("expressionlist: expression");
        $$=cplusql_new_expression_list( $1 );
    }
    |
    expressionlist COMMA expression
    {
        yacfdbg("expressionlist: expression COMMA expression");
        $$=cplusql_add_expression( $1, $3 );
    }
    ;

metric_expression:
    unary_metric LPAREN expressionlist RPAREN {$$=NULL;};

expression:
    metric_expression { $$=$1; } |
    expression LT expression {$$=cplusql_binary_exp($1,LT,$3 );} |
    expression GT expression {$$=cplusql_binary_exp($1,GT,$3 );} |
    expression AND expression     {$$=cplusql_binary_exp($1,AND,$3 );} |
    expression OR expression      {$$=cplusql_binary_exp($1,OR,$3 );} |
    expression EQUALS expression  {$$=cplusql_binary_exp($1,EQUALS,$3 );} |
    expression NOTEQUAL expression {$$=cplusql_binary_exp($1,NOTEQUAL,$3);}|
    expression MULTIPLY expression{$$=cplusql_binary_exp($1,MULTIPLY,$3 );} |
    expression DIVIDE expression  {$$=cplusql_binary_exp($1,DIVIDE,$3 );} |
    expression MODULO expression  {$$=cplusql_binary_exp($1,MODULO,$3 );} |
    expression PLUS expression    {$$=cplusql_binary_exp($1,PLUS,$3 );} |
    expression MINUS expression   {$$=cplusql_binary_exp($1,MINUS,$3 );} |
    expression BITAND expression   {$$=cplusql_binary_exp($1,BITAND,$3 );} |
    expression BITOR expression   {$$=cplusql_binary_exp($1,BITOR,$3 );} |
    expression POW expression   {$$=cplusql_binary_exp($1,POW,$3 );} |
    expression STR_PLUS expression {$$=cplusql_binary_exp($1,STR_PLUS,$3);}|
    expression STR_LT expression {$$=cplusql_binary_exp($1,STR_LT,$3 );} |
    expression STR_GT expression {$$=cplusql_binary_exp($1,STR_GT,$3 );} |
    expression STR_EQUALS expression 
        {$$=cplusql_binary_exp($1,STR_EQUALS,$3);}|
    LN LPAREN expression COMMA expression RPAREN 
        {$$=cplusql_binary_exp($3,LN,$5 );} |
    LN LPAREN expression RPAREN
        {$$=cplusql_binary_exp($3,LN,NULL);} |
    LOG2 LPAREN expression COMMA expression RPAREN 
        {$$=cplusql_binary_exp($3,LOG2,$5 );} |
    LOG2 LPAREN expression RPAREN
        {$$=cplusql_binary_exp($3,LOG2,NULL);} |
    LOG10 LPAREN expression COMMA expression RPAREN 
        {$$=cplusql_binary_exp($3,LOG10,$5 );} |
    LOG10 LPAREN expression RPAREN
        {$$=cplusql_binary_exp($3,LOG10,NULL);} |
    STRCMP LPAREN expression COMMA expression RPAREN
    {
        fprintf(stderr, 
            "strcmp(lhs,rhs) is deprecated in favor of lhs ~= rhs, "
            "since the formers usage is the opposite of what a c "
            "programmer should have come to expect.\n");
        $$=cplusql_binary_exp($3,STR_EQUALS,$5 );
    } |
    /* column expression */
    NAME DOT NAME
    {
        yacfdbg("expression: NAME DOT NAME");
        yacfdbg((const char *) $1);
        yacfdbg((const char *) $3);
        $$= cplusql_column_exp_str( $1, $3 );
    }
    |
    NAME
    {
        yacfdbg("expression: NAME");
        yacfdbg((const char *) $1);
        $$= cplusql_column_exp_str_dflt_rel( $1 );
    }
    |
    /* literal expressions */
    QUOTED_STRING                 { $$= cplusql_literal_exp( $1 ); }
    |
    NUMBER                 { $$= cplusql_literal_exp( $1 ); }
    |
    /* ifthen_expression, see below */
    ifthenelse
    |
    /* 
    function expressions 
        note: at some point, it will be prudent to move the function 
        name out of the grammar, and define unary, binary, trinary 
        functions, etc... 
    */
    HASH LPAREN expression COMMA NUMBER RPAREN 
    {
        /* expression-to-hash, num-buckets */
        $$= cplusql_hash_exp( $3, $5 ); 
    } 
    | 
    MD5 LPAREN expression RPAREN
    {
        /*  what to hash */
        yacfdbg("expression: md5 expression");
        $$= cplusql_md5_exp( $3 );
    }
    |
    CRC48 LPAREN expression RPAREN
    {
        yacfdbg("expression: crc48 expression");
        $$= cplusql_crc48_exp( $3 );
    }
    |
    MAPVALUE LPAREN NAME COMMA LIST expressionlist COMMA expression RPAREN
    {
        /* map-to-look-in, key-expression, default-value */
        yacfdbg("expression: map_value_l expression");
        //yacfdbg((const char *) $1);
        $$= cplusql_map_value_l( $3, $6, $8 ); 
    }
    |
    MAPVALUE LPAREN NAME COMMA expression COMMA expression RPAREN 
    {
        /* map-to-look-in, key-expression, default-value */
        yacfdbg("expression: map_value expression");
        $$= cplusql_map_value( $3, $5, $7 ); 
    } 
    |
    MAPEXISTS LPAREN NAME COMMA LIST expressionlist RPAREN
    {
        /* map-to-look-in, key-expression */
        $$= cplusql_map_exists_l( $3, $6 ); 
    }
    |
    MAPEXISTS LPAREN NAME COMMA expression RPAREN 
    {
        /* map-to-look-in, key-expression */
        $$= cplusql_map_exists( $3, $5 ); 
    }
    |
    ABORT LPAREN expression RPAREN 
    {
        /* map-to-look-in, key-expression */
        $$= cplusql_abort( $3 ); 
    }
    |
    /* parens expression  */
    LPAREN expression RPAREN      { $$ = $2 ; }
    |
    /* unary expression  */
    NOT expression                {$$=cplusql_not($2);}
    |
    JOIN LPAREN LPAREN expressionlist RPAREN COMMA QUOTED_STRING RPAREN
    {
        yacfdbg("expression: join expression");
        $$=cplusql_join_columns( $4, $7 );
    }
    | RANDOM LPAREN expression COMMA expression RPAREN {$$=cplusql_random( $3, $5 );}
    | LINENUMBER LPAREN NAME RPAREN {$$=cplusql_line_number( $3 );}
    | CEIL LPAREN expression RPAREN
    {
        /* expression-to-hash, num-buckets */
        yacfdbg("expression: ceil expression");
        $$= cplusql_ceil_exp( $3 );
    }
    | FLOOR LPAREN expression RPAREN
    {
        /* expression-to-hash, num-buckets */
        yacfdbg("expression: floor expression");
        $$= cplusql_floor_exp( $3 );
    }
    | CNTMATCH LPAREN quotedlist COMMA expression RPAREN
    {
        yacfdbg("expression: cntmatch expression");
        $$ = cplusql_cntmatch_exp( $3, $5 );
        ;
    }
    | BYTESTRIP LPAREN 
        expression 
        COMMA QUOTED_STRING
        COMMA QUOTED_STRING
        COMMA QUOTED_STRING
        COMMA QUOTED_STRING
        RPAREN
    {
        yacfdbg("expression: bytestrip");
        cplusql_bytestrip_exp( $3,$5,$7,$9,$11 );
    }
    | NORMALIZE LPAREN expression COMMA quotedlist COMMA quotedlist RPAREN
    {
        yacfdbg("expression: normalize expression");
        $$ = cplusql_normalize_exp( $3, $5, $7, 0 );
    }
    | SKIPNORMALIZE LPAREN expression COMMA quotedlist COMMA quotedlist RPAREN
    {
        yacfdbg("expression: normalize expression");
        $$ = cplusql_normalize_exp( $3, $5, $7, 1 );
    }
    | ICONV LPAREN expression COMMA QUOTED_STRING COMMA QUOTED_STRING RPAREN
    {
        $$ = cplusql_iconv_exp( $3, $5, $7 );
    }
    | LENGTH LPAREN expression RPAREN
    {
        yacfdbg("expression: length()");
        $$ = cplusql_length_exp( $3 );
    }
    | STRSTR LPAREN expression COMMA expression RPAREN
    {
        yacfdbg("expression: strstr");
        $$=cplusql_strstr_exp($3,$5 );
    }
    | STRMATCH LPAREN QUOTED_STRING COMMA expression RPAREN
    {
        yacfdbg("expression: regex match expression");
        $$=cplusql_regex_match_exp($3,$5,0 );
    }
    | STRCASEMATCH LPAREN QUOTED_STRING COMMA expression RPAREN
    {
        yacfdbg("expression: regex nocase match expression");
        $$=cplusql_regex_match_exp($3,$5,1 );
    }
    | TOLOWER LPAREN expression RPAREN
    {
        yacfdbg("expression: tolower( expression )");
        $$=cplusql_ftrans_exp($3, tolower );
    }
    | TOUPPER LPAREN expression RPAREN
    {
        yacfdbg("expression: toupper( expression )");
        $$=cplusql_ftrans_exp($3, toupper );
    }
    | LLFORMAT LPAREN QUOTED_STRING COMMA expression RPAREN 
    {
        yacfdbg("expression: llformat( format, expression )");
        $$=cplusql_llformat($3, $5 );
    }
    | LLFORMAT LPAREN expression RPAREN 
    {
        yacfdbg("expression: llformat( expression )");
        $$=cplusql_ll($3 );
    }
    | LDFORMAT LPAREN QUOTED_STRING COMMA expression RPAREN 
    {
        yacfdbg("expression: ldformat( format, expression )");
        $$=cplusql_ldformat($3, $5 );
    }
    | COMMAND LPAREN expression config_clause RPAREN
    {
        yacfdbg("expression: command( )");
        $$=cplusql_command_exp($3, $4 );
    }
    | CHOMP LPAREN expression RPAREN
    {
        yacfdbg("expression: chomp( )");
        $$=cplusql_chomp_exp($3 );
    }
    | WARN LPAREN expression RPAREN
    {
        yacfdbg("expression: warn( )");
        $$=cplusql_warn_exp( $3 );
    }
    | INFO LPAREN expression RPAREN
    {
        yacfdbg("expression: info( )");
        $$=cplusql_info_exp( $3 );
    }
    | DML LPAREN 
        NAME COMMA 
        NUMBER COMMA
        LPAREN expressionlist RPAREN COMMA
        LPAREN expressionlist RPAREN COMMA
        LPAREN expressionlist RPAREN 
    RPAREN
    {
        yacfdbg("expression: dml()");
        $$=cplusql_dml_exp( $3, $5, $8, $12, $16 );
    }
    |
    SPRINTF LPAREN QUOTED_STRING expressionlist RPAREN 
    {
        yacfdbg("expression: sprintf()");
        $$=cplusql_sprintf_exp( $3, $4 );
    }
    |
    SQLESCAPE LPAREN expression RPAREN 
    {
        yacfdbg("expression: sqlescape()");
        $$=cplusql_sqlescape_exp( $3 );
    }
    |
    NANOSLEEP NUMBER NUMBER
    {
        yacfdbg("expression: nanosleep");
        $$=cplusql_nanosleep_exp( $2, $3 );
    }
    |
    NANOSLEEP LPAREN NUMBER COMMA NUMBER RPAREN
    {
        yacfdbg("expression: nanosleep");
        $$=cplusql_nanosleep_exp( $3, $5 );
    }
    |
    PGLISTEN LPAREN NAME COMMA NAME RPAREN
    {
        yacfdbg("expression: pglisten");
        $$=cplusql_pglisten_exp( $3, $5 );
    }
    |
    SQLVALUE LPAREN NAME COMMA expression RPAREN
    {
        yacfdbg("expression: sqlvalue");
        $$=cplusql_sqlvalue_exp( $3, $5 );
    }
    | COALESCE LPAREN expressionlist RPAREN
    {
        yacfdbg("expression: coalesce");
        $$=cplusql_coalesce_exp( $3 );
    }
    | SUBSTR LPAREN expression COMMA expression COMMA expression RPAREN
    {
        yacfdbg("expression: substr");
        $$=cplusql_substr_exp( $3, $5, $7 );
    }
    | FACTORIAL LPAREN expression RPAREN
    {
        yacfdbg("expression: factorial");
        $$=cplusql_factorial_exp( $3 );
    }
    /* numeric start value, increment to add, reset_expression */
    | SEQUENCE LPAREN expression COMMA expression COMMA expression RPAREN
    {
        yacfdbg("expression: sequence");
        $$=cplusql_sequence_exp( $3, $5, $7 );
    }
    | STRGREATEST LPAREN expressionlist RPAREN
    {
        yacfdbg("expression: strgreatest");
        $$=cplusql_strgreatest_exp( $3 );
    }
    | STRLEAST LPAREN expressionlist RPAREN
    {
        yacfdbg("expression: strleast");
        $$=cplusql_strleast_exp( $3 );
    }
    ;

ifthenelse:
    /* ifthen_expression, THEN added reduce syntax errors */
    IF LPAREN expression RPAREN
    LCURLY expression RCURLY
    ELSE LCURLY expression RCURLY
    { $$= cplusql_ifthen_exp( $3, $6, $10); }
    |
    IF LPAREN expression RPAREN
    LCURLY expression RCURLY
    ELSE ifthenelse
    { $$= cplusql_ifthen_exp( $3, $6, $9); }
    |
    IF LPAREN expression RPAREN THEN
    LCURLY expression RCURLY
    ELSE LCURLY expression RCURLY
    { $$= cplusql_ifthen_exp( $3, $7, $11); }
    |
    IF LPAREN expression RPAREN THEN
    LCURLY expression RCURLY
    ELSE ifthenelse
    { $$= cplusql_ifthen_exp( $3, $7, $10); }
    ;

metric:
    zeroary_metric  LPAREN RPAREN 
    {
        $$=cplusql_metric_count(); 
    }
    | unary_metric  LPAREN expression RPAREN
    { 
        $$=cplusql_unary_metric($1,$3);
    }
    ;

zeroary_metric:
    COUNT
    ;

/* 
NOTE: re unary_metric
for some reason, returning $1 into $$ instead of the METRIC name results in
    large numbers, as if a pointer was being cast to int, even though
    INTMIN, etc and unary_metric are all of type <id> 
*/
unary_metric:
        INTMIN {$$= INTMIN ;}
    |   INTMAX {$$=INTMAX ;}
    |   STRMIN {$$=STRMIN ;}
    |   STRMAX {$$=STRMAX ;}
    |   FIRST {$$=FIRST ;}
    |   LAST {$$=LAST ;}
    |   AVG {$$=AVG ;}
    |   COUNT_DISTINCT {$$=COUNT_DISTINCT ;}
    |   COUNT_CHANGES {$$=COUNT_CHANGES ;}
    |   SUM {$$=SUM ;}
    ;


%%



int
yyerror(char *s)
{
    fprintf(stderr,"%i %s\n",cplusql_get_line(), s ? s : "" );
    exit(1);
}

void 
callback_yyparse(int x)
{
    yyparse();
}

