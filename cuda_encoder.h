//
// Created by yejiongtao on 2019/5/2.
//

#ifndef PARALLELEC_CUDA_ENCODER_H
#define PARALLELEC_CUDA_ENCODER_H

void init_cuda(char *parity_rows, int r, int c);
void copy_mult_table_to_cuda();

void output_input_byte_table_cuda_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                       char *inputs, int input_r, int input_c,
                                       char *outputs, int output_r, int output_c,
                                       int offset, int byte_count);

#endif // PARALLELEC_CUDA_ENCODER_H
