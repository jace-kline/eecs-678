# EECS 678 - Lab 06 Report
* Name: Jace Kline
* KUID: 2881618

## Part 1 - fork.c
1. Both the parent and the child processes will print this line. The parent will print "After fork, Process id = <child-id>". The child will print "After fork, Process id = 0". This is because the fork() system call returns a postive integer in the parent and it returns 0 in the child.
2. If this line is commented, the child will no longer execute the execlp(...) call and will instead continue to sequentially execute the following statements in the code. This will result in the following lines being printed instead of the 'ls' command: "print after execlp" and "Final statement from Process: <child-id>".
3. This line is only reached in the child process when the execlp(...) statement (from question 2) is commented out. This is because the execlp(...) system call will displace the current (child) process with the specified executable to execute, overwriting the address space. Hence, any following commands that occur after execlp(...) will not be executed if the execlp(...) command is executed correctly. This is why we must comment out the execlp(...) statement to get the targeted line to execute.
4. If the parent process ends prior to the child process, then the child process will be reassigned to the 'init' process (with id = 1). This makes the child process an orphan process. Regarding the printing, everything will be the same except for the fact that it will print the line "In Child: <child-id>, Parent: 1" instead of the default version where 1 would have been replaced by <parent-id>. Another interesting behavior that occurs is that, when this program is run in BASH, the prompt will reappear when the parent process has terminated, but then the child process will print more output roughly 4 seconds later in the terminal.

## Part 2 - mfork.c
1. In this program, there will be 8 distinct processes that exist throughout the execution. This is because at each subsequent fork() call, every created process thus far will spawn another child and therefore the number of processes will multiply by 2. This can be concisely described by the equation: (# of processes) = 2 ^ (# of fork() calls in sequence).

## Part 3 - pipe-sync.c
Relevant source code is pasted below. I used 2 pipes, one for each direction of communication. This ensures that the timing is deterministic and synchronized.
```c
  /* create pipes */
  int fd_1[2]; // For child to talk to the parent
  int fd_2[2]; // For parent to talk to the child
  pipe(fd_1);
  pipe(fd_2);

  ret = fork();
  int x = 1; // A random int to send as a pipe message
  if (ret == 0) {
    /* child process. */
    close(fd_1[0]);
    close(fd_2[1]); 
    printf("Child line 1\n");
    write(fd_1[1], &x, sizeof(int));
    read(fd_2[0], &x, sizeof(int));
    printf("Child line 2\n");
    write(fd_1[1], &x, sizeof(int));
  } else {

    /* parent process */
    close(fd_1[1]);
    close(fd_2[0]);
    read(fd_1[0], &x, sizeof(int));
    printf("Parent line 1\n");
    write(fd_2[1], &x, sizeof(int));
    read(fd_1[0], &x, sizeof(int));
    printf("Parent line 2\n");

    wait(&stat);
  }
```

## Part 4 - fifo_producer.c and fifo_consumer.c
1. If we only launch a producer and no consumer, then the producer process stalls after printing "waiting for readers...". This is because open(...) blocks until a reader for the given fifo is found.
2. If we only launch a consumer and no producer, then the consumer process stalls after printing "waiting for writers...". This is because open(...) blocks until a writer for the given fifo is found.
3. If there are multiple consumer processes reading a named pipe and only a single producer process, then the data written by the producer to the pipe will be read by only one of the consumer processes. The rest of the consumer processes will not read the data. This is because once data is read from a named pipe, the data is popped from the buffer.
4. If all the consumers are terminated, then the producer will no longer be able to write messages to the pipe. This is because the write(...) call is blocking (dependent upon a consumer). Hence, any messages written by the producer to the pipe will not be written until there is a consumer.
5. If all producers are terminated/killed, then the consumer processes are also terminated. This is because the consumers' loop condition is to read the pipe buffer continuoulsy until the size of the read in from the pipe is 0. This indicates that the loop will terminate and the program will end.

## Part 5 - shared_memory3.c
