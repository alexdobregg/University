#include "algo.h"
#include "kmp.cpp"
#include "boyermoore.cpp"
#include <iostream>
#include <fstream>
#define NUMBER_OF_TESTS 8

int main () {
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        // Este creat path-ul catre fisierele de input si catre folderul de output.
        string inpath = "./in/test";
        string outpath = "./out/test";
        char c = i + '0';
        string s;
        s = c;
        inpath.append(s + ".in");
        outpath.append(s + ".out");
        // Este deschis atat fisierul de input, cat si cel de output.
        ifstream input(inpath);
        ofstream output(outpath);
        // O variabila in care salvam fiecare linie.
        char* data = (char *) calloc(80, sizeof(char));
        // Txt este textul in care dorim sa cautam un pattern pat.
        string txt, pat;
        while (input.getline(data, 80)) {
            if (data[0] == '*') {
                break;
            }
            txt.append(data);
        }
        while (input.getline(data, 80)) {
            pat.append(data);
        }
        if (pat.length() == 0 || txt.length() == 0) {
            output << "There is no text/pattern in the input file.";
        } else {
            /* Apelul functie KMPSearch si scrierea in fisierul de output al
            rezultatului. */
            output << "KMPSearch TEST: " << endl << endl;
            auto start = high_resolution_clock::now();
            KMPSearch(pat, txt, output);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            output << "Time taken by KMPSearch: "
                << duration.count() << " microseconds" << endl;
            /* Apelul functie BoyerMooreSearch si scrierea in fisierul de output al
            rezultatului. */
            output << endl << "BoyerMooreSearch TEST: " << endl << endl;
            auto start2 = high_resolution_clock::now();
            BoyerMooreSearch(pat, txt, output);
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<microseconds>(stop2 - start2);
            output << "Time taken by BoyerMooreSearch: "
                << duration2.count() << " microseconds" << endl;
        }
        // Inchidem fisierele si eliberam memoria alocata.
        input.close();
        output.close();
        free(data);
    }
    return 0;
}
