#!/bin/bash 
NS_HOST=$1
shift 
NS_PORT=$1
shift 
RWS_HOST=$1
shift 
#order is always 0 (zero) if just one NMS is running on RWS_HOST
RWS_ORDER=$1
shift 
echo ./ReadWriteClient --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#${RWS_HOST}_${RWS_ORDER}_ReadWriteService" $@
./ReadWriteClient --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#${RWS_HOST}_${RWS_ORDER}_ReadWriteService" $@
