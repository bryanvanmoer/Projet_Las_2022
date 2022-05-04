CC=gcc
CCFLAGS=-std=c11 -pedantic -Wall -Wvla -Werror -D_POSIX_C_SOURCE -D_DEFAULT_SOURCE
ALL=maint pdr client server

all: $(ALL)

maint: maint.o
	$(CC) $(CCFLAGS) -o maint maint.o utils_V10.o

maint.o: maint.c utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c maint.c

client : client.o
	$(CC) $(CCFLAGS) -o client client.o utils_v10.o

client.o: client.c utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c client.c 

server : server.o
	$(CC) $(CCFLAGS) -o server server.o utils_v10.o 

server.o: server.c utils_v10.o memory_parameter.h 
	$(CC) $(CCFLAGS) -c server.c 

pdr: pdr.o
	$(CC) $(CCFLAGS) -o pdr pdr.o utils_V10.o

pdr.o : pdr.c utils_V10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c pdr.c
	
utils_v10.o: utils_v10.c utils_v10.h
	$(CC) $(CCFLAGS) -c utils_v10.c 



########## Cleaning ##########

clean:
	rm -f *.o
	rm -f $(ALL)
	ipcrm -a