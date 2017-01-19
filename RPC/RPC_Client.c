/*
 * Template di soluzione RPC_Client
 * Matteo Bucci
 * 0000722984
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

#define MAX_LEN 128

int main (int argc, char *argv[]){

    
    /*Variabili utilizzate*/
    char *host_addr;
    CLIENT *client;
    char input[MAX_LEN];
    char input_request[] = {"Inserisci qualcosa"};
    int i;
    
    char *parametro;
    Struttura2 *output;     //Risposta della prima funzione
    int *outputInt;         //Risposta della seconda funzione
    void *puntVoid;         //Necessario per fare chiamate senza parametri

    /*Controllo argomenti*/
    if (argc != 2){
	printf("Utilizzo: %s serverAddress \n", argv[0]);
	exit(1);
    }
    host_addr = argv[1];
    
    //Creazione gestore del trasporto
    client = clnt_create (host_addr, TEMPLATE, TEMPLATEVERS, "udp");
    if (client == NULL){
        clnt_pcreateerror(host_addr);
        exit(1);
    }

    /*Corpo del client*/
    printf("%s\n", input_request);
    while (gets(input)){
        if (strcmp(input,"1") == 0){
            /*Prima funzione*/
            /*Esempio con funzione che senza parametri di ingresso restituisce una lista in uscita*/
            output = primo_1(puntVoid, client);
            if (output == NULL){
                clnt_perror(client, host_addr);
                exit(2);
            }
            if (output->numeroValori < 0){
                printf("La funzione non ha restituito alcun valore.\n");
            }else{
                printf("La funzione ha restituito %d valori.\n", output->numeroValori);
                for (i = 0; i < output->numeroValori; i++){
                    printf("--Struttura numero %d--\n", i+1);
                    printf("\tStringa: %s\n", output->lista[i].stringa1);
                    printf("\tStringa: %s\n", output->lista[i].stringa2);
                    printf("\tValore numerico: %d\n", output->lista[i].valoreNumerico);
                }                   
            }
        }
        else if (strcmp(input,"2") == 0){
            /*Seconda funzione*/
            /*Esempio di una funzione che in ingresso richiede una stringa*/
            printf("Inserisci la stringa che intendi inviare: ");
            if (!gets(input)){
                break;  //Esce dal client
            }
            parametro = &(input[0]);
            outputInt = secondo_1(&parametro, client);
            if (outputInt == NULL){
                clnt_perror(client, host_addr);
                exit(2);
            }
            printf("Il server ha risposto con il valore %d\n", *outputInt);
        }
        else{
            printf("Richiesta non valida.\n");
        }

        printf("%s\n", input_request);
    }

    /*Terminazione client*/
    clnt_destroy (client);
    exit(0);
}
