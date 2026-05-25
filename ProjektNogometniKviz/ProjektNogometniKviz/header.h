#ifndef HEADER_H
#define HEADER_H

#include "kviz.h"

int ucitajPitanja(const char* nazivDatoteke, pitanje* nizPitanja); // 1, 12

void prikaziPitanje(pitanje jednoPitanje, int redniBroj); // 14

int provjeriOdgovor(pitanje jednoPitanje, char uneseniOdgovor); // 14

void zapisiRezultat(rezultat jedanRezultat); // 1, 12, 14

void pokreniIgru(const char* nazivDatoteke, const char* nazivTezine); // 1, 14

int procitajSortirajIPrikaziRezultate(void); // 20, 22, 23, 24

void azurirajImeIgraca(const char* staroIme, const char* novoIme); // 1. 

void obrisiRezultatIgraca(const char* imeZaBrisanje); // 1.

#endif