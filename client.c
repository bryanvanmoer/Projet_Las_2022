#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils_v10.h"
#include "memory_parameter.h"

void envoyerVirement(int sockfd,Transaction transaction);
Transaction creeTransaction(char* buffer, int emetteur);



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


void virement_recurrent(void *argv, void *port, void *ip){

    int *pipe = argv;
    int *server_port = port;
    char *server_ip = ip;
    int nbChrRd =0;
    int result;
    int cpt=0;
    struct Transaction tabTransaction[10];

    //fermeture du pipe en ecriture
    sclose(pipe[1]);

    while((nbChrRd =sread(pipe[0],&result,sizeof(int)))>0){
        int sockfd;

        //Minuterie
        if(result == -1){ 
            for(int i=0; i<cpt ; i++){
                sockfd = initSocketClient(server_ip, *server_port); 
                envoyerVirement(sockfd,tabTransaction[i]);
                sclose(sockfd);
            } 
        }
        //Terminal
        else {
            sread(pipe[0],&tabTransaction[cpt],sizeof(Transaction));
            cpt++;
        }
    }

    //fermeture du pipe en lecture
    sclose(pipe[0]);
}



int main(int argc, char** argv){

    // Arg: adr, port, num, delay
	if(argc != 5) {
        printf("Il manque %d des arguments\n",4-argc);
        exit(EXIT_FAILURE);
    }

    // Recuperation des arguments 
    char* server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int compteEmetteur = atoi(argv[3]);
    int delay = atoi(argv[4]);

    int sockfd;
    int pipe[2];
    spipe(pipe);

    // Creation des enfants
    fork_and_run3(virement_recurrent, pipe, &server_port, server_ip);
    int pid_minuterie = fork_and_run2(minuterie, pipe,&delay);

    // Close sur lecture
    sclose(pipe[0]);
      
    printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
    printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
    printf("3) q -> deconnecte le client et libère les ressources du client \n");

    char buffer[MAX];
    int nbCharRead = sread(0, buffer, MAX);
    int val = 1;

    while(nbCharRead > 0){

        sockfd = initSocketClient(server_ip, server_port);
        buffer[nbCharRead-1]= '\0';

        // +
        if (buffer[0] == '+')
        {
            Transaction transaction = creeTransaction(buffer,compteEmetteur);
            envoyerVirement(sockfd,transaction);
        }
        // *
        else if (buffer[0] == '*')
        {
            swrite(pipe[1],&val,sizeof(int));
            Transaction transaction = creeTransaction(buffer,compteEmetteur);
            swrite(pipe[1],&transaction,sizeof(Transaction));
        }
        // +
        else if (buffer[0] == 'q')
        {   
            break;
        }

        printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
        printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
        printf("3) q -> deconnecte le client et libère les ressources du client \n");

        buffer[0]='\0';
        nbCharRead=sread(0,buffer,MAX); 
    }


    //close sur ecriture
    sclose(pipe[1]);

    skill(pid_minuterie,SIGKILL);
    sclose(sockfd);

    exit(0);
}

Transaction creeTransaction(char* buffer, int emetteur){
    //Delimitateur
    char d[] = " ";
    char *p = strtok(buffer,d);
    int i=0;
    int crediteur;
    int montant;

    while(p != NULL){
        if(i==1){
            crediteur = atoi(p);
        }
        else if(i==2){
            montant = atoi(p);
        }
        i++;
        p = strtok(NULL,d);
    }

    //Place les valeurs entrées par l'utilisateur dans une structure Transaction
    
    Transaction transaction;
    transaction.debiteur = emetteur;
    transaction.crediteur = crediteur;
    transaction.montant = (double) montant;

    return transaction;
}

void envoyerVirement(int sockfd, Transaction transaction){

    swrite(sockfd, &transaction, sizeof(transaction));

    // recevoir une reponse du server

    printf("Virement effectué avec succes !\n");
}

