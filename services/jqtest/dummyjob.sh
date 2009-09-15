#!/bin/bash
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      dummyjob.sh
## Author:    mgrosso 
## Created:   Wed Sep 29 15:12:08 EDT 2004 on caliban
## Project:   
## Purpose:   
## 
## $Id: dummyjob.sh,v 1.1 2004/09/30 22:10:02 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

SLEEPTIME=$1
TARGET=$2

echo "starting job $TARGET at $( date )"
sleep $SLEEPTIME
echo "pid $$ ppid $PPID doing $TARGET from host $(uname -n ) " > $TARGET
echo "done     job $TARGET at $( date )"

