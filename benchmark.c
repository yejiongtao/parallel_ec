#include <stdio.h>
#include <stdlib.h>
#include "galois.h"
#include "coding_loop.h"
#include "reed_solomon.h"
#include "cycletimer.h"


//#define SANITY

#define DATA_COUNT 17
#define PARITY_COUNT 3
#define TOTAL_COUNT (DATA_COUNT + PARITY_COUNT)
#define BUFFER_SIZE (200 * 1000)
#define PROCESSOR_CACHE (10 * 1024 * 1024)
#define NUM_OF_BUFFERS (PROCESSOR_CACHE * 2 / DATA_COUNT / BUFFER_SIZE + 1)

//const int DATA_COUNT = 17;
//const int PARITY_COUNT = 3;
//const int TOTAL_COUNT = DATA_COUNT + PARITY_COUNT;
//const int BUFFER_SIZE = 200 * 1000;
//const int PROCESSOR_CACHE = 10 * 1024 * 1024;
//const int NUM_OF_BUFFERS = PROCESSOR_CACHE * 2 / DATA_COUNT / BUFFER_SIZE + 1;
const long MEASURE_DURATION = 2 * 1000;     // ms
int cur_buffer = 0;

char *buffers[NUM_OF_BUFFERS];     // array of pointers


void init_buffers() {
    for (int i = 0; i < NUM_OF_BUFFERS; i++) {
        buffers[i] = (char *) malloc(BUFFER_SIZE * TOTAL_COUNT * sizeof(char));
        if (buffers[i] == NULL) {
            printf("Cannot allocate buffer.\n");
            exit(1);
        }
        for (int r = 0; r < TOTAL_COUNT; r++) {
            char *row = buffers[i] + r * BUFFER_SIZE;
            for (int c = 0; c < BUFFER_SIZE; c++) {
                row[c] = (char) rand();
            }
        }
    }
}

void do_one_encode_measurement(double *megabytes, double *seconds) {
    int passes_completed = 0;
    long bytes_encoded = 0;
    double encoding_time = 0;
    while(encoding_time < MEASURE_DURATION) {
        char *shards = buffers[cur_buffer];
#ifdef SANITY
        shards = buffers[0];
#endif
        cur_buffer = (cur_buffer + 1) % NUM_OF_BUFFERS;
        double start_time = currentMilliseconds();
        encode_parity(shards, TOTAL_COUNT, BUFFER_SIZE, 0, BUFFER_SIZE);
        double end_time = currentMilliseconds();
#ifdef SANITY
        for(int i=0; i<20; i++)
            printf("%d ", shards[DATA_COUNT*BUFFER_SIZE+i]);
        printf("\n");
        return;
#endif
        encoding_time += end_time - start_time;
        bytes_encoded += BUFFER_SIZE * DATA_COUNT;
        passes_completed++;
    }
    double s = encoding_time / 1000;
    double mb = bytes_encoded / 1000000.0;
    if(megabytes != NULL && seconds != NULL) {
        *seconds += s;
        *megabytes += mb;
    }
    printf("\t\t%d passes, %.1f MB/s\n", passes_completed, mb/s);
}

void benchmark(void (*coding_loop)(char *, int, int, char *, int, int,
                                   char *, int, int, int, int),
               const char *loop_name) {
    printf("\nTEST: %s\n", loop_name);
    init_rc(DATA_COUNT, PARITY_COUNT, coding_loop);
    printf("\twarm up...\n");
    do_one_encode_measurement(NULL, NULL);
    do_one_encode_measurement(NULL, NULL);
    printf("\ttesting...\n");
    double megabytes = 0, seconds = 0;
    for(int i = 0; i < 10; i++) {
        do_one_encode_measurement(&megabytes, &seconds);
    }
    printf("\nAVERAGE: %.1f MB/s\n", megabytes / seconds);
}

int main() {
    printf("Hello, World!\n");
    init_buffers();
    generate_multiplication_table();
    benchmark(byte_input_output_exp_loop, "ByteInputOutputExp");
    benchmark(byte_input_output_table_loop, "ByteInputOutputTable");
    benchmark(byte_output_input_exp_loop, "ByteOutputInputExp");
    benchmark(byte_output_input_table_loop, "ByteOutputInputTable");
    benchmark(input_byte_output_exp_loop, "InputByteOutputExp");
    benchmark(input_byte_output_table_loop, "InputByteOutputTable");
    benchmark(input_output_byte_exp_loop, "InputOutputByteExp");
    benchmark(input_output_byte_table_loop, "InputOutputByteTable");
    benchmark(output_byte_input_exp_loop, "OutputByteInputExp");
    benchmark(output_byte_input_table_loop, "OutputByteInputTable");
    benchmark(output_input_byte_exp_loop, "OutputInputByteExp");
    benchmark(output_input_byte_table_loop, "OutputInputByteTable");
    return 0;
}