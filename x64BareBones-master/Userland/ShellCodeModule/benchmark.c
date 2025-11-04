#include <benchmark.h>

// Variables volátiles para evitar optimizaciones del compilador
static volatile uint64_t dummy = 0;

// Benchmark de operaciones aritméticas
static void benchmarkArithmetic(uint64_t iterations) {
    volatile uint64_t a = 1, b = 2, c = 0;
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
    volatile uint64_t array[100];
    for(uint64_t i = 0; i < iterations; i++) {
        for(int j = 0; j < 100; j++) {
            array[j] = i + j;
            dummy = array[j];
        }
    }
}

// Benchmark de operaciones lógicas
static void benchmarkLogical(uint64_t iterations) {
    volatile uint64_t a = 0xAAAAAAAA, b = 0x55555555, c = 0;
    for(uint64_t i = 0; i < iterations; i++) {
        c = a & b;
        c = a | b;
        c = a ^ b;
        c = ~a;
        dummy = c;
    }
}

// Benchmark de llamadas a funciones
static volatile uint64_t helper(uint64_t x) {
    return x * 2 + 1;
}

static void benchmarkFunctionCalls(uint64_t iterations) {
    volatile uint64_t result = 0;
    for(uint64_t i = 0; i < iterations; i++) {
        result = helper(i);
        dummy = result;
    }
}

// Benchmark de operaciones de punto flotante (si están habilitadas)
static void benchmarkFloatingPoint(uint64_t iterations) {
    // Nota: requiere que el CPU tenga FPU habilitado
    // En caso de excepción, retornará gracefully
    printf("  [Floating point test skipped - not implemented]\n");
}

void printBenchmarkResults(const char* test, uint64_t ticks, uint64_t iterations) {
    cpuInfo info;
    getCpuInfo(&info);
    
    // Calcular tiempo transcurrido
    // 18 ticks ≈ 1 segundo
    uint64_t seconds = ticks / 18;
    uint64_t ms = (ticks % 18) * 1000 / 18;
    
    // Calcular operaciones por segundo
    uint64_t opsPerTick = 0;
    if(ticks > 0) {
        opsPerTick = (iterations * 18) / ticks;  // ops/segundo
    }
    
    printf("  %s:\n", test);
    printf("    Tiempo: %d.%ds\n", seconds, ms/100);
    printf("    Ticks: %d\n", ticks);
    printf("    Ops/seg: ~%d\n", opsPerTick);
    printf("\n");
}

void runBenchmarks() {
    cpuInfo info;
    uint64_t startTicks, endTicks;
    uint64_t iterations = BENCHMARK_ITERATIONS;
    
    printf("\n========================================\n");
    printf("  BENCHMARK SUITE\n");
    printf("========================================\n\n");
    
    // Obtener información del sistema
    if(getCpuInfo(&info) == OK) {
        printf("Informacion del sistema:\n");
        printf("  CPU Speed: %d MHz\n", info.cpuSpeed);
        printf("  Cores activos: %d\n", info.coresActive);
        printf("  Cores detectados: %d\n", info.coresDetected);
        printf("\n");
    }
    
    printf("Ejecutando benchmarks...\n");
    printf("(Cada test ejecuta %d iteraciones)\n\n", iterations);
    
    // Test 1: Operaciones aritméticas
    startTicks = getTicks();
    benchmarkArithmetic(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Operaciones Aritmeticas", endTicks - startTicks, iterations * 4);
    
    // Test 2: Acceso a memoria
    startTicks = getTicks();
    benchmarkMemoryAccess(iterations / 100);  // Menos iteraciones porque hace 100 accesos por vuelta
    endTicks = getTicks();
    printBenchmarkResults("Acceso a Memoria", endTicks - startTicks, iterations);
    
    // Test 3: Operaciones lógicas
    startTicks = getTicks();
    benchmarkLogical(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Operaciones Logicas", endTicks - startTicks, iterations * 4);
    
    // Test 4: Llamadas a funciones
    startTicks = getTicks();
    benchmarkFunctionCalls(iterations);
    endTicks = getTicks();
    printBenchmarkResults("Llamadas a Funciones", endTicks - startTicks, iterations);
    
    // Test 5: Operaciones de punto flotante
    benchmarkFloatingPoint(iterations);
    
    printf("========================================\n");
    printf("  Benchmarks completados!\n");
    printf("========================================\n\n");
}