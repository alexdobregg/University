#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;


// A class named StocksValue which contains currentValue, minLoss
// (currentValue - minValue), maxWin(maxValue - currentValue).
class StocksValue {
 public:
    int currentValue;
    int minLoss;
    int maxWin;
};

// A function to return the max between 2 ints.
int max(int a, int b) {
    return (a < b) ? b : a;
}

// This is an adaptation of the knapsack problem solved with dynamic
// programming.
int stocksCalculator(StocksValue *stocks, int counter, int B, int L) {
    // We need a 3D array: 1 because of our stocks vector and it's elements,
    // 2 for the currentValue and 3 for minLoss.

    // We allocate it with counter + 1, B + 1 and L + 1.
    int ***dp = new int**[counter + 1];
    for (int i = 0; i < counter + 1; i++) {
        dp[i] = new int*[B + 1];
        for (int j = 0; j < B + 1; j++) {
            dp[i][j] = new int[L + 1];
        }
    }
    // For each element given.
    for (int i = 0; i <= counter; i++) {
        // For each possible currentValue.
        for (int j = 0; j <= B; j++) {
            // For each possible minLoss.
            for (int k = 0; k <= L; k++) {
                // We put 0 if one of the indices is 0.
                if (i == 0 || j == 0 || k == 0) {
                    dp[i][j][k] = 0;
                } else if (j >= stocks[i - 1].currentValue
                            && k >= stocks[i - 1].minLoss) {
                    // The positions.
                    int currJ = j - stocks[i - 1].currentValue;
                    int currK = k - stocks[i - 1].minLoss;
                    // Calculating an auxiliary solution.
                    int sol_aux = dp[i - 1][currJ][currK] +
                                     stocks[i - 1].maxWin;
                    dp[i][j][k] = max(dp[i - 1][j][k], sol_aux);
                } else {
                    dp[i][j][k] = dp[i - 1][j][k];
                }
            }
        }
    }
    // Saving the solution in a variable.
    int sol = dp[counter][B][L];
    // Deleting the memory allocated for the 3D array.
    for (int i = 0; i < counter + 1; i++) {
        for (int j = 0; j < B + 1; j++) {
            delete [] dp[i][j];
        }
        delete [] dp[i];
    }
    delete [] dp;
    return sol;
}

int main() {
    // Opening the input and output files.
    ifstream inputFile;
    inputFile.open("stocks.in");
    ofstream outputFile;
    outputFile.open("stocks.out");

    // Reading the data from the input.
    int N, B, L, counter = 0;
    int currVal, minVal, maxVal;
    inputFile >> N >> B >> L;
    StocksValue *stocks = new StocksValue[N];
    for (int i = 0; i < N; i++) {
        inputFile >> currVal >> minVal >> maxVal;
        // Filtering the stocks, keeping the good ones.
        if (currVal > B || (currVal - minVal) > L) {
            continue;
        } else {
            stocks[counter].currentValue = currVal;
            stocks[counter].minLoss = currVal - minVal;
            stocks[counter].maxWin = maxVal - currVal;
            counter++;
        }
    }

    // Calling the function stocksCalculator which gives us the answer.
    if (counter == 0) {
        outputFile << 0;
    } else {
        outputFile << stocksCalculator(stocks, counter, B, L);
    }

    // Closing the input and output files and deleting the memory allocated
    // for stocks.
    delete [] stocks;
    inputFile.close();
    outputFile.close();
    return 0;
}
