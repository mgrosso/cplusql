#!/bin/bash
NS_HOST=$1
shift
NS_PORT=$1
shift
echo ./JobQueueService --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_CORBANAME=JobQueueService --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService" $@
./JobQueueService --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} --GS_CORBANAME=JobQueueService --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService" $@
