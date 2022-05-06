#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <chrono>

#include <utility>
#include <string>

using namespace std;

#define N 16					//size of the matrix 
double M[N * N];
double L[N * N];
double U[N * N];

/* Implement the sequential version */
void lu_d(double *M, double *L, double *U, int dim) {
    // iterate across column
    for (int i = 0; i < dim; i++) {
        // iterate across row 
        for (int j = 0; j < dim; j++) {
            // Lower Triangular Matrix
            L[j*dim + i] = j < i ? 0 : M[j*dim + i];
            for (int k = 0; k < i ; k++) {
                L[j*dim + i] -= L[j*dim + k] * U[k*dim + i];
            }
        }
        // iterate across row 
        for (int j = 0; j < dim; j++) {
            // Upper Triangular Matrix
            if (j == i) {
                U[i * dim + j] = 1;
            } else if (j < i ) {
                U[i * dim + j] = 0;
            } else {
                U[i * dim + j] = M[i * dim + j] / L[i * dim + i];
                for (int k = 0; k < i ; k++) {
                    U[i * dim + j] -= L[i * dim + k] * U[k * dim + j] / L[i* dim + i];
                }
            } 
        }
    }
}
