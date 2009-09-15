#!/bin/bash 
NSS_URL=$1
shift
OUTPUT_FN=$1
shift
WRITER_ID=$1
shift
if [ -z ${NSS_URL} ]; then
    NSS_URL="corbaname::sv-dw222:9669#NameSpaceService"
fi
if [ -z ${OUTPUT_FN} ]; then
    OUTPUT_FN="clusterio_write"
fi
if [ -z ${WRITER_ID} ]; then
    WRITER_ID="dest_ut"
fi
export NSS_URL OUTPUT_FN WRITER_ID
#dest_file
../src/cplusql -ylf clusterio_write.cplusql > clusterio_write.ut1.err 2>&1 
#dest_hash_file
../src/cplusql -ylf clusterio_write.cplusql > clusterio_write.ut2.err 2>&1 
