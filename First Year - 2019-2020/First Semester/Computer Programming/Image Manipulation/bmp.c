// Dobre Gigi-Alexandru 312CD

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <math.h>

#include "bmp_header.h"

// Structura pentru pixeli.
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixels;

// Functia care va returna pixelii din imagine.
Pixels **readPixels(FILE *image, bmp_fileheader *fileheader,
                    bmp_infoheader *infoheader, int padding) {
    int i, j;
    // Aloc zona de memorie pentru pixeli.
    Pixels **pixeli = calloc(infoheader->height, sizeof(Pixels*));
    for (i = 0; i < infoheader->height; i++) {
        pixeli[i] = calloc(infoheader->width, sizeof(Pixels));
    }
    fseek(image, fileheader->imageDataOffset, SEEK_SET);
    // Citesc pixelii din imagine si ignor paddingul daca exista.
    if (padding == 4) {
        for (i = 0; i < infoheader->height; i++) {
            for (j = 0; j < infoheader->width; j++) {
                fread(&pixeli[i][j], sizeof(Pixels), 1, image);
            }
        }
    } else {
        for (i = 0; i < infoheader->height; i++) {
            for (j = 0; j < infoheader->width; j++) {
                fread(&pixeli[i][j], sizeof(Pixels), 1, image);
            }
            fseek(image, padding, SEEK_CUR);
        }
    }
    return pixeli;
}

// Functie pentru a scrie pixelii.
void Pixeliwriting(FILE *task, bmp_infoheader *infoheader, Pixels **pixeli,
                    int padding) {
    int i, j;
    unsigned char p = 0;
    if (padding == 4) {
        for (i = 0; i < infoheader->height; i++) {
            for (j = 0; j < infoheader->width; j++) {
                fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
            }
        }
    } else {
        for (i = 0; i < infoheader->height; i++) {
            for (j = 0; j < infoheader->width; j++) {
                fwrite(&pixeli[i][j], sizeof(Pixels), 1, task);
            }
            for (int pad = 0; pad < padding; pad ++) {
                fwrite(&p, sizeof(unsigned char), 1, task);
            }
        }
    }
}

void Black_White(FILE *image, char *testnr) {
    char *task1path = calloc(22, sizeof(char));
    int i, j;
    unsigned char grey;
    // Creez pathul pentru output si il deschid.
    strncat(task1path, testnr, 5);
    strcat(task1path, "_black_white.bmp");
    FILE *task1 = fopen(task1path, "wb");
    bmp_fileheader *fileheader = calloc(1, sizeof(bmp_fileheader));
    bmp_infoheader *infoheader = calloc(1, sizeof(bmp_infoheader));
    fread(fileheader, sizeof(bmp_fileheader), 1, image);
    fread(infoheader, sizeof(bmp_infoheader), 1, image);
    int padding = 4 - ((infoheader->width * 3) % 4);
    Pixels **pixeli = readPixels(image, fileheader, infoheader, padding);
    fwrite(fileheader, sizeof(bmp_fileheader), 1, task1);
    fwrite(infoheader, sizeof(bmp_infoheader), 1, task1);
    /* Calculez valoare lui grey si egalez culorile
    din pixeli cu aceasta valoare. */
        for (i = 0; i < infoheader->height; i++) {
            for (j = 0; j < infoheader->width; j++) {
                grey = (pixeli[i][j].blue + pixeli[i][j].green +
                                            pixeli[i][j].red) / 3;
                pixeli[i][j].blue = pixeli[i][j].green =
                                        pixeli[i][j].red = grey;
            }
        }
    Pixeliwriting(task1, infoheader, pixeli, padding);
    // Eliberez memoria.
    for (i = 0; i < infoheader->height; i++) {
        free(pixeli[i]);
    }
    free(pixeli);
    free(fileheader);
    free(infoheader);
    fclose(task1);
    free(task1path);
}

