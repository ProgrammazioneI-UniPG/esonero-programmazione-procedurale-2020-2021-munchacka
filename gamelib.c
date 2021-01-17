#include "gamelib.h"
#include <stdio.h>
#include <time.h>
// Definizioni delle funzioni in gamelib.c.
// Piu altre funzioni di supporto.
// Le funzioni richiamabili in main.c non devono essere static.
// Le altre devono essere static (non visibili all'esterno).

giocatore* pgiocatori;              //array
unsigned short quest_da_finire;     // numero quest da finire 
stanza* stanza_inizio;              //stanza iniziale da cui iniziano a giocare tutti
stanza* lista_stanze;               //array

static const char *str_stato_giocatore[] = { "astronauta", "impostore", "assassinato", "defenestrato" };
static const char *str_tipo_stanza[] = {"vuota", "quest_semplice", "quest_complicata", "botola"};
static const char *str_nome_giocatore[] = {"rosso", "blu", "verde", "giallo", "arancione", "nero", "bianco", "azzurro", "marrone", "viola"};

// Prende in ingresso il puntatore di una stanza e ne assegna il tipo casualmente
void static random_stanza(stanza *s) {
    int r = rand() % 100 + 1;

    if (r <= 25) s->tipo = 3;
    if (r <= 40 && r >= 25) s->tipo = 2;  
    if (r <= 70 && r >= 40) s->tipo = 1;
    if (r >= 70) s->tipo = 0; 
}

void static stampa_giocatori(int ngiocatori) {
    printf("\n");
    for (int i = 0; i < ngiocatori; i++)
        printf("Giocatore: %s, stato: %s\n", str_nome_giocatore[(pgiocatori+i)->nome], str_stato_giocatore[(pgiocatori+i)->stato]);
}

void static inizia_gioco(int errore) {
    if (errore != 0)
        printf("Gioco impostato correttamente!\n");
    else
        printf("Errore nell'impostazione del gioco!\n");
}

int imposta_gioco() {

    time_t t;                srand((unsigned) time(&t));  
    int errore; // Controllo che la memoria venga allocata correttamente

    // Acquisizione numero giocatori con controllo
    int ngiocatori;
    printf("Numero giocatori: ");
    scanf("%d", &ngiocatori);
    while (ngiocatori < 4 || ngiocatori > 10) {
        printf("max 10 min 4!\n> ");
        scanf("%d", &ngiocatori);
    }

    // Creazione array usando il puntatore
    pgiocatori = (giocatore*) malloc(sizeof(giocatore) * ngiocatori);
    if (pgiocatori == NULL) 
        printf("Errore nell'allocazione della memoria!\n");
    else errore = 1;

    // Assegnazione impostori
    int probimp = 20, cimp = 0;                             // probabilita' impostore e contatore impostori
    for (int i = 0; i < ngiocatori; i++) {
        int rand_impostore = rand() % 100 + 1;              // Viene generato un numero che ha una probabilita' del 20% di essere compreso tra 1 e 20, aumentando questa probabilita' ad ogni giocatore in piu'
        if (rand_impostore <= probimp && cimp < 3) {        // Controllo che gli impostori non siano piu' di 3
            (pgiocatori+i)->stato = 1;
            cimp++;
        } else {
            (pgiocatori+i)->stato = 0;
        }
        probimp = probimp + 10;                             // Aumento la probabilita' di essere un impostore      
    } if (cimp == 0) {
        int r = rand() % ngiocatori;                        // Nella malaugurata ipotesi che le probabilita' non bastino viene scelto un solo impostore a caso estremo
        printf("%d", r);
        (pgiocatori+r)->stato = 1;
    }

    // Assegnazione colori randomici
    int rand_colore[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};     // Creo prima un arrey che conterra' gli indici dei colori
    for (int i = 0; i < 10 - 1; i++) {                      // lo "mischio" 
        int r = rand () % 10;
        int temp = rand_colore[r];
        rand_colore[r] = rand_colore[i];
        rand_colore[i] = temp;
    }

    for (int i = 0; i < ngiocatori; i++)                    // e assegno i colori ai giocatori
        (pgiocatori+i)->nome = rand_colore[i];
    
    // Dichiarazione stanza iniziale e inizializzazione
    stanza_inizio = (stanza*) malloc(sizeof(stanza));

    if (stanza_inizio == NULL)
        printf("Errore nell'allocazione della memoria!\n");
    else errore = 1;

    random_stanza(stanza_inizio);

    stanza_inizio->avanti = NULL;
    stanza_inizio->destra = NULL;
    stanza_inizio->sinistra = NULL;
    stanza_inizio->stanza_precedente = NULL;
    stanza_inizio->next = NULL;
    stanza_inizio->emergenza_chiamata = false;
    // ----------
    
    // Assegnazione stanza iniziale a tutti i giocatori
    for (int i = 0; i < ngiocatori; i++)
        (pgiocatori+i)->posizione = stanza_inizio;

    // Setto la lista delle stanze in modo che parta dalla stanza iniziale
    lista_stanze = (stanza*) malloc(sizeof(stanza));
    if (lista_stanze == NULL)
        printf("Errore nell'allocazione della memoria!\n");
    else errore = 1;
    lista_stanze = stanza_inizio;

    // Quest da finire con controllo
    printf("Quest da finire: ");
    scanf("%hu", &quest_da_finire);
    while (quest_da_finire <= 0) {
        printf("Non corretto: ");
        scanf("%hu", &quest_da_finire);
    }

    // Piccolo menu' che inizia il gioco o stampa i giocatori
    int scelta; do {
        printf("\nCosa vuoi fare? \n1) Stampa giocatori \n2) Inizia gioco\n>");
        scanf("%d", &scelta);
        switch (scelta) {
            case 1:
                stampa_giocatori(ngiocatori);
                break;
            case 2:
                inizia_gioco(errore);
                break;
            
            default:
                printf("Non ho capito, riprova\n");
                break;
        }
    } while (scelta != 2);
    
    return ngiocatori;
}

