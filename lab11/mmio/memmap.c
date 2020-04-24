/*
 * Example of using mmap. Taken from Advanced Programming in the Unix
 * Environment by Richard Stevens.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <unistd.h>
#include <string.h>  /* memcpy */
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
  int fdin, fdout, i;
  char *src, *dst, buf[256];
  struct stat statbuf;

  src = dst = NULL;

  if (argc != 3)
    err_quit ("usage: memmap <fromfile> <tofile>");

  /* 
   * open the input file 
   */
  if ((fdin = open (argv[1], O_RDONLY)) < 0) {
    sprintf(buf, "can't open %s for reading", argv[1]);
    perror(buf);
    exit(errno);
  }

  /* 
   * open/create the output file 
   */
  if ((fdout = open (argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
    sprintf (buf, "can't create %s for writing", argv[2]);
    perror(buf);
    exit(errno);
  }

  /* 
   * 1. find size of input file 
   */
  if(fstat(fdin, &statbuf) == -1) {
    perror("Could not read stats for input file.");
    exit(errno);
  } 
    // if doesn't fail, the size should be in statbuf.st_size

  /* 
   * 2. go to the location corresponding to the last byte 
   */
  lseek(fdout, (off_t)(statbuf.st_size - 1), SEEK_SET);

  /* 
   * 3. write a dummy byte at the last location 
   */
  char dummy = 'a';
  write(fdout, &dummy, sizeof(dummy));
  /* 
   * 4. mmap the input file 
   */
  src = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
  /* 
   * 5. mmap the output file 
   */
  dst = mmap(NULL, statbuf.st_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fdout, 0);
  /* 
   * 6. copy the input file to the output file 
   */
    /* Memory can be dereferenced using the * operator in C.  This line
     * stores what is in the memory location pointed to by src into
     * the memory location pointed to by dst.
     */
  for(int i = 0; i < (int)statbuf.st_size; i++) {
    dst[i] = src[i];
  }
    // *dst = *src;
} 


