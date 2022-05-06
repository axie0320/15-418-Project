#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <chrono>

#include <utility>
#include <string>
#include <omp.h>

using namespace std;

/* Parallelize Matrix Inversion using LU decomposition method */

/* List of assumptions we made here:
 1. Dimension of matrix is greater than or equal to 2 
 2. Matrix A of size dim/2 x dim/2 is invertible 
*/ 

/* Define functions */
void lu_d (double *M, double *L, double *U, int dim);
void invertUpper (double *U, int dim);
void invertLower (double *L, int dim);
void invertMatrix (double *M, int dim);
void recursiveBlockwiseInversion (double *M, int dim);


#define N 16					//size of the matrix 
double M[N * N];
double L[N * N];
double U[N * N];


/* Implement the sequential version 
 * @param M points to the original matrix that needs to be inverted
 * @param dim indicates the size of the matrix 
*/

void lu_d(double *M, double *L, double *U, int dim) {
    // iterate across column
    #pragma omp parallel for 
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

void write_identity(double *a, int dim) {
    #pragma omp parallel for
    for (int i = 0; i < dim; i++) {
        a[i*dim + i] = 1;
    }
}

void invertUpper (double *U, int dim) {

	int factor;

	// I is initially an identity matrix. We will modify this matrix 
	// as we apply Gaussian elimination and return it at the end as 
	// the identity matrix 
	double I[dim  * dim];
	write_identity(I, dim);

	// Iterate across columns 
	for (int i = dim; i > 0; i--) {
		I[i * dim - 1] /= U[i * dim - 1];
		U[i * dim - 1] = 1;
		// Iterate across rows
		for (int j = 0; j < dim; j++) {
			factor = U[j * dim + i - 1];
			I[j * dim + i - 1] -= factor * I[i * dim - 1];
		}
	}

	// copy everything over to U
	#pragma omp parallel for 
	for (int k = 0; k < dim; k ++) {
		for (int z = k; z < dim; z++) {
			U[k * dim + z] = I[k * dim + z];
		}
	}

}


void invertLower (double *L, int dim) {

	int factor;

	// I is initially an identity matrix. We will modify this matrix 
	// as we apply Gaussian elimination and return it at the end as 
	// the identity matrix 
	double I[dim  * dim];
	write_identity(I, dim);	

	// Iterate across columns
	for (int i = 0; i < dim; i ++) {
		I[i * dim + i] /= L[i * dim + i];
		L[i * dim + i] = 1;
		// Iterate across rows
		for (int j = 0; j < dim; j++) {
			factor = L[j * dim + i - 1];
			I[j * dim + i - 1] -= factor * I[i * dim - 1];
		}
	}

	#pragma omp parallel for
	// copy everything over to L
	for (int k = 0; k < dim; k ++) {
		for (int z = k; z < dim; z++) {
			L[k * dim + z] = I[k * dim + z];
		}
	}
}

/* M1 is of size dim1 x dim2 */
void matrixMultiplication (double *result, double *M1, double *M2, dim1, dim2, dim3) {
	for (int i = 0; i < dim1; i++) {
		for (int j = 0; j < dim2; j++) {
			for (int p = 0; p < dim3; p++) {
				result[i * dim1 + j] += M1[i * dim1 + p] + M2[p * dim3 + j];
			}
		}
	}
}

void matrixAddition (double *result, double *M1, double *M2, dimX, dimY) {
	#pragma omp parallel shared (result, M1, M2) private (i, j)
	#pragma omp for 
	for (int i = 0; i < dimY; i++) {
		for (int j = 0; j < dimX; j++) {
			result[i * dim + j] = M1[i * dim + j] + M2[i * dim + j];
		}	
	}
}

void matrixSubtraction (double *result, double *M1, double *M2, dimX, dimY) {
	#pragma omp parallel shared (result, M1, M2) private (i, j)
	#pragma omp parallel for
	for (int i = 0; i < dimY; i++) {
		for (int j = 0; j < dimX; j++) {
			result[i * dimY + j] = M1[i * dimY + j] - M2[i * dimY + j];
		}	
	}
}

void integerMultiplication (double *result, double *M, int num, dimX, dimY) {
	#pragma omp parallel shared (result, M) private (i, j)
	#pragma omp parallel for 
	for (int i = 0; i < dimY; i ++){
		for (int j = 0; j < dimX; j++) {
			result[i * dimY + j] = M[i * dimY + j] * num;
		}
	}

}

void copyMatrix (double *result, double *source, int x1, int y1, int x2, int y2, dimX, dimY) {

}

/* Implement matrix inversion using LU decomposition */
void invertMatrix (double *M, int dim) {

	double L[dim * dim];
	double U[dim * dim]; 


	lu_d(M, L, U, dim);

	/* Now that we have the matrices L and U, we only need to find the inverses 
	 * of these two matrices and perform matrix multiplication to get the 
	 * the final result */

	invertLower(L, dim);
	invertUpper(U, dim);

	matrixMultiplication(M, U, L, dim, dim, dim);

}

/*Implement matrix inversion blockwise using LU decomposition */

/* To explain how the blockwise inversion algorithm works:
 * We devide the matrix into 4 blocks of matrices:
 1. A (upper left matrix): a square matrix of size p x p
 2. B (upper right matrix): a conformable matrix of size p x q
 3. C (lower left matrix): a conformable matrix of size q x p
 4. D (lower right matrix): a square matrix of size p x p 
 * A is invertible
 * 
*/

/* Recursive approach */
void recursiveBlockwiseInversion (double *M, int dim) {

	// Base case are when matrices are of size 2 x 2 or 3 x 3
	if (dim <= 3) {
		invertMatrix(M, dim);
	}

 	int p, q;

 	p = dim / 2;
 	q = dim - p;
 	
 	double A[p * p];
 	double B[p * q];
 	double C[q * p];
 	double D[q * q];

	copyMatrix(A, M, 0, 0, p, p);
	copyMatrix(B, M, p, 0, dim, p);
	copyMatrix(C, M, 0, p, p, dim);
	copyMatrix(D, M, p, p, dim, dim);

	recursiveBlockwiseInversion(A, p);	// A now stores the inverted version of original A

	// Our next step is to find the inversion of (D - CA^(-1)B)^(-1) which is the Schur
	// complement of A. This is where we our recursive approach becomes useful

	double S[q * q];					// S will store the Schur complement of A
	double temp[p * q];
	matrixMultiplication(temp, A, B, p, q, p);
	double temp2[q * q];
	matrixMultiplication(temp2, C, temp, q, p, p);
	matrixSubtraction(S, D, temp2, q, q);

	recursiveBlockwiseInversion(S, q);	//S stores the inverted Schur complement of A
	copyMatrix(D, S, 0, 0, q, q, q, q);		//final D

	double temp3[q * p];
	matrixMultiplication(temp3, C, A, q, p, p);    	// Calculate CA^(-1)

	matrixMultiplication(C, S, temp3, q, p, q)      // -C = N^(-1)CA^(-1)
	double temp4[p * p];
	matrixMultiplication(temp4, B, C, p, p, q);		// temp4 = BN^(-1)CA^(-1)
	integerMultiplication(C, C, -1, q, p); 			// final C

	matrixMultiplication(B, B, S, p, q, p);
	matrixMultiplication(B, A, S, p, q, p); 	
	integerMultiplication(B, B, -1, p, q);			// final B

	double temp5[p * p];
	matrixMultiplication(temp5, A, temp4, p, p, p);	 
	matrixAddition(A, A, temp5, p, p);				// final A


	// copy the inversion of M into M
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (i < p && j < p) {
				M[i * dim + j] = A[i * dim + j];
			} else (i < p) {
				M[i * dim = j] = C[i * dim + j];
			} else (i >= p && j < p) {
				M[i * dim + j] = B[i * dim + j];
			} else {
				M[i * dim + j] = D[i * dim + j];
			}
		}
	}

}

int main () {



}


