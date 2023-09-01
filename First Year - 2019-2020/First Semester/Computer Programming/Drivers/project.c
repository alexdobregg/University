// Dobre Gigi-Alexandru 312CD

#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <string.h>

#include "include/project.h"

int minim(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int maxim(int a, int b) {
    if (a < b) {
        return b;
    } else {
        return a;
    }
}

// Schimba elementele intre ele.
void schimbare(disteucl *lung1, disteucl *lung2) {
    disteucl temp = *lung1;
    *lung1 = *lung2;
    *lung2 = temp;
}

void bubbleSort(disteucl *lung, int n) {
    int i = 0, j;
    while (i != n - 1) {
        j = 0;
        while (j != n - i - 1) {
            // Sorteaza dupa lungime.
            if (lung[j].distanta > lung[j + 1].distanta) {
                schimbare(&lung[j], &lung[j + 1]);
            }
            j++;
        }
        i++;
    }
}

void bubbleSort2(TDriver **drivers, disteucl *lung, int n) {
    int i = 0, j;
    while (i != n - 1) {
        j = 0;
        while (j != n - i - 1) {
            // Daca distanta este egala, sorteaza dupa nume.
            if (lung[j].distanta == lung[j + 1].distanta) {
                if (strcmp(drivers[lung[j].pos]->nume,
                           drivers[lung[j + 1].pos]->nume) > 0) {
                    schimbare(&lung[j], &lung[j + 1]);
                }
            }
            j++;
        }
        i++;
    }
}

int *arrayProduct(int *array, int arrayLength) {
    int i, j, *prod;
    // prod este vectorul alocat in care voi tine produsul elementelor.
    prod = calloc(arrayLength, sizeof(int));
    if (!prod) {
        exit(1);
    }
    for (i = 0; i < arrayLength; i++) {
        prod[i] = 1;
    	for (j = 0; j < arrayLength; j++) {
            /* Verific ca pozitia pentru care calculez produsul
            sa fie diferita de cea a elementelor. */
    		if (j != i) {
    			prod[i] = prod[i] * array[j];
    		}
    	}
    }
    return prod;
}

int **rotateMatrix(int **matrix, int matrixSize) {
    int i, j, **matrice;
    // Aloc matricea pe care o returnez.
    matrice = calloc(matrixSize, sizeof(int*));
    if (!matrice) {
        exit(1);
    }
    for (i = 0; i < matrixSize; i++) {
        matrice[i] = calloc(matrixSize, sizeof(int));
        if (!matrice[i]) {
            for (j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            exit(1);
        }
    }
    i = 0;
    /* Am folosit o matrice ajutatoare, pe care o voi returna
    pentru a roti matricea initiala. */
    while (i != (matrixSize / 2)) {
        j = i;
        while (j != (matrixSize - i - 1)) {
            matrice[i][j] = matrix[i][j];

            matrix[i][j] = matrix[j][matrixSize - i - 1];

            matrix[j][matrixSize - i - 1] =
                        matrix[matrixSize - i - 1][matrixSize - j - 1];

            matrix[matrixSize - i - 1][matrixSize - j - 1] =
                        matrix[matrixSize - j - 1][i];

            matrix[matrixSize - j - 1][i] = matrice[i][j];
            j++;
        }
        i++;
    }
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            matrice[i][j] = matrix[i][j];
        }
    }
    return matrice;
}

int *subMatrixesSums(int **matrix, int queriesNo, int *queries) {
    int i, j, l = 0, *suma, min1, max1, min2, max2, s = 0;
    // Am alocat vectorul pe care il returnez.
    suma = calloc(queriesNo, sizeof(int));
    if (!suma) {
        exit(1);
    }
    while (l != queriesNo) {
        suma[l] = 0;
        // Am folosit 2 functii pentru a stabili ordinea coordonatelor.
        min1 = minim(queries[s], queries[s + 2]);
        min2 = minim(queries[s + 1], queries[s + 3]);
        max1 = maxim(queries[s], queries[s + 2]);
        max2 = maxim(queries[s + 1], queries[s + 3]);
        // Am calculat suma elementelor dintre coordonate.
        for (i = min1; i <= max1; i++) {
            for (j = min2; j <= max2; j++) {
                    suma[l]= suma[l] + matrix[i][j];
            }
        }
        s = s + 4;
        l++;
    }
    return suma;
}

TDriver *allocDriver(int ridesNo) {
    // Am alocat atat soferul, cat si vectorul de curse.
    TDriver *driver = calloc(1, sizeof(TDriver));
    if (!driver) {
        exit(1);
    }
    driver->cursa = calloc(ridesNo, sizeof(TRide));
    if (!driver->cursa) {
        exit(1);
    }
    driver->nrcurse = ridesNo;
    return driver;
}

TDriver **allocDrivers(int driversNo, int *driversRidesNo) {
    int i, j;
    // Am alocat un vector de soferi.
    TDriver **soferi = calloc(driversNo, sizeof(TDriver*));
    if (!soferi) {
        exit(1);
    }
    for (i = 0; i < driversNo; i++) {
        // Am folosit functia anterioara pentru fiecare sofer.
        soferi[i] = allocDriver(driversRidesNo[i]);
        if (!soferi[i]) {
            for (j = 0; j < i; j++) {
                free(soferi[j]);
            }
            free(soferi);
            exit(1);
        }
    }
    return soferi;
}

