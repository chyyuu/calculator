CC = gcc
LLCC = llvm-gcc
CFLAGS = -c -Wall
LFLAGS = -Wall -DREAL -lm

calc: stack.o calculator.c
	$(CC) $(LFLAGS) -o calc calculator.c stack.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) stack.c

klee:
	$(LLCC) --emit-llvm -c -g calculator.c stack.c
	llvm-ld --disable-opt *.o -o calc
krun:
	klee calc.bc
clean:
	rm -rf *.o calc *.ll *.out klee-* *~ *.bc pintck.txt *.log 
