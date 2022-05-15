#ifndef _MEMORY_PARAMETER_H_
#define _MEMORY_PARAMETER_H_

#include <stdbool.h>

// Semaphores && Shared memory.
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#define SHARED_MEMORY_KEY 123
#define SEM_KEY 456
#define PERM 0666
#define MAX_SOCKET_SIMULTANEOUS 100
#define MAX_VIREMENT_RECURRENT 100

#define MAX_SIZE_MEMORY 1000
#define MAX_BUFFER 255

double accounts[MAX_SIZE_MEMORY];

typedef struct Transaction {
	int debiteur;
    int crediteur;
	double montant;
} Transaction;

#endif  // _MEMORY_PARAMETER_H_