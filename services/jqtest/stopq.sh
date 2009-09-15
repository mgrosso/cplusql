#!/bin/bash -x

ls *.pid
kill $(cat *.pid)
sleep 2
kill -9 $(cat *.pid)
sleep 2
kill -9 $(cat *.pid)

rm -rf *.log *.err *.out *.ior *.pid core* svc.conf jqs/
