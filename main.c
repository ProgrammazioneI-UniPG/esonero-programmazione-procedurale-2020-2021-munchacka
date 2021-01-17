#include "gamelib.h"
#include <stdio.h>
#include <time.h>

// Questo file continene solo la definizione della funzione main
// con il menu principale 1-2-3

int main() {
int n, scelta = 0;
    do {
        printf("1) Imposta gioco\n2) Gioca \n3) Termina gioco\n0) Esci dal programma\n>");
        scanf("%d", &scelta);
        switch (scelta) {
            case 1: 
                n = imposta_gioco();
                break;

            case 2: 
                gioca(n);
                break;

            case 3:
                termina_gioco();
                break;
            case 0:
                scelta = 0;
            
            default:
                printf("Operazione non consentita!\n");
                break;
            }

    } while (scelta != 0);
    return 0;
}



