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

// 25. Rekurzivno binarno pretrazivanje po bodovima
static int pronadiRezultatPoBodovima(rezultat* nizRezultata, int lijevo, int desno, int ciljBodovi) { // Trazi rezultat po bodovima
    if (lijevo > desno) return -1; // Ako nema više elemenata, rezultat nije pronađen

    int sredina;

    sredina = lijevo + (desno - lijevo) / 2; // Racuna srednji indeks

    if (nizRezultata[sredina].bodovi == ciljBodovi) // Ako je broj bodova pogoden
        return sredina; // Vraca indeks

    if (nizRezultata[sredina].bodovi < ciljBodovi) // Ako je srednji rezultat manji od cilja
        return pronadiRezultatPoBodovima(nizRezultata, sredina + 1, desno, ciljBodovi); // 25. Trazi u desnoj polovici

    else // Ako je srednji rezultat veci od cilja
        return pronadiRezultatPoBodovima(nizRezultata, lijevo, sredina - 1, ciljBodovi); // 25. Trazi u lijevoj polovici
}

// 26. Komparator za qsort po bodovima
static int usporediRezultate(const void* prvi, const void* drugi) { // Funkcija za usporedbu dva rezultata

    const rezultat* rezultatPrvi = (const rezultat*)prvi; // 26. Pretvara prvi pokazivac u rezultat
    const rezultat* rezultatDrugi = (const rezultat*)drugi; // 26. Pretvara drugi pokazivac u rezultat

    return (rezultatDrugi->bodovi - rezultatPrvi->bodovi); // 23. Sortira od veceg prema manjem broju bodova
}

// 23. Sortiranje rezultata
static void sortirajRezultate(rezultat* nizRezultata, int brojRezultata) { // Sortira polje rezultata

    if (nizRezultata == NULL || brojRezultata <= 0) return; // 14. Ako nema podataka = izlaz

    qsort(nizRezultata, brojRezultata, sizeof(rezultat), usporediRezultate); // 23. Sortiranje pomocu qsort
}

// 23. Pretrazivanje po imenu
static rezultat* pronadiRezultatPoImenu(rezultat* nizRezultata, int brojRezultata, const char* trazenoIme) { // Trazi rezultat po imenu

    int indeks; // Brojac petlje

    if (nizRezultata == NULL || brojRezultata <= 0 || trazenoIme == NULL) return NULL; // 14. Provjera valjanosti ulaza

    for (indeks = 0; indeks < brojRezultata; indeks++) { // Prolazi kroz sve rezultate
        if (strcmp(nizRezultata[indeks].ime, trazenoIme) == 0) // 24. Usporeduje imena
            return &nizRezultata[indeks]; // Vraca adresu pronadenog rezultata
    }

    return NULL; // Ako nije pronadeno, vraca NULL
}

// 9. Makro funkcija: maksimum
#define MAX(a, b) ((a) > (b) ? (a) : (b)) // Vraca veci od dva broja

// 14. Ciscenje stringa
static void ocistiString(char* string, size_t velicinaStringa) { // Postavlja sve znakove stringa na nulu
    if (string == NULL || velicinaStringa == 0) return; // 14. Ako je string neispravan = izlaz

    for (size_t indeks = 0; indeks < velicinaStringa; indeks++) { // Prolazi kroz sve znakove
        string[indeks] = '\0'; // Brise znak
    }
}

// 18. Sigurno oslobadanje memorije
static void sigurnoOslobodi(void** pokazivac) { // Oslobada memoriju i postavlja pokazivac na NULL

    if (pokazivac && *pokazivac) { // 18. Provjerava pokazivac
        free(*pokazivac); // 17. Oslobada memoriju
        *pokazivac = NULL; // 18. Postavlja pokazivac na NULL
    }
}

