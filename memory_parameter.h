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

#define MAX_SIMULTANEOUS 100

#define MAX_SIZE_MEMORY 1000
#define MAX_SIZE_ERROR_MSG 4000
#define MAX 255


typedef struct Transaction {
	int debiteur;
    int crediteur;
	double montant;
} Transaction;

/*
typedef struct Account {
    int solde;
	bool decouvert;
} Account;
*/

double accounts[MAX_SIZE_MEMORY];

#endif  // _MEMORY_PARAMETER_H_