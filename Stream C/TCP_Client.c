/*
 * Template di soluzione TCP_Client
 * Matteo Bucci
 * 0000722984
 */

#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_STR_LEN 128

int main(int argc, char* argv[]){
    
    /*Variabili utilizzate*/
    int sock_fd, num, port;
    struct hostent *host;
    struct sockaddr_in serv_addr;
    char input[MAX_STR_LEN];
    char input_request[] = {"Inserisci qualcosa"};
    
    /*Controllo argomenti*/
    if (argc != 3){
        printf("Utilizzo: %s serverAddress serverPort \n", argv[0]);
        exit(1);
    }
    num = 0;
    while (argv[2][num] != '\0'){
        if ((argv[2][num] < '0') || (argv[2][num] > '9')){
            printf("Secondo argomento non intero\n");
            exit(1);
        }
        num++;
    }
    port = atoi(argv[2]);
    if (port < 1024 || port > 65535){
        printf("La porta %d non può essere utilizzata (1024 < port < 65535)\n", port);
        exit(1);
    }
    host = gethostbyname(argv[1]);
    if (host == NULL){
        printf("%s non è un host valido.\n", argv[1]);
        exit(1);
    }

    /*Configurazione indirizzo server*/
    memset((char *) &serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ((struct in_addr*) (host->h_addr))->s_addr;
    serv_addr.sin_port = htons(port);
    
    /*Preparazione socket*/
    if ((sock_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        perror("Errore nell'apertura della socket:"); 
        exit(2);
    }
    printf("Creazione socket avvenuta con successo. FD = %d\n", sock_fd);
    
    /*Connessione al server (con bind() implicita)*/
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0){
        perror("Errore nella connect:");
        exit(2);
    }
    printf("Connessione al server avvenuta con successo.\n");
    
    /*Corpo del client*/
    printf("%s\n", input_request);
    while (gets(input)){
        
        printf("%s\n", input_request);
    }
    
    /*Operazioni di chiusura*/
    printf("Chiusura client.\n");
    exit(0);
}