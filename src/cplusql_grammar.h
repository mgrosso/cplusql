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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 74 "cplusql_grammar.y"
typedef union YYSTYPE {
    char * idc;
    void * idp;
    int    id ;
} YYSTYPE;
/* Line 1285 of yacc.c.  */
#line 355 "cplusql_grammar.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



