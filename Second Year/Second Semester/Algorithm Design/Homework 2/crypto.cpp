#include <bits/stdc++.h>
#include <iostream>
#include <fstream>

using namespace std;

// A class named Proc which contains P (processors / how many crypto coins a PC
// can produce) and U (the cost of an upgrade by 1 processor).
class Proc {
 public:
    int P;
    int U;
};

// A comparing function used by the function sort to sort the elements in an
// ascending order by their number of processors
bool compareFunc(Proc proc1, Proc proc2) {
    return (proc1.P < proc2.P);
}

int cryptoCoins(Proc *procs, int N, int B) {
    int i, sums, count, dif;
    sums = 0;
    // Sorting the procs vector.
    sort(procs, procs + N, compareFunc);

    // Going through the vector procs to calculate how many crypto coins are
    // the PCs going to produce.
    for (i = 1; i < N; i++) {
        // At every step we add the upgrade price of a previous computer.
        sums += procs[i - 1].U;
        // We check if the current Pc has more processors than the previous.
        dif = procs[i].P - procs[i - 1].P;
        // We continue if it doesn't.
        if (dif == 0) {
            continue;
        }
        // We calculate how many times we can upgrade all the previous PCs.
        count = B / sums;
        // If we can't then it stops.
        if (count == 0) {
            break;
        }
        // If count != 0, then we proceed to see if we can upgrade all the PCs
        // until they can produce the same amount of crypto coins as the
        // current one.
        if (count <= dif) {
            // If count <= dif, then we can, at maximum, reach the number of
            // crypto coins produced by the current PC so we upgrade them as
            // much as we can, and then stop.
            procs[i - 1].P += count;
            B -= sums * count;
            break;
        } else {
            // If count > dif we can reach the number of crypto coins produced
            // by the current PC and there is also room for a better upgrade so
            // we continue.
            procs[i - 1].P += dif;
            B -= sums * dif;
        }
    }
    // If after all the upgrades we reach the last computer and we still have
    // money in out budget, we check to see if we can make another upgrade. If
    // we can, then all the PCs receive an upgrade.
    if (B >= 0 && i == N) {
        procs[i - 1].P += B / (sums + procs[i - 1].U);
    }
    return procs[i - 1].P;
}

int main() {
    // Opening the input and output files.
    ifstream inputFile;
    inputFile.open("crypto.in");
    ofstream outputFile;
    outputFile.open("crypto.out");

    // Reading the data from the input.
    int N, B;
    inputFile >> N >> B;
    Proc *procs = new Proc[N];
    for (int i = 0; i < N; i++) {
        inputFile >> procs[i].P >> procs[i].U;
    }

    // Calling the function cryptoCoins which gives us the answer.
    outputFile << cryptoCoins(procs, N, B);

    // Closing the input and output files and deleting the memory allocated
    // for procs.
    delete [] procs;
    inputFile.close();
    outputFile.close();
    return 0;
}
