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

char multiply_field(char a, char b);
char divide_field(char a, char b);
char exp_field(char a, char n);
void generate_multiplication_table();

#endif //PARALLELEC_GALOIS_H