// Functie pentru cazul in care inaltimea este mai mare decat lungimea.
void No_Crop1(FILE *task2, bmp_infoheader *infoheader, Pixels **pixeli,
                                                        int padding) {
    Pixels white;
    int i, j;
    white.blue = white.green = white.red = 255;
    signed int partint, dif;
    // Calculez diferenta si partea intreaga a impartirii la 2.
    dif = infoheader->height - infoheader->width;
    partint = dif / 2;
    infoheader->width = infoheader->height;
    unsigned char p = 0;
    // Scriu noul infoheader in urma modficarii.
    fwrite(infoheader, sizeof(bmp_infoheader), 1, task2);
    /* Verific daca diferenta este para sau impara si scriu
    pixelii albi si cei din matrice, adaugand padding daca exista. */
    if (dif % 2 == 0) {
        if (padding == 4) {
            for (i = 0; i < infoheader->height; i++) {
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < infoheader->width - 2 * partint; j++) {
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
            }
        } else {
            for (i = 0; i < infoheader->height; i++) {
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < infoheader->width - 2 * partint; j++) {
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (int pad = 0; pad < padding; pad ++) {
                    fwrite(&p, sizeof(unsigned char), 1, task2);
                }
            }
        }
    } else {
        if (padding == 4) {
            for (i = 0; i < infoheader->height; i++) {
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < infoheader->width - 2 * partint; j++) {
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < partint + 1; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
            }
        } else {
            for (i = 0; i < infoheader->height; i++) {
                for (j = 0; j < partint; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < infoheader->width - 2 * partint; j++) {
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                }
                for (j = 0; j < partint + 1; j++) {
                    fwrite(&white, sizeof(Pixels), 1, task2);
                }
                for (int pad = 0; pad < padding; pad ++) {
                    fwrite(&p, sizeof(unsigned char), 1, task2);
                }
            }
        }
    }
}

/* Functie pentru cazul in care lungimea este mai mare decat inaltimea
si se bazeaza pe acelasi algoritm ca cea precedenta. */
void No_Crop2(FILE *task2, bmp_infoheader *infoheader, Pixels **pixeli,
                                                        int padding) {
    Pixels white;
    int i, j;
    white.blue = white.green = white.red = 255;
    signed int partint, dif;
    dif = infoheader->width - infoheader->height;
    partint = dif / 2;
    infoheader->height = infoheader->width;
    unsigned char p = 0;
    fwrite(infoheader, sizeof(bmp_infoheader), 1, task2);
    if (dif % 2 == 0) {
        if (padding == 4) {
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
            }
            for (i = 0; i < infoheader->height - 2 * partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
            }
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
            }
        } else {
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
            for (i = 0; i < infoheader->height - 2 * partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
        }
    } else {
        if (padding == 4) {
            for (i = 0; i < partint + 1; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
            }
            for (i = 0; i < infoheader->height - 2 * partint - 1; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
            }
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
            }
        } else {
            for (i = 0; i < partint + 1; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
            for (i = 0; i < infoheader->height - 2 * partint - 1; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&pixeli[i][j], sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
            for (i = 0; i < partint; i++) {
                for (j = 0; j < infoheader->width; j++)
                    fwrite(&white, sizeof(Pixels), 1, task2);
                for (int pad = 0; pad < padding; pad ++)
                    fwrite(&p, sizeof(unsigned char), 1, task2);
            }
        }
    }
}

void No_Crop(FILE *image, char *testnr) {
    char *task2path = calloc(17, sizeof(char));
    int i;
    // Creez pathul pentru output si deschid fisierul.
    strncat(task2path, testnr, 5);
    strcat(task2path, "_nocrop.bmp");
    FILE *task2 = fopen(task2path, "wb");
    bmp_fileheader *fileheader = calloc(1, sizeof(bmp_fileheader));
    bmp_infoheader *infoheader = calloc(1, sizeof(bmp_infoheader));
    // Setez cursorul la inceputul pozei deoarece citesc poza de fiecare data.
    fseek(image, 0, SEEK_SET);
    fread(fileheader, sizeof(bmp_fileheader), 1, image);
    fread(infoheader, sizeof(bmp_infoheader), 1, image);
    int padding = 4 - ((infoheader->width * 3) % 4);
    // Tin minte inaltimea veche pentru putea elibera memoria la final.
    signed int heightaloc = infoheader->height;
    Pixels **pixeli = readPixels(image, fileheader, infoheader, padding);
    fwrite(fileheader, sizeof(bmp_fileheader), 1, task2);
    // Calculez maximul dintre inaltime si lugnime.
    int max = infoheader->height;
    if (infoheader->width > infoheader-> height) {
        max = infoheader->width;
    }
    /* In functie de care valoare maximului, apelez una dintre cele 2 functii
    secundare ale acestui task. */
    if (max == infoheader->height) {
        No_Crop1(task2, infoheader, pixeli, padding);
    } else if (max == infoheader->width) {
        No_Crop2(task2, infoheader, pixeli, padding);
    }
    // Eliberez memoria.
    for (i = 0; i < heightaloc; i++) {
        free(pixeli[i]);
    }
    free(pixeli);
    free(fileheader);
    free(infoheader);
    fclose(task2);
    free(task2path);
}

