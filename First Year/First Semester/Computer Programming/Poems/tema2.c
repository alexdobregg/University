// Dobre Gigi-Alexandru 312CD

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "util/task_helper.h"

#define N 3000

void load_file(const char *file, char *buffer);
void get_synonym(const char *word, int *n, char ***word_list);
void get_friendly_word(const char *word, char **friendly_word);

void print(char *str) {
    printf("%s\n", str);
    printf("\n");
}
// Functie care verifica daca este litera.
int litera(int car) {
    if ((car >= 'a' && car <= 'z') || (car >= 'A' && car <= 'Z')) {
        return car;
    } else {
        return 0;
    }
}
// Functie ajutatoare pentru a sorta lista cu qsort.
int string_cmp(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}

void uppercase(char *str) {
    int i;
    // Face toate majusculele litere mici.
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
    for (i = 0; str[i] != '\0'; i++) {
        // Face primul caracter de pe primul rand majuscula.
        if (i == 0 && (str[i] >= 'a' && str[i] <= 'z')) {
            str[i] = str[i] - 32;
        }
        // Face primul caracter de pe urmatoarele randuri majuscula.
        if (str[i] == '\n') {
            if (str[i + 1] >= 'a' && str[i + 1] <= 'z') {
                str[i + 1] = str[i + 1] - 32;
            }
        }
    }
}

void trimming(char *str) {
    int i, aux, c = 0, d = 0;
    char *vec, *vers;
    vec = (char *)malloc((strlen(str) + 1)*sizeof(char));
    if (vec == NULL) {
        exit(1);
    }
    // Se elimina toate spatiile si caracterele cerute care sunt in plus.
    for (i = 0; str[i] != '\0'; i++) {
        if (str[c] == ' ' ||
            str[c] == '.' ||
            str[c] == ',' ||
            str[c] == ':' ||
            str[c] == ';' ||
            str[c] == '?' ||
            str[c] == '!') {
            aux = c + 1;
            if (str[aux] != '\0') {
                while (litera(str[aux]) == 0 &&
                    str[aux] != '\0') {
                    if (str[aux] == ' ' ||
                        str[aux] == '.' ||
                        str[aux] == ',' ||
                        str[aux] == ':' ||
                        str[aux] == ';' ||
                        str[aux] == '?' ||
                        str[aux] == '!') {
                        c++;
                    }
                    aux++;
                }
            }
        }
        // Se tine minte intr-un string auxiliar noua poezie.
        vec[d] = str[c];
        c++;
        d++;
    }
    *(vec + d) = '\0';
    /* Se modifica poezia vechea in cea noua care are 
    doar un singur spatiu intre cuvinte. */
    str[0] = '\0';
    vers = strtok(vec, ".,:;?!");
    while (vers != NULL) {
        strcat(str, vers);
        vers = strtok(NULL, ".,:;?!");
    }
    free(vec);
}

void silly(char *str, float p) {
    int i;
    float sample;
    for (i = 0; str[i] != '\0'; i++) {
        // Verifica daca str[i] este litera.
        if ((str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= 'a' && str[i] <= 'z')) {
            sample = rand() % 100 / 99.0;
            if (sample < p) {
                // Daca str[i] este litera mare, il face litera mica.
                if (str[i] >= 'A' && str[i] <= 'Z') {
                    str[i] = str[i] + 32;
                    // Daca str[i] este litera mica, il face litera mare.
                } else if (str[i] >= 'a' && str[i] <= 'z') {
                    str[i] = str[i] - 32;
                }
            }
        }
    }
}

void friendly(char *str) {
    int i, j, ok = 0;
    char vec[N], v1[50], v2[50], v3[2], *diminutiv;
    strcpy(vec, str);
    str[0] = '\0';
    for (i = 0; vec[i] != '\0'; i++) {
        j = 0;
        // Am obinut fiecare cuvant pe rand din poezie.
        while (litera(vec[i]) != 0) {
            v1[j] = vec[i];
            j++;
            i++;
        }
        v1[j] = '\0';
        get_friendly_word(v1, & diminutiv);
        /* Am verificat daca are diminutiv. Daca nu are, concateneaza
        cuvantul initial, iar daca are, diminutivul; */
        if (diminutiv == NULL) {
            strcat(str, v1);
        } else {
            strcat(str, diminutiv);
        }
        // Concateneaza orice caracter care nu este litera.
        while (litera(vec[i]) == 0 && vec[i] != '\0') {
            v3[0] = vec[i];
            v3[1] = '\0';
            strcat(str, v3);
            i++;
        }
        i--;
    }
}

