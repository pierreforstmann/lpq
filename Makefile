#
# Makefile
# 
IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc

all:	test0 testlibpq3

test0:test0.o 
	$(CC) -o test0 test0.o $(LINKLIBS) -lpq
test0.o: test0.c 
	$(CC) -c test0.c $(INCLUDES)

testlibpq3:testlibpq3.o 
	$(CC) -o testlibpq3 testlibpq3.o $(LINKLIBS) -lpq
testlibpq3.o: testlibpq3.c 
	$(CC) -c testlibpq3.c $(INCLUDES)

clean:
	rm -f test0 testlibpq3 *.o
