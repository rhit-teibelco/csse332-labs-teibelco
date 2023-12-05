#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void setsighandler(int signum, void (*handler)(int)) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(signum, &act, NULL);
}

void handle_seg_fault(int sig) {
  printf("oh oh, I accessed memory incorrectly\n");
  printf("probably should exit gracefully\n");

  exit(99);

  // Question: What do you think will happen if we just use the following:
  // return;
}

int main(int argc, char **argv) {
  int *p = 0x00;

  setsighandler(SIGSEGV, handle_seg_fault);
  printf("Dereferencing p would give us: %d\n", *p);

  printf("What do you think will happen here?\n");
  return 0;
}