void rhimy1(char *str) {
    char **vers, **wordlist, poezie1[N], *p, **versuri;
    char lastl[50][50], lastw[50][50], lencuv;
    int i, j, k, r, l, n, vc, ok;
    char *ultsp1, *ultsp2, semne1[20], semne2[20];
    strcpy(poezie1, str);
    char ch[] = "\n";
    vers = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        vers[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
	if (vers == NULL) {
		exit(1);
    }
    versuri = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        versuri[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
	if (versuri == NULL) {
		exit(1);
    }
    i = vc = 0;
    // Se imparte poezia in versuri.
    p = strtok(poezie1, "\n");
    while (p != NULL) {
        vers[i] = p;
        vc++;
        i++;
        p = strtok(NULL, "\n");
    }
    // Se obtine ultimul cuvant, cat si ultima litera.
    for (i = 0; i < vc; i++) {
        j = strlen(vers[i]) - 1;
        while (litera(vers[i][j]) == 0) {
            j--;
        }
        k = r = 0;
        while (litera(vers[i][j]) != 0) {
            lastl[i][k] = vers[i][j];
            k++;
            j--;
        }
        lastl[i][k] = '\0';
        for (k = k - 1; k >= 0; k--) {
            lastw[i][r] = lastl[i][k];
            r++;
        }
        lastl[i][1] = '\0';
    }
    str[0] = '\0';
    for (i = 0; i < vc; i++) {
        if (lastl[i][0] >= 'A' && lastl[i][0] <= 'Z') {
            lastl[i][0] = lastl[i][0] + 32;
        }
    }
    i = 0;
    /* Se copiaza tot pana la ultimul cuvant, se verifica daca rimeaza,
    daca da se copiaza exact la fel, iar daca nu, se cauta sinonimele si
    verifica daca acestea la randul lor rimeaza, facandu-se
    schimbarile aferente. */
    while (i + 1 < vc) {
        ok = 0;
        ultsp1 = strrchr(vers[i], ' ') + 1;
        ultsp2 = strrchr(vers[i + 1], ' ') + 1;
        j = l = k = r = 0;
        while (ultsp1[j] != '\0') {
            if (litera(ultsp1[j]) == 0) {
                semne1[k] = ultsp1[j];
                k++;
            }
            j++;
        }
        while (ultsp2[l] != '\0') {
            if (litera(ultsp2[l]) == 0) {
                semne2[r] = ultsp2[l];
                r++;
            }
            l++;
        }
        semne1[k] = semne2[r] = '\0';
        strncat(versuri[i], vers[i], strlen(vers[i]) - strlen(ultsp1));
        strncat(versuri[i + 1], vers[i + 1], strlen(vers[i + 1]) -
												strlen(ultsp2));
        if (lastl[i][0] == lastl[i + 1][0]) {
            strcat(versuri[i], ultsp1);
            strcat(versuri[i + 1], ultsp2);
        } else {
            get_synonym(lastw[i], & n, & wordlist);
            if (wordlist != NULL) {
                qsort(wordlist, n, sizeof(char *), string_cmp);
                for (k = 0; k < n; k++) {
                    lencuv = strlen(wordlist[k]) - 1;
                    if (lastl[i + 1][0] == wordlist[k][lencuv]) {
                        strcat(versuri[i], wordlist[k]);
                        strcat(versuri[i], semne1);
                        strcat(versuri[i + 1], ultsp2);
                        ok = 1;
                        break;
                    }
                }
                if (ok == 0) {
                    strcat(versuri[i], ultsp1);
                    strcat(versuri[i + 1], ultsp2);
                }
            } else {
                get_synonym(lastw[i + 1], & n, & wordlist);
                if (wordlist != NULL) {
                    qsort(wordlist, n, sizeof(char *), string_cmp);
                    for (k = 0; k < n; k++) {
                        lencuv = strlen(wordlist[k]) - 1;
                        if (lastl[i][0] == wordlist[k][lencuv]) {
                            strcat(versuri[i + 1], wordlist[k]);
                            strcat(versuri[i + 1], semne2);
                            strcat(versuri[i], ultsp1);
                            ok = 1;
                            break;
                        }
                    }
                    if (ok == 0) {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 1], ultsp2);
                    }
                } else {
                    strcat(versuri[i], ultsp1);
                    strcat(versuri[i + 1], ultsp2);
                }
            }
        }
        i = i + 2;
    }
    // Se reconstruieste poezia in urma schimbarilor facute.
    for (i = 0; i < vc; i++) {
        strcat(str, versuri[i]);
        strcat(str, ch);
        if ((i + 1) % 4 == 0) {
            strcat(str, ch);
        }
        if (i == vc - 1) {
            str[strlen(str) - 2] = '\0';
        }
    }
}
/* Rhimy2 si Rhimy3 se bazeaza pe aceeasi metoda,
cu schimbari in functie de tipul de rima. */
void rhimy2(char *str) {
    char **vers, **wordlist, poezie1[N], *p, **versuri;
    char lastl[50][50], lastw[50][50], v1[50], v2[2], v3[2], lencuv;
    int i, j, k, r, l, n, vc, ok;
    char *ultsp1, *ultsp2, semne1[20], semne2[20];
    strcpy(poezie1, str);
    char ch[] = "\n";
    vers = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        vers[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
    if (vers == NULL) {
		exit(1);
    }
    versuri = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        versuri[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
    if (versuri == NULL) {
		exit(1);
    }
    i = vc = 0;
    p = strtok(poezie1, "\n");
    while (p != NULL) {
        vers[i] = p;
        vc++;
        i++;
        p = strtok(NULL, "\n");
    }
    for (i = 0; i < vc; i++) {
        j = strlen(vers[i]) - 1;
        while (litera(vers[i][j]) == 0) {
            j--;
        }
        k = r = 0;
        while (litera(vers[i][j]) != 0) {
            lastl[i][k] = vers[i][j];
            k++;
            j--;
        }
        lastl[i][k] = '\0';
        for (k = k - 1; k >= 0; k--) {
            lastw[i][r] = lastl[i][k];
            r++;
        }
        lastl[i][1] = '\0';
    }
    str[0] = '\0';
    for (i = 0; i < vc; i++) {
        if (lastl[i][0] >= 'A' && lastl[i][0] <= 'Z') {
            lastl[i][0] = lastl[i][0] + 32;
        }
    }
    i = 0;
    while (i + 2 < vc) {
        ok = 0;
        ultsp1 = strrchr(vers[i], ' ') + 1;
        ultsp2 = strrchr(vers[i + 2], ' ') + 1;
        j = l = k = r = 0;
        while (ultsp1[j] != '\0') {
            if (litera(ultsp1[j]) == 0) {
                semne1[k] = ultsp1[j];
                k++;
            }
            j++;
        }
        while (ultsp2[l] != '\0') {
            if (litera(ultsp2[l]) == 0) {
                semne2[r] = ultsp2[l];
                r++;
            }
            l++;
        }
        semne1[k] = semne2[r] = '\0';
        strncat(versuri[i], vers[i], strlen(vers[i]) - strlen(ultsp1));
        strncat(versuri[i + 2], vers[i + 2], strlen(vers[i + 2]) -
												strlen(ultsp2));
        if (lastl[i][0] == lastl[i + 2][0]) {
            strcat(versuri[i], ultsp1);
            strcat(versuri[i + 2], ultsp2);
        } else {
            get_synonym(lastw[i], &n, &wordlist);
            if (wordlist != NULL) {
                qsort(wordlist, n, sizeof(char *), string_cmp);
                for (k = 0; k < n; k++) {
                    lencuv = strlen(wordlist[k]) - 1;
                    if (lastl[i + 2][0] == wordlist[k][lencuv]) {
                        strcat(versuri[i], wordlist[k]);
                        strcat(versuri[i], semne1);
                        strcat(versuri[i + 2], ultsp2);
                        ok = 1;
                        break;
                    }
                }
                if (ok == 0) {
                    strcat(versuri[i], ultsp1);
                    strcat(versuri[i + 2], ultsp2);
                }
            } else {
                get_synonym(lastw[i + 2], &n, &wordlist);
                if (wordlist != NULL) {
                    qsort(wordlist, n, sizeof(char *), string_cmp);
                    for (k = 0; k < n; k++) {
                        lencuv = strlen(wordlist[k]) - 1;
                        if (lastl[i][0] == wordlist[k][lencuv]) {
                            strcat(versuri[i + 2], wordlist[k]);
                            strcat(versuri[i + 2], semne2);
                            strcat(versuri[i], ultsp1);
                            ok = 1;
                            break;
                        }
                    }
                    if (ok == 0) {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 2], ultsp2);
                    }
                } else {
                    strcat(versuri[i], ultsp1);
                    strcat(versuri[i + 2], ultsp2);
                }
            }
        }
        if (i % 2 == 1) {
            i = i + 2;
        }
        i++;
    }
    for (i = 0; i < vc; i++) {
        strcat(str, versuri[i]);
        strcat(str, ch);
        if ((i + 1) % 4 == 0) {
            strcat(str, ch);
        }
        if (i == vc - 1) {
            str[strlen(str) - 2] = '\0';
        }
    }
}

