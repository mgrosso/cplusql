#!/bin/ksh

if [[ -z $NS_HOST ]]; then
	export NS_HOST=`hostname`
fi
if [[ -z $NS_PORT ]]; then
	export NS_PORT=14205
fi

export ORBOPTS="--APPLOG_MINLOG=2"

export NUM_RWS=2
