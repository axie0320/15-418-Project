#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <chrono>

#include <utility>
#include <string>
using namespace std;

#define N 4
#define threshold 4

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
            printf("%.4f\t", mat[i * dim + j]);
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
 * 
 * Parallelization:
 * assuming matrix a iniitlaized to 0 matrix, each row writes can be done in parallel 
 */
void write_identity(double *a, int dim) {
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
void add_rows(double *mat, int dim, int i, int j, double mult) {
    #pragma omp parallel for
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
void gauss_jordan(double *mat, double *res, int dim) {
    write_identity(res, dim);

    for (int i = 0; i < dim; i++) {
        if (mat[i*dim + i] == 0) {
            // if the column we want to zero out is already zero, add a row with non-zero column to current row
            for (int r = i+1; r < dim; r++) {
                if (mat[r*dim + i] != 0) {
                    // our requirement on a being an invertible matrix means such row must exist
                    add_rows(mat, dim, i, r, 1.0);
                    add_rows(res, dim, i, r, 1.0);
                    break;
                }
            }
        }

        // update current row such that the leftmost non-zero column is 1 
        // total 2n updates which can be parallelized 
        double divider = mat[i*dim + i];

        #pragma omp parallel for
        for (int j = 0; j < dim; j++) {
            mat[i*dim + j] /= divider;
            res[i*dim + j] /= divider;
        }

        // using current row, zero out all this column for all other rows
        // we can parallelize the n^2 updates
        #pragma omp parallel for collapse(2)
        for (int r = 0; r < dim; r++) {
            if (r == i) {
                continue;
            }
            divider = mat[r*dim + i];
            for (int c = 0; c < dim; c++) {
                mat[r*dim + c] += (-1) * divider * mat[i*dim + c];
                res[r*dim + c] += (-1) * divider * res[i*dim + c];
            }
        }
    }
}


void block_inversion_GJ(double *mat, double *res, int dim) {
    if (dim <= threshold) {
        gauss_jordan(mat, res, dim);
    }

    //    [ A  B ]
    //    [ C  D ]

    // Submatrix A must be invertible square

    int dim_A = dim/2;

    double A[dim_A * dim_A];
    double B[dim_A * (dim - dim_A)];
    double C[dim_A * (dim - dim_A)];
    double D[(dim - dim_A) * (dim - dim_A)];

    double AI[dim_A * dim_A];
    double D-CAIB[(dim - dim_A) * (dim - dim_A)];

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