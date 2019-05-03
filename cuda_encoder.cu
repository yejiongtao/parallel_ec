//
// Created by yejiongtao on 2019/5/2.
//

#include "cuda_encoder.h"
#include "galois.h"
#include "cycletimer.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>
#include <stdio.h>


const int BYTES_PER_THREAD = 1;
const int THREADS_PER_BLOCK = 128;
char *cuda_parity_rows;
char *cuda_mult_table;

//#define DEBUG
#ifdef DEBUG
#define cudaCheckError(ans)  cudaAssert((ans), __FILE__, __LINE__);
inline void cudaAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr, "CUDA Error: %s at %s:%d\n",
        cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}
#else
#define cudaCheckError(ans) ans
#endif

__inline__ int updiv(int a, int b) {
    return (a + b - 1) / b;
}

void init_cuda(char *parity_rows, int r, int c) {
    cudaCheckError(cudaMalloc(&cuda_parity_rows, r * c * sizeof(char)));
    cudaCheckError(cudaMemcpy(cuda_parity_rows, parity_rows,
            r * c * sizeof(char), cudaMemcpyHostToDevice));
}

void copy_mult_table_to_cuda() {
    cudaCheckError(cudaMalloc(&cuda_mult_table, FIELD_SIZE * FIELD_SIZE * sizeof(char)));
    // because MULTIPLICATION_TABLE is a 2D array
    for(int i = 0; i < FIELD_SIZE; i++)
        cudaCheckError(cudaMemcpy(cuda_mult_table + i * FIELD_SIZE, MULTIPLICATION_TABLE[i],
                FIELD_SIZE * sizeof(char), cudaMemcpyHostToDevice));
}

__global__ void kernel_encode(char *matrix_rows, int matrix_r, int matrix_c,
                              char *inputs, int input_r, int input_c,
                              char *outputs, int output_r, int output_c,
                              int offset, int byte_count, char *mult_table) {
    int i_thread = blockIdx.x * blockDim.x + threadIdx.x;

    for (int i_output = 0; i_output < output_r; i_output++) {
        char *output_shard = outputs + i_output * output_c;
        char *matrix_row = matrix_rows + i_output * matrix_c;
        {
            int i_input = 0;
            char *input_shard = inputs + i_input * input_c;
            char *mult_table_row = mult_table + (matrix_row[i_input] & 0xFF) * FIELD_SIZE;
            for(int i_byte = i_thread * BYTES_PER_THREAD;
                        i_byte < (i_thread+1) * BYTES_PER_THREAD; i_byte++) {
                if (i_byte >= input_r)
                    break;
                output_shard[i_byte] = mult_table_row[input_shard[i_byte] & 0xFF];
            }
        }
        for (int i_input = 1; i_input < input_r; i_input++) {
            char *input_shard = inputs + i_input * input_c;
            char *mult_table_row = mult_table + (matrix_row[i_input] & 0xFF) * FIELD_SIZE;
            for(int i_byte = i_thread * BYTES_PER_THREAD;
                i_byte < (i_thread+1) * BYTES_PER_THREAD; i_byte++) {
                if (i_byte >= input_r)
                    break;
                output_shard[i_byte] ^= mult_table_row[input_shard[i_byte] & 0xFF];
            }
        }
    }
}

void output_input_byte_table_cuda_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                       char *inputs, int input_r, int input_c,
                                       char *outputs, int output_r, int output_c,
                                       int offset, int byte_count) {
    int num_blocks = updiv(updiv(byte_count, BYTES_PER_THREAD), THREADS_PER_BLOCK);

    char *cuda_inputs;
    cudaCheckError(cudaMalloc(&cuda_inputs,
            (input_c * input_r + output_c * output_r) * sizeof(char)));
    char *cuda_outputs = cuda_inputs + input_c * input_r;
    cudaCheckError(cudaMemcpy(cuda_inputs, inputs,
            input_c * input_r * sizeof(char), cudaMemcpyHostToDevice));

    kernel_encode<<<num_blocks, THREADS_PER_BLOCK>>>(
            cuda_parity_rows, matrix_r, matrix_c, cuda_inputs, input_r, input_c,
            cuda_outputs, output_r, output_c, offset, byte_count, cuda_mult_table);
    cudaCheckError(cudaDeviceSynchronize());

    cudaCheckError(cudaMemcpy(outputs, cuda_outputs,
            output_c * output_r * sizeof(char), cudaMemcpyDeviceToHost));
    cudaFree(cuda_inputs);
}
