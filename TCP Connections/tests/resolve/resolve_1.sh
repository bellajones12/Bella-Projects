#!/bin/bash

# Run server in the background
coverage run --append server.py tests/sample.conf > tmp.actual.out &
sleep 2

echo -n "Testing valid resolve: "
sleep 0.1
> tmp.actual.out
while IFS= read -r line; do
    echo "$line" | ncat localhost 1024 >> tmp.actual.out
    sleep 0.1
done < "tests/resolve/resolve_1.in"

sleep 0.1

printf '!EXIT\n' | ncat localhost 1024

sleep 2

# coverage report -m

if diff tmp.actual.out tests/resolve/resolve_1.out >/dev/null 2>&1; then
    echo "Passed!"
else
    echo "Failed"
fi
