/*
 * Template di soluzione RPC_Server
 * Matteo Bucci
 * 0000722984
 */

#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RPC_xFile.h"

#define DIM_TABELLA 20

static int inizializzato = 0;
static Struttura1 tabella[DIM_TABELLA];

void inizializza(){
   
    if (inizializzato == 1){
        return;
    }
    
    printf("Inizializzazione server iniziata\n");
    
    int i;
    for (i = 0; i < DIM_TABELLA; i++){

            tabella[i].stringa1 = (char*) malloc(2);
            strcpy(tabella[i].stringa1, "L");

            tabella[i].stringa2 = (char*) malloc(2);
            strcpy(tabella[i].stringa2, "L");

            tabella[i].valoreNumerico = -1;

    }
    
    //Elemento numero1
    free(tabella[0].stringa1);
    tabella[0].stringa1 = (char*) malloc(strlen("VillaBrutto") + 1);
    strcpy(tabella[0].stringa1, "VillaBrutto");
    free(tabella[0].stringa2);
    tabella[0].stringa2 = (char*) malloc(strlen("CignoBrutto") + 1);
    strcpy(tabella[0].stringa2, "CignoBrutto");
    tabella[0].valoreNumerico = 88;

    //Elemento numero4
    free(tabella[3].stringa1);
    tabella[3].stringa1 = (char*) malloc(strlen("FestaNormale") + 1);
    strcpy(tabella[3].stringa1, "FestaNormale");
    free(tabella[3].stringa2);
    tabella[3].stringa2 = (char*) malloc(strlen("FestaBrutto") + 1);
    strcpy(tabella[3].stringa2, "FestaBrutto");
    tabella[3].valoreNumerico = 48;

    //Elemento numero7
    free(tabella[6].stringa1);
    tabella[6].stringa1 = (char*) malloc(strlen("PulceBrutto") + 1);
    strcpy(tabella[6].stringa1, "PulceBrutto");
    free(tabella[6].stringa2);
    tabella[6].stringa2 = (char*) malloc(strlen("SignoraNormale") + 1);
    strcpy(tabella[6].stringa2, "SignoraNormale");
    tabella[6].valoreNumerico = 85;

    //Elemento numero10
    free(tabella[9].stringa1);
    tabella[9].stringa1 = (char*) malloc(strlen("AlberoBrutto") + 1);
    strcpy(tabella[9].stringa1, "AlberoBrutto");
    free(tabella[9].stringa2);
    tabella[9].stringa2 = (char*) malloc(strlen("PulceAlto") + 1);
    strcpy(tabella[9].stringa2, "PulceAlto");
    tabella[9].valoreNumerico = 16;

    //Elemento numero13
    free(tabella[12].stringa1);
    tabella[12].stringa1 = (char*) malloc(strlen("SignoreAlto") + 1);
    strcpy(tabella[12].stringa1, "SignoreAlto");
    free(tabella[12].stringa2);
    tabella[12].stringa2 = (char*) malloc(strlen("FestaDifettoso") + 1);
    strcpy(tabella[12].stringa2, "FestaDifettoso");
    tabella[12].valoreNumerico = 8;

    //Elemento numero16
    free(tabella[15].stringa1);
    tabella[15].stringa1 = (char*) malloc(strlen("AlberoBello") + 1);
    strcpy(tabella[15].stringa1, "AlberoBello");
    free(tabella[15].stringa2);
    tabella[15].stringa2 = (char*) malloc(strlen("SignoraDifettoso") + 1);
    strcpy(tabella[15].stringa2, "SignoraDifettoso");
    tabella[15].valoreNumerico = 19;

    //Elemento numero19
    free(tabella[18].stringa1);
    tabella[18].stringa1 = (char*) malloc(strlen("FestaStraordinario") + 1);
    strcpy(tabella[18].stringa1, "FestaStraordinario");
    free(tabella[18].stringa2);
    tabella[18].stringa2 = (char*) malloc(strlen("VillaBrutto") + 1);
    strcpy(tabella[18].stringa2, "VillaBrutto");
    tabella[18].valoreNumerico = 67;
    
    inizializzato = 1;
    printf("Inizializzazione server completata\n");
    
}

Struttura2 * primo_1_svc(void * nessunParametro, struct svc_req *reqstp){
    static Struttura2 result;
    
    inizializza();
    
    //ATTENZIONE: FUNZIONE DI PROVA
    printf("-Eseguita funzione 1-\n");
    result.numeroValori = 5;
    
    int i;
    for(i = 0; i < result.numeroValori; i++){
        result.lista[i] = tabella[i];
    }
    //RIMUOVERE APPENA NECESSARIO
    
    return (&result);
}

int * secondo_1_svc(char **string, struct svc_req *reqstp){
    static int result;
    
    inizializza();
    
    //ATTENZIONE: FUNZIONE DI PROVA
    printf("-Eseguita funzione 2-\n");
    result = 231;
    //RIMUOVERE APPENA NECESSARIO

    return (&result);
}