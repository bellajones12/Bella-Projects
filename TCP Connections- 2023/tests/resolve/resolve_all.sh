#!/bin/bash

# Run the server code in the background
coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2

echo -n "Testing adding, resolving, deleting then attempting to resolve: "
sleep 2

> tmp.actual.out

# Test adding a record
echo "!ADD example.com 1234" | ncat localhost 1024 >> tmp.actual.out

# Test resolving a record
echo "example.com" | ncat localhost 1024 >> tmp.actual.out

# Test deleting a record
echo "!DEL example.com" | ncat localhost 1024 >> tmp.actual.out

# Test resolving a non-existing record
echo "example.com" | ncat localhost 1024 >> tmp.actual.out


printf '!EXIT\n' | ncat localhost 1024

if diff tmp.actual.out tests/resolve/resolve_all.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi

