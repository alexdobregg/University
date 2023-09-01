#include <stdio.h>
#include <stdlib.h>

// Problema 2: Problema Rucsacului
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void sort(int *m, int *v, int n)
{
    	
    int i, key[n], j, aux;
    for (i = 0; i < n; i++) {
    	key[i] = v[i] / m[i];
    }
    for (i = 0; i < n - 1; i++) {
    	for (j = 0; j < n - i - 1; j++) {
    		if (key[j] < key[j + 1]) {
    			swap(&m[j], &m[j + 1]);
    			swap(&key[j], &key[j + 1]);
    			swap(&v[j], &v[j + 1]);
    		}
    	}
    }
}

int knapSack(int M, int *m, int *v, int n) {
	sort(m, v, n);
	int currM = 0;
	int currV = 0;
	printf("Continutul rucsacului:\n");
	for (int i = 0; i < n; i++) {
		if (currM + m[i] <= M) {
			currM += m[i];
			currV += v[i];
			printf("* Obiectul %d cu masa %d si valoare %d.\n", i + 1, m[i], v[i]);
		} else {
			int rest = M - currM;
			currV += v[i] * ((double) rest / (double) m[i]);
			printf("* Obiectul %d cu masa %d din %d (%f din 1) si valoarea %d din %d.\n", i + 1, rest, m[i], (double) rest / m[i], v[i] * rest / m[i], v[i]);
		}
	}
	return currV;
}

// Problema 3: Problema spectacolelor

void sortSpect(int *s, int *t, int n) {
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (t[j] > t[j + 1]) {
				swap(&s[j], &s[j + 1]);
				swap(&t[j], &t[j + 1]);
			}
		}
	}
}

void spect(int *s, int *t, int n) {
	sortSpect(s, t, n);
	int tPrev = 0;
	for (int i = 0; i < n; i++) {
		if (s[i] >= tPrev) {
			printf("Spectacolul cu numarul %d va avea loc -> Ora inceput: %d; Ora sfarsit: %d.\n", i + 1, s[i], t[i]);
			tPrev = t[i];
		}
	}
	
}

int main() {
	int m[] = {10, 30, 20};
	int v[] = {60, 120, 100};
	int n = 3;
	int M = 50;
	printf("1.PROBLEMA RUCSACULUI: \n\n");
	printf("Valoare finala a obiectelor care incap intr-un rucsac de %d este %d.\n\n", M, knapSack(M, m, v, n));

	int s[] = {10, 11, 12, 14, 16, 17};
	int t[] = {11, 13, 13, 18, 17, 19};
	int n2 = 6;
	printf("3.PROBLEMA SPECTACOLELOR: \n\n");
	spect(s, t, n2);

	return 0;
}
