#include <iostream>
#include "RNA.h"
#include <ctime>

using namespace std;

int main() {
    RNA rna2;
    RNA rna1;

    struct timespec endTotalTime, startTotalTime;

    clock_gettime(CLOCK_MONOTONIC_RAW, &startTotalTime);

    for(int i = 0; i < 10000000; i++){rna1.addNucleotide(T);}

    clock_gettime(CLOCK_MONOTONIC_RAW, &endTotalTime);

    cout << endTotalTime.tv_sec - startTotalTime.tv_sec + (0.000000001)*(endTotalTime.tv_nsec-startTotalTime.tv_nsec);

    return 0;
}
