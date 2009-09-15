#!/bin/bash -x


export NS_HOST=localhost
export NS_PORT=9999
export APPLOG_MINLOG=0
export NSS_IOR_FILE=NameSpaceService.ior
export NSS_IOR_URI=file://${JQS_IOR_FILE}
ulimit -c unlimited

cat >svc.conf <<ENDCAT
static Server_Strategy_Factory "-ORBConcurrency thread-per-connection"
ENDCAT

#PingClient: usage: -s <sleep micro Seconds> -u <url> -w <maxTime>\n");
function ping_client() {
    GSN=$1
    ../PingClient -s 1 -w 20 -u "corbaname::${NS_HOST}:${NS_PORT}#${GSN}" 
    if [ $? -ne 0 ] ; then
        echo "could not ping client"
        exit 1
    fi
}


./test-clean.sh
sleep 1
# not working?
nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service $ORBOPTS -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 >Naming_Service.err 2>&1 &
sleep 5

../NodeMonitorService $ORBOPTS --GS_CORBANAME=CentralNodeMonitorService >cnms.err 2>&1
ping_client CentralNodeMonitorService

../NodeMonitorService >nms.err 2>&1
ping_client $(uname -n)_0_NodeMonitorService

../NameSpaceService $ORBOPTS >nss.err 2>&1
ping_client NameSpaceService

mkdir rws
../ReadWriteService --BASE_DIR=rws $ORBOPTS >rws.err 2>&1
ping_client $(uname -n)_0_ReadWriteService
