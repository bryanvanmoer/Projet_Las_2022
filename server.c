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
	sread(newsockfd, &transaction, sizeof(Transaction));

	printf("Transaction reçue : \n Emetteur : %d \n Crediteur : %d \n Montant : %d \n", transaction.emetteur, transaction.crediteur, transaction.montant);

	// close connection client
	sclose(newsockfd);
	// close listen socket
	sclose(sockfd);
	
	return 0;
}