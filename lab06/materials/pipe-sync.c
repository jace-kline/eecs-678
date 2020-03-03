/*
  pipe-sync.c: Use Pipe for Process Synchronization
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Question: Update this program to synchronize execution of the parent and
// child processes using pipes, so that we are guaranteed the following
// sequence of print messages:
// Child line 1
// Parent line 1
// Child line 2
// Parent line 2


int main()
{
  char *s, buf[1024];
  int ret, stat;
  s  = "Use Pipe for Process Synchronization\n";

  /* create pipe */
  int fd_1[2];
  int fd_2[2];
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
}
