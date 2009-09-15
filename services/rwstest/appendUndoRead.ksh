#!/bin/ksh

writerId=$1
nssFileId=$2
fileName=$3
seqNum=$4
writeChunkSize=$5
readChunkSize=$6
primaryRws=$7

export GS_URL=`eval print \\\$GS_URL\$primaryRws`

echo "saved" > f.$$

append.ksh $writerId $nssFileId $fileName $seqNum $writeChunkSize $primaryRws
append.ksh 1$writerId $nssFileId f.$$ $seqNum $writeChunkSize $primaryRws

undo.ksh $writerId $nssFileId $primaryRws

readVerify.ksh $nssFileId f.$$ $readChunkSize $primaryRws
