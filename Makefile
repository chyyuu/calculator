CC = gcc
LLCC = llvm-gcc
CFLAGS = -c -Wall
LFLAGS = -Wall -DREAL -lm

calc: calculator.c
	$(CC) $(LFLAGS) -o calc calculator.c

#stack.o: stack.c stack.h
#	$(CC) $(CFLAGS) stack.c

klee:
	$(LLCC) --emit-llvm -c -g calculator.c
	llvm-ld --disable-opt calculator.o -o calc
krun:
	klee calc.bc
clean:
	rm -rf *.o calc *.ll *.out klee-* *~ *.bc pintck.txt *.log 
