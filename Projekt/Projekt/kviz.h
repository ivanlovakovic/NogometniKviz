#ifndef KVIZ_H
#define KVIZ_H

#include <stdio.h>

// 2. Makroi za cjelobrojne konstante (veličina polja)
#define MAX_PITANJA   10          // 2
#define MAX_DULJINA   256         // 2
#define MAX_IME       50          // 2

// 4. typedef + enum – tip za izbornik
typedef enum {
    MENU_IZLAZ = 0,         // 4, 11
    MENU_IGRAJ = 1,         // 4, 11
    MENU_REZULTATI = 2,         // 4, 11
    MENU_DATOTEKE = 3          // 4, 11
}MenuIzbor;

// 3. Složeni tip: struktura pitanja (tekst, opcije, točan odgovor)
typedef struct {
    char tekst[MAX_DULJINA];          // 12
    char opcije[4][MAX_DULJINA];      // 12
    char tocanOdgovor[2];             // 12
}pitanje;

// 3. Složeni tip: rezultat igre (ime, bodovi, težina)
typedef struct {
    char ime[MAX_IME];                // 12
    int  bodovi;                      // 2
    char tezina[20];                  // 12
}rezultat;

// 6. Static globalna varijabla za debug ili neke interne postavke
static int debugFlag = 0;            // 6

// 8. Extern globalna varijabla – statistika igre (definicija u drugoj datoteci)
extern int globalnaStatistika;       // 8

#endif