// 1. Ucitavanje pitanja iz datoteke
int ucitajPitanja(const char* nazivDatoteke, pitanje* nizPitanja) { // Cita pitanja iz datoteke

    FILE* datoteka = fopen(nazivDatoteke, "r"); // 1. Otvara datoteku za citanje

    if (datoteka == NULL) {
        printf("Greska s datotekom");
        return 0;
    }

    int indeksPitanja = 0; // Indeks trenutno ucitanog pitanja
    char redak[1024]; // Buffer for jedan redak datoteke

    while (fgets(redak, sizeof(redak), datoteka) != NULL && indeksPitanja < MAX_PITANJA) { // Cita redak po redak

        char* token = strtok(redak, ";\r\n"); // Dijeli redak po separatorima

        if (token == NULL) continue; // Ako nema teksta, preskace redak

        strcpy(nizPitanja[indeksPitanja].tekst, token); // 14. Kopira tekst pitanja

        int indeksOpcije; // Brojac za opcije

        for (indeksOpcije = 0; indeksOpcije < 4; indeksOpcije++) { // Ucitava cetiri opcije odgovora
            token = strtok(NULL, ";\r\n"); // Uzimanje sljedeceg tokena

            if (token == NULL) break; // Ako token ne postoji, prekida se program

            strcpy(nizPitanja[indeksPitanja].opcije[indeksOpcije], token); // 14. Kopira opcijuc
        }

        token = strtok(NULL, ";\r\n"); // Ucitava tocan odgovor

        if (token != NULL && token[0] != '\0') { // Ako postoji odgovor
            nizPitanja[indeksPitanja].tocanOdgovor[0] = (char)toupper((unsigned char)token[0]); // Pretvara u veliko slovo
            nizPitanja[indeksPitanja].tocanOdgovor[1] = '\0'; // Zavrsava string
        }

        else { // Ako nema odgovora
            nizPitanja[indeksPitanja].tocanOdgovor[0] = '\0'; // Postavlja prazan odgovor
        }

        indeksPitanja++; // Prelazak na sljedece pitanje
    }

    fclose(datoteka); // 19. Zatvara datoteku
    return indeksPitanja; // Vraca broj ucitanih pitanja
}

// 14. Prikaz pojedinacnog pitanja
void prikaziPitanje(pitanje jednoPitanje, int redniBrojPitanja) { // Ispisuje pitanje na ekran

    printf("\n------------------ %d. -------------------\n", redniBrojPitanja); // Ispis broja pitanja
    printf("%s\n", jednoPitanje.tekst); // Ispis teksta pitanja
    printf("A) %-20s B) %-20s\n", jednoPitanje.opcije[0], jednoPitanje.opcije[1]); // Ispis prve dvije opcije
    printf("C) %-20s D) %-20s\n", jednoPitanje.opcije[2], jednoPitanje.opcije[3]); // Ispis druge dvije opcije
    printf("-------------------------------------------\n"); // Crta ispod pitanja
    printf("Unesite odgovor (A/B/C/D): "); // Trazi unos odgovora
}

// 14. Provjera tocnosti odgovora
int provjeriOdgovor(pitanje jednoPitanje, char uneseniOdgovor) { // Provjerava odgovor
    char tocanOdgovor; // Tocan odgovor
    char odgovorKorisnika; // Korisnikov odgovor

    tocanOdgovor = (char)toupper((unsigned char)jednoPitanje.tocanOdgovor[0]); // Pretvara tocan odgovor u veliko slovo
    odgovorKorisnika = (char)toupper((unsigned char)uneseniOdgovor); // Pretvara korisnikov odgovor u veliko slovo

    return tocanOdgovor == odgovorKorisnika; // Vraca 1 ako je odgovor tocan
}

// 1. Upis rezultata
void zapisiRezultat(rezultat jedanRezultat) { // Sprema rezultat u datoteku
    FILE* datoteka = fopen("rezultati.txt", "a"); // 1. Otvara datoteku rezultata

    if (datoteka == NULL) {
        perror("fopen rezultati.txt"); // 22.
        return;
    }

    // 1. 
    fprintf(datoteka, "Ime: %-20s | Bodovi: %d/%d | Tezina: %s\n", jedanRezultat.ime, jedanRezultat.bodovi, MAX_PITANJA, jedanRezultat.tezina); // Upisuje rezultat u datoteku

    fclose(datoteka); // 19. Zatvara datoteku

    globalnaStatistika++; // 8. Povecava globalnu statistiku

    printf("\nRezultat uspjesno spremljen!\n");
}

