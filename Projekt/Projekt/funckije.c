#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "kviz.h"
#include "header.h"

#define MAX_REZULTATA 100

// 8. Globalna statistika igre
int globalnaStatistika = 0;

// 25. Rekurzivno binarno pretraživanje po bodovima
static int pronadiRezultatPoBodovima(rezultat* niz, int low, int high, int target) {
    if (low > high) return -1;

    int mid;

    mid = low + (high - low) / 2;

    if (niz[mid].bodovi == target) return mid;

    if (niz[mid].bodovi < target)
        return pronadiRezultatPoBodovima(niz, mid + 1, high, target); // 25.
    else
        return pronadiRezultatPoBodovima(niz, low, mid - 1, target);  // 25.
}



// 26. Pokazivač na funkciju: komparator za qsort (po bodovima)
static int usporediRezultate(const void* a, const void* b) {
    const rezultat* ra = (const rezultat*)a;  // 26.
    const rezultat* rb = (const rezultat*)b;  // 26.
    return (rb->bodovi - ra->bodovi);         // 23. qsort
}


// 23. Sortiranje rezultata
static void sortirajRezultate(rezultat* niz, int size) {
    if (niz == NULL || size <= 0) return;     // 14.
    qsort(niz, size, sizeof(rezultat), usporediRezultate); // 23.
}


// 24. Pretraživanje po imenu (linearno search)
static rezultat* pronadiRezultatPoImenu(rezultat* niz, int size, const char* ime) {
    int i;

    if (niz == NULL || size <= 0 || ime == NULL) return NULL; // 14.
    for (i = 0; i < size; i++) {
        if (strcmp(niz[i].ime, ime) == 0)   // 24.
            return &niz[i];
    }

    return NULL;
}



// 9. Makro funkcija: maksimum
#define MAX(a, b) ((a) > (b) ? (a) : (b))   // 9.

// 14. Čišćenje stringa (nuliranje)
static void ocistiString(char* s, size_t size) {
    if (s == NULL || size == 0) return;     // 14.

    for (size_t i = 0; i < size; i++) {
        s[i] = '\0';
    }
}

// 18. Sigurno brisanje dinamički zauzete memorije
static void sigurnoOslobodi(void** ptr) {

    if (ptr && *ptr) {                      // 18.
        free(*ptr);                         // 17.
        *ptr = NULL;                        // 18.
    }
}



// 1. CRUID – čitanje pitanja iz datoteke
int ucitajPitanja(const char* nazivDatoteke, pitanje* niz) {

    FILE* file = fopen(nazivDatoteke, "r"); // 1.

    if (file == NULL) {
        return 0;
    }

    int i = 0;
    char linija[1024];

    while (fgets(linija, sizeof(linija), file) != NULL && i < MAX_PITANJA) {
        char* token = strtok(linija, ";\r\n");
        if (token == NULL) continue;
        strcpy(niz[i].tekst, token);        // 14.

        int j;
        for (j = 0; j < 4; j++) {
            token = strtok(NULL, ";\r\n");
            if (token == NULL) break;
            strcpy(niz[i].opcije[j], token); // 14.
        }

        token = strtok(NULL, ";\r\n");
        if (token != NULL && token[0] != '\0') {
            niz[i].tocanOdgovor[0] = (char)toupper((unsigned char)token[0]);
            niz[i].tocanOdgovor[1] = '\0';
        }
        else {
            niz[i].tocanOdgovor[0] = '\0';
        }
        i++;
    }

    fclose(file);                           // 19.
    return i;
}



// 14. Prikaz pojedinačnog pitanja
void prikaziPitanje(pitanje p, int redniBroj) {
    printf("\n------------------ %d. -------------------\n", redniBroj);
    printf("%s\n", p.tekst);
    printf("A) %-20s B) %-20s\n", p.opcije[0], p.opcije[1]);
    printf("C) %-20s D) %-20s\n", p.opcije[2], p.opcije[3]);
    printf("-------------------------------------------\n");
    printf("Unesite odgovor (A/B/C/D): ");
}



// 14. Provjera točnosti odgovora
int provjeriOdgovor(pitanje p, char unos) {
    char tocan;

    tocan = (char)toupper((unsigned char)p.tocanOdgovor[0]);


    char korisnik;

    korisnik = (char)toupper((unsigned char)unos);

    return tocan == korisnik;
}


// 1. CRUID – upis rezultata
void zapisiRezultat(rezultat r) {
    FILE* f = fopen("rezultati.txt", "a");  // 1.

    if (f == NULL) {
        perror("fopen rezultati.txt");      // 22.
        return;
    }

    fprintf(f, "Ime: %-20s | Bodovi: %d/%d | Tezina: %s\n", r.ime, r.bodovi, MAX_PITANJA, r.tezina); // 1.

    fclose(f);                              // 19.

    globalnaStatistika++;                   // 8.
    printf("\nRezultat uspjesno spremljen!\n");
}



