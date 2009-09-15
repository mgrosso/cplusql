#!/bin/ksh

writerId=$1
nssFileId=$2
fileName=$3
primaryRws=$4

export GS_URL=`eval print \\\$GS_URL\$primaryRws`

set -x
../ReadWriteClient $ORBOPTS -o statread -w $writerId -f "$nssFileId" -t "file ${fileName}.rd"
set +x

ls -l $fileName ${fileName}.rd

if cmp $fileName ${fileName}.rd
then
	echo ok
else
	echo No ok: $nssFileId $fileName vs ${fileName}.rd
fi
