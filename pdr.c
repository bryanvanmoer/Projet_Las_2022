#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils_v10.h"
#include "memory_parameter.h"

int main(int argc, char const *argv[])
{

    //Verification des arguments
    if(argc != 3 ){

        printf("Ce programme fonctionne avec 2 argument : numero de compte + montant \n");
        printf("Le numero de compte se situant entre 0 et 999\n");
        exit(0);
    }

    if(atoi(argv[1]) < 0 || atoi(argv[1]) > 999){
        printf("Montant ou numero de compte invalide\n");
        exit(0);
    }

    //Recuperation des valeurs passées en argument
    int compte = atoi(argv[1]);
    int montant = atoi(argv[2]);

    //GET SEMAPHORE
    sem_get(SEM_KEY, 1);
    //GET SHARED MEMORY
    int shm_id = sshmget(SHARED_MEMORY_KEY, sizeof(double)*MAX_SIZE_MEMORY, 0);

    double* tab = sshmat(shm_id);

    if(montant == 0){
        printf("Le montant est égal a 0 !\n");

    } else if(montant > 0){
        tab[compte] += (double)montant;
        printf("DEPOT EFFECTUE: Le solde de votre compte est de : %lf\n", tab[compte]);

    } else if(montant < 0){
        printf("Solde compte : %lf\n",tab[compte]);
        printf("Montant : %lf\n", (double)montant);
        tab[compte] = tab[compte] + montant;
        printf("RETRAIT EFFECTUE: Le solde de votre compte est de : %lf\n", tab[compte]);
    }

    sshmdt(tab);    
    exit(0);

}