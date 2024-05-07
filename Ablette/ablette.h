void packetHandler(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet);
typedef struct {
    int nb_pack; 
    char adr[20];
} machine;
