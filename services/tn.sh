#!/bin/bash

NS_HOST=sf-devdw004
NS_PORT=14000
export GS_ROOTDIR=/var/local/cplusql

export PATH=$HOME/usr/local/bin:$PATH
export VALGRIND=valgrind

export VALGRIND_OPTS="-v --trace-children=yes --num-callers=20 --error-limit=no --trace-malloc=yes --leak-check=yes --show-reachable=yes --trace-signals=yes "

$VALGRIND ./TestNarrow ${NS_HOST} ${NS_PORT} $1 > $2_$1.val 2>&1

