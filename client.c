#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

#include "utils_v10.h"
#include "memory_parameter.h"

void envoyerVirement(Transaction transaction);
void envoyerVirementRecurrent(Transaction transaction);
Transaction creerTransaction(char* buffer, int emetteur);

char* server_ip;
int server_port;
int compteEmetteur;
int delay;



void minuterie(void *pipefd, void *battement) {


  int *pipe = pipefd;
  int *delay = battement;
  int val = -1;

  // Close sur lecture
  sclose(pipe[0]);
   
  while(true){
    sleep(*delay);
    swrite(pipe[1],&val,sizeof(int));
  }

  // Close sur ecriture
  sclose(pipe[1]);
}


void virementRecurrent(void *argv){

    int *pipe = argv;
    int nbChrRd =0;
    int result;
    int cpt=0;
    struct Transaction tabTransaction[MAX_VIREMENT_RECURRENT];

    //fermeture du pipe en ecriture
    sclose(pipe[1]);

    while((nbChrRd =sread(pipe[0],&result,sizeof(int)))>0){

        //Minuterie
        if(result == -1){ 
            for(int i=0; i<cpt ; i++){
                envoyerVirementRecurrent(tabTransaction[i]);
            } 
        }
        //Terminal
        else {
            sread(pipe[0],&tabTransaction[cpt],sizeof(Transaction));
            cpt++;
        }
    }
    // Fermeture du pipe en lecture
    sclose(pipe[0]);
}



int main(int argc, char** argv){

    // Arg: adr, port, num, delay
	if(argc != 5) {
        printf("Il manque %d des arguments\n",4-argc);
        exit(EXIT_FAILURE);
    }

    // Affectation des arguments 
    server_ip = argv[1];
    server_port = atoi(argv[2]);
    compteEmetteur = atoi(argv[3]);
    delay = atoi(argv[4]);

    int pipe[2];
    spipe(pipe);

    // Creation des enfants
    int pid_recurrent = fork_and_run1(virementRecurrent, pipe);
    int pid_minuterie = fork_and_run2(minuterie, pipe,&delay);

    // Close sur lecture
    sclose(pipe[0]);
      
    printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
    printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
    printf("3) q -> deconnecte le client et libère les ressources du client \n");

    char buffer[MAX_BUFFER];
    int nbCharRead = sread(0, buffer, MAX_BUFFER);
    int val = 1;

    while(nbCharRead > 0){

        buffer[nbCharRead-1]= '\0';
        // +
        if (buffer[0] == '+')
        {
            Transaction transaction = creerTransaction(buffer,compteEmetteur);
            envoyerVirement(transaction);
        }
        // *
        else if (buffer[0] == '*')
        {
            swrite(pipe[1],&val,sizeof(int));
            Transaction transaction = creerTransaction(buffer,compteEmetteur);
            swrite(pipe[1],&transaction,sizeof(Transaction));
        }
        // +
        else if (buffer[0] == 'q')
        {   
            skill(pid_recurrent,SIGKILL);
            skill(pid_minuterie,SIGKILL);
            skill(getpid(),SIGKILL);
            exit(0);
        }

        printf("1) + n2 somme -> virement sur compte n2 avec somme montant \n");
        printf("2) * n2 somme -> virement recurrent sur compte n2 avec somme montant \n");
        printf("3) q -> deconnecte le client et libère les ressources du client \n");

        buffer[0]='\0';
        nbCharRead=sread(0,buffer,MAX_BUFFER); 
    }

    //close sur ecriture
    sclose(pipe[1]);
    exit(0);
}

Transaction creerTransaction(char* buffer, int emetteur){
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

void envoyerVirement(Transaction transaction){

    double nouveauSolde;
    int sockfd = initSocketClient(server_ip, server_port);

    swrite(sockfd, &transaction, sizeof(transaction));
    sread(sockfd, &nouveauSolde, sizeof(double));

    printf("Virement effectué avec succès !\n");
    printf("Le nouveau solde de votre compte est : %lf €\n",nouveauSolde);
    printf("\n");

    //sclose
    sclose(sockfd);
}

void envoyerVirementRecurrent(Transaction transaction){

    int sockfd = initSocketClient(server_ip, server_port);

    swrite(sockfd, &transaction, sizeof(transaction));
    printf("Virement recurrent effectué avec succès !\n");
    printf("\n");

    //sclose
    sclose(sockfd);
}

