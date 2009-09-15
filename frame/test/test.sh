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
## $Id: test.sh,v 1.2 2004/01/10 18:56:13 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

FAILURES=0

ulimit -c unlimited 
export LD_LIBRARY_PATH=../src/:$LD_LIBRARY_PATH

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
    if [ -f ${TEST}.skip ] ; then
        echo "skipping $TEST "
        return
    fi
    echo -n "running $TEST ... "
    rm -f ${TEST}.out ${TEST}.err ${TEST}.*.diff ${TEST}.*.out 2>/dev/null
    if [ -r ${TEST}.env ] ; then
        . ${TEST}.env
    fi
    $TEST >${TEST}.out 2>${TEST}.err
    RET=$?
    if [ -f ${TEST}.mustfail ] ; then
        if [ $RET -eq 0 ] ; then
            puke "FAIL: supposed to have non zero exit status but didnt"
        fi
        ls core* 2>/dev/null
    else
        if [ $RET -ne 0 ] ; then
            echo "FAIL: produced non-zero exit status of $RET "
            FAILURES=$((FAILURES+1))
            return
        fi
    fi

    for ENGINEERED in $( ls ${TEST}.*.eng 2>/dev/null ) ; do
        REAL=$( basename $ENGINEERED .eng ).out
        DIFF=$( basename $ENGINEERED .eng ).diff
        if [ ! -f $REAL ] ; then
            REAL=$( basename $ENGINEERED .eng )
            if [ ! -f $REAL ] ; then
                puke "FAIL: no $REAL output file exists"
            fi
        fi
        diff -b $ENGINEERED $REAL >$DIFF 2>&1
        if [ $? -ne 0 ] ; then
            echo "FAIL: $REAL had diffs vs $ENGINEERED, see $DIFF"
            FAILURES=$((FAILURES+1))
            return
        fi
    done
    rm ${TEST}.out 2>${TEST}.err
    echo PASS
}

function usage(){
    cat <<_USAGE
$0 [ --accept test_FOO ] 

To run all tests, just run ./test.sh. To accept the standard input and output
of a test, run with --accept \$sometest

_USAGE

}

if test $# -gt 0  ; then
    if test x$1 = x--accept ; then
        if test $# -ne 2  ; then 
            usage
        fi
        TEST=$2
        echo running $TEST
        ./$TEST >${TEST}.out.eng 2>${TEST}.err.eng
        echo $?
        echo $TEST | sed -e 's/test_//' >>working_tests.txt
        exit 0
    else
        usage
    fi
fi

for N in $(cat working_tests.txt ) ; do
    run_test ./test_$N 
done
exit $FAILURES

