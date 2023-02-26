#
# Makefile
# 
IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc

all:	test0 test1 test2 testlibpq3

test0:test0.o 
	$(CC) -o test0 test0.o $(LINKLIBS) -lpq
test0.o: test0.c 
	$(CC) -c test0.c $(INCLUDES)

test1:test1.o 
	$(CC) -o test1 test1.o $(LINKLIBS) -lpq
test1.o: test1.c 
	$(CC) -c test1.c $(INCLUDES)

test2:test2.o 
	$(CC) -o test2 test2.o $(LINKLIBS) -lpq
test2.o: test2.c 
	$(CC) -c test2.c $(INCLUDES)

testlibpq3:testlibpq3.o 
	$(CC) -o testlibpq3 testlibpq3.o $(LINKLIBS) -lpq
testlibpq3.o: testlibpq3.c 
	$(CC) -c testlibpq3.c $(INCLUDES)

clean:
	rm -f test0 test1 test2 testlibpq3 *.o
