#!/bin/bash  -x
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      auto.sh
## Author:    mgrosso Matthew E Grosso
## Created:   Wed Dec 17 01:50:20 EST 2003 on erasmus.erasmus.org
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 Matthew E Grosso. All Rights Reserved.
## 
## $Id: autoclean.sh,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

find . -name "*.o" -exec rm -f {} \;
find . -name "*.output" -exec rm -f {} \;
find . -name Makefile -exec rm -f {} \;
find . -name Makefile.in -exec rm -f {} \;
rm -rf inc/config.h aclocal.m4 autom4te.cache configure config.status \
    src/cplusql_grammar.c  \
    src/cplusql_grammar.h \
    src/cplusql_grammar.output \
    src/cplusql_lexer.c
