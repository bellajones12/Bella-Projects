#!/bin/bash

coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
echo -n "Testing invalid ADD command: "
cat tests/add/invalid_add.in | ncat localhost 1024

sleep 0.2

# exiting server
printf '!EXIT\n' | ncat localhost 1024

sleep 2

echo "Passed!"
