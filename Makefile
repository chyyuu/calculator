CC = gcc
LLCC = llvm-gcc
CFLAGS = -c -g -Wall
LFLAGS = -Wall -DREAL -g -lm
TIME = 30
ROOTDIR="/ubuntu/chyyuu/develop/xqx/chy/calculator"
all:realcalc kleecalc.bc
realcalc: calculator.c
	$(CC) -DREAL -ftest-coverage -fprofile-arcs -Wall -g -lm -o realcalc calculator.c
#	$(CC) $(LFLAGS) -ftest-coverage -fprofile-arcs -o realcalc calculator.c
calc: calculator.c
	$(CC) -DREAL -ftest-coverage -fprofile-arcs -Wall -g -lm -o calc calculator.c

#stack.o: stack.c stack.h
#	$(CC) $(CFLAGS) stack.c

kleecalc.bc:
	$(LLCC) --emit-llvm -c -g calculator.c
	llvm-ld --disable-opt calculator.o -o kleecalc
kleerun: kleecalc.bc
	klee kleecalc.bc
kc: kleecalc.bc
	klee -write-cov kleecalc.bc
kcdf: kleecalc.bc
	klee -write-cov -search=dfs -max-time=${TIME} kleecalc.bc 2>&1|tee df.log
	./colorcov.sh
kcbf: kleecalc.bc
	klee -write-cov -search=bfs -max-time=${TIME} kleecalc.bc 2>&1|tee bf.log
	./colorcov.sh
kcnc: kleecalc.bc
	klee -write-cov -search=nurs:covnew -max-time=${TIME} kleecalc.bc 2>&1|tee nc.log
	./colorcov.sh
kleetest: kleecalc.bc
	klee -search=dfs -max-time=${TIME} kleecalc.bc 2>&1|tee df.log
	klee -search=bfs -max-time=${TIME} kleecalc.bc 2>&1|tee bf.log
	klee -search=random-state -max-time=${TIME} kleecalc.bc 2>&1 |tee rs.log
	klee -search=random-path -max-time=${TIME} kleecalc.bc 2>&1 |tee rp.log
	klee -search=nurs:covnew -max-time=${TIME} kleecalc.bc 2>&1 |tee nc.log
	klee -search=nurs:md2u -max-time=${TIME} kleecalc.bc 2>&1 |tee nm.log
	klee -search=nurs:depth -max-time=${TIME} kleecalc.bc 2>&1 |tee nd.log
	klee -search=nurs:icnt -max-time=${TIME} kleecalc.bc 2>&1 |tee ni.log
	klee -search=nurs:cpicnt -max-time=${TIME} kleecalc.bc 2>&1 |tee np.log
	klee -search=nurs:qc -max-time=${TIME} kleecalc.bc 2>&1 |tee nq.log
	grep Statistic *.log
	grep ERR *.log
kleetest1: kleecalc.bc
	klee -search=dfs -max-time=${TIME} kleecalc.bc 2>&1|tee df.log
kleetest2: kleecalc.bc
	klee -search=n-length -n-length=100 -max-time=${TIME} kleecalc.bc 2>&1|tee nl.log
	klee -search=subpath-n -subpath-n=100 -max-time=${TIME} kleecalc.bc 2>&1|tee sn.log
	grep Statistic *.log
	grep ERR *.log
kcov1:
	rm -rf results
	zcov-scan --look-up-dirs=2 klee.zcov .
	zcov-genhtml --root=$$(pwd) klee.zcov results 
kcov2:
	rm -rf results
	zcov scan --look-up-dirs=1 klee.zcov .
	zcov genhtml --root=$$(pwd) klee.zcov results 
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
	rm -rf results html *.gcov *.gcno *.gcda *.zcov *.cov *.info *.o calc realcalc kleecalc *.ll *.out klee-* *~ *.bc pintck.txt *.log 
