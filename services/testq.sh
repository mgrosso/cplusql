#!/bin/bash -x

if [ $# -ne 2 ] ; then 
    echo $0 nshost nsport
    exit 1
fi
export NS_HOST=$1
shift
export NS_PORT=$1
shift

export APPLOG_MINLOG=0
export JQS_IOR_FILE=JobQueueService.ior
export JQS_IOR_URI=file://${JQS_IOR_FILE}

# cant change this with command line...
#ORBOPTS="-ORBConcurrency thread-per-connection -ORBDebugLevel 1 " #-ORBConnectStrategy blocked "
#
# have to do this instead...
cat >svc.conf <<ENDCAT
static Server_Strategy_Factory "-ORBConcurrency thread-per-connection"
ENDCAT


ls *.pid
kill $(cat *.pid)
sleep 2
# not working?
nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service $ORBOPTS -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 -f Naming_Service.state >Naming_Service.err 2>&1 &
#nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 >>Naming_Service.err 2>&1 &
sleep 2
./NodeMonitorService $ORBOPTS --GS_CORBANAME=CentralNodeMonitorService >cnms.err 2>&1
sleep 2
./NodeMonitorService >nms.err 2>&1
sleep 2
for GS_INSTANCE_ID in f u b a r ; do 
    ./JobExecService $ORBOPTS --GS_INSTANCE_ID=$GS_INSTANCE_ID  --JE_MAX_SLOTS=20 >je${GS_INSTANCE_ID}.err 2>&1
    sleep 2
done
sleep 2

mkdir jqs
#rm -f jqs/node_cntl/*.oos
./JobQueueService $ORBOPTS --JQS_ROOT=jqs --GS_CORBANAME=JobQueueService --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService" >jqs.err 2>&1
