#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


int port ( int argc,char *argv[]){
    int port;
    int syntax = 0;

    // Spécifiez les options courtes et longues 'p' pour courte et port pour longue
    static struct option long_options[] = {
    {"port",     required_argument, 0,  'p' },
    {0,         0,                 0,  0 }
    };

    int opt; //variable pour l'option actuelle
    int option_index = 0;


    while ((opt = getopt_long(argc, argv, "p:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p': //Si 'option est 'p'
                port = atoi(optarg); //conversion et assignation à port (optarg contient la chaine de carac qui suit l'option -p)
                break;
            default:
                syntax = 1;
                break;
        }
    }

    printf("Port: %d\n", port);

    if (syntax) {
        fprintf(stderr, "Utilisation : %s [--port <port>]\n", argv[0]);
        exit(-1);
    }
    return port;
}