// 1. 16. 18. Pokretanje igre / alokacija
void pokreniIgru(const char* nazivDatoteke, const char* nazivTezine) { // Pokrece kviz za odabranu tezinu

    pitanje* nizPitanja = calloc(MAX_PITANJA, sizeof(pitanje)); // 16, 17. Dinamicki alocira memoriju za pitanja

    if (nizPitanja == NULL) {
        perror("calloc pitanja"); // 22.
        return;
    }

    int brojUcitanihPitanja; // Broj uspjesno ucitanih pitanja

    brojUcitanihPitanja = ucitajPitanja(nazivDatoteke, nizPitanja); // Ucitava pitanja iz datoteke

    if (brojUcitanihPitanja == 0) { // Ako nema pitanja
        printf("Datoteka %s ne postoji ili je prazna.\n", nazivDatoteke); // Ispis poruke
        sigurnoOslobodi((void**)&nizPitanja); // 18. Oslobada memoriju
        return; // Prekid funkcije
    }

    rezultat osvojeniRezultat; // Struktura za rezultat igre
    ocistiString(osvojeniRezultat.ime, sizeof(osvojeniRezultat.ime)); // 14. Brise ime
    osvojeniRezultat.bodovi = 0; // Postavlja pocetne bodove

    if (nazivTezine) { // Ako je tezina zadana
        strncpy(osvojeniRezultat.tezina, nazivTezine, sizeof(osvojeniRezultat.tezina) - 1); // 14. Kopira tekst tezine
        osvojeniRezultat.tezina[sizeof(osvojeniRezultat.tezina) - 1] = '\0'; // Zavrsava string
    }

    int indeksPitanja; // Brojac pitanja

    for (indeksPitanja = 0; indeksPitanja < brojUcitanihPitanja; indeksPitanja++) { // Petlja kroz sva pitanja
        char uneseniOdgovor; // Korisnikov odgovor
        prikaziPitanje(nizPitanja[indeksPitanja], indeksPitanja + 1); // Ispisuje pitanje

        while (1) { // Petlja za ispravan unos odgovora
            if (scanf(" %c", &uneseniOdgovor) != 1) { // Ako unos nije znak
                int znakZaCiscenje; // Znak za ciscenje buffera

                while ((znakZaCiscenje = getchar()) != '\n' && znakZaCiscenje != EOF) {} // Cisti buffer
                printf("Morate unijeti slovo (A/B/C/D)!\n");

                continue; // Ponavlja unos
            }

            while (getchar() != '\n') {} // Cisti ostatak retka

            if ((uneseniOdgovor == 'A' || uneseniOdgovor == 'a' ||
                uneseniOdgovor == 'B' || uneseniOdgovor == 'b' ||
                uneseniOdgovor == 'C' || uneseniOdgovor == 'c' ||
                uneseniOdgovor == 'D' || uneseniOdgovor == 'd')) { // Ako je odgovor validan
                break; // Izlazi iz petlje
            }
            else { // Ako odgovor nije validan
                printf("Morate unijeti slovo A, B, C ili D!\n");
            }
        }

        if (provjeriOdgovor(nizPitanja[indeksPitanja], uneseniOdgovor)) { // Provjerava odgovor
            printf("Tocno!\n"); // Ispis tocnog odgovora
            osvojeniRezultat.bodovi++; // Povecava broj bodova
        }
        else { // Ako odgovor nije tocan
            printf("Netocno! Tocan odgovor je: %s\n", nizPitanja[indeksPitanja].tocanOdgovor); // Ispis tocnog odgovora
        }
    }

    printf("\n--- REZULTAT ---\n");
    printf("Bodovi: %d/%d\n", osvojeniRezultat.bodovi, brojUcitanihPitanja); // Ispis osvojenih bodova

    printf("Unesite svoje ime: "); // Trazenje imena korisnika
    fgets(osvojeniRezultat.ime, sizeof(osvojeniRezultat.ime), stdin); // 14. Ucitava ime.
    osvojeniRezultat.ime[strcspn(osvojeniRezultat.ime, "\n")] = '\0'; // Uklanja novi red

    zapisiRezultat(osvojeniRezultat); // 1. Sprema rezultat

    sigurnoOslobodi((void**)&nizPitanja); // 18. Oslobada memoriju
}

