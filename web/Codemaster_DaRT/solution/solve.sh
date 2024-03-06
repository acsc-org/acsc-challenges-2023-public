#!/bin/bash

while :; do ./sol-sub1.sh; sleep 4; done &
pid1=$!
while :; do ./sol-sub2.sh a; done &
pid2=$!
sleep 1
while :; do ./sol-sub2.sh b; done &
pid3=$!

function func() {
    status=$?
    kill $pid1
    kill $pid2
    kill $pid3
    wait $pid1; echo "killed 1"
    wait $pid2; echo "killed 2"
    wait $pid3; echo "killed 3"
    echo exit
    exit $status
}

trap 'func' 1 2 3 15

echo goodluck
sleep 600
func 
