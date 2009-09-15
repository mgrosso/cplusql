#!/bin/bash -x
NS_HOST=$1
shift
NS_PORT=$1
shift
echo ./JobExecService --NS_HOST=$NS_HOST --NS_PORT=$NS_PORT $@
./JobExecService --NS_HOST=$NS_HOST --NS_PORT=$NS_PORT $@
