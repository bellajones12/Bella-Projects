## Part 1 ##

The code for part 1 is structed under 4 separate folders:

chk: this folder contains all the data for checking completeness of chunks, loading .bpkg objects, calling min hashes, and hashing chunks.

cryp: this folder contains functions for computing the hashes from a .data file, which can then be placed into the tree

tree: this folder continas all information relevant to the merkle tree including its construction, traversal to compute internal nodes hashes, combining the hashes, returning the minimum completed hashes and so on.

All header files are places in corresponding .h files under include.

pkgmain.c: this file contains the main function which handles user input and calls the relevant functions

## Part 2 ##

The code for part 2 is structured under one folder, with separate files:

client.c: this file contains all information for the client to create a connection to a server, and send and recieve relevant packages

config.c: this file contains functions to extract config data and create a config object

handling_connect.c: this file calls functions based on user input, and determines if the input is valid

package.c: this file creates packages, and adds them to the linked list of packages. It also contains functions to remove packages, print details of packages handled, and free allocated memory

packet.c: this file creates different packets based on the msg code needed. It also serialises and deserialises packets so that they can be sent from the client to the server and vice versa

peer.c: this file creates new peers and creates connections from clients to servers. It aslo contains functions to disconnect peers, manages the linked list of peers, and free allocated memory. It also contains the logic for the fetch function, which involves opening a .bpkg file and extracting the relevant information, sending it to the server, and then extracting the information sent back, before writing it into the relevant .data file

server.c: this file contins information for the server connection, which continues running and listening for new connections. Based on these connections, the server does different things. This file also contains functions to create new client-information structures, to keep track of connected clients

btide.c: this file contains the main function which handles user input, creates the server thread, and cleans up all resources upon exit

## Testing ##

### Part 1 ###

# Instructions to run testing script
To run the testing script, first place the executable pkgmain in the tests/part1 directory. 
*NOTE* pkgmain must be placed as a binary executable that has already been compiled.
All tests can be run through p1test.sh. First, permissions must be executed for the run.sh through: chmod +x p1test.sh
p1test.sh will then execute each test.
Each test has its own folder containing the relevant .bpkg file, expected output (if any) and the testing script to run.
Each test will either output:
"Test x passed!" or "Test x failed" to easy highlight to readers which testing was successful.
If unsuccessful, the differing outputs will be printed to allow examination. 

*NOTE* file1.data from the resources/pkgs must be placed into tests/part1 in order for testing to be successful

### Part 2 ###

# Instructions to run testing script
To run the testing script, first place the executable btide in the tests/part2 directory. 
*NOTE* btide must be placed as a binary executable that has already been compiled.
All tests can be run through p2test.sh. First, permissions must be executed for the run.sh through: chmod +x p2test.sh
p2test.sh will then execute each test.
Each test has its own folder containing the expected output (if any) and the testing script to run.
Each test will either output:
"Test x passed!" or "Test x failed" to easy highlight to readers which testing was successful.
If unsuccessful, the differing outputs will be printed to allow examination. 

*NOTE* file1.bpkg from the resources/pkgs must be placed into tests/part2