TDriver **readDrivers(FILE *inputFile, int *driversNo) {
    int i, j;
    TDriver **drivers;
    // Am citit numarul de soferi.
    fread(driversNo, sizeof(int), 1, inputFile);
    int *nrcurselor = calloc(*driversNo, sizeof(int));
    if (!nrcurselor) {
        exit(1);
    }
    // Am citit intr-un vector numarul de curse pentru fiecare sofer.
    for (i = 0; i < *driversNo; i++) {
        fread(&nrcurselor[i], sizeof(int), 1, inputFile);
    }
    drivers = allocDrivers(*driversNo, nrcurselor);
    // Am citit datele fiecarui sofer.
    for (i = 0; i < *driversNo; i++) {
        fread(&drivers[i]->nume, sizeof(char), 20, inputFile);
        fread(&drivers[i]->nrmasina, sizeof(char), 8, inputFile);
        fread(&drivers[i]->x2, sizeof(double), 1, inputFile);
        fread(&drivers[i]->y2, sizeof(double), 1, inputFile);
        drivers[i]->nrcurse = nrcurselor[i];
        for (j = 0; j < nrcurselor[i]; j++) {
           fread(&(drivers[i]->cursa[j]).x1, sizeof(double), 1, inputFile);
           fread(&(drivers[i]->cursa[j]).y1, sizeof(double), 1, inputFile);
           fread(&(drivers[i]->cursa[j]).stare, sizeof(unsigned char),
                                                        1, inputFile);
        }
    }
    free(nrcurselor);
    return drivers;
}

void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo) {
    int i;
    unsigned int j;
    // Am printat datele fiecarui sofer.
    for (i = 0; i < driversNo; i++) {
        fwrite(&drivers[i]->nume, sizeof(char), 20, outputFile);
        fwrite(&drivers[i]->nrmasina, sizeof(char), 8, outputFile);
        fwrite(&drivers[i]->x2, sizeof(double), 1, outputFile);
        fwrite(&drivers[i]->y2, sizeof(double), 1, outputFile);
        for (j = 0; j < drivers[i]->nrcurse; j++) {
           fwrite(&(drivers[i]->cursa[j]).x1, sizeof(double), 1, outputFile);
           fwrite(&(drivers[i]->cursa[j]).y1, sizeof(double), 1, outputFile);
           fwrite(&(drivers[i]->cursa[j]).stare, sizeof(unsigned char),
                                                        1, outputFile);
        }
    }
}

char *maxRatingDriverName(TDriver **drivers, int driversNo) {
    int i, aux = 0;
    unsigned int j;
    double sumstele, cursetotale, curseanul, cursedesf;
    // Am alocat vectorul in care retin ratingul fiecarui sofer.
    double *Rating = calloc(driversNo, sizeof(double));
    double max = 0;
    if (!Rating) {
        exit(1);
    }
    for (i = 0; i < driversNo; i++) {
        cursetotale = drivers[i]->nrcurse;
        sumstele = 0;
        curseanul = 0;
        cursedesf = 0;
        /* Obtin numarul de stele pentru fiecare sofer si cate curse anulate
        si in desfasurare are. */
        for (j = 0; j < drivers[i]->nrcurse; j++) {
            sumstele = sumstele + GET_STARS((drivers[i]->cursa[j]).stare);
            if (CANCELED_RIDE((drivers[i]->cursa[j]).stare)) {
                curseanul++;
            } else if (ONGOING_RIDE((drivers[i]->cursa[j]).stare)) {
                cursedesf++;
            }
        }
        // Calculez ratingul pentru fiecare sofer.
        Rating[i] = (sumstele / (cursetotale - cursedesf - curseanul));
    }
    // Retin pozitia soferului cu rating maxim.
    for (i = 0; i < driversNo; i++) {
        if (max < Rating[i]) {
            max = Rating[i];
            aux = i;
        }
    }
    free(Rating);
    return drivers[aux]->nume;
}

TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo) {
    int i, k;
    double xs, ys, xd, yd;
    // Aloc structura in care retin distanta.
    disteucl *lung = calloc (driversNo, sizeof(disteucl));
    if (!lung) {
        exit(1);
    }
    xd = desiredX;
    yd = desiredY;
    for (i = 0; i < driversNo; i++) {
        xs = drivers[i]->x2;
        ys = drivers[i]->y2;
        // Calculez distanta euclidiana si retin pozitia.
        lung[i].distanta = sqrt((xs - xd) * (xs - xd) + (ys - yd) * (ys - yd));
        lung[i].pos = i;
    }
    /* Sortez in functie de distanta, si dupa aceea dupa nume daca au
    distantele egale. */
    bubbleSort(lung, driversNo);
    bubbleSort2(drivers, lung, driversNo);
    /* Aloc vectorul de pointeri care tine minte in ordine soferii sortati
    anterior. */
    TDriver **soferi = calloc(resultsNo, sizeof(TDriver*));
    for (k = 0; k < resultsNo; k++) {
        soferi[k] = drivers[lung[k].pos];
    }
    free(lung);
    return soferi;
}

void freeDriver(TDriver *driver) {
    // Eliberez memoria alocata cursei si soferului.
    free(driver->cursa);
    free(driver);
}

void freeDrivers(TDriver **drivers, int driversNo) {
    int i;
    // Folosesc functia anterioara pentru fiecare sofer.
    for (i = 0; i < driversNo; i++) {
        freeDriver(drivers[i]);
    }
    // Eliberez memoria alocata vectorului de soferi.
    free(drivers);
}