%{ 
/*
** ----------------------------------------------------------------------
** ----------------------------------------------------------------------
**
** File:      cplusql.l
** Author:    mgrosso Matthew Grosso
** Created:   Sat Jan 19 14:15:32 EST 2002 on circe
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
** $Id: cplusql_lexer.l,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
** ----------------------------------------------------------------------
** ----------------------------------------------------------------------
*/ 

#include "cplusql_grammar.h"
#include "cplusql_api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int lexdbg=0;
void set_lexdbg( int newdbg)
{
    yy_flex_debug=lexdbg=newdbg;
};

void lexfdbg( const char *s )
{
    if( lexdbg )
    {
        fprintf(stderr,"lex %i: %s\n", cplusql_get_line(), s);
    }
};

void lexfdbgs( const char *s, const char *s2  )
{
    if( lexdbg )
    {
        fprintf(stderr,"lex %i: %s %s\n", cplusql_get_line(), s, s2);
    }
};

/* the YY_BUFFER_STATE thing is opaque to other code so no templates
    so sad to say we inflict yet another linked list on the world :-/ */

typedef struct ybuflist {
    YY_BUFFER_STATE buf;
    struct ybuflist *next;
} ybuflist_t ;

static ybuflist_t *buffers = NULL ;

void 
push_lex_buffer( const char *s )
{
    if(lexdbg)
    {
        fprintf( stderr, "pushing buffer %s\n", s );
    }
    ybuflist_t *newnode ;
    newnode = (ybuflist_t *)malloc( sizeof( ybuflist_t) );
    newnode->buf = YY_CURRENT_BUFFER ;
    newnode->next = buffers ;
    buffers = newnode ;
    yy_scan_string( s );
}

// see cplusql.h now: void lex_putback( char *s );
void lex_pushqchar( char c );

static int qcharlen;
static char varbuf[ VARBUFLEN ];

static const char *env_val;
static char *cmd_output;


%}
%array
%x QUOTE_STATE
%x CMNT_STATE
%x ENV_STATE
%x CMD_STATE
%%
"foreach"           {   lexfdbg("foreach");         return FOREACH ; }
"echo"              {   lexfdbg("echo");            return DEBUGECHO ; }
"dlopen"            {   lexfdbg("dlopen");          return DLOPEN ; }
"sync"              {   lexfdbg("sync");            return SYNC ; }
"create"            {   lexfdbg("create");          return CREATE ; }
"using"             {   lexfdbg("using");           return USING ; }
"stream"            {   lexfdbg("stream");          return STREAM; }
"map"               {   lexfdbg("map");             return MAP; }
"files"             {   lexfdbg("files");           return FILES ; }
"columns"           {   lexfdbg("columns");         return COLUMNS ; }
"column"            {   lexfdbg("column");          return COLUMN ; }
"merge"             {   lexfdbg("mergejoin");       return MERGE ; }
"in"                {   lexfdbg("in");              return IN ; }
"memory"            {   lexfdbg("memory");          return MEMORY ; }
"of"                {   lexfdbg("of");              return OF ; }
"to"                {   lexfdbg("to");              return TO ; }
"where"             {   lexfdbg("where");           return WHERE ; }
"usercolumn"        {   lexfdbg("usercolumn");      return USERCOLUMN ; }
"usercolumns"       {   lexfdbg("usercolumns");     return USERCOLUMN ; }
"if"                {   lexfdbg("if");              return IF ; }
"then"              {   lexfdbg("then");            return THEN ; }
"else"              {   lexfdbg("else");            return ELSE ; }
"parse"             {   lexfdbg("parse");           return PARSE ; }
"into"              {   lexfdbg("into");            return INTO ; }
"strict"            {   lexfdbg("strict");          return STRICT ; }
"join"            	{   lexfdbg("join");          	return JOIN ; }
"outer"            	{   lexfdbg("outer");          	return OUTER ; }
"index"             {   lexfdbg("index");           return INDEX ; }
"sort"              {   lexfdbg("sort");            return SORT ; }
"dbconnection"     	{   lexfdbg("dbconnection");   	return DBCONNECTION ; }
"query"     		{   lexfdbg("query");   		return QUERY ; }
"hashed_table" 		{   lexfdbg("hashed_table");	return HASHED_TABLE ; }
"new" 		        {   lexfdbg("new");	            return NEW ; }
"table" 		    {   lexfdbg("table");	        return TABLE ; }
"not"               {   lexfdbg("not");             return NOT ; }
"!"                 {   lexfdbg("not");             return NOT ; }
"and"               {   lexfdbg("and");             return AND ; }
"&&"                {   lexfdbg("and");             return AND ; }
"or"                {   lexfdbg("or");              return OR ; }
"||"                {   lexfdbg("or");              return OR ; }
"metrics"           {   lexfdbg("metrics");         return METRICS ; }
"presorted"         {   lexfdbg("presorted");       return PRESORTED ; }
"groupby"           {   lexfdbg("groupby");         return GROUPBY ; }
"first"             {   lexfdbg("first");           return FIRST ; }
"last"              {   lexfdbg("last");            return LAST ; }
"int_min"           {   lexfdbg("int_min");         return INTMIN ; }
"int_max"           {   lexfdbg("int_max");         return INTMAX ; }
"str_min"           {   lexfdbg("str_min");         return STRMIN ; }
"str_max"           {   lexfdbg("str_max");         return STRMAX ; }
"count"             {   lexfdbg("count");           return COUNT ; }
"count_distinct"    {   lexfdbg("count_distinct");  return COUNT_DISTINCT ; }
"count_changes"     {   lexfdbg("count_changes");   return COUNT_CHANGES ; }
"avg"               {   lexfdbg("avg");             return AVG ; }
"sum"               {   lexfdbg("sum");             return SUM ; }
"strcmp"            {   lexfdbg("STRCMP");          return STRCMP ; }
"position"          {   lexfdbg("STRSTR");          return STRSTR ; }
"length"            {   lexfdbg("LENGTH");          return LENGTH ; }
"strmatch"          {   lexfdbg("STRMATCH");        return STRMATCH ; }
"strcasematch"      {   lexfdbg("STRCASEMATCH");    return STRCASEMATCH ; }
"tolower"           {   lexfdbg("TOLOWER");         return TOLOWER ; }
"normalize"         {   lexfdbg("NORMALIZE");       return NORMALIZE ; }
"skipnormalize"     {   lexfdbg("SKIPNORMALIZE");   return SKIPNORMALIZE ; }
"bytestrip"         {   lexfdbg("BYTESTRIP");       return BYTESTRIP ; }
"iconv"             {   lexfdbg("ICONV");           return ICONV ; }
"cntmatch"          {   lexfdbg("CNTMATCH");        return CNTMATCH ; }
"toupper"           {   lexfdbg("TOUPPER");         return TOUPPER ; }
"pattern"           {   lexfdbg("PATTERN");         return PATTERN ; }
"hash"              {   lexfdbg("hash");            return HASH ; }
"crc48"             {   lexfdbg("crc48");           return CRC48 ; }
"md5"               {   lexfdbg("md5");             return MD5 ; }
"map_value"         {   lexfdbg("mapvalue");        return MAPVALUE ; }
"map_exists"        {   lexfdbg("mapexists");       return MAPEXISTS ; }
"list"              {   lexfdbg("list");            return LIST ; }
"abort"             {   lexfdbg("abort");           return ABORT ; }
"random"            {   lexfdbg("random");          return RANDOM ; }
"linenumber"        {   lexfdbg("linenumber");      return LINENUMBER ; }
"ln"                {   lexfdbg("ln");              return LN ; }
"loge"              {   lexfdbg("loge");            return LN ; }
"log2"              {   lexfdbg("log2");            return LOG2 ; }
"log10"             {   lexfdbg("log10");           return LOG10 ; }
"ldformat"          {   lexfdbg("ldformat");        return LDFORMAT ; }
"llformat"          {   lexfdbg("llformat");        return LLFORMAT ; }
"ceil"              {   lexfdbg("ceil");            return CEIL ; }
"floor"             {   lexfdbg("floor");           return FLOOR ; }
"byte"              {   lexfdbg("byte" );           return BYTE ; }
"dest"              {   lexfdbg("dest" );           return DEST ; }
"from"              {   lexfdbg("from");            return FROM ; }
"with"              {   lexfdbg("with");            return WITH ; }
"buckets"           {   lexfdbg("buckets");         return BUCKETS ; }
"filename"          {   lexfdbg("filename");        return FILENAME ; }
"template"          {   lexfdbg("template");        return TEMPLATE ; }
"delimiter"         {   lexfdbg("delimiter");       return DELIMITER ; }
"configuration"     {   lexfdbg("configuration");   return CONFIGURATION ; }
"inherit"           {   lexfdbg("inherit");         return INHERIT ; }
"configfile"        {   lexfdbg("configfile");      return CONFIGFILE ; }
"command"           {   lexfdbg("command");         return COMMAND ; }
"chomp"             {   lexfdbg("chomp");           return CHOMP ; }
"exec"              {   lexfdbg("exec");            return EXEC ; }
"jobq"              {   lexfdbg("jobq");            return JOBQ ; }
"job"               {   lexfdbg("job");             return JOB ; }
"step"              {   lexfdbg("step");            return STEP ; }
"namespace"         {   lexfdbg("namespace");       return NAMESPACE ; }
"path"              {   lexfdbg("path");            return PATH ; }
"ping"              {   lexfdbg("ping");            return PING ; }
"reconfigure"       {   lexfdbg("reconfigure");     return RECONFIGURE ; }
"touch"             {   lexfdbg("touch");           return TOUCH ; }
"mv"                {   lexfdbg("mv");              return MV ; }
"mvclobber"         {   lexfdbg("mvclobber");       return MVCLOBBER ; }
"rm"                {   lexfdbg("rm");              return RM ; }
"findrm"            {   lexfdbg("findrm");          return FINDRM ; }
"find"              {   lexfdbg("find");            return NSFIND ; }
"persist"           {   lexfdbg("persist");         return PERSIST ; }
"event"             {   lexfdbg("event");           return EVENT ; }
"nanosleep"         {   lexfdbg("nanosleep");       return NANOSLEEP ; }
"pglisten"          {   lexfdbg("pglisten");        return PGLISTEN ; }
"warn"              {   lexfdbg("warn");            return WARN ; }
"info"              {   lexfdbg("info");            return INFO ; }
"xml"               {   lexfdbg("xml");             return XML ; }
"dml"               {   lexfdbg("dml");             return DML ; }
"sprintf"           {   lexfdbg("sprintf");         return SPRINTF ; }
"sqlescape"         {   lexfdbg("sqlescape");       return SQLESCAPE ; }
"while"             {   lexfdbg("while");           return WHILE ; }
"sqlvalue"          {   lexfdbg("sqlvalue");        return SQLVALUE ; }
"coalesce"          {   lexfdbg("coalesce");        return COALESCE ; }
"phrasematch"       {   lexfdbg("phrasematch");     return PHRASEMATCH ; }
"substr"            {   lexfdbg("substr");          return SUBSTR ; }
"factorial"         {   lexfdbg("factorial");       return FACTORIAL ; }
"sequence"          {   lexfdbg("sequence");        return SEQUENCE ; }
"strgreatest"       {   lexfdbg("strgreatest");     return STRGREATEST ; }
"strleast"          {   lexfdbg("strleast");        return STRLEAST ; }
[-]?[0-9][0-9\.]*   {
                        lexfdbgs("NUMBER",yytext);
                        yylval.idc = cplusql_string( yytext );
                        return NUMBER;
                    }
