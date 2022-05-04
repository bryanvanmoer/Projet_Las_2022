CC=gcc
CCFLAGS=-std=c11 -pedantic -Wall -Wvla -Werror -D_POSIX_C_SOURCE -D_DEFAULT_SOURCE
ALL=maint stat client server

all: $(ALL)

maint: maint.o
	$(CC) $(CCFLAGS) -o maint maint.o utils_v10.o

maint.o: maint.c utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c maint.c

stat: stat.o utils_v10.o
	$(CC) $(CCFLAGS) -o stat stat.o utils_v10.o

stat.o: stat.c utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c stat.c

client : client.o
	$(CC) $(CCFLAGS) -o client client.o utils_v10.o

client.o: client.c utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c client.c 

server : server.o
	$(CC) $(CCFLAGS) -o server server.o utils_v10.o executeFile.o addFile.o

server.o: server.c utils_v10.o memory_parameter.h executeFile.o addFile.o
	$(CC) $(CCFLAGS) -c server.c 

executeFile.o: executeFile.c executeFile.h utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c executeFile.c 

addFile.o: addFile.c addFile.h utils_v10.o memory_parameter.h
	$(CC) $(CCFLAGS) -c addFile.c 

utils_v10.o: utils_v10.c utils_v10.h
	$(CC) $(CCFLAGS) -c utils_v10.c 


########## For dev purpose ##########

infos:
	ls
	ipcs


########## Cleaning ##########

clean:
	rm -f *.o
	rm -f ./executable/*
	rm -f standardStream.txt standardErrorStream.txt
	rm -f $(ALL)
	ipcrm -a