include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
ASMSOURCES= $(wildcard ./asm/*.asm)
ASMOBJECTS=$(ASMSOURCES:.asm=.o)
COBJECTS=$(SOURCES:.c=.o)
MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

all: $(MODULE)

$(MODULE): $(ASMOBJECTS) $(COBJECTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(COBJECTS) $(ASMOBJECTS) -o ../$(MODULE)


clean:
	rm -rf *.o

.PHONY: all clean print