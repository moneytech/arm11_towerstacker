CC      = gcc -std=c99 
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic -O3

.SUFFIXES: .c .o

.PHONY: all clean

all:	assemble emulate

assemble:	assemble.o util.o table.o firstpass.o DataProcHandler.o MultiplyHandler.o sdtHandler.o branchHandler.o

table.o:	table.h
firstpass.o:	firstpass.h
DataProcHandler.o:	DataProcHandler.h
MultiplyHandler.o:	MultiplyHandler.h
sdtHandler.o:	sdtHandler.h
branchHandler.o:	branchHandler.h
assemble.o:	assemble.c

emulate:	emulate.o util.o multiply.o SingleDataTransfer.o dataprocessing.o
util.o:	util.h
multiply.o:	multiply.h
SingleDataTransfer.o:	SingleDataTransfer.h
dataprocessing.o:	dataprocessing.h
emulate.o:	emulate.c


clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
