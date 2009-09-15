#!/bin/bash
NS_HOST=$1
shift 
NS_PORT=$1
shift 
NM_HOST=$1
shift 
#order is always 0 (zero) if just one NMS is running on NM_HOST
NM_ORDER=$1
shift 
echo ./NodeMonitorClient --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#${NM_HOST}_${NM_ORDER}_NodeMonitorService" $@
./NodeMonitorClient --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#${NM_HOST}_${NM_ORDER}_NodeMonitorService" $@
