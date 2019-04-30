//
// Created by yejiongtao on 2019/4/29.
//

#ifndef PARALLELEC_REED_SOLOMON_H
#define PARALLELEC_REED_SOLOMON_H

void init_rc(int data_shards, int parity_shards,
             void (*coding_loop)(char *, int, int, char *, int, int,
                                 char *, int, int, int, int));
void encode_parity(char *shards, int rows, int cols, int offset, int byte_count);

#endif //PARALLELEC_REED_SOLOMON_H
