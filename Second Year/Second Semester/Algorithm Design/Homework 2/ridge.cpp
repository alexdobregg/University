#include <bits/stdc++.h>
#include <limits>
#include <iostream>
#include <fstream>
using namespace std;

// A function to return the min between 2 long longs.
long long min(long long a, long long b) {
    return (a < b) ? a : b;
}

// This is an adaptation of the Minimize the cost to make all the adjacent
// elements distinct in an array problem solved with dynamic programming.
long long excavate(int *heights, int *costs, int N) {
    // An element might need to be decreased maximum twice.
    long long **dp = new long long*[N];
    for (int i = 0; i < N; i++) {
        dp[i] = new long long[3];
    }
    // Not decreasing the first element.
    dp[0][0] = 0;
    // Decreasing the first element by 1.
    dp[0][1] = costs[0];
    // Decreasing the first element by 2.
    dp[0][2] = costs[0] * 2;

    for (int i = 1; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            long long  minimum = LLONG_MAX;
            // Condition if previous element is >= than 0 or the current
            // element is not equal to the previous non-decreased element.
            if ((heights[i - 1] >= 0) && (heights[i] - j != heights[i - 1])) {
                minimum = min(minimum, dp[i - 1][0]);
            }
            // Condition if previous decreased element by 1 is >= than 0 or
            // the current element is not equal to the previous element
            // after being decreased by 1.
            if ((heights[i - 1] - 1 >= 0) && (heights[i] - j !=
                                            heights[i - 1] - 1)) {
                minimum = min(minimum, dp[i - 1][1]);
            }
            // Condition if previous decreased element by 2 is >= than 0 or
            // the current element is not equal to the previous element
            // after being decreased by 2.
            if ((heights[i - 1] - 2 >= 0) && (heights[i] - j !=
                                            heights[i - 1] - 2)) {
                minimum = min(minimum, dp[i - 1][2]);
            }
            // Keeping the minimum from all cases.
            dp[i][j] = j * costs[i] + minimum;
        }
    }
    // Obtaining the minimum cost.
    long long ans = LLONG_MAX;
    for (int i = 0; i < 3; i++) {
        ans = min(ans, dp[N - 1][i]);
    }
    for (int i = 0; i < N; i++) {
        delete [] dp[i];
    }
    delete [] dp;
    return ans;
}


int main() {
    // Opening the input and output files.
    ifstream inputFile;
    inputFile.open("ridge.in");
    ofstream outputFile;
    outputFile.open("ridge.out");

    // Reading the data from the input.
    int N;
    inputFile >> N;
    int *heights = new int[N];
    int *costs = new int[N];
    for (int i = 0; i < N; i++) {
        inputFile >> heights[i] >> costs[i];
    }

    // Calling the function excavate which gives us the answer.
    outputFile << excavate(heights, costs, N);

    // Closing the input and output files and deleting the memory allocated
    // for heights and costs.
    delete [] heights;
    delete [] costs;
    inputFile.close();
    outputFile.close();
    return 0;
}