// 22. 23. 24. 22. Citanje, sortiranje i prikaz rezultata
int procitajSortirajIPrikaziRezultate(void) { // Ucitava i prikazuje rezultate

    FILE* datoteka = fopen("rezultati.txt", "r"); // Otvara datoteku rezultata

    if (datoteka == NULL) {
        printf("Nema rezultata za prikazati ili datoteka ne postoji.\n");
        return 0;
    }

    fseek(datoteka, 0, SEEK_END); // Pomak na kraj datoteke
    long velicinaDatoteke = ftell(datoteka); // Dohvaca velicinu datoteke

    if (velicinaDatoteke == 0) {
        printf("Datoteka je prazna.\n");
        fclose(datoteka);
        return 0;
    }

    rewind(datoteka); // Vraca pokazivac na pocetak datoteke

    rezultat nizRezultata[MAX_REZULTATA]; // Polje za rezultate
    int brojRezultata = 0; // Broj ucitanih rezultata

    char redak[512]; // Buffer za jedan redak

    while (fgets(redak, sizeof(redak), datoteka) != NULL && brojRezultata < MAX_REZULTATA) { // Cita redak po redak
        char imeIgraca[100]; // Buffer za ime igraca
        char razinaTezine[20]; // Buffer za tezinu
        int osvojeniBodovi; // Broj bodova

        if (sscanf(redak, "Ime: %99[^|] | Bodovi: %d/%*d | Tezina: %19[^|\n]", imeIgraca, &osvojeniBodovi, razinaTezine) == 3) { // Parsira redak
            // 14.
            size_t duljinaImena = strlen(imeIgraca); // Duljina imena

            while (duljinaImena > 0 && imeIgraca[duljinaImena - 1] == ' ') // Uklanja razmake na kraju imena
                imeIgraca[--duljinaImena] = '\0'; // Brise zadnji razmak

            strncpy(nizRezultata[brojRezultata].ime, imeIgraca, sizeof(nizRezultata[brojRezultata].ime) - 1); // Kopira ime
            nizRezultata[brojRezultata].ime[sizeof(nizRezultata[brojRezultata].ime) - 1] = '\0'; // Zavrsava string

            nizRezultata[brojRezultata].bodovi = osvojeniBodovi; // Sprema bodove

            strncpy(nizRezultata[brojRezultata].tezina, razinaTezine, sizeof(nizRezultata[brojRezultata].tezina) - 1); // Kopira tezinu
            nizRezultata[brojRezultata].tezina[sizeof(nizRezultata[brojRezultata].tezina) - 1] = '\0'; // Zavrsava string

            brojRezultata++; // Povecava broj rezultata
        }
    }

    fclose(datoteka); // Zatvara datoteku

    sortirajRezultate(nizRezultata, brojRezultata); // Sortira rezultate

    printf("\nSvi Rezultati (sortirano po bodovima)\n");
    printf("__________________________________________________\n");

    for (int indeks = 0; indeks < brojRezultata; indeks++) { // Ispis svih rezultata
        printf("Ime: %-20s | Bodovi: %d | Tezina: %s\n", nizRezultata[indeks].ime, nizRezultata[indeks].bodovi, nizRezultata[indeks].tezina);
    }

    printf("__________________________________________________\n");

    char imeZaPretragu[100]; // 24. Buffer za unos imena za pretragu

    printf("Unesite ime za pretrazivanje (ili prazno za izlaz): "); // Trazenje unosa

    if (fgets(imeZaPretragu, sizeof(imeZaPretragu), stdin) != NULL) { // Ucitava ime
        imeZaPretragu[strcspn(imeZaPretragu, "\n")] = '\0'; // Uklanja novi red
        if (imeZaPretragu[0] != '\0') { // Ako korisnik nije unio prazno
            rezultat* pronadeniRezultat = pronadiRezultatPoImenu(nizRezultata, brojRezultata, imeZaPretragu); // Trazi rezultat
            if (pronadeniRezultat) { // Ako je pronaden.
                printf("Pronadeno: %s | Bodovi: %d | Tezina: %s\n", pronadeniRezultat->ime, pronadeniRezultat->bodovi, pronadeniRezultat->tezina); // Ispis pronadenog
            }
            else { // Ako nije pronaden
                printf("Nema rezultata za ime: %s\n", imeZaPretragu); // Poruka da nema rezultata
            }
        }
    }

    return 1;
}


