#include <stdio.h>
#include <stdlib.h>

#include "utils_v10.h"
#include "memory_parameter.h"

int main(int argc, char const *argv[]) {
	

	if(argc < 2 || argc > 3 || atoi(argv[1]) < 1 || atoi(argv[1]) > 3 || (atoi(argv[1]) == 3 && argc < 3)) {
		printf("Ce programme fonctionne avec des arguments :\n\t./maint type [opt]\n");
		printf("\t  - Si type est egal a 1, il cree les ressources partagees.\n");
		printf("\t  - Si type est egal a 2, il detruit les ressources partagees.\n");
		printf("\t  - Si type est egal a 3, il reserve de facon exclusive le repertoire de code partage pour la periode de temps donne [opt].\n");
		exit(0);
	}

	int type = atoi(argv[1]);

	if(type == 1){
		// Crée les ressources partagées relatives au répertoire de code tels que la mémoire partagée et les sémaphores.

		// CREATE SEMAPHORE
  		sem_create(SEM_KEY, 1, PERM, 1); //
  		// CREATE SHARED MEMORY.
		sshmget(SHARED_MEMORY_KEY, sizeof(double)*MAX_SIZE_MEMORY , IPC_CREAT | PERM);

	} else if(type == 2){
		// Détruit les ressources partagées relatives au répertoire de code.

		// GET SEMAPHORE
		int sem_id = sem_get(SEM_KEY, 1);
		// GET SHARED MEMORY
  		int shm_id = sshmget(SHARED_MEMORY_KEY, sizeof(double)*MAX_SIZE_MEMORY, 0);

  		sshmdelete(shm_id);
  		sem_delete(sem_id);

	} else if(type == 3){
		// Réserve de façon exclusive le répertoire de code partagé pour une période de temps donné.

		int opt = atoi(argv[2]);

		// GET SEMAPHORE
		int sem_id = sem_get(SEM_KEY, 1);

		sem_down0(sem_id);
		// START OF CRITICAL SECTION
		sleep(opt);
		// END OF CRITICAL SECTION
    	sem_up0(sem_id);
	}
	exit(0);
}