void rhimy3(char *str) {
    char **vers, **wordlist, poezie1[N], *p, **versuri;
    char lastl[50][50], lastw[50][50], lencuv;
    int i, j, k, r, l, n, vc, ok;
    char * ultsp1, * ultsp2, semne1[20], semne2[20];
    strcpy(poezie1, str);
    char ch[] = "\n";
    vers = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        vers[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
    if (vers == NULL) {
		exit(1);
    }
    versuri = (char **)malloc(strlen(str) * sizeof(char *));
    for (i = 0; i < strlen(str); i++) {
        versuri[i] = (char *)malloc(strlen(str) * sizeof(char));
    }
    if (versuri == NULL) {
		exit(1);
    }
    i = vc = 0;
    p = strtok(poezie1, "\n");
    while (p != NULL) {
        vers[i] = p;
        vc++;
        i++;
        p = strtok(NULL, "\n");
    }
    for (i = 0; i < vc; i++) {
        j = strlen(vers[i]) - 1;
        while (litera(vers[i][j]) == 0) {
            j--;
        }
        k = r = 0;
        while (litera(vers[i][j]) != 0) {
            lastl[i][k] = vers[i][j];
            k++;
            j--;
        }
        lastl[i][k] = '\0';
        for (k = k - 1; k >= 0; k--) {
            lastw[i][r] = lastl[i][k];
            r++;
        }
        lastl[i][1] = '\0';
    }
    str[0] = '\0';
    for (i = 0; i < vc; i++) {
        if (lastl[i][0] >= 'A' && lastl[i][0] <= 'Z') {
            lastl[i][0] = lastl[i][0] + 32;
        }
    }
    i = 0;
    while (i + 2 < vc) {
        ok = 0;
        if (i % 2 == 0) {
            ultsp1 = strrchr(vers[i], ' ') + 1;
            ultsp2 = strrchr(vers[i + 3], ' ') + 1;
            j = l = k = r = 0;
            while (ultsp1[j] != '\0') {
                if (litera(ultsp1[j]) == 0) {
                    semne1[k] = ultsp1[j];
                    k++;
                }
                j++;
            }
            while (ultsp2[l] != '\0') {
                if (litera(ultsp2[l]) == 0) {
                    semne2[r] = ultsp2[l];
                    r++;
                }
                l++;
            }
            semne1[k] = semne2[r] = '\0';
            strncat(versuri[i], vers[i], strlen(vers[i]) - strlen(ultsp1));
            strncat(versuri[i + 3], vers[i + 3], strlen(vers[i + 3]) -
													strlen(ultsp2));
            if (lastl[i][0] == lastl[i + 3][0]) {
                strcat(versuri[i], ultsp1);
                strcat(versuri[i + 3], ultsp2);
            } else {
                get_synonym(lastw[i], &n, &wordlist);
                if (wordlist != NULL) {
                    qsort(wordlist, n, sizeof(char *), string_cmp);
                    for (k = 0; k < n; k++) {
                        lencuv = strlen(wordlist[k]) - 1;
                        if (lastl[i + 3][0] == wordlist[k][lencuv]) {
                            strcat(versuri[i], wordlist[k]);
                            strcat(versuri[i], semne1);
                            strcat(versuri[i + 3], ultsp2);
                            ok = 1;
                            break;
                        }
                    }
                    if (ok == 0) {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 3], ultsp2);
                    }
                } else {
                    get_synonym(lastw[i + 3], &n, &wordlist);
                    if (wordlist != NULL) {
                        qsort(wordlist, n, sizeof(char *), string_cmp);
                        for (k = 0; k < n; k++) {
                            lencuv = strlen(wordlist[k]) - 1;
                            if (lastl[i][0] == wordlist[k][lencuv]) {
                                strcat(versuri[i + 3], wordlist[k]);
                                strcat(versuri[i + 3], semne2);
                                strcat(versuri[i], ultsp1);
                                ok = 1;
                                break;
                            }
                        }
                        if (ok == 0) {
                            strcat(versuri[i], ultsp1);
                            strcat(versuri[i + 3], ultsp2);
                        }
                    } else {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 3], ultsp2);
                    }
                }
            }
        } else if (i % 2 == 1) {
            ultsp1 = strrchr(vers[i], ' ') + 1;
            ultsp2 = strrchr(vers[i + 1], ' ') + 1;
            j = l = k = r = 0;
            while (ultsp1[j] != '\0') {
                if (litera(ultsp1[j]) == 0) {
                    semne1[k] = ultsp1[j];
                    k++;
                }
                j++;
            }
            while (ultsp2[l] != '\0') {
                if (litera(ultsp2[l]) == 0) {
                    semne2[r] = ultsp2[l];
                    r++;
                }
                l++;
            }
            semne1[k] = semne2[r] = '\0';
            strncat(versuri[i], vers[i], strlen(vers[i]) - strlen(ultsp1));
            strncat(versuri[i + 1], vers[i + 1], strlen(vers[i + 1]) -
													strlen(ultsp2));
            if (lastl[i][0] == lastl[i + 1][0]) {
                strcat(versuri[i], ultsp1);
                strcat(versuri[i + 1], ultsp2);
            } else {
                get_synonym(lastw[i], &n, &wordlist);
                if (wordlist != NULL) {
                    qsort(wordlist, n, sizeof(char *), string_cmp);
                    for (k = 0; k < n; k++) {
                        lencuv = strlen(wordlist[k]) - 1;
                        if (lastl[i + 1][0] == wordlist[k][lencuv]) {
                            strcat(versuri[i], wordlist[k]);
                            strcat(versuri[i], semne1);
                            strcat(versuri[i + 1], ultsp2);
                            ok = 1;
                            break;
                        }
                    }
                    if (ok == 0) {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 1], ultsp2);
                    }
                } else {
                    get_synonym(lastw[i + 1], &n, &wordlist);
                    if (wordlist != NULL) {
                        qsort(wordlist, n, sizeof(char *), string_cmp);
                        for (k = 0; k < n; k++) {
                            lencuv = strlen(wordlist[k]) - 1;
                            if (lastl[i][0] == wordlist[k][lencuv]) {
                                strcat(versuri[i + 1], wordlist[k]);
                                strcat(versuri[i + 1], semne2);
                                strcat(versuri[i], ultsp1);
                                ok = 1;
                                break;
                            }
                        }
                        if (ok == 0) {
                            strcat(versuri[i], ultsp1);
                            strcat(versuri[i + 1], ultsp2);
                        }
                    } else {
                        strcat(versuri[i], ultsp1);
                        strcat(versuri[i + 1], ultsp2);
                    }
                }
            }
        }
        if (i % 2 == 1) {
            i = i + 2;
        }
        i++;
    }
    for (i = 0; i < vc; i++) {
        strcat(str, versuri[i]);
        strcat(str, ch);
        if ((i + 1) % 4 == 0) {
            strcat(str, ch);
        }
        if (i == vc - 1) {
            str[strlen(str) - 2] = '\0';
        }
    }
}

