#!/bin/ksh

writerId=$1
nssFileId=$2
fileName=$3
seqNum=$4
writeChunkSize=$5
readChunkSize=$6
primaryRws=$7

append.ksh $writerId $nssFileId $fileName $seqNum $writeChunkSize $primaryRws
readVerify.ksh $nssFileId $fileName $readChunkSize $primaryRws
