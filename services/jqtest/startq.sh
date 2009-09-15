#!/bin/bash -x

if [ $# -ne 2 ] ; then 
    export NS_HOST=localhost
    export NS_PORT=9999
else
    export NS_HOST=$1
    shift
    export NS_PORT=$1
    shift
fi
if [ -z $NUMSLOTS ] ; then
    export NUMSLOTS=10
fi
if [ -z $NUMJE ] ; then
    export NUMJE=10
fi

ulimit -c unlimited

#PingClient: usage: -s <sleep micro Seconds> -u <url> -w <maxTime>\n");
function ping_client() {
    GSN=$1
    ../PingClient -w 20 -u "corbaname::${NS_HOST}:${NS_PORT}#${GSN}" 
    if [ $? -ne 0 ] ; then
        echo "could not ping client"
        exit 1
    fi
}

# cant change this with command line...
#ORBOPTS="-ORBConcurrency thread-per-connection -ORBDebugLevel 1 " #-ORBConnectStrategy blocked "
#
# have to do this instead...
cat >svc.conf <<ENDCAT
static Server_Strategy_Factory "-ORBConcurrency thread-per-connection"
ENDCAT

# not working?
nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service $ORBOPTS -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 -f Naming_Service.state >Naming_Service.err 2>&1 &
#nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 >>Naming_Service.err 2>&1 &
sleep 5
../NodeMonitorService $ORBOPTS --GS_CORBANAME=CentralNodeMonitorService >cnms.err 2>&1
ping_client CentralNodeMonitorService
../NodeMonitorService >nms.err 2>&1
ping_client $(uname -n)_0_NodeMonitorService

GS_INSTANCE_ID=0
while [ $GS_INSTANCE_ID -lt $NUMJE ] ; do
    ../JobExecService $ORBOPTS --GS_INSTANCE_ID=$GS_INSTANCE_ID  --JE_MAX_SLOTS=$NUMSLOTS >je${GS_INSTANCE_ID}.err 2>&1
    ping_client $(uname -n)_${GS_INSTANCE_ID}_JobExecService
    GS_INSTANCE_ID=$((GS_INSTANCE_ID+1))
done
sleep 2

mkdir jqs
rm -f jqs/node_cntl/*.oos

export APPLOG_MINLOG=0
../JobQueueService $ORBOPTS --JQS_ROOT=jqs --GS_CORBANAME=JobQueueService --JQS_SCHEDULER_THREAD_SLEEPTIME=10000000 --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService" >jqs.err 2>&1
ping_client JobQueueService

