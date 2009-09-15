#!/bin/bash


. test-env

/bin/echo -n "starting services ... "
./test-start-nss.sh >test-start-nss.err 2>&1
echo DONE

#fork a sleep, we'll wait for it later to be sure we have collected nodes.
sleep 3 &

function runtest () {
    CPQL=$1
    EXPECT=$2
    /bin/echo -n "testing $CPQL ... "
    ../../src/cplusql -lyf ${CPQL}.cplusql >${CPQL}.err 2>&1
    if [ $? -ne $EXPECT ] ; then
        echo "FAILED"
        echo "test $CPQL did not exit $EXPECT"
        exit 1
    fi
    for ENG in $( ls ${CPQL}.output.*.eng 2>/dev/null )  ; do 
        REAL=$( basename $ENG .eng).out
        if [ ! -f $REAL ] ; then
            echo "FAILED"
            echo "in test $REAL not produced"
            exit 1
        fi
        sort $ENG -o ${ENG}.sorted
        sort $REAL -o ${REAL}.sorted
        cmp ${ENG}.sorted ${REAL}.sorted
        if [ $? -ne 0 ] ; then
            echo "FAILED"
            echo "in test $CPQL $ENG and $REAL differed"
            exit 1
        fi
    done
    for ENG in $( ls ${CPQL}.output.*.eng 2>/dev/null )  ; do 
        REAL=$( basename $ENG .eng).out
        rm -f $REAL ${REAL}.sorted ${ENG}.sorted
    done
    echo "OK"
}

#use cases
######################################################################
## hello world
######################################################################
runtest ping_reconfigure 0

######################################################################
## filename uniqueness handling
######################################################################
#touch mv rm  with {0,1} copies, 1 rw service ( must suceed )
export COPIES=0
runtest touch_mv_rm 0

/bin/echo -n "waitfornodes ... "
wait
echo OK
export COPIES=1
runtest touch_mv_rm 0

#mv does-not-exist somewhere ( must fail )
runtest mv_dne_foo 1

#rm does-not-exist ( must fail )
runtest rm_dne 1

#rm -f does-not-exist ( must suceed )
#runtest rm_f_dne 0

#mv somewhere allready-exists ( must fail )
runtest mv_foo_exists 1

#open_seq for writing must suceed
#find() must return correct subset of those.
#find_open_seq() must return correct subset of those.
#find_rm() must delete correct subset of those.


######################################################################
## nodes available handling
######################################################################
#touch with 2 copies needed , 1 rw service running ( must fail )
#touch with 2 copies needed , 2 rw service running ( must suceed )
#reconfigure, min_disk_free to be very large
#sleep 60, touch newfile 1 copy, disk ( must fail )
#reconfigure, min_disk_free to be very small
#sleep 60, touch newfile 1 copy, disk ( must suceed )
#touch $self .oos
#sleep 60, touch newfile 1 copy, disk ( must fail )
#rm .oos
#sleep 60, touch newfile 1 copy, disk ( must suceed )

######################################################################
## nodes available handling
######################################################################
runtest write 0
runtest read 0
runtest write 0
runtest write 0
runtest read 0
runtest read 0
runtest write_hash 0
runtest write_hash_100_empties 0
#runtest write_hash_20k_empties 0
runtest read_glob 0
