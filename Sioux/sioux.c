#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include "../Libs/Reseau/libReseau.h"
#include "port.h"
#include "analyste_http.h"
#include "thread.h"

#define MAX_CONNEXIONS 10
#define SERVICES 30

int main(int argc,char *argv[]){
    int s;

    /* Lecture des arguments de la commande */
    int portt = port(argc,argv); //la fonction analyse Argument
    char service[SERVICES];
    snprintf(service,sizeof(service),"%d",portt);

    printf(" %s \n",service);
    /* Initialisation du serveur */
    s=initialisationServeur(service,MAX_CONNEXIONS);
    if(s<0){ 
        fprintf(stderr,"Port non utilisable\n"); 
        exit(EXIT_FAILURE); 
    }

    /* Lancement de la boucle d'ecoute */
    boucleServeur(s,gestionClientThread);
}

