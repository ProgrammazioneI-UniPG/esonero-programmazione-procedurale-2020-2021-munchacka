// Dichiarazione delle funzioni da chiamare in main.c
// Definizione dei tipi necessari alla libreria

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum Stato_giocatore { astronauta, impostore, assassinato, defenestrato } stato_giocatore;

typedef enum Nome_giocatore { rosso, blu, verde, giallo, arancione, nero, bianco, azzurro, marrone, viola } nome_giocatore;

typedef enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola} tipo_stanza;

typedef struct Stanza {
    struct Stanza *avanti;
    struct Stanza *destra;
    struct Stanza *sinistra;
    struct Stanza *stanza_precedente;
    struct Stanza *next;
    tipo_stanza tipo;
    bool emergenza_chiamata;
} stanza;

typedef struct Giocatore {
    stanza *posizione;
    stato_giocatore stato;
    nome_giocatore nome;
    bool nome_usato;
} giocatore;

int imposta_gioco();

void gioca(int ngiocatori);

void termina_gioco();


