// Dobre Gigi-Alexandru 322CD

#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define N 10000

static int write_stdout(const char *token, int length) {
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0) {
			return rc;
		}
		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

// Functie pentru a schimba 2 caractere intre ele.
void swap(char *x, char *y) {
	char t = *x;
	*x = *y;
	*y = t;
}


// Itoa() implementat iterativ, baza 10, %d.
char* itoa(int value, char* buffer) {
	// Se considera valoarea absoluta a lui n.
	int num = abs(value);
	int i = 0, j = 0;;
	int base = 10;
	while (num) {
		int r = num % base;
		if (r < 10) {
			buffer[i++] = r + 48;
		} else {
			buffer[i++] = (r - 10) + 65;
		}
		num /= base;
	}
	if (i == 0) {
		buffer[i++] = '0';
	}
	/* Daca valorea este negativa, iar in sirul format avem %d, 
	atunci vom pune un minus in fata. */
	if (value < 0) {
		buffer[i++] = '-';
	}
	buffer[i--] = '\0';
	// Se inverseaza stringul.
	while (j < i) {
		swap(&buffer[j++], &buffer[i--]);
	}
	return buffer;
}

// Utoa implementat iterativ pentru unsigned si hexa.
char *utoa(unsigned int num, char *buffer, int base) {
    int i = 0, j = 0;
    // Se construieste sirul inversat din numar.
    while (num) {
        int r = num % base;
        if (r < 10) {
            buffer[i++] = r + 48;
		}
        else {
            buffer[i++] = (r - 10) + 97;
		}
        num /= base;
    }
	if (i == 0) {
		buffer[i++] = '0';
	}
	buffer[i--] = '\0';
    // Inversam sirul pentru a-l returna.
    while (j < i) {
        swap(&buffer[j++], &buffer[i--]);
    }
    return buffer;
}


int iocla_printf(const char *format, ...) {
	// Un iterator pentru a trece prin fiecare caracter al sirului.
	char *iterator = (char *) format;
	// Count pentru a returna cate caractare au fost printate.
	int count = 0;
	// Variabila dI este in care este scos argumentul pentru %d.
	int dI;
	// Variabila uI este in care este scos argumentul pentru %u.
	unsigned int uI;
	// Variabila hex in care este scos argumentul pentru %x.
	int hex;
	// Sirul string in care este scos argumentul pentru %s.
	char *sI;
	// Variabila cI in care este scos argumentul pentru %c.
	int cI;
	/* Am alocat un sir cu un N destul de mare pentru transforma
	toate variabilele de mai sus in string, urmand  a
	putea fi trimise ca parametru functiei write_stdout. */
	char *string = (char *) calloc(N, sizeof(char));
	va_list arg;
	va_start(arg, format);
	// Iteram prin fiecare caracter din sirul primit ca parametru.
	for (iterator = (char *) format; *iterator != '\0'; iterator++) {
		// Printam caracterele pana cand se intalneste % sau finalul sirului.
		while (*iterator != '%' && *iterator != '\0') {
			string[0] = *iterator;
			string[1] = '\0';
			write_stdout(string, strlen(string));
			count++;
			iterator++;
		}
		// Verificam daca am ajuns la finalul sirului, caz in care ne oprim.
		if (*iterator == '\0') {
			break;
		}
		/* Verificam daca sunt 2 % consecutive pentru a printa unul dintre ele.
		Acest lucru se aplica si daca sunt mai multe, se va continua executia
		programului deoarece instructiunile de mai jos nu vor fi rulate si va fi
		verificat iar daca sunt 2 % consecutive. */
		if (*iterator == '%' && *(iterator + 1) == '%') {
			string[0] = *iterator;
			string[1] = '\0';
			write_stdout(string, strlen(string));
			count++;
		}
		// Incrementam iteratorul si verificam daca am ajuns la finalul sirului.
		iterator++;
		if (*iterator == '\0') {
			break;
		}
		// Verificam tipului argumentului pentru a-l printa in mod corespunzator.
		if (*iterator == 'd') {
			dI = va_arg(arg, int);
			/* Este tratat cazul in care avem limita inferioara pentru a putea
			evita depasirea acesteia. */
			if (dI == INT_MIN) {
				dI++;
			}
			itoa(dI, string);
			if (dI == INT_MIN + 1) {
				string[strlen(string) - 1] = '8';
			}
			count += strlen(string);
			write_stdout(string, strlen(string));
		} else if (*iterator == 'u') {
			uI = va_arg(arg, unsigned int);
			utoa(uI, string, 10);
			count+= strlen(string);
			write_stdout(string, strlen(string));
		} else if (*iterator == 'x') {
			hex = va_arg(arg, int);
			utoa(hex, string, 16);
			count += strlen(string);
			write_stdout(string, strlen(string));
		} else if (*iterator == 'c') {
			// Il extragem drept int.
			cI = va_arg(arg, int);
			// Facem conversie in char.
			string[0] = cI;
			string[1] = '\0';
			count++;
			write_stdout(string, strlen(string));
		} else if (*iterator == 's') {
			sI = va_arg(arg, char *);
			count += strlen(sI);
			write_stdout(sI, strlen(sI));
		}
	}
	// Eliberam memoria pentru a nu avea leak-uri.
	free(string);
	va_end(arg);
	return count;
}