void Filterwriting(FILE *task3, bmp_infoheader *infoheader, Pixels **pixeli,
                                int filtmatlung, int **filtmat, int padding) {
    int i, j, rand, col;
    /* Aloc o matrice pentru a tine minte valorile pixelilor matricei
    rasturnata. */
    Pixels **pixfiltered = calloc(infoheader->height, sizeof(Pixels*));
    for (i = 0; i < infoheader->height; i++) {
        pixfiltered[i] = calloc(infoheader->width, sizeof(Pixels));
    }
    // Tin minte valorile pixelilor matricei rasturnata.
    for (i = 0; i < infoheader->height; i++) {
        for (j = 0; j < infoheader->width; j++) {
            pixfiltered[i][j] = pixeli[infoheader->height - i - 1][j];
        }
    }
    int filt = filtmatlung/2;
    i = 0;
    while (i != infoheader->height) {
        j = 0;
        while (j != infoheader->width) {
            long long pixblue = 0, pixgreen = 0, pixred = 0;
            for (rand = -filt; rand <= filt ; rand ++) {
                /* Daca ma aflu pe o pozitie care are ca veicini randuri
                sau coloane care nu fac parte din matricea de pixeli
                voi ignora valorile de pe acele randuri/coloane deoarece
                vor fi setate la (0, 0, 0), si prin urmare nu vor 
                influenta suma. */
                for (col = -filt; col <= filt; col++) {
                    if ((i + rand >= 0 && i + rand < infoheader->height) &&
                             (j + col >= 0 && j + col < infoheader->width)) {
                    // Calculez valorile pixelilor conform formulei date.
                    pixblue = pixblue + pixfiltered[i + rand][j + col].blue *
                                filtmat[rand + filt][col + filt];
                    pixgreen = pixgreen + pixfiltered[i + rand][j + col].green *
                                filtmat[rand + filt][col + filt];
                    pixred = pixred + pixfiltered[i + rand][j + col].red *
                                filtmat[rand + filt][col + filt];
                    }
                }
            }
            /* Daca valorile sunt mai mici decat 0, vor fi setate la 0,
            iar daca sunt mai mari decat 255 vor fi setate la 255. */
            if (pixblue < 0) {
                pixblue = 0;
            } else if (pixblue > 255) {
                pixblue = 255;
            }
            if (pixgreen < 0) {
                pixgreen = 0;
            } else if (pixgreen > 255) {
                pixgreen = 255;
            }
            if (pixred < 0) {
                pixred = 0;
            } else if (pixred > 255) {
                pixred = 255;
            }
            // Schimb pixelii initiali cu cei noi.
            pixeli[i][j].blue = pixblue;
            pixeli[i][j].green = pixgreen;
            pixeli[i][j].red = pixred;
            j++;
        }
        i++;
    }
    // Scriu matricea cu noii pixeli.
    for (i = 0; i < infoheader->height; i++) {
        for (j = 0; j < infoheader->width; j++) {
            pixfiltered[i][j] = pixeli[infoheader->height - i - 1][j];
        }
    }
    Pixeliwriting(task3, infoheader, pixfiltered, padding);
    // Eliberez memoria.
    for (i = 0; i < infoheader->height; i++) {
        free(pixfiltered[i]);
    }
    free(pixfiltered);
}

