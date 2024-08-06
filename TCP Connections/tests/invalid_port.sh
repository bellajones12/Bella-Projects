#!/bin/bash

# Run server in the background
# coverage erase
coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2

# Terminate the server by sending EXIT command
sleep 0.1

printf '!EXIT\n' | ncat localhost 1022

sleep 2

# coverage report -m

diff tmp.actual.out tests/invalid_port.out
