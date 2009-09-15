#!/bin/bash
NS_HOST=$1
shift
NS_PORT=$1
shift
echo ./NodeMonitorService --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} $@
./ReadWriteService --NS_HOST=${NS_HOST} --NS_PORT=${NS_PORT} $@
