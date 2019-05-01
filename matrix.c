//
// Created by yejiongtao on 2019/4/29.
//

#include "matrix.h"
#include "galois.h"
#include <stdlib.h>
#include <stdio.h>


char *vandermonde(int rows, int cols) {
    char *result = alloc_matrix(rows, cols);
    if(result == NULL) {
        printf("Cannot allocate for vandermonde.\n");
        exit(1);
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            result[r * cols + c] = exp_field((char)r, (char)c);
        }
    }
    return result;
}

char *identity(int size) {
    char *result = alloc_matrix(size, size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result[i * size + j] = i == j ? (char)1 : (char)0;
        }
    }
    return result;
}

char *inverse(char *matrix, int r, int c) {
    if(r !=  c) {
        printf("Matrix doesn't have r==c for inverse.\n");
        exit(1);
    }
    char *I = identity(r);
    char *work = concat(matrix, r, c, I, r, r);
    gaussian_elimination(work, r, r * 2);
    char *result = submatrix(work, r, 2*c, 0, r, c, 2*c);
    free(I);
    free(work);
    return result;
}

char *submatrix(char *matrix, int rows, int cols, int rmin, int rmax, int cmin, int cmax) {
    if(rmin >= rmax || cmin >= cmax || rmax > rows || cmax > cols) {
        printf("Invalid range for submatrix.\n");
        exit(1);
    }
    char *result = alloc_matrix(rmax - rmin, cmax - cmin);
    for(int r = rmin; r < rmax; r++) {
        for(int c = cmin; c < cmax; c++) {
            result[(r - rmin) * (cmax - cmin) + c - cmin] = matrix[r * cols + c];
        }
    }
    return result;
}

// Returns the concatenation of this matrix1 and matrix2 on the right.
char *concat(char *matrix1, int r1, int c1, char *matrix2, int r2, int c2) {
    if(r1 != r2) {
        printf("Cannot concat matrices with different r.\n");
        exit(1);
    }
    char *result = alloc_matrix(r1, c1 + c2);
    for(int r = 0; r < r1; r++) {
        for(int c = 0; c < c1; c++)
            result[r * (c1 + c2) + c] = matrix1[r * c1 + c];
        for(int c = 0; c < c2; c++)
            result[r * (c1 + c2) + c1 + c] = matrix2[r * c2 + c];
    }
    return result;
}

char *matrix_mult(char *matrix1, int r1, int c1, char *matrix2, int r2, int c2) {
    if(c1 != r2) {
        printf("Matrices dimensions don't match for multiplication.\n");
        exit(1);
    }
    char *result = alloc_matrix(r1, c2);
    for(int r = 0; r < r1; r++) {
        for(int c = 0; c < r2; c++) {
            char value = 0;
            for(int i = 0; i < c1; i++)
                value ^= multiply_field(matrix1[r * c1 + i], matrix2[i * c2 + c]);
            result[r * c2 + c] = value;
        }
    }
    return result;
}

char *alloc_matrix(int r, int c) {
    char *result = (char*)malloc(r * c * sizeof(char));
    if(result == NULL) {
        printf("Cannot allocate for matrix.\n");
        exit(1);
    }
    return result;
}

void swap_rows(char *matrix, int rows, int cols, int r1, int r2) {
    if(r1 < 0 || rows <= r1 || r2 < 0 || rows <= r2) {
        printf("Row index out of range.\n");
        exit(1);
    }
    for(int c = 0; c < cols; c++) {
        char tmp = matrix[r1 * cols + c];
        matrix[r1 * cols + c] = matrix[r2 * cols + c];
        matrix[r2 * cols + c] = tmp;
    }
}

void gaussian_elimination(char *matrix, int rows, int columns) {
    if(columns != rows*2) {
        printf("Invalid shape for gaussian elimination.\n");
        exit(1);
    }
    // Clear out the part below the main diagonal and scale the main
    // diagonal to be 1.
    for (int r = 0; r < rows; r++) {
        // If the element on the diagonal is 0, find a row below
        // that has a non-zero and swap them.
        if (matrix[r * columns + r] == (char) 0) {
            for (int rowBelow = r + 1; rowBelow < rows; rowBelow++) {
                if (matrix[rowBelow * columns + r] != 0) {
                    swap_rows(matrix, rows, columns, r, rowBelow);
                    break;
                }
            }
        }
        // If we couldn't find one, the matrix is singular.
        if (matrix[r * columns + r] == (char) 0) {
            printf("Matrix is singular.\n");
            exit(1);
        }
        // Scale to 1.
        if (matrix[r * columns + r] != (char) 1) {
            char scale = divide_field((char) 1, matrix[r * columns + r]);
            for (int c = 0; c < columns; c++) {
                matrix[r * columns + c] = multiply_field(matrix[r * columns + c], scale);
            }
        }
        // Make everything below the 1 be a 0 by subtracting
        // a multiple of it.  (Subtraction and addition are
        // both exclusive or in the Galois field.)
        for (int rowBelow = r + 1; rowBelow < rows; rowBelow++) {
            if (matrix[rowBelow * columns + r] != (char) 0) {
                char scale = matrix[rowBelow * columns + r];
                for (int c = 0; c < columns; c++) {
                    matrix[rowBelow * columns + c] ^= multiply_field(scale, matrix[r * columns + c]);
                }
            }
        }
    }

    // Now clear the part above the main diagonal.
    for (int d = 0; d < rows; d++) {
        for (int rowAbove = 0; rowAbove < d; rowAbove++) {
            if (matrix[rowAbove * columns + d] != (char) 0) {
                char scale = matrix[rowAbove * columns + d];
                for (int c = 0; c < columns; c++) {
                    matrix[rowAbove * columns + c] ^= multiply_field(scale, matrix[d * columns + c]);
                }
            }
        }
    }
}

