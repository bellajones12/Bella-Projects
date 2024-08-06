#!/bin/bash

# Run server in the background
# coverage erase
coverage run --append server.py arg1 tests/sample.conf > tmp.actual.out &
sleep 2

echo -n "Testing invalid inputs: "
sleep 0.1

sleep 2

if diff tmp.actual.out tests/inputs/invalid_inputs.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi
