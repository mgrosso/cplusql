/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IF = 258,
     OR = 259,
     AND = 260,
     NOT = 261,
     GT = 262,
     LT = 263,
     NOTEQUAL = 264,
     EQUALS = 265,
     STR_GT = 266,
     STR_LT = 267,
     STR_EQUALS = 268,
     BITOR = 269,
     BITAND = 270,
     MINUS = 271,
     PLUS = 272,
     STR_PLUS = 273,
     MODULO = 274,
     DIVIDE = 275,
     MULTIPLY = 276,
     POW = 277,
     LPAREN = 278,
     RPAREN = 279,
     LCURLY = 280,
     RCURLY = 281,
     SEMICOLON = 282,
     COMMA = 283,
     DLOPEN = 284,
     CREATE = 285,
     STREAM = 286,
     USING = 287,
     FILES = 288,
     COLUMNS = 289,
     FILTERS = 290,
     MERGEJOIN = 291,
     OF = 292,
     TO = 293,
     WHERE = 294,
     USERCOLUMN = 295,
     DEST = 296,
     FROM = 297,
     HASH = 298,
     CRC48 = 299,
     MD5 = 300,
     CEIL = 301,
     FLOOR = 302,
     WITH = 303,
     BUCKETS = 304,
     FILENAME = 305,
     TEMPLATE = 306,
     COLUMN = 307,
     STRCMP = 308,
     THEN = 309,
     ELSE = 310,
     DOT = 311,
     ELIPSES = 312,
     DELIMITER = 313,
     CONFIGURATION = 314,
     INHERIT = 315,
     CONFIGFILE = 316,
     PARSE = 317,
     INTO = 318,
     STRICT = 319,
     SYNC = 320,
     MAP = 321,
     MAPVALUE = 322,
     MAPEXISTS = 323,
     ABORT = 324,
     PRESORTED = 325,
     GROUPBY = 326,
     INTMIN = 327,
     INTMAX = 328,
     STRMIN = 329,
     STRMAX = 330,
     FIRST = 331,
     LAST = 332,
     AVG = 333,
     COUNT = 334,
     SUM = 335,
     METRICS = 336,
     COUNT_DISTINCT = 337,
     COUNT_CHANGES = 338,
     JOIN = 339,
     RANDOM = 340,
     LINENUMBER = 341,
     DBCONNECTION = 342,
     XML = 343,
     QUERY = 344,
     HASHED_TABLE = 345,
     LIST = 346,
     COMMAND = 347,
     CHOMP = 348,
     JOB = 349,
     JOBQ = 350,
     STEP = 351,
     NAMESPACE = 352,
     STRMATCH = 353,
     STRCASEMATCH = 354,
     PATTERN = 355,
     PATH = 356,
     TOUCH = 357,
     MV = 358,
     RM = 359,
     FINDRM = 360,
     NSFIND = 361,
     PING = 362,
     RECONFIGURE = 363,
     TOLOWER = 364,
     TOUPPER = 365,
     TABLE = 366,
     NEW = 367,
     MVCLOBBER = 368,
     BYTE = 369,
     PERSIST = 370,
     MERGE = 371,
     IN = 372,
     MEMORY = 373,
     INDEX = 374,
     OUTER = 375,
     SORT = 376,
     FOREACH = 377,
     DEBUGECHO = 378,
     EXEC = 379,
     LN = 380,
     LOG2 = 381,
     LOG10 = 382,
     LLFORMAT = 383,
     LDFORMAT = 384,
     STRSTR = 385,
     LENGTH = 386,
     SKIPNORMALIZE = 387,
     NORMALIZE = 388,
     BYTESTRIP = 389,
     CNTMATCH = 390,
     PHRASEMATCH = 391,
     ICONV = 392,
     NANOSLEEP = 393,
     EVENT = 394,
     PGLISTEN = 395,
     WARN = 396,
     DML = 397,
     SQLESCAPE = 398,
     SPRINTF = 399,
     SQLVALUE = 400,
     COALESCE = 401,
     WHILE = 402,
     SUBSTR = 403,
     FACTORIAL = 404,
     SEQUENCE = 405,
     STRGREATEST = 406,
     STRLEAST = 407,
     INFO = 408,
     NUMBER = 409,
     QUOTED_STRING = 410,
     NAME = 411
   };
#endif
#define IF 258
#define OR 259
#define AND 260
#define NOT 261
#define GT 262
#define LT 263
#define NOTEQUAL 264
#define EQUALS 265
#define STR_GT 266
#define STR_LT 267
#define STR_EQUALS 268
#define BITOR 269
#define BITAND 270
#define MINUS 271
#define PLUS 272
#define STR_PLUS 273
#define MODULO 274
#define DIVIDE 275
#define MULTIPLY 276
#define POW 277
#define LPAREN 278
#define RPAREN 279
#define LCURLY 280
#define RCURLY 281
#define SEMICOLON 282
#define COMMA 283
#define DLOPEN 284
#define CREATE 285
#define STREAM 286
#define USING 287
#define FILES 288
#define COLUMNS 289
#define FILTERS 290
#define MERGEJOIN 291
#define OF 292
#define TO 293
#define WHERE 294
#define USERCOLUMN 295
#define DEST 296
#define FROM 297
#define HASH 298
#define CRC48 299
#define MD5 300
#define CEIL 301
#define FLOOR 302
#define WITH 303
#define BUCKETS 304
#define FILENAME 305
#define TEMPLATE 306
#define COLUMN 307
#define STRCMP 308
#define THEN 309
#define ELSE 310
#define DOT 311
#define ELIPSES 312
#define DELIMITER 313
#define CONFIGURATION 314
#define INHERIT 315
#define CONFIGFILE 316
#define PARSE 317
#define INTO 318
#define STRICT 319
#define SYNC 320
#define MAP 321
#define MAPVALUE 322
#define MAPEXISTS 323
#define ABORT 324
#define PRESORTED 325
#define GROUPBY 326
#define INTMIN 327
#define INTMAX 328
#define STRMIN 329
#define STRMAX 330
#define FIRST 331
#define LAST 332
#define AVG 333
#define COUNT 334
#define SUM 335
#define METRICS 336
#define COUNT_DISTINCT 337
#define COUNT_CHANGES 338
#define JOIN 339
#define RANDOM 340
#define LINENUMBER 341
#define DBCONNECTION 342
#define XML 343
#define QUERY 344
#define HASHED_TABLE 345
#define LIST 346
#define COMMAND 347
#define CHOMP 348
#define JOB 349
#define JOBQ 350
#define STEP 351
#define NAMESPACE 352
#define STRMATCH 353
#define STRCASEMATCH 354
#define PATTERN 355
#define PATH 356
#define TOUCH 357
#define MV 358
#define RM 359
#define FINDRM 360
#define NSFIND 361
#define PING 362
#define RECONFIGURE 363
#define TOLOWER 364
#define TOUPPER 365
#define TABLE 366
#define NEW 367
#define MVCLOBBER 368
#define BYTE 369
#define PERSIST 370
#define MERGE 371
#define IN 372
#define MEMORY 373
#define INDEX 374
#define OUTER 375
#define SORT 376
#define FOREACH 377
#define DEBUGECHO 378
#define EXEC 379
#define LN 380
#define LOG2 381
#define LOG10 382
#define LLFORMAT 383
#define LDFORMAT 384
#define STRSTR 385
#define LENGTH 386
#define SKIPNORMALIZE 387
#define NORMALIZE 388
#define BYTESTRIP 389
#define CNTMATCH 390
#define PHRASEMATCH 391
#define ICONV 392
#define NANOSLEEP 393
#define EVENT 394
#define PGLISTEN 395
#define WARN 396
#define DML 397
#define SQLESCAPE 398
#define SPRINTF 399
#define SQLVALUE 400
#define COALESCE 401
#define WHILE 402
#define SUBSTR 403
#define FACTORIAL 404
#define SEQUENCE 405
#define STRGREATEST 406
#define STRLEAST 407
#define INFO 408
#define NUMBER 409
#define QUOTED_STRING 410
#define NAME 411




