#!/bin/bash

# Run server in the background
# coverage erase
coverage run --append server.py tests/non_int_port.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
echo -n "Testing non int port in config: "
sleep 0.1


sleep 2

if diff tmp.actual.out tests/config/non_int_port.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi
