#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <stdbool.h>   /* using bool data type			                     */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */
#include <sys/types.h> /*needed for pid_t                                */

/* first, define the Ctrl-C-Ctrl-Z counter, initialize it with zero. */
int ctrl_c_ctrl_z_count = 0;

// Need to iterate mod 2. If goes back to 0, then we know need to reset the ctrl_c_pressed, ctrl_z_pressed.
int mod2_counter = 0;

/* boolean flags to keep track of the ^C and ^Z combinations */
bool ctrl_c_pressed = false;
bool ctrl_z_pressed = false;

/* flag to keep track of user response after exit prompt */
int got_response = 0;

#define CTRL_C_CTRL_Z_THRESHOLD  5 

pid_t my_pid;

/* Implement alarm handler - (You should terminate the program with "kill" system call and SIGQUIT signal if the user has not responded */
//Your code

void catch_alrm(int sig_num) {
  if(got_response == 0) {
    printf("\nUser taking too long to respond.\n");
    kill(my_pid, SIGQUIT);
  }
  got_response = 0;
}

void iterate_mod2_counter() {
  mod2_counter++;
  if(mod2_counter % 2 == 0) {
    ctrl_c_pressed = false;
    ctrl_z_pressed = false;
  }
}

void check_count(){
  /* Check if threshold was reached */
  if (ctrl_c_ctrl_z_count >= CTRL_C_CTRL_Z_THRESHOLD) {
    char answer[30];

    /* Prompt the user to tell us if to really exit or not - if user hasn't responded in 5 seconds, "kill" the program */
    //Your code

    alarm(5);
    
    printf("\nReally exit? [Y/n]: ");
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
      /* Keep track of the fact that the user has responded */
      //Your code
      got_response = 1;

      printf("\nContinuing\n");
      fflush(stdout);
      

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
      b. ^Z ^C (increment counter)
      c. ^C    
     and write your code for each of these combinations */
  //Your code
  if(ctrl_z_pressed || ctrl_c_pressed) {
    if(ctrl_c_pressed) {
      ctrl_c_ctrl_z_count = 0; 
      printf(" Count - Resetting\n");
    }
    else if(ctrl_z_pressed) {
      ctrl_c_ctrl_z_count++;
      printf(" - Count: %d\n", ctrl_c_ctrl_z_count);
    }
    fflush(stdout);
  }
  ctrl_c_pressed = true;

  check_count();
  iterate_mod2_counter();
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* Check for the following combinations:
      a. ^Z ^Z (reset counter)
      b. ^C ^Z (increment counter)
      c. ^Z
     and write your code for each of these combinations */
  //Your code
  if(ctrl_z_pressed || ctrl_c_pressed) {
    if(ctrl_z_pressed) {
      ctrl_c_ctrl_z_count = 0; 
      printf(" Count - Resetting\n");
    }
    else if(ctrl_c_pressed) {
      ctrl_c_ctrl_z_count++;
      printf(" - Count: %d\n", ctrl_c_ctrl_z_count);
    }
    fflush(stdout);
  }
  
  ctrl_z_pressed = true;

  check_count();
  iterate_mod2_counter();
}

int main(int argc, char* argv[])
{
  my_pid = getpid();
  printf("My PID: %d\n", my_pid);

  /* setup mask_set */
  //Your code
  // sigset_t mask_set;  /* used to set a signal masking set. */
  // sigfillset(&mask_set);
  // sigdelset(&mask_set, SIGALRM);

  /* set signal handlers */
  //Your code

  struct sigaction sa_int, sa_tstp, sa_alrm;

  sa_int.sa_handler = catch_int;
  sigfillset(&sa_int.sa_mask);
  sigdelset(&sa_int.sa_mask, SIGALRM);
  sa_int.sa_flags = 0;

  sa_tstp.sa_handler = catch_tstp;
  sigfillset(&sa_tstp.sa_mask);
  sigdelset(&sa_tstp.sa_mask, SIGALRM);
  sa_tstp.sa_flags = 0;

  sa_alrm.sa_handler = catch_alrm;
  sigfillset(&sa_alrm.sa_mask);
  sa_alrm.sa_flags = 0;
  
  sigaction(SIGINT, &sa_int, NULL);
  sigaction(SIGTSTP, &sa_tstp, NULL);
  sigaction(SIGALRM, &sa_alrm, NULL);

  while(true) pause();

  return 0;
}

