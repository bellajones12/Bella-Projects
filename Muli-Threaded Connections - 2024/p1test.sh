#!/bin/bash

# adding permissions for all files
chmod -R +x ./tests/part1

# running all tests

cd tests/part1

./test02/test02.sh

./test05/test05.sh

./test06/test06.sh

./test07/test07.sh

./test08/test08.sh

./test09/test09.sh

./test10/test10.sh


# removing file2.data after its generation from test 9, so that tests can be run again
rm file2.data