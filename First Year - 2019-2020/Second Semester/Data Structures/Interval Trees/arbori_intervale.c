#include "arbori_intervale.h"

//Functie care creeaza cate un nod.
Nod *creare_nod (ArboreDeIntervale *arbore, int capat_stanga, int capat_dreapta) {
    // Alocam memorie pentru nod si verificam daca s-a alocat.
    Nod *nod = (Nod *) calloc(1, sizeof(Nod));
    if(!nod) {
        printf("Error allocating memory");
        exit(1);
    }
    // Alocam memorie pentru interval si verificam daca s-a alocat.
    nod->interval = (Interval *) calloc(1, sizeof(Interval));
    if(!nod->interval) {
        free(nod);
        printf("Error allocating memory");
        exit(1);
    }
    // Completam campurile din nod si crestem dimensiunea arborelui.
    nod->interval->capat_stanga = capat_stanga;
    nod->interval->capat_dreapta = capat_dreapta;
    nod->info = 0;
    arbore->dimensiune++;
    /* Daca capatul din stanga este mai mic decat cel din dreapta, se va
    calcula mijlocul si se vor crea noduri in continuare, recursiv. */
    if(capat_stanga < capat_dreapta) {
        int mijloc = (capat_stanga + capat_dreapta) / 2;
        nod->copil_stanga = creare_nod(arbore, capat_stanga, mijloc);
        nod->copil_dreapta = creare_nod(arbore, mijloc + 1, capat_dreapta);
    }
    return nod;
}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {
    // Alocam memorie pentru arbore si verificam alocarea.
    ArboreDeIntervale *ADI = (ArboreDeIntervale *) calloc(1, sizeof(ArboreDeIntervale));
    if (!ADI) {
        printf("Error allocating memory");
        exit(1);
    }
    // Completam campurile si setam dimensiunea la 0.
    ADI->f_actualizare = f_actualizare;
    ADI->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;
    ADI->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    ADI->dimensiune = 0;
    // Apelam functia de creare_nod care va crea recursiv arborele de intervale.
    ADI->radacina = creare_nod(ADI, capat_stanga, capat_dreapta);
    return ADI;
}



void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    int ST = interval->capat_stanga;
    int DR = interval->capat_dreapta;
    int mijl;
    /* Daca intervalul nodului este cuprins in intervalul dat, se va actualiza
    informatia din el. */
    if(ST <= nod->interval->capat_stanga && nod->interval->capat_dreapta <= DR) {
        arbore->f_actualizare(nod, v_actualizare);
    } else {
        /* Daca nu, se va calcula mijlocul si va intra in urmatoarele recursivitati
        daca indeplindeste conditiile, iar in final se va actualiza informatia din
        nod. */
        mijl = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;
        if(ST <= mijl) {
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga, interval,
                                                                v_actualizare);
        }
        if(mijl < DR) {
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta, interval,
                                                                v_actualizare);
        }
        arbore->f_actualizare(nod, v_actualizare);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {   
    int ST = interval->capat_stanga;
    int DR = interval->capat_dreapta;
    int mijl;
    int valoareST = 0;
    int valoareDR = 0;
    if(ST <= nod->interval->capat_stanga && nod->interval->capat_dreapta <= DR) {
        return nod->info;
    } else {
        mijl = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;
        if(ST <= mijl) {
            valoareST = interogare_interval_pentru_nod(arbore, nod->copil_stanga, interval);
        }
        if(mijl < DR) {
            valoareDR = interogare_interval_pentru_nod(arbore, nod->copil_dreapta, interval);
        }
        return arbore->f_combinare_raspunsuri_copii(valoareST, valoareDR);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    nod->info = nod->interval->capat_dreapta;
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
