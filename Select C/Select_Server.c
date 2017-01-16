/*
 * Template di soluzione Select Server
 * Matteo Bucci
 * 0000722984
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DEFAULT_PORT 10123
#define BUFFER_SIZE 256

void gestore(int signo);

int main(int argc, char **argv){
    
    /*Variabili utilizzate*/
    int listen_sd, client_sd, datagram_sd, port, num, len, fd_max;
    struct sockaddr_in client_addr, server_addr;
    struct hostent *client;
    char buffer[BUFFER_SIZE];       //I datagram vengono ricevuti qui. Modificare a seconda delle esigenze
    fd_set read_fds, temp_fds;
    
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
    
    /*Configurazione socket e avvio server TCP*/
    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Errore nell'apertura della socket TCP:"); 
        exit(2);
    }
    printf("Creazione socket di ascolto TCP avvenuta con successo. FD = %d\n", listen_sd);

    if (bind(listen_sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) < 0){
        perror("Errore nella bind della socket di ascolto TCP:");
        exit(2);
    }
    printf("Bind della socket di ascolto TCP avvenuto con successo.\n");

    if (listen(listen_sd, 5) < 0){
        perror("Errore nella listen:");
        exit(2);
    }
    
    /*Configurazione socket e avvio server UDP*/
    if((datagram_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Errore nell'apertura della socket UDP:");
        exit(2);
    }
    printf("Creazione socket UDP avvenuta con successo. FD = %d\n", datagram_sd);
    
    if (bind(datagram_sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) < 0){
        perror("Errore nella bind della socket UDP:");
        exit(2);
    }
    printf("Bind della socket UDP avvenuto con successo.\n");
    
    /*Inizializzazione maschere*/
    FD_ZERO(&read_fds);
    FD_SET(datagram_sd, &read_fds);
    FD_SET(listen_sd, &read_fds);
    temp_fds = read_fds;
    fd_max = datagram_sd + 1;
    
    signal(SIGCHLD, gestore); //Aggancio il gestore
    
    
    /*Ciclo ricezioni eventi select*/
    for (;;){
        len = sizeof(struct sockaddr_in);
        
        /*Esecuzione select*/
        if ((num=select(fd_max, &temp_fds, NULL, NULL, NULL)) < 0){ 
            if (errno==EINTR){
                perror("Segnale ricevuto durante la select. Continuo ad accettare le richieste. ");
                continue;
            }else{
                perror("Errore durante la select:");
                exit(1);
            }
        }
        
        /*Datagram in arrivo*/
        if (FD_ISSET(datagram_sd, &temp_fds)){
            
            if (recvfrom(datagram_sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &len)<0){
                perror("SERVER_DATAGRAM: Errore nella ricezione del datagram:"); 
                continue;
            }
            client = gethostbyaddr((char *) &client_addr.sin_addr,sizeof(client_addr.sin_addr), AF_INET);
            if (client == NULL){
                perror("SERVER_DATAGRAM: Impossibile recuperare le informazioni del cliente:\n");
                close(client_sd);
                continue;
            }else{
                printf("SERVER_DATAGRAM: L'host client che sto servendo è %s\n", client->h_name);
            }
            
            //Server di echo di prova
            if (sendto(datagram_sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, len)<0){
                perror("SERVER_DATAGRAM: Errore nell'invio del datagram:"); 
                continue;
            }	
            
            printf("SERVER_DATAGRAM: Cliente servito con successo\n");
            
        }

        /*Nuova connessione TCP in arrivo*/
        if (FD_ISSET(listen_sd, &temp_fds)){
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
                    
                    //TODO:SVILUPPARE LA LOGICA APPLICATIVA DEL SERVER TCP
                    
                    close(client_sd);
                    exit(0);
                    /*Fine corpo figlio*/
                    break;
                case -1:
                    perror("FIGLIO: Impossibile creare il figlio per gestire una richiesta:");
                    exit(3);
                    break;
                default:
                    close(client_sd);
                }
            }
            
        /*Ripristino maschera originale*/
        temp_fds = read_fds;
    }
    
    exit(0);
}

void gestore(int signo){
    int stato;
    wait(&stato);
    printf("Il figlio è terminato con stato %d\n" , stato>>8);
}
