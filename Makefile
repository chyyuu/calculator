CC = gcc
LLCC = llvm-gcc
CFLAGS = -c -g -Wall
LFLAGS = -Wall -DREAL -g -lm

calc: calculator.c
	$(CC) $(LFLAGS) -o calc calculator.c

#stack.o: stack.c stack.h
#	$(CC) $(CFLAGS) stack.c

klee:
	$(LLCC) --emit-llvm -c -g calculator.c
	llvm-ld --disable-opt calculator.o -o calc
krun:
	klee calc.bc
kint:
	/ubuntu/chyyuu/develop/xqx/kint/build/bin/kint-build gcc -DREAL -lm -o calc calculator.c 
	/ubuntu/chyyuu/develop/xqx/kint/build/bin/pintck 
scheck:
	cppcheck calculator.c
dcheck: 
	valgrind --tool=memcheck ./calc
doc:
	doxygen Doxyfile.cfg
clean:
	rm -rf html *.o calc *.ll *.out klee-* *~ *.bc pintck.txt *.log 
