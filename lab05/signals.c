#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <stdbool.h>   /* using bool data type			                     */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */
#include <sys/types.h> /*needed for pid_t                                */

/* first, define the Ctrl-C-Ctrl-Z counter, initialize it with zero. */
int ctrl_c_ctrl_z_count = 0;

/* boolean flags to keep track of the ^C and ^Z combinations */
bool ctrl_c_pressed = false;
bool ctrl_z_pressed = false;

/* flag to keep track of user response after exit prompt */
int got_response = 0;

#define CTRL_C_CTRL_Z_THRESHOLD  5 

pid_t my_pid;

/* Implement alarm handler - (You should terminate the program with "kill" system call and SIGQUIT signal if the user has not responded */
//Your code


void catch_alarm(int sig_num) {
  if(got_response == 0) kill(getpid(), SIGQUIT);
}

void check_count(){
  /* Check if threshold was reached */
  got_response = 0;
  if (ctrl_c_ctrl_z_count >= CTRL_C_CTRL_Z_THRESHOLD) {
    char answer[30];

    /* Prompt the user to tell us if to really exit or not - if user hasn't responded in 5 seconds, "kill" the program */
    //Your code

    alarm(5);
    
    printf("\nReally exit? [Y/n]: ");
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
      printf("\nContinuing\n");
      fflush(stdout);

      /* Keep track of the fact that the user has responded */
      //Your code
      got_response = 1;

      /* Reset the ctrl_c_ctrl_z_count counter */
      //Your code
      ctrl_c_ctrl_z_count = 0;   
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* Check for the following combinations:
      a. ^C ^C (reset counter)
      b. ^C ^Z (increment counter)
      c. ^C    
     and write your code for each of these combinations */
  //Your code
  int s2 = pause();
  switch (s2) {
    case SIGINT: ctrl_c_ctrl_z_count = 0; break;
    case SIGTSTP: ctrl_c_ctrl_z_count++; break;
    default: break;
  }

  check_count();
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* Check for the following combinations:
      a. ^Z ^Z (reset counter)
      b. ^Z ^C (increment counter)
      c. ^Z
     and write your code for each of these combinations */
  //Your code
    int s2 = pause();
    switch (s2) {
      case SIGTSTP: ctrl_c_ctrl_z_count = 0; break;
      case SIGINT: ctrl_c_ctrl_z_count++; break;
      default: break;
    }
  
  check_count();
}

void handler(int sig_num) {
  switch (sig_num) {
    case SIGINT: catch_int(sig_num); break;
    case SIGTSTP: catch_tstp(sig_num); break;
    case SIGALRM: catch_alarm(sig_num); break;
    default: break;
  }
}

int main(int argc, char* argv[])
{
  my_pid = getpid();
  printf("My PID: %d\n", my_pid);

  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));

  sigset_t mask_set;  /* used to set a signal masking set. */

  /* setup mask_set */
  //Your code
  sigfillset(&mask_set);
  // sigdelset(&mask_set, SIGINT);
  // sigdelset(&mask_set, SIGTSTP);
  sigdelset(&mask_set, SIGALRM);
  sa.sa_mask = mask_set;
  

  
  /* set signal handlers */
  //Your code
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTSTP, &sa, NULL);
  sigaction(SIGALRM, &sa, NULL);
  sa.sa_handler = handler;

  while(true) pause();

  return 0;
}

