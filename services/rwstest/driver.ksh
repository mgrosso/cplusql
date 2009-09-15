#!/bin/ksh

cd `dirname $0`
. rwsEnv.ksh

rm ReadWriteService*.log

stopRws.ksh

. startRws.ksh $NUM_RWS
#. startRws.ksh 1

cp /etc/passwd ./passwd
#cat /dev/random | head -c5000 > randf
nssFile=1
primaryRws=1

{
	echo '---------------------------------'
	file=passwd
	seqNum=1
	echo
	set -x
	appendRead.ksh writerId ${nssFile} $file $seqNum 10 1000 $primaryRws
	set +x
	nssFile=$((nssFile+1))
}

{
	echo '---------------------------------'
	file=randf
	seqNum=1
	echo
	set -x
	appendRead.ksh rand ${nssFile} $file $seqNum 10 1000 $primaryRws
	set +x
	nssFile=$((nssFile+1))
}

{
	echo '---------------------------------'
	file=passwd
	echo
	set -x
	append.ksh writerId $nssFile $file $seqNum 100 $primaryRws
	statReadVerify.ksh writerId $nssFile $file $primaryRws
	set +x
	nssFile=$((nssFile+1))
}

{
	echo '---------------------------------'
	file=passwd
	seqNum=1
	echo
	set -x
	appendUndoRead.ksh test2 ${nssFile} $file $seqNum 10 1000 $primaryRws
	set +x
	nssFile=$((nssFile+1))
}

{
	echo '---------------------------------'
	file=passwd
	blockSize=1000
	seqNum=1
	export GS_URL=$GS_URL1
#	../ReadWriteClient -o fail -a "fhAppendSleep=1"
	append.ksh wid1 ${nssFile} $file $seqNum $blockSize $primaryRws &
	append.ksh wid2 ${nssFile} $file $seqNum $blockSize $primaryRws &
	append.ksh wid3 ${nssFile} $file $seqNum $blockSize $primaryRws &
	append.ksh wid4 ${nssFile} $file $seqNum $blockSize $primaryRws &
	append.ksh wid5 ${nssFile} $file $seqNum $blockSize $primaryRws &

	wait

	undo.ksh wid1 ${nssFile} $primaryRws &
	undo.ksh wid2 ${nssFile} $primaryRws &
	undo.ksh wid3 ${nssFile} $primaryRws &
	undo.ksh wid4 ${nssFile} $primaryRws &
#	undo.ksh wid5 ${nssFile} $primaryRws &

	wait

	readVerify.ksh $nssFile $file 1000 $primaryRws
	ls -l passwd*
	readVerify.ksh $nssFile $file 1000 2
	ls -l passwd*
	nssFile=$((nssFile+1))
}

#stopRws.ksh
