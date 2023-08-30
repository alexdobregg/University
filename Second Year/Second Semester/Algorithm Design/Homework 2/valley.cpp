#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;

// A function to find the index of the minimum value in
// the heights array.
int findMinIdx(int *heights, int N) {
    int min = *min_element(heights, heights + N);
    int idx;
    for (int i = 0; i < N; i++) {
        if (heights[i] == min) {
            idx = i;
            break;
        }
    }
    return idx;
}

long long excavate(int *heights, int N) {
    // We obtain the index of the minimum from our heights array.
    int minIdx = findMinIdx(heights, N);
    // After we've found our index, we need to form the valley.
    // and calculate the number of hours the process is taking.
    long long hours = 0;

    // We need to form a desceding array, going from the right to
    // our index. For that, we compare heights[i], which starts from N - 1,
    // with heights[i - 1]. If the one with the index equal to i - 1 is
    // greater, then we add the number of hours we need to make it equal to
    // the one with the index equal to i, and we do so. If not, we continue.
    for (int i = N - 1; i > minIdx; i--) {
        if (heights[i] < heights[i - 1]) {
            hours += heights[i - 1] - heights[i];
            heights[i - 1] = heights[i];
        }
    }

    // We need to form a desceding array, going from the left to our
    // index. For that, we compare heights[i], which starts from 1,
    // with heights[i - 1]. If the one with the index equal to i is
    // greater, then we add the number of hours we need to make it equal to
    // the one with the index equal to i - 1, and we do so. If not, we
    // continue.
    for (int i = 1; i < minIdx; i++) {
        if (heights[i] > heights[i - 1]) {
            hours += heights[i] - heights[i - 1];
            heights[i] = heights[i - 1];
        }
    }

    // This is a special case, one like 7 8 3. To make a valley, we need to
    // excavate 5 from the second one, so it is 7 3 3. This condition covers
    // the case when the min is the last one.
    if (minIdx == N - 1) {
        hours += heights[minIdx - 1] - heights[minIdx];
        heights[minIdx - 1] = heights[minIdx];
    }

    // This is a special case, one like 1 2 3. To make a valley, we need to
    // excavate 1 from the second one, so it is 1 1 3. This condition covers
    // the case when the min is the first one.
    if (minIdx == 0) {
        hours += heights[minIdx + 1] - heights[minIdx];
        heights[minIdx + 1] = heights[minIdx];
    }

    return hours;
}


int main() {
    // Opening the input and output files.
    ifstream inputFile;
    inputFile.open("valley.in");
    ofstream outputFile;
    outputFile.open("valley.out");

    // Reading the data from the input.
    int N;
    inputFile >> N;
    int *heights = new int[N];
    for (int i = 0; i < N; i++) {
        inputFile >> heights[i];
    }

    outputFile << excavate(heights, N);
    // Closing the input and output files and deleting the memory allocated
    // for heights.
    delete [] heights;
    inputFile.close();
    outputFile.close();
    return 0;
}
