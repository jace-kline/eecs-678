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
5. If all producers are terminated/killed, then the consumer processes are also terminated. This is because the consumers' loop condition is to read the pipe buffer continuously until the size of the read in from the pipe is 0. This indicates that the loop will terminate and the program will end.

## Part 5 - shared_memory3.c
The output of this program can be explained as follows: First, the shared_buf and the unshared_buf are declared. The shared_buf is then attached to a shared memory location created by shmget(...) above it. This makes the shared_buf buffer a shared buffer between parent and child processes in any fork() calls that occur. After declaring the two buffers, the string "First String" is copied into both buffers using the strcopy(...) function. The fork() is then called, creating a child process and, consequently, data is copied between the parent and child upon creation. However, this is where the two buffers, shared_buf and unshared_buf, are treated differently. Because shared_buf was initialized to be shared memory, the child and the parent share this buffer. On the other hand, the unshared_buf contents are simply copied between the parent and child processes, but the underlying buffer is different. Hence, when the buffers are printed initially in the child, the output contents appear the same in the child even though the unshared_buf memory location is actually different between the two as they exist in separate address spaces (because they are different processes). Then, when the do_child(...) function is called in the child process, both the shared_buf and the unshared_buf contents are overwritten by the content "Second String". However, since the shared_buf buffer is shared memory, this value is changed for the parent process as well. On the contrary, the overwritten unshared_buf buffer in the child process has no effect on the unshared_buf buffer in the parent process. The final output of the buffers from the parent process yields that the shared_buf is referencing the string "Second String" from shared memory (that was mutated by the child process), and the unshared_buf is referencing the string "First String" that was existent in the address space of the parent (unshared memory). 

## Part 6 - thread-1.c
1. Changes made to both the local and global variables of the child process will not have any effect on variables that exist in the parent process. This is because the fork() command creates an entirely new process (the child), and a new process implies that a completely separate address space is created (no data is shared). Hence, the none of the data in the stack, heap, or data sections of the child process will affect/overlap with those of the parent process.
2. Threads are different from processes because a single process can spawn multiple threads within the same address space. When creating new threads (within a process), there are many components shared amongst all of the threads. Among the shared components between the threads is the data block (for global variables). Among the components that are not shared between the threads is the stack because each thread gets their own stack upon creation. Using this information, we can conclude that the child threads' mutation of global variables will be reflected in the parent thread because the global variables exist in the data region of the address space. However, the mutation of local variables that are used in the child threads will have no effect on any local variables in the parent thread. This is because local variables exist in the stack activation records, but the stacks of all the threads are separate, therefore implying that no stack activation records are shared and therefore none of the local data is shared.
