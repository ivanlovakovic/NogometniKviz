#ifndef HEADER_H
#define HEADER_H

#include "kviz.h"

// 14. Funkcija za čitanje pitanja iz datoteke (CRUID, 1)
int ucitajPitanja(const char* nazivDatoteke, pitanje* niz);          // 1, 12

// 14. Funkcija za prikaz pojedinačnog pitanja
void prikaziPitanje(pitanje p, int redniBroj);                       // 14

// 14. Provjera točnosti unesenog odgovora
int provjeriOdgovor(pitanje p, char unos);                           // 14

// 1. CRUD – upis rezultata u tekstualnu datoteku
void zapisiRezultat(rezultat r);                                     // 1, 12, 14

// 1. CRUID + 14. Funkcija koja pokreće igru za određenu težinu
void pokreniIgru(const char* datoteka, const char* level);          // 1, 14

// 20, 23, 24. Funkcija za čitanje, sortiranje i prikaz rezultata
int procitajSortirajIPrikaziRezultate(void);                         // 20, 22, 23, 24

#endif