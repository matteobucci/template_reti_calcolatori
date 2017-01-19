/*
 * Template di soluzione RPC_xFile
 * Matteo Bucci
 * 0000722984
 */

struct Struttura1 {string stringa1<30>; string stringa2<30>; int valoreNumerico;};
    
struct Struttura2 {int numeroValori; Struttura1 lista[5];}; 
  
program TEMPLATE {
    version TEMPLATEVERS {         
        Struttura2 PRIMO() = 1;        
        int SECONDO(string) = 2;
    } = 1;
} = 0x20000013;

