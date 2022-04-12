#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <utility>
#include <string>
using namespace std;


/**
 * Write Identity Matrix to input array 
 * 
 * @param a     1 dimensional square matrix to store the identity matrix
 * @param dim   integer specifying the dimension of the identity matrix (dim * dim)
 */
void write_identity(float *a, int dim) {
    for (int i = 0; i < dim; i++) {
        a[i*dim + i] = 1;
    }
}

/**
 * Given a matrix, add row j to row i with an optional multiple  
 * 
 * @param a     input matrix to perform the row operation on
 * @param dim   integer specifying the dimension of the square matrix (dim * dim)
 * @param i     the row index that will be updated 
 * @param j     the row index that will be added 
 * @param mult  optional multiple s.t. row i' = row i + mult * row j
 */
void add_rows(float *a, int dim, int i, int j, float mult=1.0) {
    for (int idx = 0; idx < dim; idx++) {
        a[i*dim + idx] += mult * a[j*dim + idx];
    }
}

/**
 * Calculate the inverse of a matrix using Gauss-Jordan method   
 * 
 * @param a     input matrix to perform Gaussian Elimination on
 * @param res   inverse matrix of the original matrix a generated from row operations
 * @param dim   integer specifying the dimension of the matrices (dim * dim)
 * 
 * Complexity O(n^3)
 */
void gauss_jordan(float *a, float *res, int dim) {
    write_identity(res);

    for (int i = 0; i < dim; i++) {
        if (a[i*dim + i] == 0) {
            // if the column we want to zero out is already zero, add a row with non-zero column to current row
            for (int r = i+1; r < dim; r++) {
                if (a[r*dim + i] != 0) {
                    // our requirement on a being an invertible matrix means such row must exist
                    add_rows(a, dim, i, r);
                    add_rows(res, dim, i, r);
                    break;
                }
            }
        }

        // update current row such that the leftmost non-zero column is 1 
        // total 2n updates
        float divider = a[i*dim + i];
        for (int j = 0; j < dim; j++) {
            a[i*dim + j] /= divider;
            res[i*dim + j] /= divider;
        }

        // using current row, zero out all this column for all other rows
        for (int r = 0; r < dim; r++) {
            if (r != i) {
                add_rows(a, dim, r, i, (-1)*divider);
                add_rows(res, dim, r, i, (-1)*divider);
            }
        }
    }
}

int main(int argc, const char *argv[]) {
    return 0;
}