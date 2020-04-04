# Defined constants for command base name and arguments for simple test
#
STUDENT_ID=2881618

SRCDIR = ./
CFILELIST = ptcount.c ptcount_atomic.c

RAWC = $(patsubst %.c,%,$(addprefix $(SRCDIR), $(CFILELIST)))

CCFLAGS = -pedantic -Wall -std=gnu11

LOOP=100000000
LOOP_HELGRIND=1
INC=1



all: ptcount ptcount_atomic

ptcount: ptcount.c
	gcc $(CCFLAGS) -g -o $@ $^ -lpthread

ptcount_atomic: ptcount_atomic.c
	gcc $(CCFLAGS) -g -o $@ $^ -lpthread

test: all
	time ./ptcount $(LOOP) $(INC)
	time ./ptcount_atomic $(LOOP) $(INC)

test-helgrind: all
	valgrind --tool=helgrind ./ptcount $(LOOP_HELGRIND) $(INC)
	valgrind --tool=helgrind ./ptcount_atomic $(LOOP_HELGRIND) $(INC)

clean:
	rm -f ptcount ptcount_atomic

zip:
	make clean
	mkdir $(STUDENT_ID)-pthreads_intro-lab
#	get all the c files to be .txt for archiving
	$(foreach file, $(RAWC), cp $(file).c $(file)-c.txt;)
	mv *-c.txt $(STUDENT_ID)-pthreads_intro-lab/	
	cp $(CFILELIST) Makefile $(STUDENT_ID)-pthreads_intro-lab/
	zip -r $(STUDENT_ID)-pthreads_intro-lab.zip $(STUDENT_ID)-pthreads_intro-lab
	rm -rf $(STUDENT_ID)-pthreads_intro-lab