[a-zA-Z][a-zA-Z0-9_]* {
                        lexfdbgs("NAME",yytext);          
                        yylval.idc = cplusql_string( yytext );
                        return NAME ;
                    }
\.\.\.              {   lexfdbg("ELIPSES");         return ELIPSES ;}
\.                  {   lexfdbg("DOT");             return DOT ;}
\(                  {   lexfdbg("LPAREN");          return LPAREN ;}
\)                  {   lexfdbg("RPAREN");          return RPAREN ;}
\{                  {   lexfdbg("LCURLY");          return LCURLY ;}
\}                  {   lexfdbg("RCURLY");          return RCURLY ;}
;                   {   lexfdbg("SEMICOLON");       return SEMICOLON ;}
\=                  {   lexfdbg("EQUALS");          return EQUALS ;}
\!\=                {   lexfdbg("NOTEQUAL");        return NOTEQUAL ;}
\<                  {   lexfdbg("LT");              return LT ;}
\>                  {   lexfdbg("GT");              return GT ;}
~\=                 {   lexfdbg("STR_EQUALS");      return STR_EQUALS ;}
~\<                 {   lexfdbg("STR_LT");          return STR_LT ;}
~\>                 {   lexfdbg("STR_GT");          return STR_GT ;}
~\+                 {   lexfdbg("STR_PLUS");        return STR_PLUS ;}
\*                  {   lexfdbg("MULTIPLY");        return MULTIPLY ;}
\/                  {   lexfdbg("DIVIDE");          return DIVIDE ;}
\%                  {   lexfdbg("MODULO");          return MODULO ;}
\+                  {   lexfdbg("PLUS");            return PLUS ;}
\-                  {   lexfdbg("MINUS");           return MINUS ;}
\|                  {   lexfdbg("BITOR");           return BITOR ; }
\&                  {   lexfdbg("BITAND");          return BITAND ; }
\*\*                {   lexfdbg("POW");             return POW ; }
,                   {   lexfdbg("COMMA");           return COMMA ;}
[\n]                {   lexfdbg("WHITE\\n");        cplusql_new_line(); }
[ \t]+              {   lexfdbg("WHITE");           }

