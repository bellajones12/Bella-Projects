#!/bin/bash

# Run server in the background
# coverage erase
coverage run --append server.py tests/invalid2.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
echo -n "Testing invalid port in config: "
sleep 0.1

printf '!EXIT\n' | ncat localhost 1024

sleep 2

echo "Passed!"
