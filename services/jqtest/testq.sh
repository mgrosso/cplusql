#!/bin/bash

#10 je @ 10 slots each gives us 100 node seconds per second
export NUMSLOTS=10
export NUMJE=10

./stopq.sh
./startq.sh
if [ $? -ne 0 ] ; then exit 1 ; fi

#1000 jobs * 6 seconds of sleep per job = 6000 minimum cpu seconds.
#6000 / 100 = 60, so minimum time is 60 seconds.
export FIRSTBUCKET=0
export LASTBUCKET=99
export SLEEPTIME=1
export APPLOG_MINLOG=0
JOBSTART=$(date +"%Y%m%d%H%M")

#
# if problems arise, the two lines below may make debugging easier. They are
# left commented out by default since they could produce their own problems and
# neither would normally be used in a production environment.
#
#export CPLUSQL_NOTHREADS=1
#export HANG_ON_PANIC=1

../../src/cplusql -f jobloop.cplusql --NS_HOST=localhost --NS_PORT=9999
if [ $? -ne 0 ] ; then 
    exit 1 
fi
JOBEND=$(date +"%Y%m%d%H%M")

#make sure at least one minute has passed...
if [ $JOBSTART = $JOBEND ] ; then
    exit 1
fi
echo "started $JOBSTART ,  finished $JOBEND "
./stopq.sh
exit 0
