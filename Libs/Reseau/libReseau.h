int initialisationServeur(char *service,int connexions);
int boucleServeur(int ecoute,int (*traitement)(int));
