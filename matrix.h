//
// Created by yejiongtao on 2019/4/29.
//

#ifndef PARALLELEC_MATRIX_H
#define PARALLELEC_MATRIX_H

char *vandermonde(int rows, int cols);
char *inverse(char *matrix, int r, int c);
char *matrix_mult(char *matrix1, int r1, int c1, char *matrix2, int r2, int c2);
char *submatrix(char *matrix, int rows, int cols, int rmin, int rmax, int cmin, int cmax);

char *alloc_matrix(int r, int c);
char *identity(int size);
char *concat(char *matrix1, int r1, int c1, char *matrix2, int r2, int c2);
void swap_rows(char *matrix, int rows, int cols, int r1, int r2);
void gaussian_elimination(char *matrix, int rows, int columns);

#endif //PARALLELEC_MATRIX_H
