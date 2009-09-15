#!/bin/ksh

writerId=$1
nssFileId=$2
fileName=$3
seqNum=$4
writeChunkSize=$5
primaryRws=$6

export GS_URL=`eval print \\\$GS_URL\$primaryRws`
echo Appending $fileName to $nssFileId for $writerId
../ReadWriteClient $ORBOPTS -o append -s $seqNum -b $writeChunkSize -w $writerId -f "$nssFileId" -t "file $fileName"
