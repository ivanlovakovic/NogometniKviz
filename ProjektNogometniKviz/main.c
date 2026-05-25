#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(void) {
    int odabirGlavniMeni = -1;

    do {
        printf("________________________________________________\n");
        printf("\n");
        printf("                NOGOMETNI KVIZ                  \n");
        printf("________________________________________________\n");
        printf(" [1] Igraj\n");
        printf(" [2] Pogledaj rezultate\n");
        printf(" [3] Datoteke\n");
        printf("\n");
        printf(" [0] Kraj\n");
        printf("________________________________________________\n");
        printf("\nOdabir: ");

        if (scanf("%d", &odabirGlavniMeni) != 1) { // Provjerava je li korisnik unio broj
            int uneseniZnak;

            while ((uneseniZnak = getchar()) != '\n' && uneseniZnak != EOF) {} // Brise ostatak pogresnog unosa
            printf("Unesite broj!\n"); // Ispis poruke o pogrešnom unosu
            odabirGlavniMeni = -1; // Resetira odabir
            continue; // Vraca se na pocetak petlje
        }

        while (getchar() != '\n') {} // Briše visak znakova nakon unosa broja

        switch (odabirGlavniMeni) { // Odabir opcije iz glavnog izbornika
        case MENU_IGRAJ:
        {
            int odabirTezine = -1;

            do {
                printf("\n--- ODABIR TEZINE KVIZA ---\n");
                printf(" [1] Lagano\n");
                printf(" [2] Srednje\n");
                printf(" [3] Tesko\n");
                printf("\n");
                printf(" [0] Nazad\n");
                printf("\n");
                printf("Odabir: ");

                if (scanf("%d", &odabirTezine) != 1) { // Provjera ispravnosti unosa
                    int uneseniZnak; // Privremena varijabla za ciscenje buffera

                    while ((uneseniZnak = getchar()) != '\n' && uneseniZnak != EOF) {} // Ciscenje buffera
                    printf("Unesite broj!\n"); // Poruka za pogresan unos
                    odabirTezine = -1; // Reset odabira tezine
                    continue; // Povratak na pocetak petlje
                }

                while (getchar() != '\n') {} // Ciscenje ostatka unosa

                switch (odabirTezine) {
                case 1:
                    pokreniIgru("lagano.txt", "Lagano");
                    break;
                case 2:
                    pokreniIgru("srednje.txt", "Srednje");
                    break;
                case 3:
                    pokreniIgru("tesko.txt", "Tesko");
                    break;
                case 0:
                    break;
                default:
                    printf("Nepostojeca opcija.\n");
                    break;
                }

            } while (odabirTezine != 0); // Ponavlja dok korisnik ne odabere 0

            break;
        }

        case MENU_REZULTATI:
        {
            procitajSortirajIPrikaziRezultate();

            break;
        }

        case MENU_DATOTEKE:
        {
            int odabirDatoteke = -1;

            do {
                printf("\n--- MENI ZA DATOTEKE ---\n");
                printf(" [1] Obrisi sve rezultate\n");
                printf(" [2] Napravi backup svih rezultata\n");
                printf(" [3] Azuriraj ime igraca\n");
                printf(" [4] Obrisi rezultat igraca\n");
                printf(" [0] Nazad\n");
                printf("Odabir: ");

                if (scanf("%d", &odabirDatoteke) != 1) { // Provjera unosa
                    int uneseniZnak; // Privremena varijabla za ciscenje buffera

                    while ((uneseniZnak = getchar()) != '\n' && uneseniZnak != EOF) {} // Ciscenje buffera
                    printf("Unesite broj!\n"); // Poruka o gresci
                    odabirDatoteke = -1; // Reset odabira
                    continue; // Povratak na pocetak petlje
                }

                while (getchar() != '\n') {} // Ciscenje preostalog unosa

                switch (odabirDatoteke) {
                case 1: // Brisanje datoteke rezultati.txt
                {
                    if (remove("rezultati.txt") == 0) { // 21. briše rezultati.txt
                        printf("Datoteka rezultati.txt je obrisana.\n");
                    }
                    else {
                        printf("Datoteku rezultati.txt nije bilo moguce obrisati.\n");
                    }

                    break;
                }

                case 2: // Izrada backupa
                {
                    int rezultatBrisanjaBackup; // Povratna vrijednost brisanja stare backup datoteke

                    rezultatBrisanjaBackup = remove("backup.txt"); // 1. Obriši stari backup

                    if (rezultatBrisanjaBackup == 0) {
                        printf("Stari backup.txt je obrisan.\n");
                    }

                    FILE* datotekaIzvor = fopen("rezultati.txt", "r"); // Otvara datoteku rezultata (citanje)

                    if (datotekaIzvor == NULL) {
                        printf("rezultati.txt ne postoji ili je prazan.\n");
                        break;
                    }

                    FILE* datotekaOdrediste = fopen("backup.txt", "w"); // Otvara backup datoteku (pisanje)

                    if (datotekaOdrediste == NULL) {
                        printf("Ne mogu otvoriti backup.txt.\n");
                        fclose(datotekaIzvor);
                        break;
                    }

                    char trenutniZnak; // Varijabla za kopiranje (znak po znak)

                    while ((trenutniZnak = fgetc(datotekaIzvor)) != EOF) // Cita znakove iz rezultati.txt
                        fputc(trenutniZnak, datotekaOdrediste); // Upisuje znakove u backup.txt

                    fclose(datotekaIzvor);
                    fclose(datotekaOdrediste);

                    printf("Backup rezultati.txt -> backup.txt napravljen.\n");
                    break;
                }

                case 3:
                {
                    char staroIme[100];
                    char novoIme[100];

                    printf("Unesite ime igraca koje zelite promijeniti : ");
                    fgets(staroIme, sizeof(staroIme), stdin); // Sigurno ucitavanje starog imena s tipkovnice
                    staroIme[strcspn(staroIme, "\n")] = '\0'; // Uklanjanje znaka novog reda iz unesenog niza znakova

                    printf("Unesite novo ime za tog igraca: "); // Ispis poruke za unos novog imena
                    fgets(novoIme, sizeof(novoIme), stdin); // Sigurno ucitavanje novog imena s tipkovnice
                    novoIme[strcspn(novoIme, "\n")] = '\0'; // Uklanjanje znaka novog reda iz novog imena

                    if (staroIme[0] != '\0' && novoIme[0] != '\0') { // Provjera jesu li oba unesena imena valjana i nisu prazna
                        azurirajImeIgraca(staroIme, novoIme); // Poziv funkcije koja obavlja update operaciju u datoteci
                    }
                    else { // Ako je barem jedan unos ostao prazan string
                        printf("Imena ne smiju biti prazna!\n"); // Ispis poruke upozorenja korisniku
                    }
                    break; // Izlazak iz case 3 bloka
                }

                case 4:
                {
                    char imeZaBrisanje[100];

                    printf("Unesite ime igraca ciji rezultat zelite obrisati: "); // Ispis poruke za unos imena za brisanje
                    fgets(imeZaBrisanje, sizeof(imeZaBrisanje), stdin); // Sigurno ucitavanje imena igraca pomocu fgets
                    imeZaBrisanje[strcspn(imeZaBrisanje, "\n")] = '\0'; // Uklanjanje znaka za novi red sa zavrsetka stringa

                    if (imeZaBrisanje[0] != '\0') { // Provjera je li korisnik unio tekst prije pritiska tipke Enter
                        obrisiRezultatIgraca(imeZaBrisanje); // Poziv funkcije koja brise taj specificni zapis iz datoteke
                    }
                    else { // Ako je korisnik proslijedio prazan unos
                        printf("Ime ne smije biti prazno!\n"); // Ispis poruke upozorenja o praznom unosu
                    }
                    break; // Izlazak iz case 4 bloka
                }

                case 0: // Povratak
                    break;

                default: // Neispravan unos
                    printf("Nepostojeca opcija.\n");
                    break;
                }

            } while (odabirDatoteke != 0); // Ponavlja dok korisnik ne odabere 0

            break;
        }

        case MENU_IZLAZ:
            printf("Izlaz iz kviza.\n");
            break;

        default:
            printf("Opcija ne postoji.\n");
            break;
        }

    } while (odabirGlavniMeni != 0); // Petlja radi dok korisnik ne izabere izlaz

    return 0;
}