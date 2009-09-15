#!/bin/ksh

pids=`ls ReadWriteService*.pid 2> /dev/null`
status=$?
if [[ $status -eq 0 ]]; then
	for f in *.pid ; do
		echo $f
		pid=`cat $f`
		if [[ $pid -gt 0 ]]; then
			kill $pid
		fi
		rm $f
	done
fi

#rm -f ReadWriteService*.log ReadWriteService*.err ReadWriteService*.out