\$\(                {
                        lexfdbg("DOLLAR LPARENS");          
                        cplusql_lex_push_state( 0 );
                        BEGIN CMD_STATE ;
                    }
<CMD_STATE>[^)]+    {
                        //TODO: deal with \)
                        lexfdbgs("CMD",yytext);          
                        cmd_output = cplusql_eval( yytext );
                        if(! cmd_output || !*cmd_output )
                        {
                            fprintf( stderr, "warning: $(%s) produced no output\n",yytext);
                        }
                    }
<CMD_STATE>\)       {
						//discard const property. this is c code, no const_cast
                        lex_putback( (char *)cmd_output );
                        BEGIN cplusql_lex_pop_state();
                    }
\$\{                {
                        lexfdbg("DOLLAR LBRACE");          
                        cplusql_lex_push_state( 0 );
                        BEGIN ENV_STATE ;
                    }
<ENV_STATE>[^}]+    {
                        lexfdbgs("ENV",yytext);          
                        env_val = cplusql_getvar( yytext );
                        if(! env_val || !*env_val )
                        {
                            fprintf( stderr, "warning: ${%s} is empty\n",yytext);
                        }
                    }
<ENV_STATE>\}       {
						//discard const property. this is c code, 
                        //no const_cast
                        lex_putback( (char *)env_val );
                        BEGIN cplusql_lex_pop_state(); 
                    }
