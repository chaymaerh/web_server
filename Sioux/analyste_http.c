/**** Minimal web server ****/

/** Include files **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <time.h>
#include "thread.h"
#define MAX_LIGNE 5000

#define WEB_DIR  "web"
#define PAGE_NOTFOUND "vote-reponse.html"
#define MAX_BUFFER 1024
#define MAX_PATH 1200 


#define CODE_OK  200
#define CODE_NOTFOUND 404
#define CODE_UNIMPLEMENTED 501

#include "../Libs/Reseau/libReseau.h"


int gestionClient(int s){
    /* Obtient une structure de fichier*/
    FILE *dialogue=fdopen(s,"a+");
    if(dialogue==NULL){
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }

    /* Echo */
    char ligne[MAX_LIGNE];
    while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
    {
        char cmd[MAX_BUFFER];
        char page[MAX_BUFFER];
        char proto[MAX_BUFFER];
        char path[MAX_PATH];
        char type[MAX_BUFFER];
        
        if(sscanf(ligne,"%s %s %s",cmd,page,proto)!=3)
        {
            fprintf(dialogue,"%s %d\r\n",proto,CODE_UNIMPLEMENTED);
            return -1;

        };

        while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL){
            if(strcmp(ligne,"\r\n")==0) break;
        }
        
        if(strcasecmp(cmd,"GET")==0) {
            int code=CODE_OK;
            struct stat fstat;
            sprintf(path,"%s%s",WEB_DIR,page);

            if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode)){
                sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
                stat(path,&fstat);
                code=CODE_NOTFOUND;
            }
                
            strcpy(type,"text/html");
            char *end=page+strlen(page);

            if(strcmp(end-4,".png")==0)
                strcpy(type,"image/png");
            if(strcmp(end-4,".jpg")==0)
                strcpy(type,"image/jpg");
            if(strcmp(end-4,".gif")==0)
                strcpy(type,"image/gif");
            
            fprintf(dialogue,"%s %d\r\n",proto,code);
            fprintf(dialogue,"Server: CWeb\r\n");
            fprintf(dialogue,"Content-type: %s\r\n",type);
            fprintf(dialogue,"Content-length: %ld\r\n",fstat.st_size);
            fprintf(dialogue,"\r\n");
            fflush(dialogue);

            int fd=open(path,O_RDONLY);
                if(fd>=0){
                    int bytes;
                    while((bytes=read(fd,ligne,MAX_LIGNE))>0) fwrite(ligne,bytes,1,dialogue);
                    close(fd);
                }
        }

            if (strstr(page, "vote") != NULL) {

            char *query_string = strchr(page, '?');
            if (query_string != NULL) {
                query_string++;
                char selectedPhoto[MAX_BUFFER];
                if (sscanf(query_string, "photo=%s", selectedPhoto) == 1) {

                    char csv_filename[MAX_PATH];
                    snprintf(csv_filename, sizeof(csv_filename), "%s/vote.csv", WEB_DIR);
                    FILE *csv_file = fopen(csv_filename, "a");

                    if (csv_file != NULL) {

                        fprintf(csv_file, "%s\n", selectedPhoto);
                        fclose(csv_file);
                    }
                }
            }
        }
    }


    /* Termine la connexion */
    fclose(dialogue);
    return 0;
}

