#!/bin/ksh

writerId=$1
nssFileId=$2
primaryRws=$3

export GS_URL=`eval print \\\$GS_URL\$primaryRws`

echo Undo of $writerId for $nssFileId
../ReadWriteClient $ORBOPTS -o undo -w $writerId -f "$nssFileId"
