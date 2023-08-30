#include "pozitie_libera.h"


DateIntrare *citeste_fisier_in(char *nume_fisier_intrare) {
    /* Deschidem fisierul, alocam memoria, facem verificarile
    si extragem datele din fisier. */
    FILE *file = fopen(nume_fisier_intrare, "rt");
    if(!file) {
        printf("Error opening file");
        exit(1);
    }
    DateIntrare *date = (DateIntrare *) calloc(1, sizeof(DateIntrare));
    if(!date) {
        fclose(file);
        printf("Error allocating memory");
        exit(1);
    }
    int i;
    fscanf(file, "%d", &date->numarul_maxim_din_intrari);
    fscanf(file, "%d", &date->numar_intrari);
    date->intrari = (int *) calloc(date->numar_intrari, sizeof(int));
    for(i = 0; i < date->numar_intrari; i++) {
        fscanf(file, "%d", &date->intrari[i]);
    }
    fclose(file);
    return date;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

int *calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    int valmax = date_intrare->numarul_maxim_din_intrari;
    int i, k, valInterogare = 0;
    int *vectorR = (int *) calloc(valmax + 1, sizeof(int));
    // Alocam memorie pentru vector si facem verificarea.
    if(!vectorR) {
        printf("Error allocating memory");
        exit(1);
    }
    int gol = -1;
    // Setam elemntele din vector la variabila gol.
    for(i = 0; i < valmax + 1; i++) {
        vectorR[i] = gol;
    }
    // Construim arborele, facem alocarea intervalului si verificam alocarea.
    ArboreDeIntervale *arbore = construieste_arbore(0, valmax, 0,
            actualizare_cu_delta, suma_raspunsurilor);
    Interval *inT = (Interval *) calloc(1, sizeof(Interval));
    if(!inT) {
        free(vectorR);
        printf("Error allocating memory");
        exit(1);
    }
    for(i = 0; i < date_intrare->numar_intrari; i++) {
        // Interogam nodul care are valoarea ce trebuie pusa in vector.
        inT->capat_stanga = inT->capat_dreapta = date_intrare->intrari[i];
        valInterogare = interogare_interval_in_arbore(arbore, inT);
        /* daca acea valoare este 0, inseamna ca pozitia in vector este goala,
        iar daca nu, inseamna ca nu este si trebuie pusa pe urm pozitie goala. */
        if(valInterogare == 0) {
            // Completam valoarea in vector si actualizam info din nod.
            actualizare_interval_in_arbore(arbore, inT, 1);
            vectorR[date_intrare->intrari[i]] = date_intrare->intrari[i];
        } else {
            k = date_intrare->intrari[i] - 1;
            // Iteram pentru a cauta o pozitie goala, folosind interogarea.
            while(valInterogare != 0 && k >= 0) {
                inT->capat_stanga = inT->capat_dreapta = k--;
                valInterogare = interogare_interval_in_arbore(arbore, inT);
            }
            /* Verificam suplimentar pentru cazul in care nu mai sunt pozitii libere
            si completam in vector, actualizand info-ul din nodul specific. */
            if(vectorR[++k] == gol) {
                actualizare_interval_in_arbore(arbore, inT, 1);
                vectorR[k] = date_intrare->intrari[i];
            }
        }
    }
    return vectorR;
}


int *calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    // Alocam memorie pentru vector si facem verificarea.
    int valmax = date_intrare->numarul_maxim_din_intrari;
    int *vectorR = (int *) calloc(valmax + 1, sizeof(int));
    if(!vectorR) {
        printf("Error allocating memory");
        exit(1);
    }
    int i, j;
    int gol = -1;
    // Setam elemntele din vector la variabila gol.
    for(i = 0; i < valmax + 1; i++) {
        vectorR[i] = gol;
    }
    for(i = 0; i < date_intrare->numar_intrari; i++) {
        /* Daca pozitia este goala, se va completa, altfel se va cauta
        urmatoarea pozitie goala spre 0. */
        if(vectorR[date_intrare->intrari[i]] == gol) {
            vectorR[date_intrare->intrari[i]] = date_intrare->intrari[i];
        } else {
            for(j = date_intrare->intrari[i]; j >= 0; j--) {
                if(vectorR[j] == gol) {
                    vectorR[j] = date_intrare->intrari[i];
                    break;
                }
            } 
        }
    }
    return vectorR;
}