void stampa_stanze() {
    printf("\nLista stanze\n");
    int cstanze = 0;
    stanza* supp = stanza_inizio;
    do {
        printf("Stanza n: %d, tipo %s\n", cstanze, str_tipo_stanza[supp->tipo]);
        supp = supp->next;
        cstanze++;
    } while (supp != NULL);
}

// Crea la nuova stanza in cui il giocatore si spostera' e la aggiunge alla lista
stanza* crea_stanza(int giocatore) {
    
    stanza* nuova_stanza = (stanza*) malloc(sizeof(stanza));
    random_stanza(nuova_stanza);
    nuova_stanza->next = NULL;
    nuova_stanza->stanza_precedente = (pgiocatori+giocatore)->posizione;

    (pgiocatori+giocatore)->posizione = nuova_stanza;

    printf("\nUna nuova stanza e' stata creata! Tipo: %s\n", str_tipo_stanza[nuova_stanza->tipo]);

    // In lista_stanze viene salvato il puntatore all'ultima stanza in modo da non dover riscorrere sempre la lista
    lista_stanze->next = nuova_stanza;
    lista_stanze = lista_stanze->next;
   
    return nuova_stanza;
}

void static avanza(int giocatore) {
    int s = 0;
    printf("Dove vuoi andare: \n1) Avanti 2) Destra 3) Sinistra 4) Rimanere fermo\n> ");
    scanf("%d", &s);
    while(s > 4 || s < 0) {
        printf("Non penso di aver capito bene..\n> ");
        scanf("%d", &s);
    }
    switch (s) {
        case 1:
            if ((pgiocatori+giocatore)->posizione->avanti == NULL)  {
                (pgiocatori+giocatore)->posizione->avanti = crea_stanza(giocatore);
                stampa_stanze();
            } else (pgiocatori+giocatore)->posizione = (pgiocatori+giocatore)->posizione->avanti;
            break;
        case 2:
            if ((pgiocatori+giocatore)->posizione->destra == NULL)  {
                (pgiocatori+giocatore)->posizione->destra = crea_stanza(giocatore);
                stampa_stanze();
            } else {
                (pgiocatori+giocatore)->posizione = (pgiocatori+giocatore)->posizione->destra;
            } break;
        case 3:
            if ((pgiocatori+giocatore)->posizione->sinistra == NULL)  {
            (pgiocatori+giocatore)->posizione->sinistra = crea_stanza(giocatore);
                stampa_stanze();
            } else {
                (pgiocatori+giocatore)->posizione = (pgiocatori+giocatore)->posizione->sinistra;
            } break;
        
        case 4:
            printf("Sei rimasto fermo!\n");
            break;
    }

    

    
}