// 1. UPDATE DELETE
void azurirajImeIgraca(const char* staroIme, const char* novoIme) {
    FILE* datoteka = fopen("rezultati.txt", "r");

    if (datoteka == NULL) {
        printf("Datoteka rezultati.txt ne postoji.\n");
        return;
    }

    rezultat privremeniNiz[MAX_REZULTATA]; // Kreira lokalni privremeni niz struktura u koji ce se ucitati podaci

    int brojac = 0;
    char redak[512];
    int izmijenjeno = 0;

    while (fgets(redak, sizeof(redak), datoteka) != NULL && brojac < MAX_REZULTATA) { // Cita liniju po liniju datoteke unutar granica kapaciteta niza
        char imeIgraca[100]; // Buffer za pohranu imena izdvojenog iz retka
        char razinaTezine[20]; // Buffer za pohranu tezine izdvojene iz retka
        int osvojeniBodovi; // Varijabla za pohranu bodova izdvojenih iz retka

        if (sscanf(redak, "Ime: %99[^|] | Bodovi: %d/%*d | Tezina: %19[^|\n]", imeIgraca, &osvojeniBodovi, razinaTezine) == 3) {
            size_t duljina = strlen(imeIgraca); // Racuna duljinu ucitanog imena igraca

            while (duljina > 0 && imeIgraca[duljina - 1] == ' ') // Petlja za uklanjanje razmaka s kraja imena igraca
                imeIgraca[--duljina] = '\0'; // Skracuje string postavljanjem nul-potomka na mjesto razmaka

            if (strcmp(imeIgraca, staroIme) == 0) { // Provjerava je li ucitano ime upravo ono koje se trazi za izmjenu
                strncpy(privremeniNiz[brojac].ime, novoIme, sizeof(privremeniNiz[brojac].ime) - 1); // Kopira novo zadano ime u privremenu strukturu
                privremeniNiz[brojac].ime[sizeof(privremeniNiz[brojac].ime) - 1] = '\0'; // Osigurava zavrsetak stringa s nul-potomkom
                izmijenjeno = 1; // Postavlja zastavicu na istinu (1) jer je napravljena modifikacija zapisa
            }
            else { // Ako ucitano ime ne odgovara traženom imenu za izmjenu
                strncpy(privremeniNiz[brojac].ime, imeIgraca, sizeof(privremeniNiz[brojac].ime) - 1); // Prepisuje nepromijenjeno ime u strukturu niza
                privremeniNiz[brojac].ime[sizeof(privremeniNiz[brojac].ime) - 1] = '\0'; // Osigurava ispravan zavrsetak tekstualnog niza
            }

            privremeniNiz[brojac].bodovi = osvojeniBodovi; // Prepisuje bodove u trenutni element privremenog niza struktura
            strncpy(privremeniNiz[brojac].tezina, razinaTezine, sizeof(privremeniNiz[brojac].tezina) - 1); // Prepisuje razinu tezine u strukturu
            privremeniNiz[brojac].tezina[sizeof(privremeniNiz[brojac].tezina) - 1] = '\0'; // Osigurava zavrsetak niza znakova tezine
            brojac++; // Povecava brojac za pomak na iduce polje u privremenom nizu struktura
        }
    }

    fclose(datoteka);

    if (izmijenjeno) { // Ako je barem jedan zapis modificiran
        datoteka = fopen("rezultati.txt", "w");

        if (datoteka == NULL) {
            perror("fopen rezultati.txt");
            return;
        }

        for (int i = 0; i < brojac; i++) { // Prolazi kroz sve sakupljene elemente u memorijskom privremenom nizu
            fprintf(datoteka, "Ime: %-20s | Bodovi: %d/%d | Tezina: %s\n", privremeniNiz[i].ime, privremeniNiz[i].bodovi, MAX_PITANJA, privremeniNiz[i].tezina); // Upisuje novi redak
        }

        fclose(datoteka);
        printf("Zapis je uspjesno azuriran (UPDATE).\n");
    }
    else { // Ako niti jedan zapis nije odgovarao trazenom starom imenu
        printf("Igrac s imenom '%s' nije pronaden.\n", staroIme);
    }
}