<INITIAL>\/\/         {
                        lexfdbg("begin c++ style comment");      
                        BEGIN CMNT_STATE; 
                    }
^[ \t]*#            {   
                        lexfdbg("HASH begin comment");      
                        BEGIN CMNT_STATE; 
                    }
<CMNT_STATE>[^\n]+  {   /* COMMENT */ ; }
<CMNT_STATE>\n      {
                        lexfdbg("end COMMENT at \\n");      
                        cplusql_new_line();
                        BEGIN 0; 
                    }
["]                 {
                        lexfdbg("QUOTE start");       
                        cplusql_lex_push_state( 0 );
                        BEGIN QUOTE_STATE; 
                        qcharlen=0;
                        memset( varbuf, '\0', VARBUFLEN );
                    }
<QUOTE_STATE>\$\(   {
                        lexfdbg("DOLLAR LPAREN in quote state");          
                        cplusql_lex_push_state( QUOTE_STATE );
                        BEGIN CMD_STATE ;
                    }
<QUOTE_STATE>\$\{   {
                        lexfdbg("DOLLAR LBRACE in quote state");          
                        cplusql_lex_push_state( QUOTE_STATE );
                        BEGIN ENV_STATE ;
                    }
<QUOTE_STATE>\n     {
                        lexfdbg("QUOTED_STRING \\n" ); 
                        lex_pushqchar( *yytext );
                        cplusql_new_line();
                    }
<QUOTE_STATE>[^"\n] {
                        lexfdbgs("QUOTED_STRING", yytext); 
                        lex_pushqchar( *yytext );
                    }
<QUOTE_STATE>["]    {
                        lexfdbg("QUOTE end");       
                        BEGIN cplusql_lex_pop_state(); 
                        yylval.idc = cplusql_string( varbuf );
                        return QUOTED_STRING;
                    }
<QUOTE_STATE>\\["]  {   lexfdbg("backslash double quote");       lex_pushqchar( 0x22 ); }
<QUOTE_STATE>\\\(  {   lexfdbg("backslash lparen");  lex_pushqchar( 0x28 ); }
<QUOTE_STATE>\\\\   {   lexfdbg("backslash");       lex_pushqchar( (char)0x5c ); }

<<EOF>>             { 
    if( ! buffers )
    {
        lexfdbg( "nothing left to lex\n" );
        return 0;
    }
    lexfdbg( "popping another buffer\n" );
    yy_delete_buffer(YY_CURRENT_BUFFER);
    ybuflist_t *discard = buffers ;
    buffers = discard->next ;
    YY_BUFFER_STATE prevbuf = discard->buf ;
    free( discard );
    yy_switch_to_buffer( prevbuf );
}



%%
/*
int main()
{
    yylex();
}
*/

int yywrap()
{
    return 1;
}

void lex_putback( char *s )
{
    char *i;
    if(s)
    {
        for( 
            i=s+strlen(s)-1; 
            i >= s;
            --i 
        )
        {
            lexfdbgs("UNPUT",i);          
            unput(*i);
        }
    }
}

void lex_pushqchar( char c )
{
    varbuf[ qcharlen ] = c;
    if( ++ qcharlen == VARBUFLEN )
    {
        cplusql_puke("exceeded max cplusql quoted string length. if you really intended to have a string that long, edit cplusql.h, increase VARBUFLEN, and recompile." );
    }
    lexfdbgs("PUSHQCHAR", varbuf );          
}
