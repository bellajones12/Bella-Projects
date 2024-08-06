#!/bin/bash

# adding permissions for all files
chmod -R +x ./tests/part2

# running all tests

cd tests/part2

./test01/test01.sh

./test02/test02.sh

./test03/test03.sh

./test04/test04.sh

./test05/test05.sh

./test06/test06.sh