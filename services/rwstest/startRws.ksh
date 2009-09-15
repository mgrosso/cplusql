#!/bin/ksh

NUM=$1
cnt=1

cat >svc.conf <<EOF
static Server_Strategy_Factory "-ORBConcurrency thread-per-connection"
EOF

while [[ $cnt -le $NUM ]]; do
	export BASE_DIR=`pwd`/rws${cnt}
	export GS_URL${cnt}="corbaname::${NS_HOST}:${NS_PORT}#ReadWriteService_${cnt}"
	export GS_URL=`eval print \\\$GS_URL\$cnt`

	echo ${cnt}: $GS_URL
	mkdir -p $BASE_DIR
	rm -rf ${BASE_DIR}/*

	../ReadWriteService --APPLOG_MINLOG=0 --RWS_ROOT=rws --GS_CORBANAME=ReadWriteService_${cnt} --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService"
	../PingClient -u "corbaname::${NS_HOST}:${NS_PORT}#ReadWriteService_${cnt}" --APPLOG_MINLOG=2

#	gdb ../ReadWriteService

	cnt=$((cnt + 1))
done
