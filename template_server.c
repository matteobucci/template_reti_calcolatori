#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DEFAULT_PORT 10000;

long handle (int h);

int main(int argc, char **argv){
    
    u_short port;                   //La porta su cui il server è in ascolto
    struct sockaddr_in servaddr;    //La struttura che ospita l'indirizzo del server
    struct sockaddr_in cliaddr;     //La struttura che ospita l'indirizzo di un client
    int lencl;                      //La lunghezza dell'indirizzo del figlio
    int listenfd;                   //Il file descriptor della porta in ascolto
    int clientfd;                   //Il file descriptor della socket connessa ad un client
    int num = 0;                    //Contatore utilizzato per controllare la porta
    fd_set read_hs, temp_hs;        //Maschere di lettura utilizzate nell'accept
    int fd_max;                     //Il file descriptor con valore più alto utilizzato dal server
    int nready;                     //Il numero di file descriptor pronti dopo una select
    
    /* CONTROLLO ARGOMENTI  */
    if(argc==1){
        //Non sono stati passati argomenti. Utilizzo la porta di default
        port = DEFAULT_PORT;
    }else{
        //Controllo che la porta sia un intero
        while( argv[1][num]!= '\0' ){
                if( (argv[1][num] < '0') || (argv[1][num] > '9') ){
                        printf("Error: %s port\n", argv[0]);
                        printf("%s is not and integer\n", argv[1]);
                        exit(2);
                }
                num++;
        }
        port = atoi(argv[1]);
        //Controllo che la porta appartenga al range delle porte disponibili
        if (port < 1024 || port > 65535){
                printf("Error: %s port\n", argv[0]);
                printf("1024 <= port <= 65535\n");
                exit(2);  	
        }
    }
    /* FINE CONTROLLO ARGOMENTI  */
    
    printf("Server started\n");
    printf("Used port: %d\n", port);
    
    /* INIZIALIZZAZIONE INDIRIZZO SERVER E BIND  */
    memset ((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    /* FINE INIZIALIZZAZIONE INDIRIZZO SERVER E BIND  */
    
    /* CREAZIONE SOCKET TCP  E BINDING */
    listenfd=socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd <0){
        perror("Opening TCP socket failed: "); 
        exit(1);
        
    }
    
    printf("TCP socket opened. FD=%d\n", listenfd);
   
    if (bind(listenfd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0){
        perror("Binding TCP socket failed :"); 
        exit(3);
        
    }
    
    printf("Bind socket TCP ok\n");

    if (listen(listenfd, 5)<0){
        perror("Listen failed: ");
        exit(1);
        
    }
    
    printf("Listening...\n");
    
    /* INIZIALIZZAZIONE MASCHERE  */
    FD_ZERO(&read_hs); //Pulisco la maschera
    FD_SET(listenfd, &read_hs); //Metto in ascolto la socket di listen nella maschera
    temp_hs = read_hs; //Copio la maschera di riferimento nella maschera temporanea
    fd_max = listenfd + 1; //Per il momento il file descriptor con valore più alto lo ha la socket in ascolto
    /* FINE INIZIALIZZAZIONE MASCHERE  */
    
    /* CICLO DI RICEZIONE EVENTI DALLA SELECT  */
	for(;;){
		
		if ((nready=select(fd_max, &temp_hs, NULL, NULL, NULL))<0){ //Effettuo la select
			if (errno==EINTR){
                            continue;
                        }
			else {
                            perror("Error on the select: ");
                            exit(1);
                            
                        }
		}

		/* GESTIONE NUOVA CONNESSIONE */
		if (FD_ISSET(listenfd, &temp_hs)){
			printf("Request of connection received\n");
                        
			lencl = sizeof(struct sockaddr_in);
			if((clientfd = accept(listenfd,(struct sockaddr *)&cliaddr,&lencl))<0){
				if (errno==EINTR){
                                    continue;
                                }
				else {
                                    perror("Error on the accept: ");
                                    exit(1);
                                }
			}else{
                            FD_SET(clientfd, &read_hs);
                            if(clientfd > fd_max){
                                fd_max = clientfd + 1;
                            }
                        }
                    
		}
		/* FINE GESTIONE NUOVA CONNESSIONE */
                
                /* GESTIONE DATI IN ARRIVO */
                int h;
                for (h = listenfd + 1; h < fd_max; h++){ 		//Controllo tutti i file descriptor che ospitano socket aperte
                    if (FD_ISSET(h, &temp_hs)){ 			//Il file descriptor corrispondente è pronto a essere letto senza sospendere il server
                        if (handle (h) == 0){  				//handle(int) è la funzione che gestisce la richiesta. Ritorna 0 in caso di chiusura della connessione da parte del figlio
                            FD_CLR(h, &read_hs); 			//Rimuovo il file descriptor dalla maschera in questione
                            close(h); 
                        }
                    }
                }
                /* FINE GESTIONE DATI IN ARRIVO */
                
                temp_hs = read_hs; //Aggiorno temp_hs in qualsiasi caso in quanto è stato modificato dalla select.
        }
    
    exit(0);
}

long handle (int h){ 
    return 0;
}

