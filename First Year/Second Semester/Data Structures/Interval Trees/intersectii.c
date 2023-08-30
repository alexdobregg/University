#include "intersectii.h"

// Structura ajutatoare pentru cerinta cu arbori.
typedef struct Points {
    Punct2D point;
    int tip;
    int poz;
} Points;

// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    // Am deschis fisierul si am verificat daca s-a deschis cu succes.
    FILE *file = fopen(nume_fisier_intrare, "rt");
    if(!file) {
        printf("Error opening file");
        exit(1);
    }
    /* In continuare, am alocat memorie pentru toate structurile si in cazul
    in care una dintre alocari nu s-a realizat, tot ce a fost alocat inainte
    se dezaloca si se inchide fisierul. */
    ListaIntervale2D *listint = (ListaIntervale2D *) calloc(1, sizeof(ListaIntervale2D));
    if(!listint) {
        fclose(file);
        printf("Error allocating memory");
        exit(1);
    }
    int i, j, nrInt;
    fscanf(file, "%d", &nrInt);
    listint->intervale = (Interval2D **) calloc(nrInt, sizeof(Interval2D *));
    if(!listint->intervale) {
        fclose(file);
        free(listint);
        printf("Error allocating memory");
        exit(1);
    }
    for(i = 0; i < nrInt; i++) {
        listint->intervale[i] = (Interval2D *) calloc(1, sizeof(Interval2D));
        if(!listint->intervale[i]) {
            for (j = 0; j < i; j++) {
                free(listint->intervale[j]);
            }
            fclose(file);
            free(listint);
            printf("Error allocating memory");
            exit(1);
        }
        listint->intervale[i]->punct_stanga = (Punct2D *) calloc(1, sizeof(Punct2D));
        if(!listint->intervale[i]->punct_stanga) {
            for (j = 0; j < i; j++) {
                free(listint->intervale[j]->punct_stanga);
                free(listint->intervale[j]->punct_dreapta);
                free(listint->intervale[j]);
            }
            fclose(file);
            free(listint);
            printf("Error allocating memory");
            exit(1);
        }
        listint->intervale[i]->punct_dreapta = (Punct2D *) calloc(1, sizeof(Punct2D));
        if(!listint->intervale[i]->punct_stanga) {
            for (j = 0; j < i; j++) {
                free(listint->intervale[j]->punct_stanga);
                free(listint->intervale[j]->punct_dreapta);
                free(listint->intervale[j]);
            }
            fclose(file);
            free(listint->intervale[i]->punct_stanga);
            free(listint);
            printf("Error allocating memory");
            exit(1);
        }
    }
    // Se citesc intervalele din fisier.
    for(i = 0; i < nrInt; i++) {
        fscanf(file, "%d", &listint->intervale[i]->punct_stanga->x);
        fscanf(file, "%d", &listint->intervale[i]->punct_stanga->y);
        fscanf(file, "%d", &listint->intervale[i]->punct_dreapta->x);
        fscanf(file, "%d", &listint->intervale[i]->punct_dreapta->y);
    }
    listint->dimensiune = nrInt;
    fclose(file);
    return listint;
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

int OrizVert(Punct2D *p1, Punct2D *p2) {
    // Intervalul creaza un segment vertical sau orizontal.
    int ov;
    if(p1->x == p2->x) {
        // Vertical.
        ov = 0;
    } else if(p1->y == p2->y) {
        // Orizontal.
        ov = 1;
    }
    return ov;
}

int max(int a, int b) {
    if(a < b) {
        return b;
    }
    return a;
}

int min(int a, int b) {
    if(a < b) {
        return a;
    }
    return b;
}

// Functie ajutatoare pentru qsort.
int comparepoints(const void *a, const void *b) {
  Points *pointA = (Points *)a;
  Points *pointB = (Points *)b;
  return (pointA->point.x - pointB->point.x);
}

