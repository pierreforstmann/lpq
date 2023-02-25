IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc

testlibpq3:testlibpq3.o 
	$(CC) -o testlibpq3 testlibpq3.o $(LINKLIBS) -lpq
testlibpq3.o: testlibpq3.c 
	$(CC) -c testlibpq3.c $(INCLUDES)
clean:
	rm -f testlibpq3 *.o