void obrisiRezultatIgraca(const char* imeZaBrisanje) {
    FILE* datoteka = fopen("rezultati.txt", "r");

    if (datoteka == NULL) {
        printf("Datoteka rezultati.txt ne postoji.\n");
        return;
    }

    rezultat privremeniNiz[MAX_REZULTATA];

    int brojac = 0;
    char redak[512];
    int pronaden = 0;

    while (fgets(redak, sizeof(redak), datoteka) != NULL && brojac < MAX_REZULTATA) { // Cita retke do kraja datoteke i puni memorijski buffer
        char imeIgraca[100]; // Kreira buffer za privremeni upis procitanog imena
        char razinaTezine[20]; // Kreira buffer za privremeni upis pročitane tezine
        int osvojeniBodovi; // Definira varijablu za privremeni upis procitanih bodova

        if (sscanf(redak, "Ime: %99[^|] | Bodovi: %d/%*d | Tezina: %19[^|\n]", imeIgraca, &osvojeniBodovi, razinaTezine) == 3) {
            size_t duljina = strlen(imeIgraca); // Dohvaca trenutnu duljinu procitanog tekstualnog niza imena

            while (duljina > 0 && imeIgraca[duljina - 1] == ' ') // Uklanja prazne razmake na zavrsetku stringa imena
                imeIgraca[--duljina] = '\0'; // Krati tekstualni niz dodavanjem nul-znaka na kraj

            if (strcmp(imeIgraca, imeZaBrisanje) == 0) { // Provjerava je li ucitano ime upravo ono koje se želi izbrisati
                pronaden = 1;
                continue; // Preskace preostali dio petlje i ide na sljedeci redak
            }

            strncpy(privremeniNiz[brojac].ime, imeIgraca, sizeof(privremeniNiz[brojac].ime) - 1); // Kopira sacuvano ime u strukturu unutar privremenog polja
            privremeniNiz[brojac].ime[sizeof(privremeniNiz[brojac].ime) - 1] = '\0'; // Osigurava postojanje nul-potomka na kraju imena
            privremeniNiz[brojac].bodovi = osvojeniBodovi; // Kopira bodove igraca u strukturu elementa niza

            strncpy(privremeniNiz[brojac].tezina, razinaTezine, sizeof(privremeniNiz[brojac].tezina) - 1); // Kopira tezinu kviza u element strukture niza
            privremeniNiz[brojac].tezina[sizeof(privremeniNiz[brojac].tezina) - 1] = '\0'; // Postavlja zavrsni nul-znak na string razine tezine
            brojac++; // Inkrementira brojac sacuvanih zapisa i pomice indeks u privremenom polju
        }
    }

    fclose(datoteka);

    if (pronaden) { // Ako je trazeni zapis pronaden i uspjesno izbacen iz novog niza
        datoteka = fopen("rezultati.txt", "w");

        if (datoteka == NULL) {
            perror("fopen rezultati.txt");
            return;
        }

        for (int i = 0; i < brojac; i++) { // Prolazi kroz sve sacuvane filtrirane zapise
            fprintf(datoteka, "Ime: %-20s | Bodovi: %d/%d | Tezina: %s\n", privremeniNiz[i].ime, privremeniNiz[i].bodovi, MAX_PITANJA, privremeniNiz[i].tezina); // Zapisuje redak natrag u datoteku
        }

        fclose(datoteka);
        printf("Zapis za igraca '%s' je obrisan.\n", imeZaBrisanje);
    }
    else { // Ako se ime niti jednog igraca nije poklapalo sa zadanim parametrom brisanja
        printf("Igrac s imenom '%s' nije pronaden u datoteci.\n", imeZaBrisanje);
    }
}