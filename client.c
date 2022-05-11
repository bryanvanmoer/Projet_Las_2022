#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils_v10.h"
#include "memory_parameter.h"

void envoyerVirement(char* buffer, int size,int numero,int sockfd);

int main(int argc, char** argv){

    //Arg: adr, port, num, delay

	if(argc != 4) {
    printf("Il manque %d des arguments\n",4-argc);
    exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int compteEmetteur = atoi(argv[3]);
    //int delay = atoi(argv[4]);

    int sockfd = ssocket(); 

    // prepare socket to connect
    sconnect(server_ip, server_port, sockfd);
      
    printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
    printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
    printf("3) q -> deconnecte le client et libère les ressources du client \n");

    char buffer[MAX];
    //char *message = "";
    int nbCharRead = sread(0, buffer, MAX);

    while(nbCharRead > 0){
        sockfd = initSocketClient(server_ip, server_port);
        buffer[nbCharRead-1]= '\0';

        // +
        if (buffer[0] == '+')
        {
            printf("inside\n");
            envoyerVirement(buffer, nbCharRead, compteEmetteur, sockfd);
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
    char crediteur = p[1];
    char montant = p[2];

    printf("crediteur: %s \n", &crediteur);
    printf("crediteur: %s \n", &montant);
    exit(0);
}
