#!/bin/ksh

nssFileId=$1
fileName=$2
readChunkSize=$3
primaryRws=$4

export GS_URL=`eval print \\\$GS_URL\$primaryRws`

../ReadWriteClient $ORBOPTS -o read -s $readChunkSize -f "$nssFileId" -t "file ${fileName}.rd"

if cmp $fileName ${fileName}.rd
then
	echo ok
else
	echo No ok: $nssFileId $fileName vs ${fileName}.rd
fi
