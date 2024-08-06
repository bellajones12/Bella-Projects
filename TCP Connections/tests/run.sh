#!/bin/bash

coverage erase
chmod -R +x /home/tests/
# ./tests/invalid_port.sh

# adding testing
./tests/add/add_1.sh
./tests/add/invalid_add.sh

# config testing
./tests/config/invalid_config.sh
./tests/config/invalid_config2.sh
./tests/config/invalid_config3.sh
./tests/config/non_int_port.sh

# resolve testing
./tests/resolve/resolve_1.sh
./tests/resolve/invalid_resolve.sh
./tests/resolve/resolve_all.sh

# input testing
./tests/inputs/invalid_inputs.sh

# exit testing
./tests/exit/exit_1.sh