// Decrementa il contatore quest_da_finire in base al tipo di stanza e controlla che il gioco finisca con la vittoria degli astronauta facendo 
// ritornare il gioco al menu' principale
int static esegui_quest (int giocatore) {
    if ((pgiocatori+giocatore)->stato == 0) {
        if ((pgiocatori+giocatore)->posizione->tipo == 1) {
            quest_da_finire--;
            if (quest_da_finire <= 0) {
                termina_gioco(1);
                return 1;
            }
            
            printf("Quest semplice eseguita, ne mancando %d\n", quest_da_finire);
        } 
        if ((pgiocatori+giocatore)->posizione->tipo == 2) {
            quest_da_finire = quest_da_finire - 2;    
            if (quest_da_finire <=  0) {
                termina_gioco(1);
                return 1;
            }
            
            printf("Quest complicata eseguita, ne mancando %d\n", quest_da_finire);
        } else {
            printf("You simply can't do it!\n");
            return 0;
        }
            
    } else {
        printf("You simply can't do it!\n");
        return 0;
    }

    return 0;
}

int static uccidi_astronauta(int giocatore, int ngiocatori) {
    int c = 1, giocatore_da_uccidere, prob = 0, flag = 0;                   
    if ((pgiocatori+giocatore)->stato == 1) {
        // Controlla chi l'impostore puo' uccidere nella stessa stanza e stampa un menu' di uccisione
        printf("Puoi uccidere: \n");
        for (int i = 0; i < ngiocatori; i++) {
            if (((pgiocatori+giocatore)->posizione == (pgiocatori+i)->posizione && (pgiocatori+giocatore) != (pgiocatori+i)) && (pgiocatori+i)->stato == 0) {
                printf("%d) %s\n", i, str_nome_giocatore[(pgiocatori+i)->nome]);
                c++;
            }
        } if (c == 1) {
            printf("nessuno! :(\n");
            return 1;
        }

        printf("\nScegli chi uccidere: ");
        scanf("%d", &giocatore_da_uccidere);

        do {
            if ((pgiocatori+giocatore_da_uccidere)->stato == 0 && giocatore_da_uccidere < ngiocatori) {
                (pgiocatori+giocatore_da_uccidere)->stato = 2;
                flag = 1;
            } else {
                printf("Non puoi ucciderlo, riprovare: ");
                scanf("%d", &giocatore_da_uccidere);
            }
        } while (flag != 1);
        
        printf("\nOh no %s e' stato ucciso! Anyways..\n", str_nome_giocatore[(pgiocatori+giocatore_da_uccidere)->nome]);

        // Impostazione della probabilita' di essere scoperto ed esecuzione
        for (int i = 0; i < ngiocatori; i++) {
            if ((pgiocatori+i)->posizione == (pgiocatori+giocatore)->posizione && (pgiocatori+i)->stato == 0 ) 
                prob = prob + 50;
            if ((pgiocatori+giocatore)->posizione->stanza_precedente == (pgiocatori+i)->posizione && (pgiocatori+i)->stato == 0) {
                prob = prob + 20;
            }
        }
        int r = rand() % 100 + 1;
        if (r <= prob) {
            printf("OPS SEI STATO SCOPERTO E DEFENESTRATO!\n");
            (pgiocatori+giocatore)->stato = 3;
        } else 
            return 1;

    } else {
        printf("You simply can't do it");
        return 0;
    }
    return 0;
}

