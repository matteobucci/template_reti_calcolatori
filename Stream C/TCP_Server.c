/*
 * Template di soluzione TCP Server
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
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#define DEFAULT_PORT 10123

void gestore(int signo);

int main(int argc, char* argv[]){
   
    /*Variabili utilizzate*/
    int listen_sd, client_sd, port, num, len;
    struct sockaddr_in client_addr, server_addr;
    struct hostent *client;
    
    /*Controllo argomenti*/
    if (argc == 1){
        port = DEFAULT_PORT;
    }else if (argc == 2){
        num = 0;
        while (argv[1][num] != '\0'){
            if ((argv[1][num] < '0') || (argv[1][num] > '9')){
                printf("La porta inserita non è un intero\n");
                exit(1);
            }
            num++;
        }
        port = atoi(argv[1]);
        if (port < 1024 || port > 65535){
            printf("La porta %d non può essere utilizzata (1024 < port < 65535)\n", port);
            exit(1);
        }
    }else{
        printf("Utilizzo: %s [serverPort]\n", argv[0]);
        exit(1);
    }
    
    /*Configurazione indirizzo server*/
    memset((char *) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    /*Configurazione socket e avvio server*/
    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Errore nell'apertura della socket:"); 
        exit(2);
    }
    printf("Creazione socket di ascolto avvenuta con successo. FD = %d\n", listen_sd);

    if (bind(listen_sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) < 0){
        perror("Errore nella bind della socket di ascolto:");
        exit(2);
    }
    printf("Bind della socket di ascolto avvenuto con successo.\n");

    if (listen(listen_sd, 5) < 0){
        perror("Errore nella listen:");
        exit(2);
    }
    
    signal(SIGCHLD, gestore); //Aggancio il gestore
    len = sizeof(struct sockaddr_in); //Lunghezza indirizzo del client. Mi servirà per effettuare l'accept.
    
    printf("Listen effettuata con successo. Il server è in ascolto...\n");
    
    /*Ciclo di gestione delle richieste del server*/
    for (;;){
        if ((client_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &len)) < 0){
            if (errno == EINTR){
                perror("Segnale ricevuto durante l'accept. Continuo ad accettare le richieste.");
                continue;
            }
            else{
                perror("Errore durante l'accept:");
                exit(1);
            }
        }
        
        switch (fork()){
            case 0:
                /*Corpo figlio*/
                client = gethostbyaddr((char *) &client_addr.sin_addr,sizeof(client_addr.sin_addr), AF_INET);
                if (client == NULL){
                    perror("FIGLIO: Impossibile recuperare le informazioni del cliente:\n");
                    close(client_sd);
                    exit(3);
                    }
                else{
                    printf("FIGLIO: L'host client che sto servendo è %s\n", client->h_name);
                }
                
                
                close(client_sd);
                exit(0);
                /*Fine corpo figlio*/
                break;
            case -1:
                perror("Impossibile creare il figlio per gestire una richiesta:");
                exit(3);
                break;
            default:
                close(client_sd);
        }
    }
}

void gestore(int signo){
    int stato;
    wait(&stato);
    printf("Il figlio è terminato con stato %d\n" , stato>>8);
}
