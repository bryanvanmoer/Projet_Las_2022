#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils_v10.h"
#include "memory_parameter.h"

int main(int argc, char const *argv[])
{

    if (argc != 3)
    {
        printf("Ce programme fonctionne avec 2 argument :\n\t./compte \n\t ./montant \n");
        exit(0);
    }

    //recuperer les params et les mettre dans des variables
    int account = atoi(argv[1]);
    int amount = atoi(argv[2]);

    //GET SEMAPHORE
	int sem_id = sem_get(SEM_KEY, 1);
	//GET SHARED MEMORY
	int shm_id = sshmget(SHARED_MEMORY_KEY, sizeof(int), 0);

	void* shm = sshmat(shm_id);
	
	/*  
	sem_down0(sem_id);

	int* shmInt = (int*) shm;

	int size = shmInt[0];
    */

    if (amount == 0)
    {
        printf("Le montant ne peux pas etre nul\n");
        exit(0);
    }

    // depot
    // Si le montant est positif, le programme effectue un dépôt sur le compte en banque et
    // affiche le nouveau solde du compte. Par exemple, l'exécution de la commande “pdr 24 200”
    // permet d’effectuer un dépôt de 200 euros sur le compte 24.
    else if (amount > 0)
    {
        accounts[account] = accounts[account] + amount;
        printf("DEPOT EFFECTUE: Le nouveau solde du compte est de %lf \n", accounts[account]);
        exit(0);
    }

    // retrait
    // Si le montant est négatif, le programme effectue un retrait sur le compte en banque et
    // affiche le nouveau solde du compte. L'exécution de la commande “pdr 24 -200”
    // permet d’effectuer un retrait de 200 euros sur le compte 24
    else
    {
        accounts[account] = accounts[account] - amount;
        printf("RETRAIT EFFECTUE: Le nouveau solde du compte est de %lf \n", accounts[account]);
        exit(0);
    }
    sshmdt(shm);    
    exit(0);
}