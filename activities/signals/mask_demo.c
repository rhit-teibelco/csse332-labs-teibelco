#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void mask_signal(int signum)
{
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, signum);
  if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    perror("sigprocmask:");
}

static void unmask_signal(int signum)
{
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, signum);
  if(sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
    perror("sigprocmask:");
}

static void setsighandler(int signum, void (*handler)(int)) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(signum, &act, NULL);
}

int num_sigint = 0;

void handle_sig_int(int sig) {
  printf("Ouch, received interrupt signal number %d\n", ++num_sigint);
}

int main(int argc, char **argv) {
  setsighandler(SIGINT, handle_sig_int);
  printf("Process %s (%d) started...\n", argv[0], getpid());

  while(num_sigint < 3);

  printf("[%s:%d] Tired of you interrupting me...\n", argv[0], getpid());
  mask_signal(SIGINT);
  printf("[%s:%d] Let's see you try now...\n", argv[0], getpid());
  while(1);
}

