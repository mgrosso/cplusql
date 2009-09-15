#!/bin/bash
NS_HOST=$1
shift
NS_PORT=$1
shift
#echo nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 -f Naming_Service.state \>\>Naming_Service.err 2\>\&1 \&
echo nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 \>\>Naming_Service.err 2\>\&1 \&
#was not working with -f flag nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 -f Naming_Service.state >>Naming_Service.err 2>&1 &
nohup $TAO_ROOT/orbsvcs/Naming_Service/Naming_Service -ORBEndpoint iiop://${NS_HOST}:${NS_PORT} -p Naming_Service.pid -m 0 >>Naming_Service.err 2>&1 &
