//
// Created by yejiongtao on 2019/4/29.
//

#ifndef PARALLELEC_CODING_LOOP_H
#define PARALLELEC_CODING_LOOP_H

void output_input_byte_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c, char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void output_input_byte_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c, char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void output_byte_input_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void output_byte_input_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void input_output_byte_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void input_output_byte_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void input_byte_output_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void input_byte_output_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void byte_input_output_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void byte_input_output_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void byte_output_input_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count);

void byte_output_input_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count);

void output_input_byte_table_threads_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                          char *inputs, int input_r, int input_c,
                                          char *outputs, int output_r, int output_c,
                                          int offset, int byte_count);

#endif //PARALLELEC_CODING_LOOP_H
