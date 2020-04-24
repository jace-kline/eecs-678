#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void err_quit (const char * mesg)
{
  printf ("%s\n", mesg);
  exit(1);
}

void err_sys (const char * mesg)
{
  perror(mesg);
  exit(errno);
}

int main (int argc, char *argv[])
{
  int fdin, fdout, bufsz;
  char *src;
  struct stat statbuf;

  if (argc != 4)
    err_quit ("usage: read_write <fromfile> <tofile> <buf_size>");

  /* open the input file */
  if ((fdin = open (argv[1], O_RDONLY)) < 0) {
    char buf[256];
    sprintf(buf, "can't open %s for reading", argv[1]);
    perror(buf);
    exit(errno);
  }

  /* open/create the output file */
  if ((fdout = open (argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
    char buf[256];
    sprintf (buf, "can't create %s for writing", argv[2]);
    perror(buf);
    exit(errno);
  }

  /* Allocate a buffer of the size specified */
  bufsz = atoi(argv[3]);
  src = malloc(bufsz);
  
  /* And use it to copy the file */

  // Get info on the source file to extract # of bytes (file size)
  if(fstat(fdin, &statbuf) == -1) {
    perror("Could not read stats for input file.");
    exit(errno);
  }
  
  int srcbytes = (int)statbuf.st_size; // # of bytes in source file
  int rembufsz = srcbytes % bufsz; // Size of the remainder buffer to read to
  int iters = srcbytes / bufsz; // How many times we can iterate (read) with the input buffer size
  char *src_rem = malloc(rembufsz);

  // Copy the portion with specified buffer size
  for (int i = 0; i < iters; i++) {
    if((read (fdin, src, bufsz)) > 0) {
      write (fdout, src, bufsz);
    }
  }

  // Copy the remainder (to make file sizes of source and destination match)
  if((read (fdin, src_rem, rembufsz)) > 0) {
    write (fdout, src_rem, rembufsz);
  }
} /* main */