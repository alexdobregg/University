#include "algo.h"

void badCharHeuristic(string pat, int patlen, int badchar[NO_OF_CHARS]) {
    int i;
    // Initializam toate aparitiile cu -1.
    for (i = 0; i < NO_OF_CHARS; i ++) {
        badchar[i] = -1;
    }
    // Inlocuim valoarea ultimei aparitii a unui caracter.
    for (i = 0; i < patlen; i++) {
        badchar[(int) pat[i]] = i;
    }
}

void BoyerMooreSearch(string pat, string txt, std::ofstream& output) {
    int patlen = pat.length();
    int txtlen = txt.length();
    /* Este initializat vectorul de badchar[] si este preprocesat pentru
    pattern-ul dat. */
    int badchar[NO_OF_CHARS];
    badCharHeuristic(pat, patlen, badchar);
    // s este shiftarea pattern-ului in conformitate cu textul.
    int s = 0;
    int count = 0;
    while (s <= (txtlen - patlen)) {
        int j = patlen - 1;
        // Cat timp caracterele fac match la shift-ul s, se decrementeaza j.
        while (j >= 0 && pat[j] == txt[s + j]) {
            j--;
        }
        // Daca am gasit pattern-ul, j va fi -1 la terminarea while-ului anterior.
        if (j == -1) {
            output << "Found pattern " << "'" << pat << "'" <<
                         " at index " << s << endl;
            count++;
            if (s + patlen < txtlen) {
                s += patlen - badchar[(int) txt[s + patlen]];
            } else {
                s += 1;
            }
        } else {
            s += max(1, j - badchar[(int) txt[s + j]]);
        }
    }
    if (count == 0) {
        output << "The pattern was not found in the text." << endl;
    }
}