void Filter(FILE *image, char *testnr, char *filterpath) {
    char *task3path = calloc(17, sizeof(char));
    int i, j, filtmatlung, **filtmat;
    // Creez pathul pentru output si deschid fisierul.
    strncat(task3path, testnr, 5);
    strcat(task3path, "_filter.bmp");
    FILE *task3 = fopen(task3path, "wb");
    bmp_fileheader *fileheader = calloc(1, sizeof(bmp_fileheader));
    bmp_infoheader *infoheader = calloc(1, sizeof(bmp_infoheader));
    fseek(image, 0, SEEK_SET);
    fread(fileheader, sizeof(bmp_fileheader), 1, image);
    fread(infoheader, sizeof(bmp_infoheader), 1, image);
    int padding = 4 - ((infoheader->width * 3) % 4);
    Pixels **pixeli = readPixels(image, fileheader, infoheader, padding);
    fwrite(fileheader, sizeof(bmp_fileheader), 1, task3);
    fwrite(infoheader, sizeof(bmp_infoheader), 1, task3);
    /* Deschid fisierul cu informatiile despre filtru si extrag
    lungimea matricii si elementele ei. */
    FILE *filter = fopen(filterpath, "rt");
    fscanf(filter, "%d", &filtmatlung);
    filtmat = calloc(filtmatlung, sizeof(int*));
    for (i = 0; i < filtmatlung; i++) {
        filtmat[i] = calloc(filtmatlung, sizeof(int));
    }
    for (i = 0; i < filtmatlung; i++) {
        for (j = 0; j < filtmatlung; j++) {
            fscanf(filter, "%d", &filtmat[i][j]);
        }
    }
    Filterwriting(task3, infoheader, pixeli, filtmatlung, filtmat, padding);
    // Eliberez memoria.
    for (i = 0; i < infoheader->height; i++) {
        free(pixeli[i]);
    }
    free(pixeli);
    free(fileheader);
    free(infoheader);
    fclose(task3);
    free(task3path);
    fclose(filter);
    for (i = 0; i < filtmatlung; i++) {
        free(filtmat[i]);
    }
    free(filtmat);
}

void Poolingwriting(FILE *task4, bmp_infoheader *infoheader, Pixels **pixeli,
                             char *poolingtype, int poollung, int padding) {
    int i, j, rand, col;
    /* Aloc o matrice pentru a tine minte valorile pixelilor matricei
    rasturnata. */
    Pixels **pixfiltered = calloc(infoheader->height, sizeof(Pixels*));
    for (i = 0; i < infoheader->height; i++) {
        pixfiltered[i] = calloc(infoheader->width, sizeof(Pixels));
    }
    // Tin minte valorile pixelilor matricei rasturnata.
    for (i = 0; i < infoheader->height; i++) {
        for (j = 0; j < infoheader->width; j++) {
            pixfiltered[i][j] = pixeli[infoheader->height - i - 1][j];
        }
    }
    Pixels pixl;
    int pol = poollung/2;
    i = 0;
    while (i != infoheader->height) {
        j = 0;
        while (j != infoheader->width) {
            /* Pentru a stabili minimul si maximul zonei, voi seta
            pixelii din pixl sa aiba valoarea pixelului curent. */ 
            pixl = pixfiltered[i][j];
            /* Daca ma aflu pe o pozitie care are ca veicini randuri
                sau coloane care nu fac parte din matricea de pixeli
                voi ignora valorile de pe acele randuri/coloane deoarece
                vor fi setate la (0, 0, 0). */
            for (rand = -pol; rand <= pol ; rand ++) {
                if (i + rand < 0 || i + rand >= infoheader->height) {
                    continue;
                }
                for (col = -pol; col <= pol; col++) {
                    if (j + col < 0 || j + col >= infoheader->width) {
                        continue;
                    }
                    /* Compar valoarea in functie de tipul de pooling, astfel
                    voi obtine minimul sau maximul. */
                    if (strstr(poolingtype, "m") != NULL) {
                        if (pixl.blue > pixfiltered[i + rand][j + col].blue) {
                            pixl.blue = pixfiltered[i + rand][j + col].blue;
                        }
                        if (pixl.green > pixfiltered[i + rand][j + col].green) {
                            pixl.green = pixfiltered[i + rand][j + col].green;
                        }
                        if (pixl.red > pixfiltered[i + rand][j + col].red) {
                            pixl.red = pixfiltered[i + rand][j + col].red;
                        }
                    } else if (strstr(poolingtype, "M") != NULL) {
                        if (pixl.blue < pixfiltered[i + rand][j + col].blue) {
                            pixl.blue = pixfiltered[i + rand][j + col].blue;
                        }
                        if (pixl.green < pixfiltered[i + rand][j + col].green) {
                            pixl.green = pixfiltered[i + rand][j + col].green;
                        }
                        if (pixl.red < pixfiltered[i + rand][j + col].red) {
                            pixl.red = pixfiltered[i + rand][j + col].red;
                        }
                    }
                }
            }
            /* Daca tipul de pooling este minim, atunci in functie de 
            dimensiunea zonei minimul pentru primele linii si coloane
            va fi (0, 0, 0) deoarece atat ar fi fost valorile
            pixelilor din exteriorul matricii. */
            if (strstr(poolingtype, "m") != NULL) {
                if (poollung == 3) {
                    if (i == 0 || i == infoheader->height - 1 || j == 0 ||
                                                j == infoheader->width - 1 ) {
                        pixl.blue = pixl.green = pixl.red = 0;
                    }
                } else if (poollung == 5) {
                    if (i == 0 || i == 1 || i == infoheader->height - 1 ||
                         i == infoheader->height - 2 || j == 0 || j == 1 ||
                                                j == infoheader->width - 1 ||
                                                j == infoheader->width - 2) {
                        pixl.blue = pixl.green = pixl.red = 0;
                    }
                }
            }
            pixeli[i][j] = pixl;
            j++;
        }
        i++;
    }
    // Scriu matricea de pixeli cu valorile noi.
    for (i = 0; i < infoheader->height; i++) {
        for (j = 0; j < infoheader->width; j++) {
            pixfiltered[i][j] = pixeli[infoheader->height - i - 1][j];
        }
    }
    Pixeliwriting(task4, infoheader, pixfiltered, padding);
    // Eliberez memoria.
    for (i = 0; i < infoheader->height; i++) {
        free(pixfiltered[i]);
    }
    free(pixfiltered);
}