int main(void) {
    srand(42);
    char s[N], r[50], *prob, *rhyme;
    int i;
    while (1 != 0) {
        fgets(r, 50, stdin);
        if (strstr(r, "load") != NULL) {
            if (strstr(r, "poems/test1/test1") != NULL) {
                load_file("poems/test1/test1", s);
            } else if (strstr(r, "poems/test2/test2") != NULL) {
                load_file("poems/test2/test2", s);
            } else if (strstr(r, "poems/test3/test3") != NULL) {
                load_file("poems/test3/test3", s);
            } else if (strstr(r, "poems/test4/test4") != NULL) {
                load_file("poems/test4/test4", s);
            } else if (strstr(r, "poems/test5/test5") != NULL) {
                load_file("poems/test5/test5", s);
            }
        } else if (strstr(r, "uppercase") != NULL) {
            uppercase(s);
        } else if (strstr(r, "trimming") != NULL) {
            trimming(s);
        } else if (strstr(r, "make_it_silly") != NULL) {
            // Se extrage probabilitatea din sirul introdus.
            prob = strtok(r, " ");
            prob = strtok(NULL, "\n");
            silly(s, atof(prob));
        } else if (strstr(r, "make_it_friendlier") != NULL) {
            friendly(s);
        } else if (strstr(r, "make_it_rhyme") != NULL) {
            // Se extrage tipul de rima.
            rhyme = strtok(r, " ");
            rhyme = strtok(NULL, "\n");
            if (strstr(rhyme, "imperecheata") != NULL) {
                rhimy1(s);
            } else if (strstr(rhyme, "incrucisata") != NULL) {
                rhimy2(s);
            } else if (strstr(rhyme, "imbratisata") != NULL) {
                rhimy3(s);
            }
        } else if (strstr(r, "print") != NULL) {
            print(s);
        } else if (strstr(r, "quit") != NULL) {
            return 0;
        }
    }
}