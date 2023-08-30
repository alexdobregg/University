// Dobre Gigi-Alexandru 312 CD
#include <stdio.h>

/* Functia transf este una care ne va ajuta sa transformam restul ramas, 
reprezentand o parte din bitii setati, in urma impartirii lui MSK_2 la 8,
intr-un numar pe un octet. */
unsigned char transf(unsigned char x) {
    unsigned char i;
    /* Fiecare caz reprezinta resturile posibile incepand cu 1 si pana la 7,
     cazul cu restul 0 fiind specificat in interiorul lui int main(){}. */
    switch (x) {
    case (1):
        i = 1 << 7;
        break;
    case (2):
        i = (1 << 7) + (1 << 6);
        break;
    case (3):
        i = (1 << 7) + (1 << 6) + (1 << 5);
        break;
    case (4):
        i = (1 << 7) + (1 << 6) + (1 << 5) + (1 << 4);
        break;
    case (5):
        i = (1 << 7) + (1 << 6) + (1 << 5) + (1 << 4) + (1 << 3);
        break;
    case (6):
        i = (1 << 7) + (1 << 6) + (1 << 5) + (1 << 4) + (1 << 3) + (1 << 2);
        break;
    case (7):
        i = (1 << 7) + (1 << 6) + (1 << 5);
        i = i + (1 << 4) + (1 << 3) + (1 << 2) + (1 << 1);
        break;
    }
    return i;
}
int main() {
    unsigned char M1[500], M2[500], M3[500], M4[500], MSK_2[500];
    unsigned char I11[500], I12[500], I13[500], I14[500];
    /* Vectorii M1, M2, M3, M4 apartin lui MSK_1;
    I11, I12, I13, I14 lui IP_1; I21, I22, I23, I24 lui IP_2. */
    unsigned char I21[500], I22[500], I23[500], I24[500];
    // Matricele NET1, NET2, NET3, NET4 si MSKNET corespund lui NET_N.
    unsigned char NET1[500][500], NET2[500][500];
    unsigned char NET3[500][500], NET4[500][500];
    unsigned char MSKNET[500][500];
    // nrset reprezinta numarul de seturi, iar q este numarul fiecarui set.
    int i, N[500], nrset, q;
    scanf("%d\n", & nrset);
    for (q = 1; q <= nrset; q++) {
        scanf("%hhu.%hhu.%hhu.%hhu", & M1[q], & M2[q], & M3[q], & M4[q]);
        scanf("%hhu", & MSK_2[q]);
        scanf("%hhu.%hhu.%hhu.%hhu", & I11[q], & I12[q], & I13[q], & I14[q]);
        scanf("%hhu.%hhu.%hhu.%hhu", & I21[q], & I22[q], & I23[q], & I24[q]);
        scanf("%d", & N[q]);
        for (i = 1; i <= N[q]; i++) {
            scanf("%hhu.%hhu.%hhu.", & NET1[i][q], & NET2[i][q], & NET3[i][q]);
            scanf("%hhu/%hhu", & NET4[i][q], & MSKNET[i][q]);
        }
    }
    for (q = 1; q <= nrset; q++) {
        // Task 0
        printf("%d\n", q);
        printf("-0 %hhu.%hhu.%hhu.%hhu", I11[q], I12[q], I13[q], I14[q]);
        printf("/%hhu\n", MSK_2[q]);
        // Task 1
        printf("-1 ");
        // rest este restul impartirii lui MSK_2 la 8.
        unsigned char rest, i1, i2, i3, i4;
        /* Fiecare caz reprezinta caturile posibile 
		in urma impartirii lui MSK_2 la 8. */
        switch (MSK_2[q] / 8) {
        case (0):
            if (MSK_2[q] == 0) {
                printf("0.0.0.0\n");
                i1 = 0;
                i2 = 0;
                i3 = 0;
                i4 = 0;
            } else {
                rest = MSK_2[q] % 8;
                printf("%hhu.0.0.0\n", transf(rest));
                i1 = transf(rest);
                i2 = 0;
                i3 = 0;
                i4 = 0;
            }
            break;
        case (1):
            rest = MSK_2[q] % 8;
            printf("255.%hhu.0.0\n", transf(rest));
            i1 = 255;
            i2 = transf(rest);
            i3 = 0;
            i4 = 0;
            break;
        case (2):
            rest = MSK_2[q] % 8;
            printf("255.255.%hhu.0\n", transf(rest));
            i1 = 255;
            i2 = 255;
            i3 = transf(rest);
            i4 = 0;
            break;
        case (3):
            rest = MSK_2[q] % 8;
            printf("255.255.255.%hhu\n", transf(rest));
            i1 = 255;
            i2 = 255;
            i3 = 255;
            i4 = transf(rest);
            break;
        case (4):
            printf("255.255.255.255\n");
            i1 = 255;
            i2 = 255;
            i3 = 255;
            i4 = 255;
            break;
        }
        // Task 2
        printf("-2 %o.%o.%o.%o ", M1[q], M2[q], M3[q], M4[q]);
        printf("%X.%X.%X.%X\n", M1[q], M2[q], M3[q], M4[q]);
        // Task 3
        printf("-3 ");
        printf("%hhu.%hhu.", (I11[q] & i1), (I12[q] & i2));
        printf("%hhu.%hhu\n", (I13[q] & i3), (I14[q] & i4));
        // Task 4
        printf("-4 ");
        printf("%hhu.%hhu.", (I11[q] | ~i1), (I12[q] | ~i2));
        printf("%hhu.%hhu\n", (I13[q] | ~i3), (I14[q] | ~i4));
        // Task 5
        printf("-5 ");
        if ((I11[q] & i1) == (I21[q] & i1)) {
            if ((I12[q] & i2) == (I22[q] & i2)) {
                if ((I13[q] & i3) == (I23[q] & i3)) {
                    if ((I14[q] & i4) == (I24[q] & i4)) {
                        printf("DA\n");
                    } else {
                        printf("NU\n");
                    }
                } else {
                    printf("NU\n");
                }
            } else {
                printf("NU\n");
            }
        } else {
            printf("NU\n");
        }
        // Task 6
        printf("-6 ");
        /* Ne-am folosit de mai multe variabile pentru a transforma MSK_1 dintr-o
        compozitie de 4 variabile, fiecare pe un octet, intr-un int pe 32 de biti. */
        unsigned int p = 31, j, k, l, m, n, NRM = 0, ok, T;
        for (j = 8; j >= 1; j--) {
            if ((M1[q] & (1 << (j - 1))) != 0) {
                NRM = NRM + (1 << p);
                p--;
            } else {
                p--;
            }
        }
        for (k = 8; k >= 1; k--) {
            if ((M2[q] & (1 << (k - 1))) != 0) {
                NRM = NRM + (1 << p);
                p--;
            } else {
                p--;
            }
        }
        for (l = 8; l >= 1; l--) {
            if ((M3[q] & (1 << (l - 1))) != 0) {
                NRM = NRM + (1 << p);
                p--;
            } else {
                p--;
            }
        }
        for (m = 8; m >= 1; m--) {
            if ((M4[q] & (1 << (m - 1))) != 0) {
                NRM = NRM + (1 << p);
                p--;
            } else {
                p--;
            }
        }
        ok = 0;
        p = 31;
        while ((NRM & (1 << p)) != 0) {
            if (p == 0) {
                break;
            }
            p--;
        }
        if (p != 0) {
            for (n = p; n >= 1; n--) {
                if (((NRM & (1 << (n - 1))) != 0)) {
                    ok++;
                    break;
                }
            }
            if (ok != 0) {
                printf("NU\n");
                T = 0;
            } else {
                printf("DA\n");
                T = 1;
            }
        } else {
            printf("DA\n");
            T = 1;
        }
        // Task 7
        // Variabila T reprezinta valoarea de adevar a taskului precedent.
        printf("-7 ");
        if (T == 1) {
            printf("%hhu.%hhu.%hhu.%hhu\n", M1[q], M2[q], M3[q], M4[q]);
        } else {
            unsigned int MASK1 = 0;
            unsigned char MASK11, MASK12, MASK13, MASK14;
            p = 31;
            while ((NRM & (1 << p)) != 0) {
                MASK1 = MASK1 + (1 << p);
                if (p == 0) {
                    break;
                }
                p--;
            }
            MASK11 = MASK1 >> 24;
            MASK12 = MASK1 >> 16;
            MASK13 = MASK1 >> 8;
            MASK14 = MASK1;
            printf("%hhu.%hhu.%hhu.%hhu\n", MASK11, MASK12, MASK13, MASK14);
        }
        // Task 8
        printf("-8 ");
        for (j = 8; j >= 1; j--) {
            printf("%hhu", (I11[q] & (1 << (j - 1))) != 0);
        }
        printf(".");
        for (k = 8; k >= 1; k--) {
            printf("%hhu", (I12[q] & (1 << (k - 1))) != 0);
        }
        printf(".");
        for (l = 8; l >= 1; l--) {
            printf("%hhu", (I13[q] & (1 << (l - 1))) != 0);
        }
        printf(".");
        for (m = 8; m >= 1; m--) {
            printf("%hhu", (I14[q] & (1 << (m - 1))) != 0);
        }
        printf("\n");
        // Task 9
        printf("-9 ");
        for (i = 1; i <= N[q]; i++) {
            unsigned int var1 = 0;
            unsigned char z, MASKNET1, MASKNET2, MASKNET3, MASKNET4;
            if (MSKNET[i][q] == 0) {
                MASKNET1 = 0;
                MASKNET2 = 0;
                MASKNET3 = 0;
                MASKNET4 = 0;
            } else if (MSKNET[i][q] == 32) {
                MASKNET1 = 255;
                MASKNET2 = 255;
                MASKNET3 = 255;
                MASKNET4 = 255;
            } else {
                for (z = 1; z <= (32 - MSKNET[i][q]); z++) {
                    var1 = var1 + (1 << (z - 1));
                }
                MASKNET1 = (~var1) >> 24;
                MASKNET2 = (~var1) >> 16;
                MASKNET3 = (~var1) >> 8;
                MASKNET4 = (~var1);
            }
            if ((I21[q] & MASKNET1) == (NET1[i][q] & MASKNET1)) {
                if ((I22[q] & MASKNET2) == (NET2[i][q] & MASKNET2)) {
                    if ((I23[q] & MASKNET3) == (NET3[i][q] & MASKNET3)) {
                        if ((I24[q] & MASKNET4) == (NET4[i][q] & MASKNET4)) {
                            printf("%d ", (i - 1));
                        }
                    }
                }
            }
        }
        printf("\n");
    }
    return 0;
}
