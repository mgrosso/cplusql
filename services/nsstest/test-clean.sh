#!/bin/bash -x

ls *.pid
kill $(cat *.pid)
rm -f *.log *.err *.ior *.pid *.out core core.*
rm -rf rws node_cntl
