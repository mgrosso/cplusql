// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      cplusq.h
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
// $Id: cplusql_api.h,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CPLUSQL_H
#define CPLUSQL_H 1
#ifdef __cplusplus
extern "C" {
#endif

#define VARBUFLEN 8192

#define CPLUSQL_NAMESPACE  "NAMESPACE"
#define CPLUSQL_NAMESPACE_DFLT  "default"
#define CPLUSQL_USE_FILE_SYSTEM "file"

#define CPLUSQL_READ_BUFSIZE "CPLUSQL_READ_BUFSIZE"
#define CPLUSQL_READ_BUFSIZE_DFLT "262144"

#define CPLUSQL_MAX_BAD_PARSE "CPLUSQL_MAX_BAD_PARSE"
#define CPLUSQL_MAX_BAD_PARSE_DFLT "0"

#define CPLUSQL_DELIMITER "DELIMITER"
#define CPLUSQL_DELIMITER_DFLT "|"

#define CPLUSQL_NOTHREADS "CPLUSQL_NOTHREADS"
#define CPLUSQL_NOTHREADS_DFLT "0"

typedef void * cplusql_id_t;

void cplusql_puke( const char *s );

//strings, and lists of strings
char * cplusql_string( const char *s );
void   cplusql_free_string( char *s );
cplusql_id_t cplusql_string_set( cplusql_id_t charvec, char *str );
cplusql_id_t cplusql_new_string_set( char * str );

//config functions
char * cplusql_eval( const char *command );
const char * cplusql_getvar( const char *k );
const char * cplusql_getvar_wdflt( const char *k, const char *dflt );
cplusql_id_t cplusql_add_config( char *name, cplusql_id_t cfg );
cplusql_id_t cplusql_empty_config();
cplusql_id_t cplusql_start_config( char *k, char *v );
cplusql_id_t cplusql_addto_config( cplusql_id_t cfg, char *k, char *v );


cplusql_id_t cplusql_copy_named_cfg( char *name );
cplusql_id_t cplusql_merge_cfg( cplusql_id_t lhs, cplusql_id_t rhs );
cplusql_id_t cplusql_default_cfg();
cplusql_id_t cplusql_addto_default_cfg( cplusql_id_t addme );
int cplusql_init_default_cfg( int *argc, char **argv );
cplusql_id_t cplusql_cfg_file( char *filename );

cplusql_id_t cplusql_foreach( char *varname, cplusql_id_t values, char *template_code);

//various expressions
cplusql_id_t cplusql_expression( cplusql_id_t i );
cplusql_id_t cplusql_column_exp_str( char * rel, char * col );
cplusql_id_t cplusql_column_exp_str_dflt_rel( char * col );
cplusql_id_t cplusql_literal_exp( char * str );
cplusql_id_t cplusql_not( cplusql_id_t rhs );
cplusql_id_t cplusql_binary_exp( cplusql_id_t lhs, int opr, cplusql_id_t rhs );
cplusql_id_t cplusql_ifthen_exp( 
    cplusql_id_t conditional, cplusql_id_t texp, cplusql_id_t fexp 
);
cplusql_id_t cplusql_hash_exp( cplusql_id_t column, char *buckets );
cplusql_id_t cplusql_crc48_exp( cplusql_id_t column );
cplusql_id_t cplusql_md5_exp( cplusql_id_t column );
cplusql_id_t cplusql_ceil_exp( cplusql_id_t column );
cplusql_id_t cplusql_floor_exp( cplusql_id_t column );

cplusql_id_t cplusql_unary_metric( int metric, cplusql_id_t expression );
cplusql_id_t cplusql_metric_sum( cplusql_id_t expression );
cplusql_id_t cplusql_metric_int_min( cplusql_id_t expression );
cplusql_id_t cplusql_metric_int_max( cplusql_id_t expression );
cplusql_id_t cplusql_metric_count_distinct( cplusql_id_t expression );
cplusql_id_t cplusql_metric_count_changes( cplusql_id_t expression );
cplusql_id_t cplusql_metric_count();
cplusql_id_t cplusql_metric_first( cplusql_id_t expression );
cplusql_id_t cplusql_metric_last( cplusql_id_t expression );

cplusql_id_t cplusql_metric_string_min( cplusql_id_t expression );
cplusql_id_t cplusql_metric_string_max( cplusql_id_t expression );
cplusql_id_t cplusql_metric_avg( cplusql_id_t expression );

//user columns
cplusql_id_t cplusql_user_column_def( char *name );
cplusql_id_t cplusql_user_column( char *name, cplusql_id_t expression );
cplusql_id_t cplusql_parsed_columns_nexp( 
    char* relname, cplusql_id_t expression, 
    int strict, char *delim, cplusql_id_t column_names
);
cplusql_id_t cplusql_parsed_columns_exp( 
    cplusql_id_t expression, 
    int strict, char *delim, cplusql_id_t column_names
);
cplusql_id_t cplusql_parsed_columns_regex( 
    cplusql_id_t expression, 
    char *pattern,
    cplusql_id_t case_insensitive,
    cplusql_id_t column_names
);
cplusql_id_t cplusql_parsed_columns_regexlist( 
    cplusql_id_t expression, 
    cplusql_id_t patterns, 
    cplusql_id_t case_insensitive,
    cplusql_id_t column_names
);

cplusql_id_t cplusql_ldformat( char *format, cplusql_id_t expression);
cplusql_id_t cplusql_llformat( char *format, cplusql_id_t expression);
cplusql_id_t cplusql_ll( cplusql_id_t expression);
cplusql_id_t cplusql_ftrans_exp( cplusql_id_t expression, int (*f)(int));

cplusql_id_t cplusql_named_metric( char *name, cplusql_id_t metric );


cplusql_id_t cplusql_command( 
    char * name, 
    char *command, 
    cplusql_id_t config 
);
cplusql_id_t cplusql_add_command( cplusql_id_t command );
cplusql_id_t cplusql_run_command( cplusql_id_t command );

cplusql_id_t cplusql_jobq( 
	char * jobqname, 
	cplusql_id_t config 
);

cplusql_id_t cplusql_job( 
	char * jobname, 
	char *jobqname, 
	char *cmd,
	cplusql_id_t config 
);

cplusql_id_t cplusql_namespace( 
	char * name, 
	cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_ping( char * name);

cplusql_id_t cplusql_namespace_reconfigure( 
    char * name, 
    cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_mv( 
    char * name, 
    char * from, 
    char * to, 
    cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_mvclobber( 
    char * name, 
    char * from, 
    char * to, 
    cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_rm( 
    char * name, 
    char * file, 
    cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_findrm( 
    char * name, 
    char * path, 
    char * pattern, 
    cplusql_id_t config 
);

cplusql_id_t cplusql_namespace_touch( 
    char * name, 
    char * newfile, 
    cplusql_id_t config 
);

cplusql_id_t
cplusql_namespace_find(
    char *name,
    char *path,
    char *pattern,
    cplusql_id_t config 
);

//destinations
cplusql_id_t cplusql_dest_hash( 
    char * source,
    char * filetemplate, 
    cplusql_id_t hashexp,
    cplusql_id_t config
);
cplusql_id_t cplusql_dest_file( 
    char * source,
    char * filename,
    cplusql_id_t config
);

cplusql_id_t cplusql_dest_new_table( 
    char * source,
    char * dbconn,
    cplusql_id_t config
);

cplusql_id_t cplusql_dest_table( 
    char * source,
    char * dbconn,
    cplusql_id_t config
);

//relations
char * cplusql_src_relation( char * name );
char * cplusql_src_ads_map( char * name );

//makes a relation persistent
cplusql_id_t 
cplusql_persist( char *to, char *from );

cplusql_id_t
cplusql_hashed_table(
	char * name,
	char * filetemplate,
	char * querytemplate,
	char * buckets,
    cplusql_id_t columns,
    cplusql_id_t config
);

cplusql_id_t
cplusql_db_conn( char * name,
				char * user,
				char * pass,
				char * url,
    			cplusql_id_t config
);

cplusql_id_t cplusql_run_sql( 
        char *dbconnection,
        char *query,
        cplusql_id_t config
);
cplusql_id_t cplusql_add_sql( cplusql_id_t sqlcommand);
cplusql_id_t cplusql_sql( 
        char *dbconnection,
        char *query,
        cplusql_id_t config
);

cplusql_id_t
cplusql_relation_db( char * name,
				char * dbconn,
				char * query,
				cplusql_id_t columns,
				cplusql_id_t config
);

cplusql_id_t
cplusql_relation_pgcopy( 
                char * name,
				char * dbconn,
				char * table,
				cplusql_id_t columns,
				cplusql_id_t config
);

cplusql_id_t
cplusql_relation_pgcopy_short( 
                char * name,
				char * dbconn,
				char * table,
				cplusql_id_t config
);

cplusql_id_t
cplusql_event_stream_nanosleep( 
                char * name,
				char * secs,
				char * nsecs,
				cplusql_id_t config
);

cplusql_id_t
cplusql_nanosleep_exp( 
				char * secs,
				char * nsecs
);

cplusql_id_t
cplusql_relation_exp(
                char * name,
				cplusql_id_t usercolumnset,
				cplusql_id_t whiletrue,
				cplusql_id_t config
);

cplusql_id_t
cplusql_pglisten_exp(
                char * db_name,
				char * event_name
);

cplusql_id_t
cplusql_coalesce_exp( cplusql_id_t expargs );

cplusql_id_t
cplusql_strgreatest_exp( cplusql_id_t expargs );

cplusql_id_t
cplusql_strleast_exp( cplusql_id_t expargs );

cplusql_id_t
cplusql_sqlvalue_exp(
                char * db_name,
				cplusql_id_t sql
);

cplusql_id_t
cplusql_relation_xmlquery( 
                char * name,
				char * event_source,
				char * xpath_query,
				cplusql_id_t columns,
				cplusql_id_t xml,
				cplusql_id_t config
);

cplusql_id_t
cplusql_relation_dbrepeat( 
                char * name,
				char * event_source,
				char * db_name,
				cplusql_id_t query_exp,
				cplusql_id_t columns
);

cplusql_id_t
cplusql_dml_exp( 
    char *dbname,
    char *numrows,
    cplusql_id_t beginsql,
    cplusql_id_t sql,
    cplusql_id_t commitsql
);


cplusql_id_t
cplusql_sprintf_exp( char *format, cplusql_id_t values );

cplusql_id_t
cplusql_sqlescape_exp( cplusql_id_t operand );

cplusql_id_t
cplusql_byte_source( 
    char *name,
    cplusql_id_t filenames,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_fileset( 
    char *name,
    cplusql_id_t filenames,
    char *delimiter,
    cplusql_id_t columns,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_pattern( 
    char *name,
    char *prefix,
    char *pattern,
    char *delimiter,
    cplusql_id_t columns,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_from( 
    char *name,
    char *from,
    cplusql_id_t usercolumns,
    cplusql_id_t parsedcolumns,
    cplusql_id_t where_clause ,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_presorted( 
    char *name,
    char *from,
    cplusql_id_t projection_columns,
    cplusql_id_t metrics,
    cplusql_id_t where_clause ,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_indexscan( 
    char *name,
    char *from,
    char *index,
    cplusql_id_t config
);

cplusql_id_t 
cplusql_relation_mergejoin( 
    char *name,
    char *left,
    cplusql_id_t leftexp,
    char *right,
    cplusql_id_t rightexp,
    cplusql_id_t where,
    cplusql_id_t config
);

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
);

cplusql_id_t
cplusql_index(
    char *name,
    char *from,
    cplusql_id_t key,
    cplusql_id_t where,
    cplusql_id_t config
);


cplusql_id_t 
cplusql_map_from_relation( 
    char *name,
    char *from,
    cplusql_id_t key_expresion,
    cplusql_id_t val_expresion
);

cplusql_id_t 
cplusql_map_from_config( 
    char *name,
    cplusql_id_t config
);

cplusql_id_t
cplusql_map_from_ads_map(
    char *name,
    char *from,
    char *col
);

cplusql_id_t 
cplusql_map_value_l( 
    char *name,
    cplusql_id_t list,
    cplusql_id_t dflt
);

cplusql_id_t 
cplusql_map_value( 
    char *name,
    cplusql_id_t key,
    cplusql_id_t dflt
);

cplusql_id_t 
cplusql_map_exists_l( 
    char *name,
    cplusql_id_t list
);

cplusql_id_t 
cplusql_map_exists( 
    char *name,
    cplusql_id_t key
);
cplusql_id_t        
cplusql_abort( cplusql_id_t expression );
cplusql_id_t        
cplusql_warn_exp( cplusql_id_t expression );
cplusql_id_t        
cplusql_info_exp( cplusql_id_t expression );

cplusql_id_t        
cplusql_random( cplusql_id_t minexp, cplusql_id_t maxexp );

cplusql_id_t        
cplusql_line_number( char *name );

cplusql_id_t cplusql_join_columns(
	cplusql_id_t column_names, char *delim
);
cplusql_id_t cplusql_add_expression(
	cplusql_id_t list,
    cplusql_id_t exp
);
cplusql_id_t cplusql_new_expression_list(
	cplusql_id_t exp
);

cplusql_id_t
cplusql_regex_match_exp( 
    char *pattern, cplusql_id_t rhs, int case_insensitive 
);

cplusql_id_t
cplusql_strstr_exp(
    cplusql_id_t haystack,
	cplusql_id_t needle
);

cplusql_id_t
cplusql_substr_exp(
    cplusql_id_t operand,
	cplusql_id_t offset,
	cplusql_id_t length
);

cplusql_id_t
cplusql_cntmatch_exp( cplusql_id_t words, cplusql_id_t operand );

cplusql_id_t
cplusql_parsed_matchlist(
    cplusql_id_t matchlist,
	cplusql_id_t operand,
    cplusql_id_t column_names
);

cplusql_id_t
cplusql_length_exp( cplusql_id_t operand );

cplusql_id_t
cplusql_bytestrip_exp( 
    cplusql_id_t operand, 
    char *squeeze,
    char *skip,
    char *from,
    char *to
    );

cplusql_id_t
cplusql_normalize_exp( 
    cplusql_id_t operand, 
    cplusql_id_t stop_prefixes, 
    cplusql_id_t stop_words,
    int skip);

cplusql_id_t
cplusql_factorial_exp( cplusql_id_t operand );

cplusql_id_t
cplusql_sequence_exp( 
    cplusql_id_t start, 
    cplusql_id_t increment, 
    cplusql_id_t reset);

cplusql_id_t
cplusql_iconv_exp( 
    cplusql_id_t operand, 
    char *to,
    char *from
    );

cplusql_id_t
cplusql_command_exp( 
    cplusql_id_t operand, 
    cplusql_id_t config
    );

cplusql_id_t
cplusql_chomp_exp( cplusql_id_t operand );

cplusql_id_t
cplusql_range_string_set( char *from, char *to );
cplusql_id_t
cplusql_range_step_string_set( char *from, char *to, char *step );

cplusql_id_t
cplusql_echo( cplusql_id_t exp );

cplusql_id_t
cplusql_exec( cplusql_id_t exp );

cplusql_id_t
cplusql_command_string_set(cplusql_id_t exp );

//called from cplusql.y
void        cplusql_sync();
void        cplusql_yyparse();
void        cplusql_run();
unsigned    cplusql_get_line();

//called from cplusql.l
void        cplusql_new_line();
void        cplusql_lex_push_state( int state );
int         cplusql_lex_pop_state();

//the corresponding pop() is local to flex code.
void        push_lex_buffer( const char *s );

//implemented by cplusql.y code
void callback_yyparse();
int  get_yacdbg();
void set_yacdbg( int v );
//implemented by cplusql.l code
void set_lexdbg( int v );
void lex_putback( char *s );

#ifdef __cplusplus
}
#endif /* __cplusplus extern "C" {} */
#endif /* CPLUSQL_H */
