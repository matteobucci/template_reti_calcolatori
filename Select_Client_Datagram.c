/*
 * Template di soluzione UDP_Client
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
    int sock_fd, num, port, len;
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
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM,0)) < 0){
        perror("Errore nell'apertura della socket:"); 
        exit(2);
    }
    printf("Creazione socket avvenuta con successo. FD = %d\n", sock_fd);
    
    /*Corpo del client*/
    printf("%s\n", input_request);
    while (gets(input)){
        printf("%s\n", input_request);
	if((num = sendto(sock_fd, input, sizeof(input), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) <= 0){
            perror("Errore nell'invio del datagram:");
        }else{
            printf("Datagram inviato con successo. (%d byte)\n", num);
        }
	
	if((num = recvfrom(sock_fd, input, sizeof(input), 0, (struct sockaddr *)&serv_addr, &len)) <= 0){
            perror("Errore nella ricezione del datagram:");
        }else{
            printf("Datagram ricevuto. Dati: %s\n", input);
        }

    }
    
    /*Operazioni di chiusura*/
    printf("Chiusura client.\n");
    exit(0);
}
