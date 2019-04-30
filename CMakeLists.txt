cmake_minimum_required(VERSION 3.12)
project(parallelEC C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Release)

add_executable(parallelEC benchmark.c reed_solomon.c reed_solomon.h galois.c galois.h coding_loop.c coding_loop.h matrix.c matrix.h cycletimer.c cycletimer.h)