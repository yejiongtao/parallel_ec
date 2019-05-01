//
// Created by yejiongtao on 2019/4/29.
//

#ifndef PARALLELEC_GALOIS_H
#define PARALLELEC_GALOIS_H

/**
 * The number of elements in the field.
 */
#define FIELD_SIZE 256

extern const short LOG_TABLE[];
extern const char EXP_TABLE[];
extern char MULTIPLICATION_TABLE[][FIELD_SIZE];

char divide_field(char a, char b);
char exp_field(char a, char n);
void generate_multiplication_table();

inline char multiply_field(char a, char b) {
    if(a == 0 || b == 0)
        return 0;
    int log_a = LOG_TABLE[a & 0xff];
    int log_b = LOG_TABLE[b & 0xff];
    int log_result = log_a + log_b;
    return EXP_TABLE[log_result];
}

#endif //PARALLELEC_GALOIS_H
