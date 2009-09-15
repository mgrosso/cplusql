#!/bin/bash -x
cat <<ALABALA | $CPLUSQL_BIN -ylf ceil_floor_test.cplusql 
1
1.1
1.5
1.6
2
ALABALA
