#!/bin/bash

TA=0
TB=0

/usr/bin/time -f %e ./integral 1 2> /tmp/time1; sleep 1; TA=`cat /tmp/time1`
/usr/bin/time -f %e ./integral 2 2> /tmp/time2; sleep 1; TB=`cat /tmp/time2`
echo "----------------------"
echo "(("$TB*2-$TA")"/$TB")"*100 | bc -lq
