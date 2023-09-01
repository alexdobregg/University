#include <stdio.h>
#include <stdlib.h>

// Implementare Hanoi exercitiul 1.
void hanoi(int n, char a, char b, char c) {
	if (n == 1) {
		printf("Muta discul %d de pe %c pe %c\n", n, a, b);
	} else {
		hanoi(n - 1, a, c, b);
		printf("Muta discul %d de pe %c pe %c\n", n, a, b);
		hanoi(n - 1, c, b, a);
	}
}

// Implementare gasire element maxim dintr-un vector exercitiul 2.
int findMaxElement(int *v, int start, int sfarsit) {
	if (start == sfarsit) {
		return v[start];
	} else {
		int mijl = start + (sfarsit - start) / 2;
		int maxSt = findMaxElement(v, start, mijl);
		int maxDr = findMaxElement(v, mijl + 1, sfarsit);
		return (maxSt > maxDr) ? maxSt : maxDr;
	}

}

// Implementare merge-sort exercitiul 3.
void merge(int *v, int start, int mijl, int sfarsit) {
	int i, j, k;
	int n1 = mijl - start + 1;
	int n2 = sfarsit - mijl;

	int S[n1], D[n2];

	for (i = 0; i < n1; i++) {
		S[i] = v[start + i];
	}

	for (j = 0; j < n2; j++) {
		D[j] = v[mijl + j + 1];
	}

	i = j = 0;
	k = start;

	while (i < n1 && j < n2) {
		if (S[i] <= D[j]) {
			v[k] = S[i++];
		} else {
			v[k] = D[j++];
		}
		k++;
	}

	while (i < n1) {
		v[k++] = S[i++];
	}
	while(j < n2) {
		v[k++] = D[j++];
	}
}


void merge_sort(int *v, int start, int sfarsit) {
	if (start < sfarsit) {
		int mijl = start + (sfarsit - start) / 2;
		merge_sort(v, start, mijl);
		merge_sort(v, mijl + 1, sfarsit);
		merge(v, start, mijl, sfarsit);
	}
} 

// Implementare binary search exercitiul 4.
int binary_search(int *v, int x, int start, int sfarsit) {
	if (start > sfarsit) {
		return -1;
	} else {
		int mijl = start + (sfarsit - start) / 2;
		if (v[mijl] == x) {
			return mijl;
		} else if (v[mijl] > x) {
			binary_search(v, x, start, mijl);
		} else {
			binary_search(v, x, mijl + 1, sfarsit);
		}
	}
}

// Implementare numar aparitii intr-un vector nesortat exercitiul 5.
void aparitii(int *v, int x, int start, int sfarsit, int *nrAparitii) {
	if (start == sfarsit) {
		if (v[start] == x) {
			(*nrAparitii)++;
		}
	} else {
		int mijl = start + (sfarsit - start) / 2;
		aparitii(v, x, start, mijl, nrAparitii);
		aparitii(v, x, mijl + 1, sfarsit, nrAparitii);
	}
}


int main() {

	printf("Hanoi: \n");
	hanoi(3, 'A', 'B', 'C');

	int v[] = {3, 2, 5, 2, 4};
	printf("\nElementul maxim dintr-un vector:\n%d\n", findMaxElement(v, 0, 4));

	int v2[] = {3, 2, 1, 4, 3, 5, 2};
	printf("\nMerge-Sort: \n");
	merge_sort(v2, 0, 6);
	for (int i = 0; i < 7; i++) {
		printf("%d ", v2[i]);	
	}

	int v3[] = {1, 3, 4, 5, 7, 8, 9};
	printf("\n\nBinary Search: \nIndexul numarului este: %d\n", binary_search(v3, 3, 0, 6));

	int v4[] = {2, 1, 3, 1, 1, 4, 2, 1, 8, 1};
	int nrAparitii = 0;
	aparitii(v4, 1, 0, 9, &nrAparitii);
	printf("\nAparitii: \n%d\n", nrAparitii);


	return 0;
}