Points *create_points(ListaIntervale2D *lista, int lung) {
    Points *puncte = (Points *) calloc(lung, sizeof(Points));
    if(!puncte) {
        printf("Error allocating memory");
        exit(1);
    }
    int i, j = 0;
    for(i = 0; i < lista->dimensiune; i++) {
        /* Se completeaza structura de puncte, retinand daca este
        capat stanga sau dreapta, precum si de la ce interval provine. */
        puncte[j].point = *lista->intervale[i]->punct_stanga;
        puncte[j + 1].point = *lista->intervale[i]->punct_dreapta;
        int ov = OrizVert(&puncte[j].point, &puncte[j + 1].point);
        if(ov == 1) {
            // Capat stanga interval orizontal.
            puncte[j].tip = 1;
            // Capat dreapta interval orizontal.
            puncte[j + 1].tip = 2;
        } else if(ov == 0) {
            // Capat stanga interval vertical.
            puncte[j].tip = 3;
            // Capat dreapta interval vertical.
            puncte[j + 1].tip = 4;
        }
        puncte[j].poz = i;
        puncte[j + 1].poz = i;
        j = j + 2;
    }
    // Se sorteaza structurile.
    qsort(puncte, lung, sizeof(Points), comparepoints);
    return puncte;
}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    int i, j = 0, ymax = 0;
    // Obtinem ymax.
    for(i = 0; i < lista->dimensiune; i++) {
        if(ymax < max(lista->intervale[i]->punct_stanga->y,
                        lista->intervale[i]->punct_dreapta->y)) {
            ymax = max(lista->intervale[i]->punct_stanga->y,
                        lista->intervale[i]->punct_dreapta->y);
        }
    }
    // Se construieste arborele.
    ArboreDeIntervale *arbore = construieste_arbore(0, ymax, 0, actualizare_cu_delta, suma_raspunsurilor);
    int Intersectii = 0;
    int lung = 2 * lista->dimensiune;
    Points *puncte = create_points(lista, lung);
    Interval *inT = (Interval *) calloc(1, sizeof(Interval));
    if(!inT) {
        free(puncte);
        printf("Error allocating memory");
        exit(1);
    }
    for(i = 0; i < lung; i++) {
        if(puncte[i].tip == 1) {
            /* Daca este capat stanga al unui interval orizontal se
            actualizeaza intervalul [y, y] cu 1. */
            inT->capat_stanga = inT->capat_dreapta = puncte[i].point.y;
            actualizare_interval_in_arbore(arbore, inT, 1); 
        } else if(puncte[i].tip == 2) {
            /* Daca este capat dreapta al unui interval orizontal se
            actualizeaza intervalul [y, y] cu -1. */
            inT->capat_stanga = inT->capat_dreapta = puncte[i].point.y;
            actualizare_interval_in_arbore(arbore, inT, -1);
        } else if(puncte[i].tip == 3) {
            /* Daca punctul apartine unui interval vertical si este capatul
            stanga, se cauta capatul dreapta care apartine aceluiasi interval. */
            inT->capat_stanga = puncte[i].point.y;
            for(j = 0; j < lung; j++) {
                if(puncte[j].poz == puncte[i].poz && puncte[j].tip == 4) {
                    inT->capat_dreapta = puncte[j].point.y;
                    // Se obtin numarul de intersectii.
                    Intersectii += interogare_interval_in_arbore(arbore, inT);
                    break;
                }
            }
        }
    }
    free(puncte);
    return Intersectii;
}

int intersection(Interval2D *I1, Interval2D *I2) {
    Punct2D *I11 = I1->punct_stanga;
    Punct2D *I12 = I1->punct_dreapta;
    Punct2D *I21 = I2->punct_stanga;
    Punct2D *I22 = I2->punct_dreapta;

    int ov1 = OrizVert(I11, I12);
    int ov2 = OrizVert(I21, I22);
    
    // Daca cele 2 intervale se intersecteaza, se va returna 1, altfel 0.
    if(ov1 == 1 && ov2 == 0) {
        if((I11->y <= max(I21->y, I22->y) && I11->y >= min(I21->y, I22->y)) &&
            (I21->x <= max(I11->x, I12->x) && I21->x >= min(I11->x, I12->x))) {
            return 1;
        }
    }
    if(ov1 == 0 && ov2 == 1) {
        if((I21->y <= max(I11->y, I12->y) && I21->y >= min(I11->y, I12->y)) &&
            (I11->x <= max(I21->x, I22->x) && I11->x >= min(I21->x, I22->x))) {
            return 1;
        }
    }
    return 0;

}

int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    int i, j, Intersectii = 0;
    // Se calculeaza numarul de intersectii.
    for(i = 0; i < lista->dimensiune; i++) {
        for(j = i; j < lista->dimensiune; j++) {
            Intersectii += intersection(lista->intervale[i], lista->intervale[j]);
        }
    }
    return Intersectii;
}
