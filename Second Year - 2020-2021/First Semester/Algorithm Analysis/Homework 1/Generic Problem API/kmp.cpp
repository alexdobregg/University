#include "algo.h"
 
void computeLPSArray(string pat, int patlen, int* lps) { 
    // Lungimea celui mai lung prefix sufix anterior
    int len = 0; 
    // lps[0] va fi intotdeauna 0
    lps[0] = 0; 
    // Este calculat lps[i] pentru i = 1 : patlen
    int i = 1; 
    while (i < patlen) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i++] = len; 
        } else { 
            if (len != 0) { 
                len = lps[len - 1]; 
            } else { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 

void KMPSearch(string pat, string txt, std::ofstream& output) { 
    int patlen = pat.length(); 
    int txtlen = txt.length(); 
    /* Este initializat si preprocesat lps[] care va contine cele mai lungi
    prefixe sufixe valori pentru pattern-ul nostru. */ 
    int lps[patlen]; 
    computeLPSArray(pat, patlen, lps);
    // Indexul pentru txt.
    int i = 0; 
    // Indexul pentru pat.
    int j = 0;
    int count = 0;
    while (i < txtlen) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
        if (j == patlen) {
            output << "Found pattern " << "'" << pat << "'" <<
                         " at index " << i - j << endl;
            j = lps[j - 1];
            count++;
        } else if (i < txtlen && pat[j] != txt[i]) { 
            if (j != 0) {
                j = lps[j - 1]; 
            } else {
                i = i + 1;
            }
        } 
    }
    if (count == 0) {
        output << "The pattern was not found in the text." << endl;
    }
}