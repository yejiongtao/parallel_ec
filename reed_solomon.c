//
// Created by yejiongtao on 2019/4/29.
//

#include "reed_solomon.h"
#include "galois.h"
#include "matrix.h"
#include "coding_loop.h"
#include <stdio.h>
#include <stdlib.h>


char *matrix = NULL;       // total_shard_count x data_shard_count
char *parity_rows = NULL;  // parity_shard_count x data_shard_count
int data_shard_count, parity_shard_count, total_shard_count;

void (*encode_shards)(char *, int, int, char *, int, int,
                      char *, int, int, int, int);

void init_rc(int data_shards, int parity_shards,
             void (*coding_loop)(char *, int, int, char *, int, int,
                                 char *, int, int, int, int)) {
    if (matrix != NULL)
        free(matrix);
    data_shard_count = data_shards;
    parity_shard_count = parity_shards;
    total_shard_count = data_shards + parity_shards;
    encode_shards = coding_loop;

    char *vd = vandermonde(total_shard_count, data_shard_count);
    // Multiple by the inverse of the top square of the matrix.
    // This will make the top square be the identity matrix, but
    // preserve the property that any square subset of rows is
    // invertible.
    char *top = submatrix(vd, total_shard_count, data_shard_count,
                          0, data_shard_count, 0, data_shard_count);
    char *I = inverse(top, data_shard_count, data_shard_count);
    matrix = matrix_mult(vd, total_shard_count, data_shard_count,
                         I, data_shard_count, data_shard_count);
    parity_rows = matrix + data_shard_count * data_shard_count;
    free(vd);
    free(top);
    free(I);
}

void encode_parity(char *shards, int rows, int cols, int offset, int byte_count) {
    if (rows != total_shard_count) {
        printf("Wrong number of shards.\n");
        exit(1);
    }
    if (offset < 0 || byte_count < 0 || offset + byte_count > cols) {
        printf("Illegal arguments for encode_parity.\n");
        exit(1);
    }
    encode_shards(parity_rows, parity_shard_count, data_shard_count,
                  shards, data_shard_count, cols,
                  shards + (data_shard_count * cols), parity_shard_count, cols,
                  offset, byte_count);
}

