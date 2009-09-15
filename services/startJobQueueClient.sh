#!/bin/bash 
NS_HOST=$1
shift 
NS_PORT=$1
shift 
echo ./JobQueueClient --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#JobQueueService" $@
./JobQueueClient --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_URL="corbaname::${NS_HOST}:${NS_PORT}#JobQueueService" $@
