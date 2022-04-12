#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <omp.h>

#include <utility>
#include <string>
using namespace std;

/**
 * Write Identity Matrix to input array 
 * 
 * @param a     1 dimensional square matrix to store the identity matrix
 * @param dim   integer specifying the dimension of the identity matrix (dim * dim)
 * 
 * Parallelization:
 * assuming matrix a iniitlaized to 0 matrix, each row writes can be done in parallel 
 */
void write_identity(float *a, int dim) {
    #pragma omp parallel for
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
 * 
 * Parallelization:
 * Each column on the specified row can be safely updated in parallel 
 */
void add_rows(float *a, int dim, int i, int j, float mult) {
    #pragma omp parallel for
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
 * Complexity
 * Work: O(n^3)
 * Span: O(n)
 * 
 * Parallelization:
 * We can parallelize the updates on the given row when we try to convert the column of interest to 1
 * (further improvement: can improve work from 2n to n since all elements before col of interest are 0
 *  and the result matrix has only n-col updates needed)
 * We can also parallelize the reduction of all other rows' column to 0 
 * 
 * Note: though parallelization in this approach can achieve significant speedup given sufficient 
 * hardware resources, but performance is likely to suffer when dealing with extremely large matrices. 
 * We believe Gauss-Jordan done through block recursion will perform much better in those cases, which 
 * motivates our future explorations.
 */
void gauss_jordan(float *a, float *res, int dim) {
    write_identity(res);

    for (int i = 0; i < dim; i++) {
        if (a[i*dim + i] == 0) {
            // if the column we want to zero out is already zero, add a row with non-zero column to current row
            for (int r = i+1; r < dim; r++) {
                if (a[r*dim + i] != 0) {
                    // our requirement on a being an invertible matrix means such row must exist
                    add_rows(a, dim, i, r, 1.0);
                    add_rows(res, dim, i, r, 1.0);
                    break;
                }
            }
        }

        // update current row such that the leftmost non-zero column is 1 
        // total 2n updates which can be parallelized 
        float divider = a[i*dim + i];

        #pragma omp parallel for
        for (int j = 0; j < dim; j++) {
            a[i*dim + j] /= divider;
            res[i*dim + j] /= divider;
        }

        // using current row, zero out all this column for all other rows
        // we can parallelize the n^2 updates
        #pragma omp parallel for collapse(2)
        for (int r = 0; r < dim; r++) {
            for (int c = 0; c < dim; c++) {
                if (r == i) {
                    break;
                }
                a[r*dim + c] += (-1) * divider * a[i*dim + c];
                res[r*dim + c] += (-1) * divider * res[i*dim + c];
            }
        }
    }
}

int main(int argc, const char *argv[]) {
    return 0;
}