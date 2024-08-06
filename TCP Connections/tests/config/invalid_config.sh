#!/bin/bash

# Run server in the background
# coverage erase
coverage run --append server.py tests/conf_not_found.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
echo -n "Testing config not found: "
sleep 0.1


sleep 2

if diff tmp.actual.out tests/config/invalid_config.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi
