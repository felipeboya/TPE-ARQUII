#include <benchmark.h>

// Variable global para evitar optimizaciones del compilador
static uint64_t dummy = 0;

// Benchmark de operaciones aritméticas
static void benchmarkArithmetic(uint64_t iterations) {
    uint64_t a = 1, b = 2, c = 0;
    for(uint64_t i = 0; i < iterations; i++) {
        c = a + b;
        c = a - b;
        c = a * b;
        c = a / (b + 1);
        dummy = c;
    }
}

// Benchmark de acceso a memoria
static void benchmarkMemoryAccess(uint64_t iterations) {
    uint64_t array[100];
    for(uint64_t i = 0; i < iterations; i++) {
        for(uint64_t j = 0; j < 100; j++) {
            array[j] = i + j;
            dummy = array[j];
        }
    }
}

// Benchmark de operaciones de comparación y módulo
static void benchmarkComparison(uint64_t iterations) {
    uint64_t a = 12345, b = 67890, c = 0;
    for(uint64_t i = 0; i < iterations; i++) {
        if(a < b) c = a + b;
        if(a > b) c = a - b;
        if(a == b) c = a * b;
        c = a % (b + 1);
        dummy = c;
    }
}

// Benchmark de llamadas a funciones
static uint64_t helper(uint64_t x) {
    return x * 2 + 1;
}

static void benchmarkFunctionCalls(uint64_t iterations) {
    uint64_t result = 0;
    for(uint64_t i = 0; i < iterations; i++) {
        result = helper(i);
        dummy = result;
    }
}

// Benchmark de operaciones de punto flotante (si están habilitadas)
static void benchmarkFloatingPoint(uint64_t iterations) {
    printf("  [Floating point test skipped - not implemented]\n");
}

void printBenchmarkResults(const char* test, uint64_t ticks, uint64_t iterations) {
    cpuInfo info;
    getCpuInfo(&info);
    
    // Calcular tiempo transcurrido
    uint64_t seconds = ticks / 18;
    uint64_t ms = (ticks % 18) * 1000 / 18;
    
    // Calcular operaciones por segundo
    uint64_t opsPerTick = 0;
    if(ticks > 0) {
        opsPerTick = (iterations * 18) / ticks;  // ops/segundo
    }
    
    printf("  %s:\n", test);
    printf("    Time: %d.%ds\n", seconds, ms/100);
    printf("    Ticks: %d\n", ticks);
    printf("    Ops/sec: ~%d\n", opsPerTick);
    printf("\n");
}

void runBenchmarks() {
    cpuInfo info;
    uint64_t startTicks, endTicks;
    uint64_t iterations = BENCHMARK_ITERATIONS;
    
    printf("\n========================================\n");
    printf("            BENCHMARK SUITE\n");
    printf("========================================\\n\n");
    
    // Obtener información del sistema
    if(getCpuInfo(&info) == OK) {
        printf("System information:\n");
        printf("  CPU Speed: %d MHz\n", info.cpuSpeed);
        printf("  Active cores: %d\n", info.coresActive);
        printf("  Detected cores: %d\n", info.coresDetected);
        printf("\n");
    }
    
    printf("Running benchmarks...\n");
    printf("(Each test runs %d iterations)\n\n", iterations);
    
    // Operaciones aritméticas
    startTicks = getTicks();
    benchmarkArithmetic(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Arithmetic Operations", endTicks - startTicks, iterations * 4);
    
    // Acceso a memoria
    startTicks = getTicks();
    benchmarkMemoryAccess(iterations / 100);  // Menos iteraciones porque hace 100 accesos por vuelta
    endTicks = getTicks();
    printBenchmarkResults("Memory Access", endTicks - startTicks, iterations);
    
    // Operaciones de comparación
    startTicks = getTicks();
    benchmarkComparison(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Comparison Operations", endTicks - startTicks, iterations * 4);
    
    // Llamadas a funciones
    startTicks = getTicks();
    benchmarkFunctionCalls(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Function Calls", endTicks - startTicks, iterations);
    
    // Operaciones de punto flotante
    benchmarkFloatingPoint(iterations);
    
    printf("=========================================\n");
    printf("         Benchmarks completed!\n");
    printf("=========================================\n\n");
}