/* Copy the first part of user declarations.  */
#line 1 "cplusql_grammar.y"
 
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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 74 "cplusql_grammar.y"
typedef union YYSTYPE {
    char * idc;
    void * idp;
    int    id ;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 467 "cplusql_grammar.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 479 "cplusql_grammar.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  122
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2532

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  157
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  53
/* YYNRULES -- Number of rules. */
#define YYNRULES  217
/* YYNRULES -- Number of states. */
#define YYNSTATES  714

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   411

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    47,    49,    56,    59,    62,    69,    76,
      83,    85,    90,    99,   104,   110,   114,   119,   126,   133,
     139,   148,   157,   167,   174,   182,   194,   206,   211,   213,
     215,   216,   218,   222,   225,   228,   230,   233,   234,   238,
     244,   246,   248,   256,   261,   270,   282,   297,   313,   323,
     335,   346,   355,   363,   374,   384,   397,   405,   416,   417,
     419,   430,   432,   434,   435,   438,   445,   453,   461,   469,
     470,   472,   473,   476,   477,   480,   484,   487,   491,   492,
     495,   499,   502,   505,   508,   509,   512,   516,   519,   525,
     533,   536,   540,   542,   546,   549,   553,   555,   564,   572,
     582,   593,   595,   599,   604,   606,   610,   614,   618,   622,
     626,   630,   634,   638,   642,   646,   650,   654,   658,   662,
     666,   670,   674,   678,   685,   690,   697,   702,   709,   714,
     721,   725,   727,   729,   731,   733,   740,   745,   750,   760,
     769,   777,   784,   789,   793,   796,   805,   812,   817,   822,
     827,   834,   847,   856,   865,   874,   879,   886,   893,   900,
     905,   910,   917,   922,   929,   935,   940,   945,   950,   969,
     975,   980,   984,   991,   998,  1005,  1010,  1019,  1024,  1033,
    1038,  1043,  1055,  1065,  1078,  1089,  1093,  1098,  1100,  1102,
    1104,  1106,  1108,  1110,  1112,  1114,  1116,  1118
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     158,     0,    -1,   159,    -1,   158,   159,    -1,   161,    -1,
     160,    27,    -1,   181,    -1,   182,    -1,   184,    -1,   202,
      -1,   175,    -1,   167,    -1,   172,    -1,   173,    -1,   174,
      -1,   168,    -1,   169,    -1,   170,    -1,   171,    -1,   162,
      -1,   164,    -1,   163,    -1,   166,    -1,   165,    -1,   122,
     156,   117,   198,   155,    27,    -1,   123,   205,    -1,   124,
     205,    -1,    30,    92,   156,    32,   155,   176,    -1,   124,
      89,    32,   156,   155,   176,    -1,    30,    89,    32,   156,
     155,   176,    -1,    65,    -1,    30,    95,   156,   176,    -1,
      30,    94,   156,    32,   156,    92,   155,   176,    -1,    30,
      97,   156,   176,    -1,    97,   156,   102,   155,   176,    -1,
      97,   156,   107,    -1,    97,   156,   108,   176,    -1,    97,
     156,   103,   155,   155,   176,    -1,    97,   156,   113,   155,
     155,   176,    -1,    97,   156,   104,   155,   176,    -1,    97,
     156,   105,   101,   155,   100,   155,   176,    -1,    97,   156,
     106,   101,   155,   100,   155,   176,    -1,    30,    66,   156,
      32,   186,    42,   205,    38,   205,    -1,    30,    66,   156,
      32,    59,   178,    -1,    30,    66,   156,    32,   185,    38,
     156,    -1,    30,    87,   156,    23,   155,    28,   155,    28,
     155,    24,   176,    -1,    30,    90,   156,    51,   155,    89,
     155,    49,   154,   196,   176,    -1,    30,    59,   156,   178,
      -1,   179,    -1,   177,    -1,    -1,   178,    -1,    23,   179,
      24,    -1,    61,   155,    -1,    60,   156,    -1,   177,    -1,
     178,   177,    -1,    -1,   156,    10,   180,    -1,   179,    28,
     156,    10,   180,    -1,   155,    -1,   154,    -1,    30,   114,
      31,   156,    32,   195,   176,    -1,   115,   156,    42,   156,
      -1,    30,    31,   156,    32,   195,   190,   196,   176,    -1,
      30,    31,   156,    32,   101,   155,   100,   155,   190,   196,
     176,    -1,    30,    31,   156,    32,   116,    84,    37,   186,
     205,    38,   186,   205,   197,   176,    -1,    30,    31,   156,
      32,   183,    84,    37,   186,   205,    38,   186,   119,   156,
     197,   176,    -1,    30,    31,   156,    32,   186,   191,   187,
     197,   176,    -1,    30,    31,   156,    32,   186,    70,    71,
     196,   193,   197,   176,    -1,    30,    31,   156,    32,    87,
     156,    89,   155,   196,   176,    -1,    30,    31,   156,    32,
     156,   119,   156,   176,    -1,    30,    31,   156,    32,    66,
     156,   176,    -1,    30,    31,   156,    32,    87,   156,   111,
     155,   196,   176,    -1,    30,    31,   156,    32,    87,   156,
     111,   155,   176,    -1,    30,    31,   156,    32,   186,    88,
      89,   155,   196,    42,   205,   176,    -1,    30,    31,   156,
     191,   147,   205,   176,    -1,    30,    31,   156,    32,   186,
      87,   156,    89,   205,   196,    -1,    -1,   120,    -1,    30,
     119,   156,    32,   186,   121,    32,   205,   197,   176,    -1,
     156,    -1,   156,    -1,    -1,   187,   188,    -1,    62,   205,
     189,   190,    63,   196,    -1,    62,   205,   100,   155,   176,
      63,   196,    -1,    62,   205,   100,   198,   176,    63,   196,
      -1,    62,   205,    32,   136,   198,    63,   196,    -1,    -1,
      64,    -1,    -1,    58,   155,    -1,    -1,    40,   192,    -1,
     191,    28,   192,    -1,   156,   205,    -1,    23,   156,    24,
      -1,    -1,    81,   194,    -1,   193,    28,   194,    -1,   156,
     207,    -1,    33,   198,    -1,    34,   200,    -1,    -1,    39,
     205,    -1,    23,   199,    24,    -1,    23,    24,    -1,    23,
     154,    57,   154,    24,    -1,    23,   154,    57,   154,    96,
     154,    24,    -1,    92,   205,    -1,   199,    28,   155,    -1,
     155,    -1,    23,   201,    24,    -1,    23,    24,    -1,   201,
      28,   156,    -1,   156,    -1,    30,    41,    42,   186,    51,
     155,   205,   176,    -1,    30,    41,    42,   156,    50,   155,
     176,    -1,    30,    41,    42,   156,    38,   111,    32,   156,
     176,    -1,    30,    41,    42,   156,    38,   112,   111,    32,
     156,   176,    -1,   205,    -1,   203,    28,   205,    -1,   209,
      23,   203,    24,    -1,   204,    -1,   205,     8,   205,    -1,
     205,     7,   205,    -1,   205,     5,   205,    -1,   205,     4,
     205,    -1,   205,    10,   205,    -1,   205,     9,   205,    -1,
     205,    21,   205,    -1,   205,    20,   205,    -1,   205,    19,
     205,    -1,   205,    17,   205,    -1,   205,    16,   205,    -1,
     205,    15,   205,    -1,   205,    14,   205,    -1,   205,    22,
     205,    -1,   205,    18,   205,    -1,   205,    12,   205,    -1,
     205,    11,   205,    -1,   205,    13,   205,    -1,   125,    23,
     205,    28,   205,    24,    -1,   125,    23,   205,    24,    -1,
     126,    23,   205,    28,   205,    24,    -1,   126,    23,   205,
      24,    -1,   127,    23,   205,    28,   205,    24,    -1,   127,
      23,   205,    24,    -1,    53,    23,   205,    28,   205,    24,
      -1,   156,    56,   156,    -1,   156,    -1,   155,    -1,   154,
      -1,   206,    -1,    43,    23,   205,    28,   154,    24,    -1,
      45,    23,   205,    24,    -1,    44,    23,   205,    24,    -1,
      67,    23,   156,    28,    91,   203,    28,   205,    24,    -1,
      67,    23,   156,    28,   205,    28,   205,    24,    -1,    68,
      23,   156,    28,    91,   203,    24,    -1,    68,    23,   156,
      28,   205,    24,    -1,    69,    23,   205,    24,    -1,    23,
     205,    24,    -1,     6,   205,    -1,    84,    23,    23,   203,
      24,    28,   155,    24,    -1,    85,    23,   205,    28,   205,
      24,    -1,    86,    23,   156,    24,    -1,    46,    23,   205,
      24,    -1,    47,    23,   205,    24,    -1,   135,    23,   198,
      28,   205,    24,    -1,   134,    23,   205,    28,   155,    28,
     155,    28,   155,    28,   155,    24,    -1,   133,    23,   205,
      28,   198,    28,   198,    24,    -1,   132,    23,   205,    28,
     198,    28,   198,    24,    -1,   137,    23,   205,    28,   155,
      28,   155,    24,    -1,   131,    23,   205,    24,    -1,   130,
      23,   205,    28,   205,    24,    -1,    98,    23,   155,    28,
     205,    24,    -1,    99,    23,   155,    28,   205,    24,    -1,
     109,    23,   205,    24,    -1,   110,    23,   205,    24,    -1,
     128,    23,   155,    28,   205,    24,    -1,   128,    23,   205,
      24,    -1,   129,    23,   155,    28,   205,    24,    -1,    92,
      23,   205,   176,    24,    -1,    93,    23,   205,    24,    -1,
     141,    23,   205,    24,    -1,   153,    23,   205,    24,    -1,
     142,    23,   156,    28,   154,    28,    23,   203,    24,    28,
      23,   203,    24,    28,    23,   203,    24,    24,    -1,   144,
      23,   155,   203,    24,    -1,   143,    23,   205,    24,    -1,
     138,   154,   154,    -1,   138,    23,   154,    28,   154,    24,
      -1,   140,    23,   156,    28,   156,    24,    -1,   145,    23,
     156,    28,   205,    24,    -1,   146,    23,   203,    24,    -1,
     148,    23,   205,    28,   205,    28,   205,    24,    -1,   149,
      23,   205,    24,    -1,   150,    23,   205,    28,   205,    28,
     205,    24,    -1,   151,    23,   203,    24,    -1,   152,    23,
     203,    24,    -1,     3,    23,   205,    24,    25,   205,    26,
      55,    25,   205,    26,    -1,     3,    23,   205,    24,    25,
     205,    26,    55,   206,    -1,     3,    23,   205,    24,    54,
      25,   205,    26,    55,    25,   205,    26,    -1,     3,    23,
     205,    24,    54,    25,   205,    26,    55,   206,    -1,   208,
      23,    24,    -1,   209,    23,   205,    24,    -1,    79,    -1,
      72,    -1,    73,    -1,    74,    -1,    75,    -1,    76,    -1,
      77,    -1,    78,    -1,    82,    -1,    83,    -1,    80,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   106,   106,   110,   117,   118,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   142,   152,   161,   170,   178,   186,
     194,   202,   210,   218,   226,   232,   238,   244,   250,   256,
     262,   269,   278,   284,   290,   298,   306,   314,   320,   325,
     332,   334,   342,   348,   355,   362,   367,   371,   372,   373,
     380,   381,   385,   393,   399,   406,   413,   421,   428,   435,
     442,   449,   455,   461,   468,   475,   482,   488,   497,   502,
     511,   518,   527,   545,   546,   550,   556,   562,   568,   575,
     575,   578,   583,   590,   595,   601,   609,   614,   621,   626,
     632,   640,   644,   652,   660,   665,   673,   679,   685,   690,
     695,   703,   710,   719,   725,   733,   740,   749,   755,   761,
     767,   775,   781,   789,   792,   793,   794,   795,   796,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   812,   814,   816,   818,   820,   822,   824,
     833,   841,   849,   851,   854,   862,   868,   875,   881,   889,
     896,   902,   908,   915,   918,   920,   925,   926,   927,   933,
     939,   945,   956,   961,   966,   970,   975,   980,   985,   990,
     995,  1000,  1005,  1010,  1015,  1020,  1025,  1030,  1035,  1047,
    1053,  1059,  1065,  1071,  1077,  1082,  1087,  1092,  1098,  1103,
    1108,  1117,  1122,  1127,  1132,  1139,  1143,  1150,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "OR", "AND", "NOT", "GT", "LT",
  "NOTEQUAL", "EQUALS", "STR_GT", "STR_LT", "STR_EQUALS", "BITOR",
  "BITAND", "MINUS", "PLUS", "STR_PLUS", "MODULO", "DIVIDE", "MULTIPLY",
  "POW", "LPAREN", "RPAREN", "LCURLY", "RCURLY", "SEMICOLON", "COMMA",
  "DLOPEN", "CREATE", "STREAM", "USING", "FILES", "COLUMNS", "FILTERS",
  "MERGEJOIN", "OF", "TO", "WHERE", "USERCOLUMN", "DEST", "FROM", "HASH",
  "CRC48", "MD5", "CEIL", "FLOOR", "WITH", "BUCKETS", "FILENAME",
  "TEMPLATE", "COLUMN", "STRCMP", "THEN", "ELSE", "DOT", "ELIPSES",
  "DELIMITER", "CONFIGURATION", "INHERIT", "CONFIGFILE", "PARSE", "INTO",
  "STRICT", "SYNC", "MAP", "MAPVALUE", "MAPEXISTS", "ABORT", "PRESORTED",
  "GROUPBY", "INTMIN", "INTMAX", "STRMIN", "STRMAX", "FIRST", "LAST",
  "AVG", "COUNT", "SUM", "METRICS", "COUNT_DISTINCT", "COUNT_CHANGES",
  "JOIN", "RANDOM", "LINENUMBER", "DBCONNECTION", "XML", "QUERY",
  "HASHED_TABLE", "LIST", "COMMAND", "CHOMP", "JOB", "JOBQ", "STEP",
  "NAMESPACE", "STRMATCH", "STRCASEMATCH", "PATTERN", "PATH", "TOUCH",
  "MV", "RM", "FINDRM", "NSFIND", "PING", "RECONFIGURE", "TOLOWER",
  "TOUPPER", "TABLE", "NEW", "MVCLOBBER", "BYTE", "PERSIST", "MERGE", "IN",
  "MEMORY", "INDEX", "OUTER", "SORT", "FOREACH", "DEBUGECHO", "EXEC", "LN",
  "LOG2", "LOG10", "LLFORMAT", "LDFORMAT", "STRSTR", "LENGTH",
  "SKIPNORMALIZE", "NORMALIZE", "BYTESTRIP", "CNTMATCH", "PHRASEMATCH",
  "ICONV", "NANOSLEEP", "EVENT", "PGLISTEN", "WARN", "DML", "SQLESCAPE",
  "SPRINTF", "SQLVALUE", "COALESCE", "WHILE", "SUBSTR", "FACTORIAL",
  "SEQUENCE", "STRGREATEST", "STRLEAST", "INFO", "NUMBER", "QUOTED_STRING",
  "NAME", "$accept", "cplusqlfile", "statement_semicolon", "statement",
  "foreach_statement", "echo_statement", "exec_statement",
  "command_statement", "sql_exec_statement", "sql_command_statement",
  "synchronize_statement", "jobq_statement", "job_statement",
  "namespace_statement", "namespace_command_statement", "map_statement",
  "db_conn_statement", "hashed_table_statement", "config_statement",
  "config_clause", "config_object", "configuration", "configpairs",
  "configvalue", "bytestream_statement", "stream_statement",
  "outer_clause", "index_statement", "ads_map", "relation",
  "parsedcolumns", "parsedcolumnset", "strict", "delimiter",
  "usercolumnset", "usercolumn", "metricset", "named_metric", "fileset",
  "columnset", "where_clause", "quotedlist", "quoted_string_set",
  "quotednamelist", "unquoted_name_set", "dest_statement",
  "expressionlist", "metric_expression", "expression", "ifthenelse",
  "metric", "zeroary_metric", "unary_metric", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   157,   158,   158,   159,   159,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   171,   171,   171,   171,   171,
     171,   171,   172,   172,   172,   173,   174,   175,   175,   175,
     176,   176,   177,   177,   177,   178,   178,   179,   179,   179,
     180,   180,   181,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   183,   183,
     184,   185,   186,   187,   187,   188,   188,   188,   188,   189,
     189,   190,   190,   191,   191,   191,   192,   192,   193,   193,
     193,   194,   195,   196,   197,   197,   198,   198,   198,   198,
     198,   199,   199,   200,   200,   201,   201,   202,   202,   202,
     202,   203,   203,   204,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   206,   206,   206,   206,   207,   207,   208,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     2,     2,     6,     6,     6,
       1,     4,     8,     4,     5,     3,     4,     6,     6,     5,
       8,     8,     9,     6,     7,    11,    11,     4,     1,     1,
       0,     1,     3,     2,     2,     1,     2,     0,     3,     5,
       1,     1,     7,     4,     8,    11,    14,    15,     9,    11,
      10,     8,     7,    10,     9,    12,     7,    10,     0,     1,
      10,     1,     1,     0,     2,     6,     7,     7,     7,     0,
       1,     0,     2,     0,     2,     3,     2,     3,     0,     2,
       3,     2,     2,     2,     0,     2,     3,     2,     5,     7,
       2,     3,     1,     3,     2,     3,     1,     8,     7,     9,
      10,     1,     3,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     4,     6,     4,     6,     4,     6,
       3,     1,     1,     1,     1,     6,     4,     4,     9,     8,
       7,     6,     4,     3,     2,     8,     6,     4,     4,     4,
       6,    12,     8,     8,     8,     4,     6,     6,     6,     4,
       4,     6,     4,     6,     5,     4,     4,     4,    18,     5,
       4,     3,     6,     6,     6,     4,     8,     4,     8,     4,
       4,    11,     9,    12,    10,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      57,    57,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,    57,     2,     0,     4,    19,    21,    20,    23,
      22,    11,    15,    16,    17,    18,    12,    13,    14,    10,
      49,    48,     6,     7,     8,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,   210,
     211,   212,   213,   214,   217,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,   152,   151,   124,    25,   154,     0,     0,
      26,     0,     1,     3,     5,     0,    52,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    50,     0,     0,
       0,     0,     0,     0,     0,    35,    50,     0,     0,     0,
       0,   164,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      60,    58,     0,    78,     0,     0,    82,     0,    55,    47,
       0,     0,     0,     0,     0,     0,    31,    51,    33,     0,
       0,    50,     0,    50,     0,     0,    36,     0,    63,     0,
       0,     0,     0,   163,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   191,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,   150,   128,   127,   126,   125,   130,   129,   141,   140,
     142,   137,   136,   135,   134,   139,   133,   132,   131,   138,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    82,
       0,    93,    91,     0,     0,    94,     0,     0,     0,     0,
       0,    56,     0,    81,     0,     0,     0,    50,     0,    50,
       0,     0,    82,     0,    34,    50,    39,     0,     0,    50,
     107,     0,   112,     0,   110,     0,     0,     0,   157,   156,
     168,   169,     0,     0,     0,   162,     0,     0,   167,     0,
     185,     0,     0,   179,   180,   144,     0,   146,     0,   148,
       0,     0,   182,     0,     0,   175,     0,     0,     0,     0,
       0,     0,     0,   186,     0,   190,     0,     0,   195,     0,
       0,   197,     0,   199,   200,   187,   123,    50,    59,   102,
      50,     0,     0,     0,     0,     0,     0,     0,     0,    83,
       0,     0,     0,    96,    95,    50,     0,     0,    50,     0,
      43,     0,     0,     0,    29,     0,    27,     0,    50,     0,
      37,     0,     0,    38,     0,   106,     0,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,   122,     0,     0,
      28,    72,     0,     0,     0,     0,    50,     0,     0,     0,
       0,   104,    92,     0,    50,    97,    76,     0,     0,   118,
      50,    44,     0,     0,     0,    50,    62,     0,    50,    50,
       0,   111,     0,     0,   155,   149,     0,     0,     0,   161,
       0,   166,   177,   178,   143,   145,   147,   181,   183,   176,
       0,     0,     0,   170,     0,   192,   193,     0,   194,     0,
       0,     0,    50,    91,     0,    71,     0,    98,     0,     0,
       0,     0,    84,    50,     0,   103,    64,    50,     0,   117,
       0,     0,     0,    32,   104,    40,    41,   108,     0,     0,
       0,     0,     0,   160,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    74,    50,     0,     0,     0,     0,   104,
       0,     0,   105,    89,    68,   114,   116,     0,   119,    50,
      42,     0,     0,    50,     0,     0,     0,   122,   159,   165,
     173,   172,     0,   174,     0,   196,   198,    70,    73,    50,
       0,     0,     0,    99,     0,    50,    77,     0,     0,    90,
       0,    91,   113,     0,   120,    50,    50,    80,   109,     0,
     202,     0,   158,     0,     0,    65,     0,     0,   207,   101,
       0,     0,   100,    69,    50,     0,    50,    50,     0,   115,
      45,    46,     0,     0,   204,     0,     0,   104,     0,     0,
       0,    75,     0,     0,     0,     0,   201,     0,     0,     0,
      50,   104,   205,     0,     0,     0,     0,    85,   203,   171,
       0,    66,    50,   206,    88,    86,    87,     0,    67,     0,
       0,     0,     0,   188
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   236,
     228,   237,    31,   221,    32,    33,   330,    34,   344,   227,
     501,   562,   641,   431,   225,   335,   599,   633,   332,   504,
     563,   251,   363,   565,   607,    35,   293,   115,   294,   117,
     659,   660,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -597
static const short int yypact[] =
{
       3,  -117,    15,   -99,   -93,  -597,   -69,   -60,   -55,   955,
     607,    96,   222,  -597,    86,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,    87,  -597,  -597,  -597,  -597,     1,   -42,    81,   -18,
      -1,    24,   126,    25,    30,    52,    54,    57,   112,    60,
    -597,  -597,   253,   124,    70,   166,   955,   955,   206,   207,
     208,   209,   210,   212,   213,   215,   216,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,   217,   220,   221,
     223,   224,   225,   226,   247,   249,   250,   251,   252,   254,
     256,   258,   261,   263,   265,   266,   267,   268,   -15,   269,
     272,   290,   291,   292,   293,   294,   299,   300,   301,   304,
     306,   308,  -597,  -597,   167,  -597,  2494,  -597,   309,   244,
    2494,   -70,  -597,  -597,  -597,   156,  -597,    29,   177,    12,
     302,   312,   180,   287,   307,   310,    12,    12,   184,   311,
     186,   192,   193,   248,   273,  -597,    12,   195,   196,    -3,
     955,  2510,  1780,   955,   955,   955,   955,   955,   955,   198,
     211,   955,   328,   955,   219,   955,   955,   214,   228,   955,
     955,   955,   955,   955,  1071,   231,   955,   955,   955,   955,
     955,    -3,   955,   233,   238,   237,   955,   239,   955,   242,
     243,   955,   955,   955,   955,   955,   955,   955,   255,   955,
     955,   955,   955,   955,   955,   955,   955,   955,   955,   955,
     955,   955,   955,   955,   955,   955,   955,   955,   259,  -597,
    -597,  -597,   352,   -21,   -16,   -19,     2,   325,  -597,    12,
     -45,   257,   262,   288,   313,   260,  -597,    12,  -597,   348,
     264,    12,   314,    12,   316,   317,  -597,   318,  -597,    20,
     955,   319,  1801,  -597,  1414,  1822,  1843,  1864,  1885,  1436,
     366,   374,  1906,   955,  1458,   380,   443,  1927,   377,   379,
    1948,  1969,  1348,  1370,  1392,   382,  1990,   390,  1480,  2011,
    1502,  1524,  1546,   416,  1568,   418,  -597,   448,  2032,   449,
    2053,   955,   450,    13,  2494,  1590,  2074,  1612,    14,    43,
    2095,  -597,  2510,  2510,   420,   420,   420,   420,   420,   420,
     420,   509,   537,   111,   111,   111,   386,   386,   386,  -597,
      58,   324,   -70,    -3,   326,   327,   329,   396,  -597,   368,
     401,   -10,   430,   335,   955,  -597,   -16,   955,    10,   337,
     338,  -597,    12,   452,   459,   456,   471,    12,   411,    12,
     409,   469,  -597,   384,  -597,    12,  -597,   406,   407,    12,
    -597,   453,  -597,    66,  2494,   482,    -4,   357,  -597,  -597,
    -597,  -597,   955,   723,   839,  -597,    69,   955,  -597,   488,
    -597,   955,   955,  -597,  -597,  -597,   955,  -597,   955,  -597,
     955,   955,  -597,   955,   955,  -597,    -3,    -3,   358,   955,
     360,   362,   363,  -597,   367,  -597,    92,   955,  -597,   955,
     955,  -597,   955,  -597,  -597,  -597,  -597,    12,  -597,  -597,
      12,   -62,   422,   483,   376,   496,   464,   381,   447,   511,
     387,   510,   519,  2494,  -597,   443,   514,   437,    12,   955,
      12,   394,   955,   405,  -597,   412,  -597,   413,    12,   530,
    -597,   414,   415,  -597,   410,  -597,   419,  -597,   955,   541,
     547,  2116,   955,  1634,   955,  2137,   545,  2158,  -597,  2179,
    2200,  2221,  2242,  2263,  2284,  2305,  2326,   548,   549,   550,
    2347,   551,   557,   558,   555,  -597,  2368,  2494,  1656,  1678,
    -597,  -597,   431,   432,   433,   264,    12,   264,   510,   486,
     434,   -11,  -597,   562,    12,  -597,  -597,   435,   560,  -597,
     443,  -597,  1224,   565,   546,    12,  -597,   955,    12,    12,
      -8,  -597,  1700,   955,  -597,  -597,   566,   955,   148,  -597,
     441,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
      -3,    -3,   442,  -597,   444,  -597,  -597,   567,  -597,   955,
     955,   510,    31,   430,   955,  -597,   955,   517,   955,   510,
     955,   955,  -597,    12,    -6,  -597,  -597,    12,   445,  -597,
     955,   454,   451,  -597,   246,  -597,  -597,  -597,   457,   552,
    1720,   955,  2389,  -597,   578,   579,   580,   584,   590,   955,
    2410,  2431,    12,  -597,    12,   510,  1256,  1288,   462,     4,
    1320,   573,  2494,   289,  -597,  -597,  -597,   154,  -597,    12,
    2494,   595,   510,    12,   596,    28,   553,  2452,  -597,  -597,
    -597,  -597,   466,  -597,   187,  -597,  -597,  -597,  -597,    12,
     264,   264,   349,  -597,   462,    12,  -597,   955,   487,  -597,
     -13,   430,  -597,   468,  -597,    12,    12,  -597,  -597,   955,
    -597,    33,  -597,   594,   597,  -597,   955,   507,  -597,  -597,
     604,   606,  -597,  -597,   443,    -3,    12,    12,   568,  -597,
    -597,  -597,  1740,   955,  -597,   477,   610,   246,   478,   612,
     955,  -597,   576,   581,   582,   510,  -597,  1760,   617,   955,
      12,   603,  -597,  2473,   510,   510,   510,  -597,  -597,  -597,
     197,  -597,    12,  -597,  -597,  -597,  -597,   615,  -597,   623,
     955,   200,   624,  -597
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -597,  -597,   637,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   -29,
       5,  -125,   654,   334,  -597,  -597,  -597,  -597,  -597,  -217,
    -597,  -597,  -597,  -538,   331,   321,  -597,    32,   343,  -476,
    -540,  -170,  -597,  -597,  -597,  -597,  -193,  -597,    -9,  -596,
    -597,  -597,    26
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -83
static const short int yytable[] =
{
     116,   120,   298,   299,   229,    30,   331,   333,   183,   336,
     249,   283,   323,   345,   342,   595,   577,    30,   605,   650,
     249,   458,   557,   353,   320,   126,     1,   492,   560,   125,
     224,    55,   634,     2,   613,     1,    55,   408,   413,    11,
     338,   409,   409,   560,   360,   324,    37,   151,   152,   493,
     459,   561,   339,   649,     1,   674,    38,    50,   673,   635,
     426,   223,    51,     3,     4,   503,   325,   414,     5,   224,
     376,   409,     3,     4,    39,   592,   594,   427,   428,   250,
     326,    40,   416,   601,   219,   220,   409,    52,   578,   250,
     455,     3,     4,   466,   456,   327,    53,   409,   406,   328,
       6,    54,    41,   668,    42,    43,   121,    44,   238,    45,
      46,   343,    47,   124,   127,   125,   485,   246,     7,   629,
     409,   436,   437,   128,   636,     8,     9,    10,   337,    48,
     213,   214,   215,   216,    49,   329,   646,   690,   129,   184,
     334,   252,   666,   138,   254,   255,   256,   257,   258,   259,
     606,   702,   262,   419,   264,   130,   266,   267,   132,    11,
     270,   271,   272,   273,   274,   276,   148,   278,   279,   280,
     281,   282,   583,   284,   361,   362,   409,   288,   642,   290,
     131,   133,   643,   295,   296,   297,   134,   149,   300,   150,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   135,   697,
     136,   654,   354,   137,   356,   409,   139,   440,   704,   705,
     706,   707,   122,   198,   712,   409,   477,   478,   409,   153,
     154,   155,   156,   157,   341,   158,   159,   379,   160,   161,
     162,   364,   341,   163,   164,     1,   165,   166,   167,   168,
     199,   200,     2,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   526,
     169,   528,   170,   171,   172,   173,   218,   174,   554,   175,
     556,   176,     3,     4,   177,   560,   178,     5,   179,   180,
     181,   182,   185,   199,   200,   186,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   222,   187,   188,   189,   190,   191,   444,     6,
     446,   638,   192,   193,   194,   433,   450,   195,   435,   196,
     453,   197,   217,   226,   230,   231,   232,     7,   233,   234,
     239,   241,   235,   240,     8,     9,    10,   242,   243,   244,
     247,   263,   248,   639,   260,   140,   141,   142,   143,   144,
     145,   146,   322,   461,   463,   465,   147,   261,   467,   268,
     585,   586,   469,   470,   245,   265,   340,   471,    11,   472,
     351,   473,   474,   269,   475,   476,   277,   285,   490,   640,
     480,   491,   286,   287,   373,   289,   624,   291,   486,   292,
     487,   488,   374,   489,   378,   381,   506,   382,   216,   509,
     391,   301,   346,   656,   657,   321,   350,   347,   393,   516,
     352,    67,    68,    69,    70,    71,    72,    73,   658,    74,
     510,    75,    76,   512,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   348,   399,   341,   401,   199,   200,   522,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     1,   555,   349,   355,
     667,   357,   358,   359,   365,   566,   402,   404,   407,   417,
     423,   569,   420,   421,   422,   425,   573,   424,   430,   575,
     576,   432,   438,   439,   -82,   682,   700,   441,   442,   443,
     445,   447,   323,     3,     4,   449,   451,   452,   574,   457,
     454,   460,   468,   479,   580,   481,   482,   711,   582,   483,
     495,   484,   494,   593,   209,   210,   211,   212,   213,   214,
     215,   216,   496,   497,   604,   498,   500,   499,   608,   336,
     590,   591,   502,   505,   503,   596,   507,   597,   508,   600,
     511,   602,   603,   210,   211,   212,   213,   214,   215,   216,
     513,   610,   517,   627,   520,   628,   523,   514,   515,   518,
     519,   524,   617,   530,   521,   558,   540,   541,   542,   544,
     644,   545,   546,   547,   647,   564,   551,   552,   553,   559,
     589,   567,   568,   571,   581,   572,   584,   587,   598,   588,
     655,   609,   619,   620,   621,   612,   663,   615,   651,   611,
      55,   614,   622,    56,   623,   637,   670,   671,   632,   645,
     648,   653,   675,   665,   669,   676,   678,   679,   664,   680,
      57,   685,   688,   689,   691,   681,   692,   683,   684,   694,
     672,   699,   560,   709,   695,   696,   710,   677,   713,   123,
      58,    59,    60,    61,    62,    36,   418,   434,   661,     0,
      63,   701,   429,     0,   687,     0,   662,     0,     0,     0,
       0,   693,     0,   708,    64,    65,    66,     0,     0,    67,
      68,    69,    70,    71,    72,    73,     0,    74,     0,    75,
      76,    77,    78,    79,   448,     0,   119,     0,     0,    80,
      81,     0,     0,     0,     0,    82,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,    85,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,    56,
       0,     0,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,     0,    97,    98,    57,    99,   100,   101,
     102,   103,   104,   105,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,    58,    59,    60,    61,
      62,     0,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,    66,     0,     0,    67,    68,    69,    70,    71,
      72,    73,     0,    74,     0,    75,    76,    77,    78,    79,
       0,     0,     0,     0,   462,    80,    81,     0,     0,     0,
       0,    82,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,    85,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,    56,     0,     0,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,     0,
      97,    98,    57,    99,   100,   101,   102,   103,   104,   105,
       0,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,     0,    58,    59,    60,    61,    62,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,    66,     0,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
       0,    75,    76,    77,    78,    79,     0,     0,     0,     0,
     464,    80,    81,     0,     0,     0,     0,    82,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,    85,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,    56,     0,     0,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,     0,    97,    98,    57,    99,
     100,   101,   102,   103,   104,   105,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,     0,     0,    58,    59,
      60,    61,    62,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,    66,     0,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,     0,    75,    76,    77,
      78,    79,     0,     0,     0,     0,     0,    80,    81,     0,
       0,     0,     0,    82,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,    85,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,    56,     0,     0,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,     0,    97,    98,    57,    99,   100,   101,   102,   103,
     104,   105,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,     0,    58,    59,    60,    61,    62,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
      66,     0,     0,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    75,    76,    77,    78,    79,     0,     0,
       0,     0,     0,    80,    81,     0,     0,     0,     0,    82,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,     0,    97,    98,
       0,    99,   100,   101,   102,   103,   104,   105,     0,   106,
     107,   108,   109,   110,   111,   112,   275,   114,   199,   200,
       0,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     199,   200,   570,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   199,   200,   630,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,   200,   631,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   199,   200,   503,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,   385,     0,   199,   200,   386,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   387,     0,   199,   200,   388,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,   389,     0,   199,   200,
     390,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
     199,   200,   367,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,   199,   200,   372,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,   199,   200,   377,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,   199,   200,   394,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,     0,     0,   199,   200,
     396,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
     199,   200,   397,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,   199,   200,   398,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,   199,   200,   400,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,   199,   200,   410,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,     0,     0,   199,   200,
     412,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
     199,   200,   527,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,   199,   200,   549,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,   199,   200,   550,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   199,   200,   579,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   199,   200,   616,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   199,   200,   686,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   199,   200,   698,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   253,   199,   200,     0,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,     0,   366,   199,   200,     0,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,   368,   199,   200,     0,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,   369,   199,   200,
       0,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,   370,   199,
     200,     0,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,     0,   371,
     199,   200,     0,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
     375,   199,   200,     0,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
       0,   380,   199,   200,     0,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,   383,   199,   200,     0,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,     0,   384,   199,   200,     0,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   392,   199,   200,     0,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,     0,   395,   199,   200,     0,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,   403,   199,   200,     0,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,   405,   199,   200,
       0,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,   411,   199,
     200,     0,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,     0,   415,
     199,   200,     0,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
     525,   199,   200,     0,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
       0,   529,   199,   200,     0,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,   531,   199,   200,     0,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,     0,   532,   199,   200,     0,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   533,   199,   200,     0,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,     0,   534,   199,   200,     0,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,   535,   199,   200,     0,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,   536,   199,   200,
       0,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,   537,   199,
     200,     0,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,     0,   538,
     199,   200,     0,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
     539,   199,   200,     0,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
       0,   543,   199,   200,     0,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,   548,   199,   200,     0,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,     0,   618,   199,   200,     0,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   625,   199,   200,     0,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,     0,   626,   199,   200,     0,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,   652,   199,   200,     0,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,   703,   199,   200,
       0,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216
};

static const short int yycheck[] =
{
       9,    10,   195,   196,   129,     0,   223,    23,    23,    28,
      23,   181,    33,   230,    59,   553,    24,    12,    24,   615,
      23,    25,   498,   240,   217,    24,    23,    89,    39,    28,
      40,     3,    28,    30,   574,    23,     3,    24,    24,   156,
      38,    28,    28,    39,    24,    66,    31,    56,    57,   111,
      54,    62,    50,    25,    23,   651,    41,   156,    25,   599,
      70,    32,   155,    60,    61,    34,    87,    24,    65,    40,
     263,    28,    60,    61,    59,   551,   552,    87,    88,    92,
     101,    66,    24,   559,   154,   155,    28,   156,    96,    92,
      24,    60,    61,    24,    28,   116,   156,    28,   291,   120,
      97,   156,    87,   641,    89,    90,    10,    92,   137,    94,
      95,   156,    97,    27,   156,    28,    24,   146,   115,   595,
      28,   111,   112,    42,   600,   122,   123,   124,   147,   114,
      19,    20,    21,    22,   119,   156,   612,   677,   156,   154,
     156,   150,   155,    31,   153,   154,   155,   156,   157,   158,
     156,   691,   161,   323,   163,   156,   165,   166,    32,   156,
     169,   170,   171,   172,   173,   174,    42,   176,   177,   178,
     179,   180,    24,   182,   154,   155,    28,   186,    24,   188,
     156,   156,    28,   192,   193,   194,   156,   117,   197,    23,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   156,   685,
     156,    24,   241,   156,   243,    28,   156,   342,   694,   695,
     696,    24,     0,    56,    24,    28,   396,   397,    28,    23,
      23,    23,    23,    23,   229,    23,    23,   266,    23,    23,
      23,   250,   237,    23,    23,    23,    23,    23,    23,    23,
       4,     5,    30,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   462,
      23,   464,    23,    23,    23,    23,    32,    23,   495,    23,
     497,    23,    60,    61,    23,    39,    23,    65,    23,    23,
      23,    23,    23,     4,     5,    23,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,   156,    23,    23,    23,    23,    23,   347,    97,
     349,    32,    23,    23,    23,   334,   355,    23,   337,    23,
     359,    23,    23,   156,    32,    23,   156,   115,    51,    32,
     156,   155,    32,    32,   122,   123,   124,   155,   155,   101,
     155,    23,   156,    64,   156,   102,   103,   104,   105,   106,
     107,   108,    10,   372,   373,   374,   113,   156,   377,   155,
     540,   541,   381,   382,   101,   156,    51,   386,   156,   388,
      32,   390,   391,   155,   393,   394,   155,   154,   417,   100,
     399,   420,   154,   156,    28,   156,   589,   155,   407,   156,
     409,   410,    28,   412,    24,    28,   435,    28,    22,   438,
      28,   156,   155,   630,   631,   156,   156,   155,    28,   448,
     156,    72,    73,    74,    75,    76,    77,    78,    79,    80,
     439,    82,    83,   442,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   155,    28,   440,    28,     4,     5,   458,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   496,   155,   155,
     640,   155,   155,   155,   155,   504,    28,    28,    28,   155,
      84,   510,   156,   156,   155,    84,   515,   119,    58,   518,
     519,   156,   155,   155,    42,   665,   689,    38,    42,    28,
      89,    92,    33,    60,    61,   121,   100,   100,   517,    27,
      57,   154,    24,   155,   523,   155,   154,   710,   527,   156,
      37,   154,   100,   552,    15,    16,    17,    18,    19,    20,
      21,    22,   156,    37,   563,    71,    89,   156,   567,    28,
     549,   550,   155,    24,    34,   554,    32,   556,   111,   558,
     156,   560,   561,    16,    17,    18,    19,    20,    21,    22,
     155,   570,    32,   592,   154,   594,    25,   155,   155,   155,
     155,    24,   581,    28,   155,    89,    28,    28,    28,    28,
     609,    24,    24,    28,   613,    23,   155,   155,   155,   155,
      23,   156,    32,    28,    28,    49,   155,   155,    81,   155,
     629,   156,    24,    24,    24,   154,   635,    55,    55,   155,
       3,   154,    28,     6,    24,    42,   645,   646,   156,    24,
      24,   155,    28,   136,   156,    28,   119,    23,   637,    23,
      23,    63,   155,    23,   156,   664,    24,   666,   667,    63,
     649,    24,    39,    28,    63,    63,    23,   656,    24,    12,
      43,    44,    45,    46,    47,     1,   322,   336,   632,    -1,
      53,   690,   331,    -1,   673,    -1,   634,    -1,    -1,    -1,
      -1,   680,    -1,   702,    67,    68,    69,    -1,    -1,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,    -1,    82,
      83,    84,    85,    86,   351,    -1,    89,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,     6,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,   137,   138,    23,   140,   141,   142,
     143,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,    43,    44,    45,    46,
      47,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    -1,    82,    83,    84,    85,    86,
      -1,    -1,    -1,    -1,    91,    92,    93,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,     6,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    23,   140,   141,   142,   143,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    85,    86,    -1,    -1,    -1,    -1,
      91,    92,    93,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,     6,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,   137,   138,    23,   140,
     141,   142,   143,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    -1,    -1,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    -1,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,     6,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,   137,   138,    23,   140,   141,   142,   143,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    72,    73,    74,    75,    76,    77,    78,
      -1,    80,    -1,    82,    83,    84,    85,    86,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       4,     5,    38,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     4,     5,    38,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     4,     5,    38,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     4,     5,    34,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    -1,     4,     5,    28,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    -1,     4,     5,    28,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    -1,     4,     5,
      28,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
       4,     5,    28,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,     4,     5,    28,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,     4,     5,    28,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,     4,     5,    28,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,     4,     5,
      28,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
       4,     5,    28,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,     4,     5,    28,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,     4,     5,    28,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,     4,     5,    28,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,     4,     5,
      28,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
       4,     5,    28,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,     4,     5,    28,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,     4,     5,    28,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,     4,     5,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,     4,     5,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,     4,     5,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,     4,     5,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,     4,     5,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,     4,
       5,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
       4,     5,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,     4,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,     4,     5,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,     4,
       5,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
       4,     5,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,     4,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,     4,     5,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,     4,
       5,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
       4,     5,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,     4,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,     4,     5,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    23,    30,    60,    61,    65,    97,   115,   122,   123,
     124,   156,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     177,   179,   181,   182,   184,   202,   179,    31,    41,    59,
      66,    87,    89,    90,    92,    94,    95,    97,   114,   119,
     156,   155,   156,   156,   156,     3,     6,    23,    43,    44,
      45,    46,    47,    53,    67,    68,    69,    72,    73,    74,
      75,    76,    77,    78,    80,    82,    83,    84,    85,    86,
      92,    93,    98,    99,   109,   110,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   137,   138,   140,
     141,   142,   143,   144,   145,   146,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   204,   205,   206,   209,    89,
     205,    10,     0,   159,    27,    28,    24,   156,    42,   156,
     156,   156,    32,   156,   156,   156,   156,   156,    31,   156,
     102,   103,   104,   105,   106,   107,   108,   113,    42,   117,
      23,   205,   205,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,   154,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    56,     4,
       5,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    32,   154,
     155,   180,   156,    32,    40,   191,   156,   186,   177,   178,
      32,    23,   156,    51,    32,    32,   176,   178,   176,   156,
      32,   155,   155,   155,   101,   101,   176,   155,   156,    23,
      92,   198,   205,    24,   205,   205,   205,   205,   205,   205,
     156,   156,   205,    23,   205,   156,   205,   205,   155,   155,
     205,   205,   205,   205,   205,   155,   205,   155,   205,   205,
     205,   205,   205,   198,   205,   154,   154,   156,   205,   156,
     205,   155,   156,   203,   205,   205,   205,   205,   203,   203,
     205,   156,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     203,   156,    10,    33,    66,    87,   101,   116,   120,   156,
     183,   186,   195,    23,   156,   192,    28,   147,    38,    50,
      51,   177,    59,   156,   185,   186,   155,   155,   155,   155,
     156,    32,   156,   186,   176,   155,   176,   155,   155,   155,
      24,   154,   155,   199,   205,   155,    24,    28,    24,    24,
      24,    24,    28,    28,    28,    24,   203,    28,    24,   176,
      24,    28,    28,    24,    24,    24,    28,    24,    28,    24,
      28,    28,    24,    28,    28,    24,    28,    28,    28,    28,
      28,    28,    28,    24,    28,    24,   203,    28,    24,    28,
      28,    24,    28,    24,    24,    24,    24,   155,   180,   198,
     156,   156,   155,    84,   119,    84,    70,    87,    88,   191,
      58,   190,   156,   205,   192,   205,   111,   112,   155,   155,
     178,    38,    42,    28,   176,    89,   176,    92,   195,   121,
     176,   100,   100,   176,    57,    24,    28,    27,    25,    54,
     154,   205,    91,   205,    91,   205,    24,   205,    24,   205,
     205,   205,   205,   205,   205,   205,   205,   198,   198,   155,
     205,   155,   154,   156,   154,    24,   205,   205,   205,   205,
     176,   176,    89,   111,   100,    37,   156,    37,    71,   156,
      89,   187,   155,    34,   196,    24,   176,    32,   111,   176,
     205,   156,   205,   155,   155,   155,   176,    32,   155,   155,
     154,   155,   205,    25,    24,    24,   203,    28,   203,    24,
      28,    24,    24,    24,    24,    24,    24,    24,    24,    24,
      28,    28,    28,    24,    28,    24,    24,    28,    24,    28,
      28,   155,   155,   155,   186,   176,   186,   196,    89,   155,
      39,    62,   188,   197,    23,   200,   176,   156,    32,   176,
      38,    28,    49,   176,   205,   176,   176,    24,    96,    26,
     205,    28,   205,    24,   155,   198,   198,   155,   155,    23,
     205,   205,   196,   176,   196,   190,   205,   205,    81,   193,
     205,   196,   205,   205,   176,    24,   156,   201,   176,   156,
     205,   155,   154,   197,   154,    55,    26,   205,    24,    24,
      24,    24,    28,    24,   203,    24,    24,   176,   176,   196,
      38,    38,   156,   194,    28,   197,   196,    42,    32,    64,
     100,   189,    24,    28,   176,    24,   196,   176,    24,    25,
     206,    55,    24,   155,    24,   176,   186,   186,    79,   207,
     208,   209,   194,   176,   205,   136,   155,   198,   190,   156,
     176,   176,   205,    25,   206,    28,    28,   205,   119,    23,
      23,   176,   198,   176,   176,    63,    26,   205,   155,    23,
     197,   156,    24,   205,    63,    63,    63,   196,    26,    24,
     203,   176,   197,    24,   196,   196,   196,    24,   176,    28,
      23,   203,    24,    24
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 107 "cplusql_grammar.y"
    {
        yacfdbg("cplusqlfile: statement semicolon");
    }
    break;

  case 3:
#line 111 "cplusql_grammar.y"
    {
        yacfdbg("cplusqlfile:   cplusqlfile statement semicolon");
    }
    break;

  case 24:
#line 143 "cplusql_grammar.y"
    {
        yacfdbg("foreach_statement: ");
        fprintf(stderr, "foreach: var %s template %s\n", yyvsp[-4].idc, yyvsp[-1].idc );
        cplusql_foreach( yyvsp[-4].idc, yyvsp[-2].idp, yyvsp[-1].idc );
        yyval.idp = NULL;
    }
    break;

  case 25:
#line 153 "cplusql_grammar.y"
    {
        yacfdbg("echo_statement: ");
        cplusql_echo( yyvsp[0].idp );
        yyval.idp = NULL;
    }
    break;

  case 26:
#line 162 "cplusql_grammar.y"
    {
        yacfdbg("eval_statement: ");
        cplusql_exec( yyvsp[0].idp );
        yyval.idp = NULL;
    }
    break;

  case 27:
#line 171 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_command( yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("created command");
    }
    break;

  case 28:
#line 179 "cplusql_grammar.y"
    {
        yacfdbg("exec sql");
        cplusql_run_sql( yyvsp[-2].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 29:
#line 187 "cplusql_grammar.y"
    {
        yacfdbg("command sql");
        cplusql_add_sql( cplusql_sql( yyvsp[-2].idc, yyvsp[-1].idc, yyvsp[0].idp ));
    }
    break;

  case 30:
#line 195 "cplusql_grammar.y"
    {
        yacfdbg("sync_statement");
        cplusql_sync();
    }
    break;

  case 31:
#line 203 "cplusql_grammar.y"
    {
		yyval.idp = cplusql_jobq( yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("created jobq");
	}
    break;

  case 32:
#line 211 "cplusql_grammar.y"
    {
		yyval.idp = cplusql_job( yyvsp[-5].idc, yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("created job");
	}
    break;

  case 33:
#line 219 "cplusql_grammar.y"
    {
		yyval.idp = cplusql_namespace( yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("created namespace");
	}
    break;

  case 34:
#line 227 "cplusql_grammar.y"
    {
        yacfdbg("touch");
        yyval.idp = cplusql_namespace_touch( yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
	}
    break;

  case 35:
#line 233 "cplusql_grammar.y"
    {
        yacfdbg("ping");
        yyval.idp = cplusql_namespace_ping( yyvsp[-1].idc );
	}
    break;

  case 36:
#line 239 "cplusql_grammar.y"
    {
        yacfdbg("reconfigure");
        yyval.idp = cplusql_namespace_reconfigure( yyvsp[-2].idc, yyvsp[0].idp );
    }
    break;

  case 37:
#line 245 "cplusql_grammar.y"
    {
        yacfdbg("mv");
        yyval.idp = cplusql_namespace_mv( yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[-1].idc, yyvsp[0].idp );
	}
    break;

  case 38:
#line 251 "cplusql_grammar.y"
    {
        yacfdbg("mv");
        yyval.idp = cplusql_namespace_mvclobber( yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[-1].idc, yyvsp[0].idp );
	}
    break;

  case 39:
#line 257 "cplusql_grammar.y"
    {
        yacfdbg("rm");
        yyval.idp = cplusql_namespace_rm( yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
	}
    break;

  case 40:
#line 264 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_namespace_findrm( yyvsp[-6].idc, yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("findrm");
	}
    break;

  case 41:
#line 271 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_namespace_find( yyvsp[-6].idc, yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
        yacfdbg("find");
	}
    break;

  case 42:
#line 279 "cplusql_grammar.y"
    {
        cplusql_map_from_relation( yyvsp[-6].idc, yyvsp[-4].idp, yyvsp[-2].idp, yyvsp[0].idp );
        yacfdbg("create map from relation");
    }
    break;

  case 43:
#line 285 "cplusql_grammar.y"
    {
        cplusql_map_from_config( yyvsp[-3].idc, yyvsp[0].idp );
        yacfdbg("create map from config");
    }
    break;

  case 44:
#line 291 "cplusql_grammar.y"
    {
        cplusql_map_from_ads_map( yyvsp[-4].idc, yyvsp[-2].idp, yyvsp[0].idc );
        yacfdbg("create map from ads map");
    }
    break;

  case 45:
#line 299 "cplusql_grammar.y"
    {
        yacfdbg("db_conn:");
        yyval.idp = cplusql_db_conn( yyvsp[-8].idc, yyvsp[-6].idc, yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[0].idp );
    }
    break;

  case 46:
#line 307 "cplusql_grammar.y"
    {
        yacfdbg("hashed_table:");
        yyval.idp = cplusql_hashed_table( yyvsp[-8].idc, yyvsp[-6].idc, yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 47:
#line 315 "cplusql_grammar.y"
    {
        yacfdbg("config: CREATE configuration...");
        yyval.idp=cplusql_add_config( yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 48:
#line 321 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_addto_default_cfg( yyvsp[0].idp );
    }
    break;

  case 49:
#line 326 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_addto_default_cfg( yyvsp[0].idp );
    }
    break;

  case 50:
#line 332 "cplusql_grammar.y"
    { yyval.idp=cplusql_default_cfg();}
    break;

  case 51:
#line 335 "cplusql_grammar.y"
    {
        yacfdbg("config_clause:");
        yyval.idp=yyvsp[0].idp;
    }
    break;

  case 52:
#line 343 "cplusql_grammar.y"
    {
        yacfdbg("config_object: configpairs:");
        yyval.idp=yyvsp[-1].idp;
    }
    break;

  case 53:
#line 349 "cplusql_grammar.y"
    {
        yacfdbg("config_object: configfile QUOTED_STRING:");
        yacfdbg((const char *) yyvsp[0].idc);
        yyval.idp=cplusql_cfg_file( yyvsp[0].idc );
    }
    break;

  case 54:
#line 355 "cplusql_grammar.y"
    {
        yacfdbg("config_object: INHERIT");
        yacfdbg((const char *) yyvsp[0].idc);
        yyval.idp=cplusql_copy_named_cfg( yyvsp[0].idc ); }
    break;

  case 55:
#line 363 "cplusql_grammar.y"
    {
        yacfdbg("configuration: config_object:");
        yyval.idp=yyvsp[0].idp;
    }
    break;

  case 56:
#line 367 "cplusql_grammar.y"
    { yyval.idp=cplusql_merge_cfg( yyvsp[-1].idp, yyvsp[0].idp ); }
    break;

  case 57:
#line 371 "cplusql_grammar.y"
    { yyval.idp=cplusql_empty_config(); }
    break;

  case 58:
#line 372 "cplusql_grammar.y"
    { yyval.idp=cplusql_start_config( yyvsp[-2].idc, yyvsp[0].idc ); }
    break;

  case 59:
#line 374 "cplusql_grammar.y"
    {
        yyval.idp=cplusql_addto_config( yyvsp[-4].idp, yyvsp[-2].idc, yyvsp[0].idc );
    }
    break;

  case 60:
#line 380 "cplusql_grammar.y"
    { yyval.idc=yyvsp[0].idc ; }
    break;

  case 61:
#line 381 "cplusql_grammar.y"
    { yyval.idc=yyvsp[0].idc ; }
    break;

  case 62:
#line 386 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: files");
        yyval.idp = cplusql_byte_source( yyvsp[-3].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 63:
#line 394 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: persist ");
        yyval.idp = cplusql_persist( yyvsp[-2].idc, yyvsp[0].idc );
    }
    break;

  case 64:
#line 401 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: files");
        yyval.idp = cplusql_relation_fileset( yyvsp[-5].idc, yyvsp[-3].idp, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 65:
#line 408 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: files");
        yyval.idp = cplusql_relation_pattern( yyvsp[-8].idc, yyvsp[-5].idc, yyvsp[-3].idc, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 66:
#line 415 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: mergejoin");
        //TODO use where_clause, $12
        yyval.idp = cplusql_relation_mergejoin( yyvsp[-11].idc, yyvsp[-6].idp, yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 67:
#line 423 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: mergejoin");
        yyval.idp = cplusql_relation_memjoin( yyvsp[-12].idc, yyvsp[-10].id, yyvsp[-7].idp, yyvsp[-6].idp, yyvsp[-4].idp, yyvsp[-2].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 68:
#line 430 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: from stream");
        yyval.idp = cplusql_relation_from( yyvsp[-6].idc, yyvsp[-4].idp, yyvsp[-3].idp, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 69:
#line 437 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: presorted groupby");
        yyval.idp = cplusql_relation_presorted( yyvsp[-8].idc, yyvsp[-6].idp, yyvsp[-3].idp, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 70:
#line 444 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: db");
        yyval.idp = cplusql_relation_db( yyvsp[-7].idc, yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 71:
#line 450 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: indexscan");
        yyval.idp = cplusql_relation_indexscan( yyvsp[-5].idc, yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 72:
#line 456 "cplusql_grammar.y"
    {
        fprintf(stderr, "create stream from map not implemented\n" );
        yyval.idp = NULL;//
    }
    break;

  case 73:
#line 463 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: db");
        yyval.idp = cplusql_relation_pgcopy( yyvsp[-7].idc, yyvsp[-4].idc, yyvsp[-2].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 74:
#line 470 "cplusql_grammar.y"
    {
        yacfdbg("stream_statement: db short");
        yyval.idp = cplusql_relation_pgcopy_short( yyvsp[-6].idc, yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 75:
#line 478 "cplusql_grammar.y"
    {
        yacfdbg("xml stream");
        yyval.idp = cplusql_relation_xmlquery( yyvsp[-9].idc, yyvsp[-7].idp, yyvsp[-4].idc, yyvsp[-3].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 76:
#line 483 "cplusql_grammar.y"
    {
        yacfdbg("expression stream");
        yyval.idp = cplusql_relation_exp( yyvsp[-4].idc, yyvsp[-3].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 77:
#line 490 "cplusql_grammar.y"
    {
        yacfdbg("dbrepeat stream");
        yyval.idp = cplusql_relation_dbrepeat( yyvsp[-7].idc, yyvsp[-5].idp, yyvsp[-3].idc, yyvsp[-1].idp, yyvsp[0].idp);
    }
    break;

  case 78:
#line 497 "cplusql_grammar.y"
    {
        yacfdbg("outer_clause: false");
        yyval.id = 0 ;
    }
    break;

  case 79:
#line 503 "cplusql_grammar.y"
    {
        yacfdbg("outer_clause: true");
        yyval.id = 1 ;
    }
    break;

  case 80:
#line 512 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_index( yyvsp[-7].idc, yyvsp[-5].idp, yyvsp[-2].idp, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 81:
#line 519 "cplusql_grammar.y"
    {
        yacfdbg("relation: NAME");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp=cplusql_src_ads_map( yyvsp[0].idc );
    }
    break;

  case 82:
#line 528 "cplusql_grammar.y"
    {
        yacfdbg("relation: NAME");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp=cplusql_src_relation( yyvsp[0].idc );
    }
    break;

  case 83:
#line 545 "cplusql_grammar.y"
    { yyval.idp=NULL; }
    break;

  case 85:
#line 551 "cplusql_grammar.y"
    {
        yacfdbg("parse expression using delimiter");
        yyval.idp=cplusql_parsed_columns_exp( yyvsp[-4].idp, yyvsp[-3].id, yyvsp[-2].idp, yyvsp[0].idp ); 
    }
    break;

  case 86:
#line 557 "cplusql_grammar.y"
    {
        yacfdbg("parse expression using pattern");
        yyval.idp=cplusql_parsed_columns_regex( yyvsp[-5].idp, yyvsp[-3].idc, yyvsp[-2].idp, yyvsp[0].idp ); 
    }
    break;

  case 87:
#line 563 "cplusql_grammar.y"
    {
        yacfdbg("parse expression using pattern");
        yyval.idp=cplusql_parsed_columns_regexlist( yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-2].idp, yyvsp[0].idp ); 
    }
    break;

  case 88:
#line 569 "cplusql_grammar.y"
    {
        yacfdbg("cntmatch expression using list into name, name");
        yyval.idp=cplusql_parsed_matchlist( yyvsp[-2].idp, yyvsp[-5].idp, yyvsp[0].idp);
    }
    break;

  case 89:
#line 575 "cplusql_grammar.y"
    { yyval.id=0; }
    break;

  case 90:
#line 575 "cplusql_grammar.y"
    { yyval.id=1;}
    break;

  case 91:
#line 578 "cplusql_grammar.y"
    { 
        //result gets leaked by the way
        yyval.idp=0;
    }
    break;

  case 92:
#line 584 "cplusql_grammar.y"
    {
        yyval.idp=yyvsp[0].idc;
    }
    break;

  case 93:
#line 590 "cplusql_grammar.y"
    { 
        yacfdbg("usercolumnset: empty");
        yyval.idp=NULL;
    }
    break;

  case 94:
#line 596 "cplusql_grammar.y"
    {
        yacfdbg("usercolumnset: usercolumn");
        yyval.idp=NULL;
    }
    break;

  case 95:
#line 602 "cplusql_grammar.y"
    { 
        yacfdbg("usercolumnset: COMMA usercolumn");
        yyval.idp=NULL;
    }
    break;

  case 96:
#line 610 "cplusql_grammar.y"
    {
        yyval.idp=cplusql_user_column( yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 97:
#line 615 "cplusql_grammar.y"
    {
        yyval.idp=cplusql_user_column_def( yyvsp[-1].idc );
    }
    break;

  case 98:
#line 621 "cplusql_grammar.y"
    {
        yacfdbg("metricset: empty");
        yyval.idp=NULL;
    }
    break;

  case 99:
#line 627 "cplusql_grammar.y"
    {
        yacfdbg("metricset: metrics");
        yyval.idp=NULL;
    }
    break;

  case 100:
#line 633 "cplusql_grammar.y"
    {
        yacfdbg("metricset: COMMA named_metric");
        yyval.idp=NULL;
    }
    break;

  case 101:
#line 640 "cplusql_grammar.y"
    { yyval.idp=cplusql_named_metric( yyvsp[-1].idc, yyvsp[0].idp ); }
    break;

  case 102:
#line 645 "cplusql_grammar.y"
    { 
        yacfdbg("fileset: ");
        yyval.idp = yyvsp[0].idp ;
    }
    break;

  case 103:
#line 653 "cplusql_grammar.y"
    {
        yacfdbg("columnset: ");
        yyval.idp = yyvsp[0].idp ;
    }
    break;

  case 104:
#line 660 "cplusql_grammar.y"
    {
        yacfdbg("empty where_clause: ");
        yyval.idp = NULL ;
    }
    break;

  case 105:
#line 666 "cplusql_grammar.y"
    {
        yacfdbg("where_clause: ");
        yyval.idp = yyvsp[0].idp;
    }
    break;

  case 106:
#line 674 "cplusql_grammar.y"
    {
        yacfdbg("quotedlist: ");
        yyval.idp = yyvsp[-1].idp ;
    }
    break;

  case 107:
#line 680 "cplusql_grammar.y"
    {
        yacfdbg("quotedlist: ");
        yyval.idp = cplusql_new_string_set(0);
    }
    break;

  case 108:
#line 686 "cplusql_grammar.y"
    { 
        yacfdbg("quotedlist: number ... number ");
        yyval.idp = cplusql_range_string_set(yyvsp[-3].idc, yyvsp[-1].idc);
    }
    break;

  case 109:
#line 691 "cplusql_grammar.y"
    {
        yacfdbg("quotedlist: number ... number step number");
        yyval.idp = cplusql_range_step_string_set(yyvsp[-5].idc, yyvsp[-3].idc, yyvsp[-1].idc );
    }
    break;

  case 110:
#line 696 "cplusql_grammar.y"
    {
        yacfdbg("quotedlist: COMMAND expression");
        yyval.idp = cplusql_command_string_set(yyvsp[0].idp );
    }
    break;

  case 111:
#line 704 "cplusql_grammar.y"
    {
        yacfdbg("quoted_string_set: quoted_string_set, QUOTED_STRING");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp = cplusql_string_set( yyvsp[-2].idp, yyvsp[0].idc );
    }
    break;

  case 112:
#line 711 "cplusql_grammar.y"
    {
        yacfdbg("quoted_string_set: QUOTED_STRING");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp = cplusql_new_string_set( yyvsp[0].idc );
    }
    break;

  case 113:
#line 720 "cplusql_grammar.y"
    {
        yacfdbg("quotednamelist: ");
        yyval.idp = yyvsp[-1].idp ;
    }
    break;

  case 114:
#line 726 "cplusql_grammar.y"
    {
        yacfdbg("quotednamelist: empty");
        yyval.idp = cplusql_new_string_set(0);
    }
    break;

  case 115:
#line 734 "cplusql_grammar.y"
    {
        yacfdbg("unquoted_name_set: unquoted_name_set, NAME");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp = cplusql_string_set( yyvsp[-2].idp, yyvsp[0].idc );
    }
    break;

  case 116:
#line 741 "cplusql_grammar.y"
    {
        yacfdbg("unquoted_name_set: NAME");
        yacfdbg((const char *) yyvsp[0].idc );
        yyval.idp = cplusql_new_string_set( yyvsp[0].idc );
    }
    break;

  case 117:
#line 750 "cplusql_grammar.y"
    {
        yacfdbg("dest_statement: hashed files");
        yyval.idp= cplusql_dest_hash( yyvsp[-4].idp, yyvsp[-2].idc, yyvsp[-1].idp, yyvsp[0].idp );
    }
    break;

  case 118:
#line 756 "cplusql_grammar.y"
    {
        yacfdbg("dest_statement: file");
        yyval.idp= cplusql_dest_file( yyvsp[-3].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 119:
#line 762 "cplusql_grammar.y"
    {
        yacfdbg("dest_statement: table");
        yyval.idp= cplusql_dest_table( yyvsp[-5].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 120:
#line 768 "cplusql_grammar.y"
    {
        yacfdbg("dest_statement: new table");
        yyval.idp= cplusql_dest_new_table( yyvsp[-6].idc, yyvsp[-1].idc, yyvsp[0].idp );
    }
    break;

  case 121:
#line 776 "cplusql_grammar.y"
    {
        yacfdbg("expressionlist: expression");
        yyval.idp=cplusql_new_expression_list( yyvsp[0].idp );
    }
    break;

  case 122:
#line 782 "cplusql_grammar.y"
    {
        yacfdbg("expressionlist: expression COMMA expression");
        yyval.idp=cplusql_add_expression( yyvsp[-2].idp, yyvsp[0].idp );
    }
    break;

  case 123:
#line 789 "cplusql_grammar.y"
    {yyval.idp=NULL;}
    break;

  case 124:
#line 792 "cplusql_grammar.y"
    { yyval.idp=yyvsp[0].idp; }
    break;

  case 125:
#line 793 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,LT,yyvsp[0].idp );}
    break;

  case 126:
#line 794 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,GT,yyvsp[0].idp );}
    break;

  case 127:
#line 795 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,AND,yyvsp[0].idp );}
    break;

  case 128:
#line 796 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,OR,yyvsp[0].idp );}
    break;

  case 129:
#line 797 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,EQUALS,yyvsp[0].idp );}
    break;

  case 130:
#line 798 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,NOTEQUAL,yyvsp[0].idp);}
    break;

  case 131:
#line 799 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,MULTIPLY,yyvsp[0].idp );}
    break;

  case 132:
#line 800 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,DIVIDE,yyvsp[0].idp );}
    break;

  case 133:
#line 801 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,MODULO,yyvsp[0].idp );}
    break;

  case 134:
#line 802 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,PLUS,yyvsp[0].idp );}
    break;

  case 135:
#line 803 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,MINUS,yyvsp[0].idp );}
    break;

  case 136:
#line 804 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,BITAND,yyvsp[0].idp );}
    break;

  case 137:
#line 805 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,BITOR,yyvsp[0].idp );}
    break;

  case 138:
#line 806 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,POW,yyvsp[0].idp );}
    break;

  case 139:
#line 807 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,STR_PLUS,yyvsp[0].idp);}
    break;

  case 140:
#line 808 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,STR_LT,yyvsp[0].idp );}
    break;

  case 141:
#line 809 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,STR_GT,yyvsp[0].idp );}
    break;

  case 142:
#line 811 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-2].idp,STR_EQUALS,yyvsp[0].idp);}
    break;

  case 143:
#line 813 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-3].idp,LN,yyvsp[-1].idp );}
    break;

  case 144:
#line 815 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-1].idp,LN,NULL);}
    break;

  case 145:
#line 817 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-3].idp,LOG2,yyvsp[-1].idp );}
    break;

  case 146:
#line 819 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-1].idp,LOG2,NULL);}
    break;

  case 147:
#line 821 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-3].idp,LOG10,yyvsp[-1].idp );}
    break;

  case 148:
#line 823 "cplusql_grammar.y"
    {yyval.idp=cplusql_binary_exp(yyvsp[-1].idp,LOG10,NULL);}
    break;

  case 149:
#line 825 "cplusql_grammar.y"
    {
        fprintf(stderr, 
            "strcmp(lhs,rhs) is deprecated in favor of lhs ~= rhs, "
            "since the formers usage is the opposite of what a c "
            "programmer should have come to expect.\n");
        yyval.idp=cplusql_binary_exp(yyvsp[-3].idp,STR_EQUALS,yyvsp[-1].idp );
    }
    break;

  case 150:
#line 834 "cplusql_grammar.y"
    {
        yacfdbg("expression: NAME DOT NAME");
        yacfdbg((const char *) yyvsp[-2].idc);
        yacfdbg((const char *) yyvsp[0].idc);
        yyval.idp= cplusql_column_exp_str( yyvsp[-2].idc, yyvsp[0].idc );
    }
    break;

  case 151:
#line 842 "cplusql_grammar.y"
    {
        yacfdbg("expression: NAME");
        yacfdbg((const char *) yyvsp[0].idc);
        yyval.idp= cplusql_column_exp_str_dflt_rel( yyvsp[0].idc );
    }
    break;

  case 152:
#line 849 "cplusql_grammar.y"
    { yyval.idp= cplusql_literal_exp( yyvsp[0].idc ); }
    break;

  case 153:
#line 851 "cplusql_grammar.y"
    { yyval.idp= cplusql_literal_exp( yyvsp[0].idc ); }
    break;

  case 155:
#line 863 "cplusql_grammar.y"
    {
        /* expression-to-hash, num-buckets */
        yyval.idp= cplusql_hash_exp( yyvsp[-3].idp, yyvsp[-1].idc ); 
    }
    break;

  case 156:
#line 869 "cplusql_grammar.y"
    {
        /*  what to hash */
        yacfdbg("expression: md5 expression");
        yyval.idp= cplusql_md5_exp( yyvsp[-1].idp );
    }
    break;

  case 157:
#line 876 "cplusql_grammar.y"
    {
        yacfdbg("expression: crc48 expression");
        yyval.idp= cplusql_crc48_exp( yyvsp[-1].idp );
    }
    break;

  case 158:
#line 882 "cplusql_grammar.y"
    {
        /* map-to-look-in, key-expression, default-value */
        yacfdbg("expression: map_value_l expression");
        //yacfdbg((const char *) $1);
        yyval.idp= cplusql_map_value_l( yyvsp[-6].idc, yyvsp[-3].idp, yyvsp[-1].idp ); 
    }
    break;

  case 159:
#line 890 "cplusql_grammar.y"
    {
        /* map-to-look-in, key-expression, default-value */
        yacfdbg("expression: map_value expression");
        yyval.idp= cplusql_map_value( yyvsp[-5].idc, yyvsp[-3].idp, yyvsp[-1].idp ); 
    }
    break;

  case 160:
#line 897 "cplusql_grammar.y"
    {
        /* map-to-look-in, key-expression */
        yyval.idp= cplusql_map_exists_l( yyvsp[-4].idc, yyvsp[-1].idp ); 
    }
    break;

  case 161:
#line 903 "cplusql_grammar.y"
    {
        /* map-to-look-in, key-expression */
        yyval.idp= cplusql_map_exists( yyvsp[-3].idc, yyvsp[-1].idp ); 
    }
    break;

  case 162:
#line 909 "cplusql_grammar.y"
    {
        /* map-to-look-in, key-expression */
        yyval.idp= cplusql_abort( yyvsp[-1].idp ); 
    }
    break;

  case 163:
#line 915 "cplusql_grammar.y"
    { yyval.idp = yyvsp[-1].idp ; }
    break;

  case 164:
#line 918 "cplusql_grammar.y"
    {yyval.idp=cplusql_not(yyvsp[0].idp);}
    break;

  case 165:
#line 921 "cplusql_grammar.y"
    {
        yacfdbg("expression: join expression");
        yyval.idp=cplusql_join_columns( yyvsp[-4].idp, yyvsp[-1].idc );
    }
    break;

  case 166:
#line 925 "cplusql_grammar.y"
    {yyval.idp=cplusql_random( yyvsp[-3].idp, yyvsp[-1].idp );}
    break;

  case 167:
#line 926 "cplusql_grammar.y"
    {yyval.idp=cplusql_line_number( yyvsp[-1].idc );}
    break;

  case 168:
#line 928 "cplusql_grammar.y"
    {
        /* expression-to-hash, num-buckets */
        yacfdbg("expression: ceil expression");
        yyval.idp= cplusql_ceil_exp( yyvsp[-1].idp );
    }
    break;

  case 169:
#line 934 "cplusql_grammar.y"
    {
        /* expression-to-hash, num-buckets */
        yacfdbg("expression: floor expression");
        yyval.idp= cplusql_floor_exp( yyvsp[-1].idp );
    }
    break;

  case 170:
#line 940 "cplusql_grammar.y"
    {
        yacfdbg("expression: cntmatch expression");
        yyval.idp = cplusql_cntmatch_exp( yyvsp[-3].idp, yyvsp[-1].idp );
        ;
    }
    break;

  case 171:
#line 952 "cplusql_grammar.y"
    {
        yacfdbg("expression: bytestrip");
        cplusql_bytestrip_exp( yyvsp[-9].idp,yyvsp[-7].idc,yyvsp[-5].idc,yyvsp[-3].idc,yyvsp[-1].idc );
    }
    break;

  case 172:
#line 957 "cplusql_grammar.y"
    {
        yacfdbg("expression: normalize expression");
        yyval.idp = cplusql_normalize_exp( yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-1].idp, 0 );
    }
    break;

  case 173:
#line 962 "cplusql_grammar.y"
    {
        yacfdbg("expression: normalize expression");
        yyval.idp = cplusql_normalize_exp( yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-1].idp, 1 );
    }
    break;

  case 174:
#line 967 "cplusql_grammar.y"
    {
        yyval.idp = cplusql_iconv_exp( yyvsp[-5].idp, yyvsp[-3].idc, yyvsp[-1].idc );
    }
    break;

  case 175:
#line 971 "cplusql_grammar.y"
    {
        yacfdbg("expression: length()");
        yyval.idp = cplusql_length_exp( yyvsp[-1].idp );
    }
    break;

  case 176:
#line 976 "cplusql_grammar.y"
    {
        yacfdbg("expression: strstr");
        yyval.idp=cplusql_strstr_exp(yyvsp[-3].idp,yyvsp[-1].idp );
    }
    break;

  case 177:
#line 981 "cplusql_grammar.y"
    {
        yacfdbg("expression: regex match expression");
        yyval.idp=cplusql_regex_match_exp(yyvsp[-3].idc,yyvsp[-1].idp,0 );
    }
    break;

  case 178:
#line 986 "cplusql_grammar.y"
    {
        yacfdbg("expression: regex nocase match expression");
        yyval.idp=cplusql_regex_match_exp(yyvsp[-3].idc,yyvsp[-1].idp,1 );
    }
    break;

  case 179:
#line 991 "cplusql_grammar.y"
    {
        yacfdbg("expression: tolower( expression )");
        yyval.idp=cplusql_ftrans_exp(yyvsp[-1].idp, tolower );
    }
    break;

  case 180:
#line 996 "cplusql_grammar.y"
    {
        yacfdbg("expression: toupper( expression )");
        yyval.idp=cplusql_ftrans_exp(yyvsp[-1].idp, toupper );
    }
    break;

  case 181:
#line 1001 "cplusql_grammar.y"
    {
        yacfdbg("expression: llformat( format, expression )");
        yyval.idp=cplusql_llformat(yyvsp[-3].idc, yyvsp[-1].idp );
    }
    break;

  case 182:
#line 1006 "cplusql_grammar.y"
    {
        yacfdbg("expression: llformat( expression )");
        yyval.idp=cplusql_ll(yyvsp[-1].idp );
    }
    break;

  case 183:
#line 1011 "cplusql_grammar.y"
    {
        yacfdbg("expression: ldformat( format, expression )");
        yyval.idp=cplusql_ldformat(yyvsp[-3].idc, yyvsp[-1].idp );
    }
    break;

  case 184:
#line 1016 "cplusql_grammar.y"
    {
        yacfdbg("expression: command( )");
        yyval.idp=cplusql_command_exp(yyvsp[-2].idp, yyvsp[-1].idp );
    }
    break;

  case 185:
#line 1021 "cplusql_grammar.y"
    {
        yacfdbg("expression: chomp( )");
        yyval.idp=cplusql_chomp_exp(yyvsp[-1].idp );
    }
    break;

  case 186:
#line 1026 "cplusql_grammar.y"
    {
        yacfdbg("expression: warn( )");
        yyval.idp=cplusql_warn_exp( yyvsp[-1].idp );
    }
    break;

  case 187:
#line 1031 "cplusql_grammar.y"
    {
        yacfdbg("expression: info( )");
        yyval.idp=cplusql_info_exp( yyvsp[-1].idp );
    }
    break;

  case 188:
#line 1042 "cplusql_grammar.y"
    {
        yacfdbg("expression: dml()");
        yyval.idp=cplusql_dml_exp( yyvsp[-15].idc, yyvsp[-13].idc, yyvsp[-10].idp, yyvsp[-6].idp, yyvsp[-2].idp );
    }
    break;

  case 189:
#line 1048 "cplusql_grammar.y"
    {
        yacfdbg("expression: sprintf()");
        yyval.idp=cplusql_sprintf_exp( yyvsp[-2].idc, yyvsp[-1].idp );
    }
    break;

  case 190:
#line 1054 "cplusql_grammar.y"
    {
        yacfdbg("expression: sqlescape()");
        yyval.idp=cplusql_sqlescape_exp( yyvsp[-1].idp );
    }
    break;

  case 191:
#line 1060 "cplusql_grammar.y"
    {
        yacfdbg("expression: nanosleep");
        yyval.idp=cplusql_nanosleep_exp( yyvsp[-1].idc, yyvsp[0].idc );
    }
    break;

  case 192:
#line 1066 "cplusql_grammar.y"
    {
        yacfdbg("expression: nanosleep");
        yyval.idp=cplusql_nanosleep_exp( yyvsp[-3].idc, yyvsp[-1].idc );
    }
    break;

  case 193:
#line 1072 "cplusql_grammar.y"
    {
        yacfdbg("expression: pglisten");
        yyval.idp=cplusql_pglisten_exp( yyvsp[-3].idc, yyvsp[-1].idc );
    }
    break;

  case 194:
#line 1078 "cplusql_grammar.y"
    {
        yacfdbg("expression: sqlvalue");
        yyval.idp=cplusql_sqlvalue_exp( yyvsp[-3].idc, yyvsp[-1].idp );
    }
    break;

  case 195:
#line 1083 "cplusql_grammar.y"
    {
        yacfdbg("expression: coalesce");
        yyval.idp=cplusql_coalesce_exp( yyvsp[-1].idp );
    }
    break;

  case 196:
#line 1088 "cplusql_grammar.y"
    {
        yacfdbg("expression: substr");
        yyval.idp=cplusql_substr_exp( yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-1].idp );
    }
    break;

  case 197:
#line 1093 "cplusql_grammar.y"
    {
        yacfdbg("expression: factorial");
        yyval.idp=cplusql_factorial_exp( yyvsp[-1].idp );
    }
    break;

  case 198:
#line 1099 "cplusql_grammar.y"
    {
        yacfdbg("expression: sequence");
        yyval.idp=cplusql_sequence_exp( yyvsp[-5].idp, yyvsp[-3].idp, yyvsp[-1].idp );
    }
    break;

  case 199:
#line 1104 "cplusql_grammar.y"
    {
        yacfdbg("expression: strgreatest");
        yyval.idp=cplusql_strgreatest_exp( yyvsp[-1].idp );
    }
    break;

  case 200:
#line 1109 "cplusql_grammar.y"
    {
        yacfdbg("expression: strleast");
        yyval.idp=cplusql_strleast_exp( yyvsp[-1].idp );
    }
    break;

  case 201:
#line 1120 "cplusql_grammar.y"
    { yyval.idp= cplusql_ifthen_exp( yyvsp[-8].idp, yyvsp[-5].idp, yyvsp[-1].idp); }
    break;

  case 202:
#line 1125 "cplusql_grammar.y"
    { yyval.idp= cplusql_ifthen_exp( yyvsp[-6].idp, yyvsp[-3].idp, yyvsp[0].idp); }
    break;

  case 203:
#line 1130 "cplusql_grammar.y"
    { yyval.idp= cplusql_ifthen_exp( yyvsp[-9].idp, yyvsp[-5].idp, yyvsp[-1].idp); }
    break;

  case 204:
#line 1135 "cplusql_grammar.y"
    { yyval.idp= cplusql_ifthen_exp( yyvsp[-7].idp, yyvsp[-3].idp, yyvsp[0].idp); }
    break;

  case 205:
#line 1140 "cplusql_grammar.y"
    {
        yyval.idp=cplusql_metric_count(); 
    }
    break;

  case 206:
#line 1144 "cplusql_grammar.y"
    { 
        yyval.idp=cplusql_unary_metric(yyvsp[-3].id,yyvsp[-1].idp);
    }
    break;

  case 208:
#line 1160 "cplusql_grammar.y"
    {yyval.id= INTMIN ;}
    break;

  case 209:
#line 1161 "cplusql_grammar.y"
    {yyval.id=INTMAX ;}
    break;

  case 210:
#line 1162 "cplusql_grammar.y"
    {yyval.id=STRMIN ;}
    break;

  case 211:
#line 1163 "cplusql_grammar.y"
    {yyval.id=STRMAX ;}
    break;

  case 212:
#line 1164 "cplusql_grammar.y"
    {yyval.id=FIRST ;}
    break;

  case 213:
#line 1165 "cplusql_grammar.y"
    {yyval.id=LAST ;}
    break;

  case 214:
#line 1166 "cplusql_grammar.y"
    {yyval.id=AVG ;}
    break;

  case 215:
#line 1167 "cplusql_grammar.y"
    {yyval.id=COUNT_DISTINCT ;}
    break;

  case 216:
#line 1168 "cplusql_grammar.y"
    {yyval.id=COUNT_CHANGES ;}
    break;

  case 217:
#line 1169 "cplusql_grammar.y"
    {yyval.id=SUM ;}
    break;


    }

/* Line 1010 of yacc.c.  */
#line 3728 "cplusql_grammar.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1173 "cplusql_grammar.y"




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


