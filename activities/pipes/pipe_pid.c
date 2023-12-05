#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * pipe_pid.c
 *
 * Task:
 * =====
 * In this exercise, we would like to child to send its process id back to its
 * parent. Yes we can get it from the return value of fork, but we'd like to
 * have a backup way for getting it as well.
 *
 * HINT 1:
 * =======
 * You might find the functions sprintf, snprintf, and atoi very helpful.
 *
 * HINT 2:
 * =======
 *  Alternatively, remember that everything is just a bunch of bytes.  How many
 *  bytes is an integer?
 */

int main(int argc, char **argv) {
  pid_t pid;
  int fd[2];
  int nbytes;
  char readbuff[512];

  //Step 1: Call pipe(fd)
  if (pipe(fd) < 0){
    perror("PANIC: pipe failed: ");
    exit(99);
  }


  //Step 2: We fork
  pid = fork();
  if(pid == 0) {
    // child, I am the writer
    //Child closes reading end
    close(fd[0]);


    //Write message
    write(fd[1], pid, strlen(pid));
    exit(0);
  }

  // parent, I am the reader
  exit(0);
}