int static chiamata_emergenza(int i, int ngiocatori) {
    int c = 0, prob = 0;
    int n = rand() % 100 + 1;

    // Controllo se la chiamata d'emergenza e' stata effettivamente gia' (bad bum bass) "chiamata;
    if ((pgiocatori+i)->posizione->emergenza_chiamata) {
        printf("Emergenza gia' chiamata!\n");
        return 0;
    } else {

        // Controllo ci siano morti nella stanza
        printf("CHIAMATA DI EMERGENZA\n");
        for (int j = 0; j < ngiocatori; j++) {
            if ((pgiocatori+j)->stato == 2) c++;
        }
        if (c==0) {
            printf("Non ci sono morti qui");
            return 1;
        } else {
            for (int k = 0; k < ngiocatori; k++) {

                // Imposto le probabilita per ogni astronauta
                if ((pgiocatori+k)->stato == 0) {
                    prob = 30;
                    for (int l = 0; l < ngiocatori; l++) {
                        if ((pgiocatori+l)->stato == 1) prob = prob + 20;
                        if ((pgiocatori+l)->stato == 0) prob = prob - 30;
                    }
                    if (n<=prob) {
                        printf("%s e' stato defenestrato!", str_nome_giocatore[(pgiocatori+k)->nome]);
                        (pgiocatori+k)->stato = 3;
                        (pgiocatori+k)->posizione->emergenza_chiamata = true;
                        return 1;
                    } 
                }

                // Imposto le probabilita per ogni impostore
                if ((pgiocatori+k)->stato == 1) {
                    prob = 30;
                    for (int l = 0; l < ngiocatori; l++) {
                        if ((pgiocatori+l)->stato == 0) prob = prob + 20;
                        if ((pgiocatori+l)->stato == 1) prob = prob - 30;
                    }
                    if (n<=prob) {
                        printf("%s e' stato defenestrato!", str_nome_giocatore[(pgiocatori+k)->nome]);
                        (pgiocatori+k)->stato = 3;
                        (pgiocatori+k)->posizione->emergenza_chiamata = true;
                        return 1;
                    }
                }
            } 
        }
    }  
    return 0; 
}

// Semplicemte cambio il tipo alla stanza
int static sabotaggio(int i) {
    if ((pgiocatori+i)->stato == 1 && ((pgiocatori+i)->posizione->tipo == 1 || (pgiocatori+i)->posizione->tipo == 2)) {
        printf("La stanza e' stata sabotata!\n");
        (pgiocatori+i)->posizione->tipo = 0;
        return 1;
    } else {
        printf("\nYou simply can't do it");
        return 0;
    }
}

int static conta_botole() {
    int nbotole = 0;
    stanza* botola = stanza_inizio;
    while (botola != NULL) {
        if (botola->tipo == 3) {
            nbotole++;
        }
        botola = botola->next;
    }
    return nbotole;
}

int static conta_stanze() {
    int nstanze = 0;
    stanza* stanze = stanza_inizio;
    while (stanze != NULL) {
        nstanze++;
        stanze = stanze->next;
    }
    return nstanze;
}

int static usa_botola(int i) {
    // Controlla anche che 
    if ((pgiocatori+i)->posizione->tipo == 3 && (pgiocatori+i)->stato == 1 && (pgiocatori+i)->posizione->next != NULL) {   
        if (conta_botole() > 1) {
            int nbotola = 0, rand_botola;
            rand_botola = rand() % conta_botole() + 1;
            stanza* stanza_casuale = stanza_inizio;
            while (stanza_casuale != NULL) {
                if (stanza_casuale->tipo == 3)
                    nbotola++;

                if (nbotola == rand_botola) {
                    (pgiocatori+i)->posizione = stanza_casuale;
                    printf("Ti sei spostato in una stanza con la botola!");
                    return 1;
                }
                stanza_casuale = stanza_casuale->next;
            }
        } else {
            int nstanza = 0, rand_stanza;
            rand_stanza = rand() % conta_stanze() + 1;
            stanza* stanza_casuale = stanza_inizio;
            while (stanza_casuale != NULL) {
                nstanza++;
                if (nstanza == rand_stanza) {
                    (pgiocatori+i)->posizione = stanza_casuale;
                    printf("Ti sei spostato in una stanza casuale!");
                    return 1;
                }
                stanza_casuale = stanza_casuale->next;
            }

        }
    } else {
        printf("YOu simply can't do it!\n");
        return 0;
    }
    return 0;
}

