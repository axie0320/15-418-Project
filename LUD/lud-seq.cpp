#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <utility>
#include <string>
using namespace std;

#define N 16					//size of the matrix 
#define bsize 8					//size of each block in the matrix 
#define numBlocks N/bsize


float A[N * N];

void generateMatrix(){
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[(i * N) + j] = rand() % 200 + 2;
		}
	}
}

void iterateRows(int i, int j) {
	for (int ii = i * bsize; ii < (i * bsize) + (bsize - 1); ii++) {
		for (int jj = ii + 1; jj < bsize; jj++) {
			for (int kk = j * bsize; kk < (j * bsize) + b_size; kk++) {
				A[(jj * N) + kk] = A[(jj * N) + kk] - (A[(jj*size) + ii] * A[(ii*size) + kk]);
			}
		}
	}
}

void iterateCols(int i, int j) {
	for (int ii = i * bsize; ii < (i * bsize) + bsize; ii++) {
		for (int jj = j * bsize; jj < (j * bsize) + bsize; jj++) {
			A[(jj * N) + ii] /= A[(ii * N) + ii];
			for (int kk = ii + 1; kk < (i * bsize) + bsize; kk++) {
				A[(jj * N) + kk] = A[(jj * N) + kk] - (A[(jj * N) + ii] * A[(ii * N) + kk]);
			}
		}
	}
}


void iterateDiag(int i){
	for (int ii = i * bsize; ii < (i * bsize) + bsize - 1; ii++) {
		for (int jj = ii+1; jj < (i*b_size)+b_size; jj++) {
			A[(jj * N) + ii] /= A[(ii * N) + ii];
			for(int kk = ii + 1; kk < (i * bsize) + bsize; kk++) {
				A[(jj * N) + kk] = A[(jj * N) + kk] - (A[(jj * N) + ii] * A[(ii * N) + kk]);
			}
		}
	}
}


void LU_D (){
	for (int i = 0; i < numBlocks; i++) {
    	    iterateDiag(i);
	    for (int j = i + 1; j < numBlocks; j++) {
	        iterateRows(i, j);
	    }
	    for (int j = i + 1; j < numBlocks; j++) {
	        iterateCols(i, j);
          	for (int k = i + 1; k < numBlocks; k++) {
				for (int ii = i * bsize; ii < (i * bsize) + bsize; ii++) {
					for (int jj = j * bsize; jj < (j * bsize) + bsize; jj++) {
						for(int kk = k * bsize; kk < (k * bsize) + bsize; kk++) {
							A[(jj * bsize) + kk] = A[(jj * bsize) + kk] - (A[(jj * N) + ii] * A[(ii * N) + kk]);
			}
		}
	}	            
	        }
	    }
	}
}

int main () {

	generateMatrix();
	LU_D();

	return 0;

}

/*
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
*/