// 1. CRUID + 16. Dinamička alokacija + 18. Sigurno brisanje
//   - korisnik MORA unijeti slovo A/B/C/D
void pokreniIgru(const char* datoteka, const char* level) {

    pitanje* pitanja = calloc(MAX_PITANJA, sizeof(pitanje)); // 16, 17.

    if (pitanja == NULL) {
        perror("calloc pitanja");           // 22.
        return;
    }

    int brojUcitanih;

    brojUcitanih = ucitajPitanja(datoteka, pitanja);

    if (brojUcitanih == 0) {
        printf("Datoteka %s ne postoji ili je prazna.\n", datoteka);
        sigurnoOslobodi((void**)&pitanja);  // 18.
        return;
    }

    rezultat rez;
    ocistiString(rez.ime, sizeof(rez.ime)); // 14.
    rez.bodovi = 0;

    if (level) {
        strncpy(rez.tezina, level, sizeof(rez.tezina) - 1); // 14.
        rez.tezina[sizeof(rez.tezina) - 1] = '\0';
    }

    for (int i = 0; i < brojUcitanih; i++) {
        char unos;
        prikaziPitanje(pitanja[i], i + 1);

        while (1) {
            if (scanf(" %c", &unos) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                printf("Morate unijeti slovo (A/B/C/D)!\n");
                continue;
            }

            while (getchar() != '\n') {}

            if ((unos == 'A' || unos == 'a' ||
                unos == 'B' || unos == 'b' ||
                unos == 'C' || unos == 'c' ||
                unos == 'D' || unos == 'd')) {
                break;
            }
            else {
                printf("Morate unijeti slovo A, B, C ili D!\n");
            }
        }

        if (provjeriOdgovor(pitanja[i], unos)) {
            printf("Tocno!\n");
            rez.bodovi++;
        }
        else {
            printf("Netocno! Tocan odgovor je: %s\n", pitanja[i].tocanOdgovor);
        }
    }

    printf("\n--- REZULTAT ---\n");
    printf("Bodovi: %d/%d\n", rez.bodovi, brojUcitanih);

    printf("Unesite svoje ime: ");
    fgets(rez.ime, sizeof(rez.ime), stdin); // 14.
    rez.ime[strcspn(rez.ime, "\n")] = '\0';

    zapisiRezultat(rez);                    // 1.

    sigurnoOslobodi((void**)&pitanja);      // 18.
}



// 20. fseek/ftell/rewind; 23. qsort; 24. search; 22. errno, perror
int procitajSortirajIPrikaziRezultate(void) {

    FILE* f = fopen("rezultati.txt", "r");

    if (f == NULL) {
        printf("Nema rezultata za prikazati ili datoteka ne postoji.\n");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long velicina = ftell(f);

    if (velicina == 0) {
        printf("Datoteka je prazna.\n");
        fclose(f);
        return 0;
    }

    rewind(f);

    rezultat rezultati[MAX_REZULTATA];
    int brRez = 0;

    char linija[512];

    while (fgets(linija, sizeof(linija), f) != NULL && brRez < MAX_REZULTATA) {
        char ime[100];
        char tezina[20];
        int bodovi;

        // 1. Ovaj format odgovara retcima:
        if (sscanf(linija,
            "Ime: %99[^|] | Bodovi: %d/%*d | Tezina: %19[^|\n]",
            ime, &bodovi, tezina) == 3) {

            // 14. Ukloni razmake na kraju imena
            size_t len = strlen(ime);
            while (len > 0 && ime[len - 1] == ' ')
                ime[--len] = '\0';

            strncpy(rezultati[brRez].ime, ime, sizeof(rezultati[brRez].ime) - 1);
            rezultati[brRez].ime[sizeof(rezultati[brRez].ime) - 1] = '\0';

            rezultati[brRez].bodovi = bodovi;

            strncpy(rezultati[brRez].tezina, tezina, sizeof(rezultati[brRez].tezina) - 1);
            rezultati[brRez].tezina[sizeof(rezultati[brRez].tezina) - 1] = '\0';

            brRez++;
        }
    }
    fclose(f);

    sortirajRezultate(rezultati, brRez);

    printf("\nSVI REZULTATI (sortirano po bodovima)\n");
    printf("__________________________________________________\n");

    for (int i = 0; i < brRez; i++) {
        printf("Ime: %-20s | Bodovi: %d | Tezina: %s\n", rezultati[i].ime, rezultati[i].bodovi, rezultati[i].tezina);
    }

    printf("__________________________________________________\n");

    // 24. Pretraživanje po imenu
    char ime[100];

    printf("Unesite ime za pretrazivanje (ili prazno za izlaz): ");

    if (fgets(ime, sizeof(ime), stdin) != NULL) {
        ime[strcspn(ime, "\n")] = '\0';
        if (ime[0] != '\0') {
            rezultat* r = pronadiRezultatPoImenu(rezultati, brRez, ime);
            if (r) {
                printf("Pronadeno: %s | Bodovi: %d | Tezina: %s\n", r->ime, r->bodovi, r->tezina);
            }
            else {
                printf("Nema rezultata za ime: %s\n", ime);
            }
        }
    }

    return 1;
}
