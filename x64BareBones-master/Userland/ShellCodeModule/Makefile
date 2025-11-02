include ../Makefile.inc

MODULE=0000-shellCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

INCLUDES    := -I./include

all: $(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) $(INCLUDES) -T shellCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)
clean:
	rm -rf asm/*.o *.o

.PHONY: all clean print
