#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "ablette.h"

#define TAILLE_TAB 200

machine tab[TAILLE_TAB];
machine *last = tab;

void insertion_sort(machine arr[], int n) {
    int i, j;
    machine key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j].nb_pack < key.nb_pack) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    struct ip * ip_header;
    struct tcphdr *tcp_header;
   
    ip_header = (struct ip *)(packet + 14);

    if (ip_header->ip_p == IPPROTO_TCP) {
        tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl << 2)); 

        char ip_source[INET_ADDRSTRLEN];
        char ip_destination[INET_ADDRSTRLEN];

        // Convertir les adresses IP en format lisible
        inet_ntop(AF_INET, &(ip_header->ip_src), ip_source, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip_header->ip_dst), ip_destination, INET_ADDRSTRLEN);

        printf("Source: %s:%d, Destination Port: %d \n", ip_source, ntohs(tcp_header->th_sport), ntohs(tcp_header->th_dport));

        machine *tmp = tab;
        // tableau sources
        while(strcmp(ip_source, tmp->adr) != 0) {
            tmp++;
            if(tmp == &(tab[TAILLE_TAB - 1]))
            {
                last->nb_pack++;
                strcpy(last->adr,ip_source);
                last++; 
                break;
            }
        }

        if(tmp != &(tab[TAILLE_TAB - 1]))
        {
            tmp->nb_pack++;
        }
        

        insertion_sort(tab, TAILLE_TAB);

        printf("Actuel \n\n");
        tmp = tab;
        while(tmp != last)
        {
            printf("%s %d\n",tmp->adr, tmp->nb_pack);
            tmp++;
        }

        printf("\n\n");

    }
}

int main(int argc, char *argv[]) {
    char *interface = " ";
    // Interface réseau WIFI par défaut sur macOS
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    char *port = "443";

    // Options de ligne de commande pour personnaliser les ports
    int option;
    while ((option = getopt(argc, argv, "p:i:")) != -1) {
        switch (option) {
            case 'p':
                port = optarg;
                break;
            case 'i':
                interface = optarg;
                break;
            default:
                fprintf(stderr, "Utilisation : %s [-p port1,port2] [interface]\n", argv[0]);
                exit(1);
        }
    }

    if (optind < argc) {
        interface = argv[optind];
    }
    // Ouvrir l'interface réseau en mode promiscuité.
    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Impossible d'ouvrir l'interface %s : %s\n", interface, errbuf);
        return 2;
    }

    // Créer une expression de filtre basée sur les ports personnalisés
    char filter[100];
    snprintf(filter, sizeof(filter), "tcp port %s", port);

    // Compiler et appliquer le filtre
    struct bpf_program fp;
    pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &fp);
    // capturer des paquets et appelez packet_handler pour chaque paquet.
    pcap_loop(handle, 0, packet_handler, NULL);
    
    return 0;
}
