#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(void) {
    int izbor = -1;

    do {
        printf("________________________________________________\n");
        printf("              NOGOMETNI KVIZ 2026               \n");
        printf("________________________________________________\n");
        printf(" [1] Igraj\n");
        printf(" [2] Pogledaj rezultate\n");
        printf(" [3] Datoteke\n");
        printf(" [0] Kraj\n");
        printf("\nOdabir: ");

        if (scanf("%d", &izbor) != 1) {
            int znak;

            while ((znak = getchar()) != '\n' && znak != EOF) {}
            printf("Unesite broj!\n");
            izbor = -1;
            continue;
        }

        while (getchar() != '\n') {}

        switch (izbor) {
        case MENU_IGRAJ:
        {
            int tezina = -1;

            do {
                printf("\n--- ODABIR TEZINE KVIZA ---\n");
                printf(" [1] Lagano\n");
                printf(" [2] Srednje\n");
                printf(" [3] Tesko\n");
                printf("\n");
                printf(" [0] Nazad\n");
                printf("\n");
                printf("Odabir: ");

                if (scanf("%d", &tezina) != 1) {
                    int znak;

                    while ((znak = getchar()) != '\n' && znak != EOF) {}
                    printf("Unesite broj!\n");
                    tezina = -1;
                    continue;
                }

                while (getchar() != '\n') {}

                switch (tezina) {
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

            } while (tezina != 0);

            break;
        }

        case MENU_REZULTATI:
        {
            procitajSortirajIPrikaziRezultate();

            break;
        }

        case MENU_DATOTEKE:
        {
            int podizbor = -1;

            do {
                printf("\n--- MENI ZA DATOTEKE ---\n");
                printf(" [1] Obrisi rezultati.txt\n");
                printf(" [2] Napravi backup rezultati.txt\n");
                printf(" [0] Nazad\n");
                printf("Odabir: ");

                if (scanf("%d", &podizbor) != 1) {
                    int znak;

                    while ((znak = getchar()) != '\n' && znak != EOF) {}
                    printf("Unesite broj!\n");
                    podizbor = -1;
                    continue;
                }

                while (getchar() != '\n') {}

                switch (podizbor) {
                case 1:
                {
                    // 21. JEDINO ovdje se briše rezultati.txt
                    if (remove("rezultati.txt") == 0) {
                        printf("Datoteka rezultati.txt je obrisana.\n");
                    }

                    else {
                        printf("Datoteku rezultati.txt nije bilo moguce obrisati.\n");
                    }

                    break;
                }

                case 2:
                {
                    // 1. Obriši stari backup
                    int brisanjeStari;

                    brisanjeStari = remove("backup.txt");

                    if (brisanjeStari == 0) {
                        printf("Stari backup.txt je obrisao.\n");
                    }

                    // 2. Kopiraj rezultati.txt -> backup.txt bez brisanja izvora
                    FILE* src = fopen("rezultati.txt", "r");

                    if (src == NULL) {
                        printf("rezultati.txt ne postoji ili je prazan.\n");
                        break;
                    }

                    FILE* dst = fopen("backup.txt", "w");

                    if (dst == NULL) {
                        printf("Ne mogu otvoriti backup.txt.\n");
                        fclose(src);
                        break;
                    }

                    char c;

                    while ((c = fgetc(src)) != EOF)
                        fputc(c, dst);

                    fclose(src);
                    fclose(dst);

                    printf("Backup rezultati.txt -> backup.txt napravljen.\n");
                    break;
                }

                case 0:

                    break;

                default:

                    printf("Nepostojeca opcija.\n");

                    break;
                }

            } while (podizbor != 0);

            break;
        }

        case MENU_IZLAZ:

            printf("Izlaz iz kviza.\n");

            break;

        default:

            printf("Opcija ne postoji.\n");

            break;
        }

    } while (izbor != 0);

    return 0;
}