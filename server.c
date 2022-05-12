#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils_v10.h"
#include "memory_parameter.h"

int main(int argc, char const *argv[]) {


	if(argc != 2){
		printf("Ce programme fonctionne avec 1 argument :\n\t./server port\n");
		exit(0);
	}

	//Recuperation du port 	
	int server_port = atoi(argv[1]);

	//Creation du socket 
	int sockfd = initSocketServer(server_port, MAX_SIMULTANEOUS);
	printf("Le serveur tourne sur le port : %i \n",server_port);
	
	Transaction transaction;

	/* Ecoute apres un client */
	int newsockfd = saccept(sockfd);

	/* Lit les transactions du client */
	sread(newsockfd, &transaction, sizeof(transaction));

	printf("Transaction reçue : \n Emetteur : %d \n Crediteur : %d \n Montant : %lf \n", transaction.debiteur, transaction.crediteur, transaction.montant);


    //GET SEMAPHORE
    sem_get(SEM_KEY, 1);
    
    //GET SHARED MEMORY
    int shm_id = sshmget(SHARED_MEMORY_KEY, sizeof(double)*MAX_SIZE_MEMORY, 0);

    double* tab = sshmat(shm_id);

    tab[transaction.debiteur] -= transaction.montant; 
    tab[transaction.crediteur] += transaction.montant;
    sshmdt(tab);

	// close connection client
	sclose(newsockfd);
	// close listen socket
	sclose(sockfd);
	
	return 0;
}