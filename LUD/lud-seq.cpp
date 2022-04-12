#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <utility>
#include <string>
using namespace std;


void lu_d(float* a, float* l, float* u, int dim) {
    // iterate across column
    for (int i = 0; i < dim; i++) {
        // iterate across row 
        for (int j = 0; j < dim; j++) {
            // Lower Triangular Matrix
            l[j*dim + i] = j < i ? 0 : a[j*dim + i];
            for (int k = 0; k < i ; k++) {
                l[j*dim + i] -= l[j*dim + k] * u[k*dim + i];
            }
        }

        // iterate across row 
        for (int j = 0; j < dim; j++) {
            // Upper Triangular Matrix
            if (j == i) {
                u[i * dim + j] = 1;
            } else if (j < i ) {
                u[i * dim + j] = 0;
            } else {
                u[i * dim + j] = a[i * dim + j] / l[i * dim + i];
                for (int k = 0; k < i ; k++) {
                    u[i * dim + j] -= l[i * dim + k] * u[k * dim + j] / l[i* dim + i];
                }
            } 
        }
    }
}


int main (int argc, const char *argv[]) {

    return 0;
}