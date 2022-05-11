#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>

#include "utils_v10.h"
#include "memory_parameter.h"

#define VAL 7


int main(int argc, char** argv){

    //Arg: adr, port, num, delay

	if(argc != 4) {
    printf("Il manque %d des arguments",4-argc);
    exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    //int delay = atoi(argv[3]);

    int sockfd = ssocket(); 

    // prepare socket to connect
    sconnect(server_ip, server_port, sockfd);
      
    int val = VAL;
    printf("Client envoie la valeur 7 au serveur\n");
    swrite(sockfd,&val,sizeof(int));
    sclose(sockfd);

    return 0;
    
	
}
