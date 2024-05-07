#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netinet/in.h>
#include "libReseau.h"
#include <netinet/tcp.h>
#include <pthread.h>

#include "../../Sioux/thread.h"
#include <netdb.h>
#include <arpa/inet.h>

int initialisationServeur(char *service,int connexions){
  struct addrinfo precisions,*resultat,*origine;
    int statut;
    int server_fd;

/* Construction de la structure adresse */
  memset(&precisions,0,sizeof precisions);
  precisions.ai_family=AF_UNSPEC;
  precisions.ai_socktype=SOCK_STREAM;
  precisions.ai_flags=AI_PASSIVE;
  statut=getaddrinfo(NULL,service,&precisions,&origine);

  if(statut<0){ perror("initialisationServeur.getaddrinfo");
    exit(EXIT_FAILURE);
  }
  struct addrinfo *p;
  for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
    if(p->ai_family==AF_INET6)
    {
      resultat=p;
      break;

    }

/* Creation d'une socket */
  server_fd=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
  if(server_fd<0){
    perror("initialisationServeur.socket");
    exit(EXIT_FAILURE);

  }

  /* Options utiles */
  int vrai=1;
  if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
    perror("initialisationServeur.setsockopt (REUSEADDR)");
    exit(EXIT_FAILURE);
    }
  if(setsockopt(server_fd,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
    perror("initialisationServeur.setsockopt (NODELAY)");
    exit(EXIT_FAILURE);
    }

  /* Specification de l'adresse de la socket */
  statut=bind(server_fd,resultat->ai_addr,resultat->ai_addrlen);
  if(statut<0)
  {
    freeaddrinfo(origine);
    perror("Bind-Lors de la liaison du socket");
    shutdown(server_fd,SHUT_RDWR);
    return -1;
  }

  /* Liberation de la structure d'informations */
  freeaddrinfo(origine);

  /* Taille de la queue d'attente */
  statut=listen(server_fd,connexions);
  if(statut<0){
    perror("Listen-Erreur lors de l'écoute");
    shutdown(server_fd,SHUT_RDWR);
    return -2; }

  return server_fd;
}

int boucleServeur(int ecoute,int (*traitement)(int)) {
    int dialogue;
    while (1) {
      /* Attente d'une connexion */
      if((dialogue=accept(ecoute,NULL,NULL))<0)
      return -1;

      /* Passage de la socket de dialogue a la fonction de traitement */
      if(traitement(dialogue)<0){
        shutdown(ecoute,SHUT_RDWR);
        return 0;
      }
  }
}