void Pooling(FILE *image, char *testnr, char *poolingpath) {
    char *task4path = calloc(18, sizeof(char));
    int i, poollung;
    char *poolingtype = calloc(2, sizeof(char));
    // Creez pathul pentru output si deschid fisierul.
    strncat(task4path, testnr, 5);
    strcat(task4path, "_pooling.bmp");
    FILE *task4 = fopen(task4path, "wb");
    bmp_fileheader *fileheader = calloc(1, sizeof(bmp_fileheader));
    bmp_infoheader *infoheader = calloc(1, sizeof(bmp_infoheader));
    fseek(image, 0, SEEK_SET);
    fread(fileheader, sizeof(bmp_fileheader), 1, image);
    fread(infoheader, sizeof(bmp_infoheader), 1, image);
    int padding = 4 - ((infoheader->width * 3) % 4);
    Pixels **pixeli = readPixels(image, fileheader, infoheader, padding);
    fwrite(fileheader, sizeof(bmp_fileheader), 1, task4);
    fwrite(infoheader, sizeof(bmp_infoheader), 1, task4);
    /* Deschid fisierul ce contine informatii despre pooling si extrag
    din acesta tipul de pooling si dimensiunea matricii pentru a aplica
    poolingul pe imagine. */
    FILE *pooling = fopen(poolingpath, "rt");
    fscanf(pooling, "%s", poolingtype);
    fscanf(pooling, "%d", &poollung);
    Poolingwriting(task4, infoheader, pixeli, poolingtype, poollung, padding);
    // Eliberez memoria.
    for (i = 0; i < infoheader->height; i++) {
        free(pixeli[i]);
    }
    free(pixeli);
    free(fileheader);
    free(infoheader);
    fclose(task4);
    free(task4path);
    fclose(pooling);
    free(poolingtype);
}

int main() {
    char *testnr, *filterpath, *poolingpath, *clusterpath, *imagepath;
    // Aloc memoria pentru pathurile si numarul testului extrase din input.
    testnr = calloc(12, sizeof(char));
    filterpath = calloc(27, sizeof(char));
    poolingpath = calloc(28, sizeof(char));
    clusterpath = calloc(31, sizeof(char));
    // Aloc memoria pentru pathuri imaginii.
    imagepath = calloc(23, sizeof(char));
    FILE *input = fopen("input.txt", "rt");
    if (!input) {
        exit(1);
    }
    /* Creez pathul catre imagine si citesc din input valorile alocate
    anterior. */
    strcat(imagepath, "input/images/");
    fscanf(input, "%s", testnr);
    fscanf(input, "%s", filterpath);
    fscanf(input, "%s", poolingpath);
    fscanf(input, "%s", clusterpath);
    strcat(imagepath, testnr);
    /* Deschid imaginea, imagine din care datele vor fi citite de fiecare
    data in cadrul functiilor specifice pentru fiecare task. */
    FILE *image = fopen(imagepath, "rb");
    if (!image) {
        exit(1);
    }
    Black_White(image, testnr);
    No_Crop(image, testnr);
    Filter(image, testnr, filterpath);
    Pooling(image, testnr, poolingpath);
    // Eliberez memoria.
    free(testnr);
    free(filterpath);
    free(poolingpath);
    free(clusterpath);
    free(imagepath);
    fclose(input);
    fclose(image);
    return 0;
}