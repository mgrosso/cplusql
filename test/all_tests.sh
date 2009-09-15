#!/bin/bash 
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      all_tests.sh
## Author:    mgrosso Matthew Grosso
## Created:   Sat Oct  1 19:35:32 PDT 2005
## Project:   cplusql
## Purpose:   
## 
## Copyright (c) 2003 Matt Grosso. All Rights Reserved.
## 
## $Id: all_tests.sh,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------


FAILURE_COUNT=0
COUNT=0
ALLTESTS=$(cat working_tests.txt | grep -v "^$" |grep -v "^#")
for T in $ALLTESTS ; do
    ./test.sh ../src/cplusql  cplusql ${T}
    FAILURE_COUNT=$(($FAILURE_COUNT+$?))
    COUNT=$((COUNT+1))
done
echo $COUNT tests complete. $FAILURE_COUNT failures.
exit $FAILURE_COUNT
