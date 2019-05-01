//
// Created by yejiongtao on 2019/4/29.
//

#include "coding_loop.h"
#include "galois.h"

void output_input_byte_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    for (int i_output = 0; i_output < output_r; i_output++) {
        char *output_shard = outputs + i_output * output_c;
        char *matrix_row = matrix_rows + i_output * matrix_c;
        {
            int i_input = 0;
            char *input_shard = inputs + i_input * input_c;
            char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] = mult_table_row[input_shard[i_byte] & 0xFF];
        }
        for (int i_input = 1; i_input < input_r; i_input++) {
            char *input_shard = inputs + i_input * input_c;
            char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] ^= mult_table_row[input_shard[i_byte] & 0xFF];
        }
    }
}

void output_input_byte_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    for (int i_output = 0; i_output < output_r; i_output++) {
        char *output_shard = outputs + i_output * output_c;
        char *matrix_row = matrix_rows + i_output * matrix_c;
        {
            int i_input = 0;
            char *input_shard = inputs + i_input * input_c;
            char matrix_byte = matrix_row[i_input];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] = multiply_field(matrix_byte, input_shard[i_byte]);
        }
        for (int i_input = 1; i_input < input_r; i_input++) {
            char *input_shard = inputs + i_input * input_c;
            char matrix_byte = matrix_row[i_input];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] ^= multiply_field(matrix_byte, input_shard[i_byte]);
        }
    }
}

void output_byte_input_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    for (int i_output = 0; i_output < output_r; i_output++) {
        char *output_shard = outputs + i_output * output_c;
        char *matrix_row = matrix_rows + i_output * matrix_c;
        for (int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            int value = 0;
            for (int i_input = 0; i_input < input_r; i_input++) {
                char *input_shard = inputs + i_input * input_c;
                char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
                value ^= mult_table_row[input_shard[i_byte] & 0xFF];
            }
            output_shard[i_byte] = (char) value;
        }
    }
}

void output_byte_input_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    for (int i_output = 0; i_output < output_r; i_output++) {
        char *output_shard = outputs + i_output * output_c;
        char *matrix_row = matrix_rows + i_output * matrix_c;
        for (int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            int value = 0;
            for (int i_input = 0; i_input < input_r; i_input++) {
                char *input_shard = inputs + i_input * input_c;
                value ^= multiply_field(matrix_row[i_input], input_shard[i_byte]);
            }
            output_shard[i_byte] = (char) value;
        }
    }
}

void input_output_byte_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    {
        int i_input = 0;
        char *input_shard = inputs + i_input * input_c;
        for (int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * matrix_c;
            char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] = mult_table_row[input_shard[i_byte] & 0xFF];
        }
    }
    for (int i_input = 1; i_input < input_r; i_input++) {
        char *input_shard = inputs + i_input * input_c;
        for (int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * matrix_c;
            char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] ^= mult_table_row[input_shard[i_byte] & 0xFF];
        }
    }
}

void input_output_byte_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    {
        int i_input = 0;
        char *input_shard = inputs + i_input * input_c;
        for (int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * matrix_c;
            char matrix_byte = matrix_row[i_input];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] = multiply_field(matrix_byte, input_shard[i_byte]);
        }
    }
    for (int i_input = 1; i_input < input_r; i_input++) {
        char *input_shard = inputs + i_input * input_c;
        for (int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * matrix_c;
            char matrix_byte = matrix_row[i_input];
            for (int i_byte = offset; i_byte < offset + byte_count; i_byte++)
                output_shard[i_byte] ^= multiply_field(matrix_byte, input_shard[i_byte]);
        }
    }
}

void input_byte_output_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    {
        int i_input = 0;
        char *input_shard = inputs + i_input * input_c;
        for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            char input_byte = input_shard[i_byte];
            char *mult_table_row = MULTIPLICATION_TABLE[input_byte & 0xFF];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] = mult_table_row[matrix_row[i_input] & 0xFF];
            }
        }
    }
    for(int i_input = 1; i_input < input_r; i_input++) {
        char *input_shard = inputs + i_input * input_c;
        for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            char input_byte = input_shard[i_byte];
            char *mult_table_row = MULTIPLICATION_TABLE[input_byte & 0xFF];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] ^= mult_table_row[matrix_row[i_input] & 0xFF];
            }
        }
    }
}

void input_byte_output_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    {
        int i_input = 0;
        char *input_shard = inputs + i_input * input_c;
        for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] = multiply_field(matrix_row[i_input], input_byte);
            }
        }
    }
    for(int i_input = 1; i_input < input_r; i_input++) {
        char *input_shard = inputs + i_input * input_c;
        for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] ^= multiply_field(matrix_row[i_input], input_byte);
            }
        }
    }
}

void byte_input_output_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
        {
            int i_input = 0;
            char *input_shard = inputs + i_input * input_c;
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
                output_shard[i_byte] = mult_table_row[input_byte & 0xFF];
            }
        }
        for(int i_input = 1; i_input < input_r; i_input++) {
            char *input_shard = inputs + i_input * input_c;
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                char *mult_table_row = MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF];
                output_shard[i_byte] ^= mult_table_row[input_byte & 0xFF];
            }
        }
    }
}

void byte_input_output_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
        {
            int i_input = 0;
            char *input_shard = inputs + i_input * input_c;
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] = multiply_field(matrix_row[i_input], input_byte);
            }
        }
        for(int i_input = 1; i_input < input_r; i_input++) {
            char *input_shard = inputs + i_input * input_c;
            char input_byte = input_shard[i_byte];
            for(int i_output = 0; i_output < output_r; i_output++) {
                char *output_shard = outputs + i_output * output_c;
                char *matrix_row = matrix_rows + i_output * matrix_c;
                output_shard[i_byte] ^= multiply_field(matrix_row[i_input], input_byte);
            }
        }
    }
}

void byte_output_input_table_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                  char *inputs, int input_r, int input_c,
                                  char *outputs, int output_r, int output_c,
                                  int offset, int byte_count) {
    for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
        for(int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * output_c;
            int value = 0;
            for(int i_input = 0; i_input < input_r; i_input++) {
                char *input_shard = inputs + i_input * input_c;
                value ^= MULTIPLICATION_TABLE[matrix_row[i_input] & 0xFF][input_shard[i_byte] & 0xFF];
            }
            output_shard[i_byte] = (char)value;
        }
    }
}

void byte_output_input_exp_loop(char *matrix_rows, int matrix_r, int matrix_c,
                                char *inputs, int input_r, int input_c,
                                char *outputs, int output_r, int output_c,
                                int offset, int byte_count) {
    for(int i_byte = offset; i_byte < offset + byte_count; i_byte++) {
        for(int i_output = 0; i_output < output_r; i_output++) {
            char *output_shard = outputs + i_output * output_c;
            char *matrix_row = matrix_rows + i_output * output_c;
            int value = 0;
            for(int i_input = 0; i_input < input_r; i_input++) {
                char *input_shard = inputs + i_input * input_c;
                value ^= multiply_field(matrix_row[i_input], input_shard[i_byte]);
            }
            output_shard[i_byte] = (char)value;
        }
    }
}
