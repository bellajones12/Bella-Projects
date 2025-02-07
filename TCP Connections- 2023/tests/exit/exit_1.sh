#!/bin/bash

# Run server in the background

coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2


# Terminate the server by sending EXIT command
echo -n "Testing exit: "

cat tests/exit/exit_1.in | ncat localhost 1024


sleep 2

if diff tmp.actual.out tests/exit/exit_1.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi

coverage report -m
