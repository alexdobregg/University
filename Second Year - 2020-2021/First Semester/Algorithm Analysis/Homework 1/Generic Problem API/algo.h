#ifndef ALGO_H
#define ALGO_H
#define NO_OF_CHARS 256
#include <bits/stdc++.h>
using namespace std::chrono;
using namespace std;

void KMPSearch(string pat, string txt, std::ofstream& output);
void BoyerMooresearch(string pat, string txt, std::ofstream& output);
void computeLPSArray(string pat, int patlen, int* lps);
void badCharHeuristic(string pat, int patlen, int badchar[NO_OF_CHARS]);

#endif
