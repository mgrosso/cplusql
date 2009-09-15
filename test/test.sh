#!/bin/bash 
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      test.sh
## Author:    mgrosso Matthew Grosso
## Created:   Thu Feb 27 02:56:40 EST 2003 on circe.looksmart.net
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 LookSmart. All Rights Reserved.
## 
## $Id: test.sh,v 1.5 2003/06/03 21:50:59 djain Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

ulimit -c unlimited 

EXE=$1
SUFFIX=$2
TEST=$3

function puke {
    echo $@
    exit 1
}

function puke_if {
    E=$1
    shift
    if [ $E -ne 0 ] ; then
        puke $@
    fi
}

function run_test {
    TEST=$1
    echo -n "running $TEST ... "
    rm -f ${TEST}.out ${TEST}.err ${TEST}.*.diff ${TEST}.*.out 2>/dev/null
    if [ -r ${TEST}.env ] ; then
        . ${TEST}.env
    fi
    if [ ! -f ${TEST}.${SUFFIX} ] ; then
        puke "cant test $TEST without ${TEST}.${SUFFIX}"
    fi
    $EXE --file=${TEST}.${SUFFIX} -y -l >${TEST}.out 2>${TEST}.err
    RET=$?
    if [ -f ${TEST}.mustfail ] ; then
        if [ $RET -eq 0 ] ; then
            puke "FAIL: supposed to have non zero exit status but didnt"
        fi
        ls core* 2>/dev/null
    else
        puke_if $RET "FAIL: produced non-zero exit status"
    fi

    for ENGINEERED in $( ls ${TEST}.*.eng 2>/dev/null ) ; do
        REAL=$( basename $ENGINEERED .eng ).out
        DIFF=$( basename $ENGINEERED .eng ).diff
        if [ ! -f $REAL ] ; then
            puke "FAIL: no $REAL output file exists"
        fi
        diff $ENGINEERED $REAL >$DIFF 2>&1
        puke_if $? "FAIL: $REAL had diffs vs $ENGINEERED, see $DIFF"
    done
    echo PASS
}

run_test $TEST 
