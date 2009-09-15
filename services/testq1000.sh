#!/bin/bash -x

if [ $# -ne 2 ] ; then 
    echo $0 nshost nsport
    exit 1
fi
export NS_HOST=$1
shift
export NS_PORT=$1
shift

#ORBOPTS="-ORBConcurrency thread-per-connection -ORBDebugLevel 1 " #-ORBConnectStrategy blocked "
ls *.pid
kill $(cat *.pid)
sleep 5
# not working?
nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service $ORBOPTS -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 -f Naming_Service.state >Naming_Service.err 2>&1 &
#nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 >>Naming_Service.err 2>&1 &
sleep 5
./NodeMonitorService $ORBOPTS --GS_CORBANAME=CentralNodeMonitorService >cnms.err 2>&1
sleep 5
./NodeMonitorService >nms.err 2>&1
sleep 5
for GS_INSTANCE_ID in f u b a r ; do 
    ./JobExecService $ORBOPTS --GS_INSTANCE_ID=$GS_INSTANCE_ID  --JE_MAX_SLOTS=3 >je${GS_INSTANCE_ID}.err 2>&1
done
sleep 5

mkdir jqs
#rm -f jqs/node_cntl/*.oos
./JobQueueService $ORBOPTS --JQS_ROOT=jqs --GS_CORBANAME=JobQueueService \
    --JQS_SCHEDULER_THREAD_SLEEPTIME=1000000 \
    --GS_DEFAULT_SUBSCRIBE_TARGET_URL="corbaname::${NS_HOST}:${NS_PORT}#CentralNodeMonitorService" \
    >jqs.err 2>&1
sleep 5