void gioca(int ngiocatori) {
    // Controllo che il gioco sia stato impostato o che non ci siano errori
    if (pgiocatori == NULL && stanza_inizio == NULL && lista_stanze == NULL) printf("Si prega di impostare il gioco correttamente\n");

    int scelta = 0;
    int fine;
    do {
        for (int i = 0; i < ngiocatori; i++) {

            // Se un giocatore risulta assasinato o defenestrato semplicemente salta il turno
            if ((pgiocatori+i)->stato == 3 || (pgiocatori+i)->stato == 2) {
                printf("\n------------------------ %s SEI FUORI DAL GIOCO! ------------------------ \n", str_nome_giocatore[(pgiocatori+i)->nome]);
                continue;
            } 

            printf("------------------------ Turno di %s ------------------------ \n", str_nome_giocatore[(pgiocatori+i)->nome]);
            if ((pgiocatori+i)->stato == 0) printf("Astronauta | ");
            if ((pgiocatori+i)->stato == 1) printf("Impostore | ");
            printf("Tipo stanza: %s\n", str_tipo_stanza[(pgiocatori+i)->posizione->tipo]);
            printf("Giocatori presenti nella stanza\n");
            
            int c = 0;
            for (int j = 0; j < ngiocatori; j++) {
                if ((pgiocatori+i)->posizione == (pgiocatori+j)->posizione && (pgiocatori+i)->nome != (pgiocatori+j)->nome) {
                    if ((pgiocatori+j)->stato == 3) continue;
                    if ((pgiocatori+j)->stato == 2) printf("%s MORTO\n", str_nome_giocatore[(pgiocatori+j)->nome]); 
                    else {
                        printf("%s %s\n", str_nome_giocatore[(pgiocatori+j)->nome], str_stato_giocatore[(pgiocatori+j)->stato]); 
                        c++; 
                    } 
                }
            } if (c==0) printf("sei solo nella stanza!\n");

            printf("1) Avanza \n2) Esegui quest \n3) Chiamata emergenza \n4) Uccidi austronauta \n5) Usa botola \n6) Sabotaggio\n> ");
            scanf("%d", &scelta);
            switch (scelta) {
                case 1:
                    avanza(i);
                    break;
                case 2:
                    
                    fine = esegui_quest(i);
                    if (fine == 1) return;
                    if (fine == 0) i--;
                case 3:
                    if (chiamata_emergenza(i, ngiocatori) == 0) 
                        i--;
                    break;
                case 4:
                    if (uccidi_astronauta(i, ngiocatori) == 0) 
                        i--;
                    break;
                case 5:
                    if (usa_botola(i) == 0)
                        i--;
                    break;
                case 6:
                    if (sabotaggio(i) == 0)
                        i--;
                    break;

                default:
                    printf("Non ho capito riprovare!\n");
                    i--;
                    break;
            }

            int all_out = 0;
            for (int i = 0; i < ngiocatori; i++) {
                if ((pgiocatori+i)->stato == 0)
                    all_out = 1;
            }
            if (all_out == 0) 
                termina_gioco(all_out);
            

        }
    } while (quest_da_finire >= 0);

}

void termina_gioco(int dead) {

    if (dead == 0) {
        printf("Congratulazioni il gioco e' finito, gli impostori hanno vinto!\n");
        quest_da_finire = 0;
    }
    else printf("Congratulazioni il gioco e' finito, gli astronauti hanno vinto!\n");

    printf("Inizio deallocamento.... \n");
    if (pgiocatori == NULL || stanza_inizio == NULL ) {
        printf("Niente da deallocare!\n");
        if (stanza_inizio->next == NULL) {
            free(stanza_inizio);
            free(lista_stanze);
        }
    } else {
        quest_da_finire = 0;
        free(pgiocatori);
        printf("Giocatori deallocati!\n");
        lista_stanze = stanza_inizio;
        stanza* dealloc;
        while ((dealloc = lista_stanze) != NULL) {
            lista_stanze = lista_stanze->next;
            free(dealloc);
        }
        free(stanza_inizio);
        free(lista_stanze);
        
        printf("Stanze deallocate!\n"); 


    }  
}



