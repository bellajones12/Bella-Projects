CC=gcc
CFLAGS=-Wall -Wvla -Werror -std=c2x -g -fsanitize=address
# CFLAGS=-Wall -Wvla -Werror -std=c2x -g
# CFLAGS=-g
LDFLAGS=-lm -lpthread
INCLUDE=-Iinclude

.PHONY: clean

# Required for Part 1 - Make sure it outputs a .o file
# to either objs/ or ./
# In your directory
pkgchk.o: src/chk/pkgchk.c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)

merkletree.o: src/tree/merkletree.c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)

part1.o: src/chk/pkgchk.c src/tree/merkletree.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@

pkgchecker: src/pkgmain.c src/chk/pkgchk.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@

# Required for Part 2 - Make sure it outputs `btide` file
# in your directory ./
btide: src/btide.c src/net/package.c src/chk/pkgchk.c src/tree/merkletree.c src/crypt/sha256.c src/net/config.c src/net/client.c src/net/server.c src/net/peer.c src/net/packet.c src/net/handling_connect.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@

pkgmain: src/pkgmain.c src/chk/pkgchk.c src/tree/merkletree.c src/crypt/sha256.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@


# Alter your build for p1 tests to build unit-tests for your
# merkle tree, use pkgchk to help with what to test for
# as well as some basic functionality
p1tests:
	bash p1test.sh

# Alter your build for p2 tests to build IO tests
# for your btide client, construct .in/.out files
# and construct a script to help test your client
# You can opt to constructing a program to
# be the tests instead, however please document
# your testing methods
p2tests:
	bash p2test.sh

all: $(TARGET)

btide_prac: src/net/packet.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@

run:
	 ./btide config.cfg

run2:
	 ./btide config1.cfg

	
socket: socket.c
	$(CC) $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $@
	


clean:
	rm -f *.o
	rm -f pkgmain
	rm -f btide
    