#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <chrono>

#include <utility>
#include <string>
using namespace std;

#define N 4

static int _argc;
static const char **_argv;

const char *get_option_string(const char *option_name, const char *default_value) {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0)
            return _argv[i + 1];
    return default_value;
}

int get_option_int(const char *option_name, int default_value) {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0)
            return atoi(_argv[i + 1]);
    return default_value;
}

double get_option_double(const char *option_name, double default_value) {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0)
            return (double)atof(_argv[i + 1]);
    return default_value;
}

void print_matrix(double *mat, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%.3f\t", mat[i * dim + j]);
        }
        printf("\n");
    }
}

void generate_matrix(double *mat, int dim){
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			mat[(i * dim) + j] = rand() % 10 + 2;
		}
	}
}

/**
 * Write Identity Matrix to input array 
 * 
 * @param a     1 dimensional square matrix to store the identity matrix
 * @param dim   integer specifying the dimension of the identity matrix (dim * dim)
 */
void write_identity(double *mat, int dim) {
    for (int i = 0; i < dim; i++) {
        mat[i*dim + i] = 1;
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
void add_rows(double *mat, int dim, int i, int j, double mult=1.0) {
    for (int idx = 0; idx < dim; idx++) {
        mat[i*dim + idx] += mult * mat[j*dim + idx];
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
void gauss_jordan(double *mat, double *res, int dim) {
    write_identity(res, dim);

    for (int i = 0; i < dim; i++) {
        if (mat[i*dim + i] == 0) {
            // if the column we want to zero out is already zero, add a row with non-zero column to current row
            for (int r = i+1; r < dim; r++) {
                if (mat[r*dim + i] != 0) {
                    // our requirement on a being an invertible matrix means such row must exist
                    add_rows(mat, dim, i, r);
                    add_rows(res, dim, i, r);
                    break;
                }
            }
        }

        // update current row such that the leftmost non-zero column is 1 
        // total 2n updates
        double divider = mat[i*dim + i];
        for (int j = 0; j < dim; j++) {
            mat[i*dim + j] /= divider;
            res[i*dim + j] /= divider;
        }

        // using current row, zero out all this column for all other rows
        for (int r = 0; r < dim; r++) {
            if (r != i) {
                divider = mat[r*dim + i];
                add_rows(mat, dim, r, i, (-1)*divider);
                add_rows(res, dim, r, i, (-1)*divider);
            }
        }
    }
}

int main(int argc, const char *argv[]) {

    _argc = argc - 1;
    _argv = argv + 1;


    int dim = get_option_int("-d", 4);

    double A[dim * dim];

    double AI[dim * dim];

    generate_matrix(A, dim);

    print_matrix(A, dim);

    
    gauss_jordan(A, AI, dim);
    printf("\n");
    print_matrix(AI, dim);
    printf("\n");
    print_matrix(A, dim);
    return 0;
}