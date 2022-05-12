#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils_v10.h"
#include "memory_parameter.h"

void envoyerVirement(char* buffer, int size,int numero,int sockfd);

void minuterie(void *pipefd, void *temps) {

  int *delay = temps;
  int *pipe = pipefd;
  int i=-1;
  //close sur lecture
  sclose(pipe[0]);
  
  while(true){
    sleep(*delay);
    swrite(pipe[1],&i,sizeof(int));
  }

  //close sur ecriture
  sclose(pipe[1]);
}

int main(int argc, char** argv){

    //Arg: adr, port, num, delay
	if(argc != 4) {
        printf("Il manque %d des arguments\n",4-argc);
        exit(EXIT_FAILURE);
    }

    char* server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int compteEmetteur = atoi(argv[3]);
    //int delay = atoi(argv[4]);

    //int sockfd = ssocket(); 

    // prepare socket to connect
    //sconnect(server_ip, server_port, sockfd);
      
    printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
    printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
    printf("3) q -> deconnecte le client et libère les ressources du client \n");

    char buffer[MAX];
    //char *message = "";
    int nbCharRead = sread(0, buffer, MAX);
    int sockfd;

    while(nbCharRead > 0){
        sockfd = initSocketClient(server_ip, server_port);
        buffer[nbCharRead-1]= '\0';

        // +
        if (buffer[0] == '+')
        {
            envoyerVirement(buffer, nbCharRead, compteEmetteur, sockfd);
        }
        // *
        else if (buffer[0] == '*')
        {
            //envoyerVirement(buffer, nbCharRead, compteEmetteur, sockfd);
        }
        // +
        else if (buffer[0] == 'q')
        {
            break;
        }
    }

    //swrite(sockfd,&val,sizeof(int));
    sclose(sockfd);

    return 0;
}

void envoyerVirement(char* buffer, int size,int emetteur,int sockfd){
    //delimitateur
    char d[] = " ";
    char *p = strtok(buffer,d);
    int i=0;
    int crediteur;
    int montant;

    while(p != NULL){
        if(i==1){
            crediteur = atoi(p);
            printf("crediteur: %d \n", crediteur);
        }
        else if(i==2){
            montant = atoi(p);
            printf("montant: %d \n", montant);
        }
        i++;
        p = strtok(NULL,d);
    }

    Transaction transaction;
    transaction.debiteur = emetteur;
    transaction.crediteur = crediteur;
    transaction.montant = (double) montant;

    swrite(sockfd, &transaction, sizeof(transaction));
    sclose(sockfd);

    exit(0);
}
