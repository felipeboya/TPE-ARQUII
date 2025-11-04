#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <library.h>
#include <utils.h>

#define BENCHMARK_ITERATIONS 100000000 

void runBenchmarks();
void printBenchmarkResults(const char* test, uint64_t ticks, uint64_t iterations);

#endif