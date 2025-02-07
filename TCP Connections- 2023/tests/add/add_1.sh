#!/bin/bash

# Run server in the background
coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
echo -n "Testing ADD command: "
cat tests/add/add_1.in | ncat 'localhost' 1024

sleep 0.2

# exiting server
printf '!EXIT\n' | ncat 'localhost' 1024

echo "Passed!"

sleep 2
