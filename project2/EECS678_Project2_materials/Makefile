build: queue scheduler simulator
	gcc ./src/libpriqueue/libpriqueue.o ./src/libscheduler/libscheduler.o ./src/simulator.o -o ./src/simulator -lm -lpthread

queue: ./src/libpriqueue/libpriqueue.c
	gcc -c ./src/libpriqueue/libpriqueue.c -o ./src/libpriqueue/libpriqueue.o

scheduler: ./src/libscheduler/libscheduler.c
	gcc -c ./src/libscheduler/libscheduler.c -o ./src/libscheduler/libscheduler.o

simulator: ./src/simulator.c
	gcc -c ./src/simulator.c -o ./src/simulator.o

queueTest: queueTestBuild
	./src/queuetest

queueTestBuild: queue ./src/queuetest.c
	gcc ./src/libpriqueue/libpriqueue.o ./src/queuetest.c -o ./src/queuetest

clean:
	rm ./src/libpriqueue/libpriqueue.o ./src/libscheduler/libscheduler.o ./src/simulator.o ./src/simulator ./src/queuetest c1_diff c2_diff c3_diff

.PHONY:
	build
