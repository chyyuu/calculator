CC = gcc
LLCC = llvm-gcc
CFLAGS = -c -g -Wall
LFLAGS = -Wall -DREAL -g -lm
TIME = 4
calc: calculator.c
	$(CC) $(LFLAGS) -o calc calculator.c

#stack.o: stack.c stack.h
#	$(CC) $(CFLAGS) stack.c

calc.bc:
	$(LLCC) --emit-llvm -c -g calculator.c
	llvm-ld --disable-opt calculator.o -o calc
kleerun: calc.bc
	klee calc.bc
kleetest: calc.bc
	klee -search=dfs -max-time=${TIME} calc.bc 2>&1|tee df.log
	klee -search=bfs -max-time=${TIME} calc.bc 2>&1|tee bf.log
	klee -search=random-state -max-time=${TIME} calc.bc 2>&1 |tee rs.log
	klee -search=random-path -max-time=${TIME} calc.bc 2>&1 |tee rp.log
	klee -search=nurs:covnew -max-time=${TIME} calc.bc 2>&1 |tee nc.log
	klee -search=nurs:md2u -max-time=${TIME} calc.bc 2>&1 |tee nm.log
	klee -search=nurs:depth -max-time=${TIME} calc.bc 2>&1 |tee nd.log
	klee -search=nurs:icnt -max-time=${TIME} calc.bc 2>&1 |tee ni.log
	klee -search=nurs:cpicnt -max-time=${TIME} calc.bc 2>&1 |tee np.log
	klee -search=nurs:qc -max-time=${TIME} calc.bc 2>&1 |tee nq.log
	grep Statistic *.log
	grep ERR *.log
kleetest2: calc.bc
	klee -search=n-length -max-time=${TIME} calc.bc 2>&1|tee nl.log
	klee -search=subpath-n -max-time=${TIME} calc.bc 2>&1|tee sn.log
	grep Statistic *.log
	grep ERR *.log
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
