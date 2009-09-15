#!/bin/bash -x
NSHOSTPORT=$1
shift 
JEHOST=$1
shift 
# some usefull args ... --DORMSTALE=1 --DOKILL=1  --DOSTATUS=1 --ID=myjobid --COMMAND="sleep 30"
./JobExecClient --URL="corbaname::${NSHOSTPORT}#${JEHOST}_0_JobExecService" $@
