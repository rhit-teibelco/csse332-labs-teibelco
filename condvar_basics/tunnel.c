#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// constants to control number of cars and threads
#define NUM_EW_CARS 15
#define NUM_WE_CARS 15
#define NUM_AMBULANCE 7
#define TOTAL_THREADS NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE

// constants to define ambulance direction
#define EW_DIRECTION 1
#define WE_DIRECTION 2

// car speeds
#define EW_SPEED 3
#define WE_SPEED 2

// state variables
int numEW = 0;
int numWE = 0;
int isAmbulance = 0;
int ambulancesWaiting = 0;

// concurrency means
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t keyEW = PTHREAD_COND_INITIALIZER;
pthread_cond_t keyWE = PTHREAD_COND_INITIALIZER;

void *ewcar(void *arg)
{
  int tid = *(int*)arg;

  pthread_mutex_lock(&lock); //lock

  if (numEW >= 3 || isAmbulance || ambulancesWaiting >= 0){
    pthread_cond_wait(&keyEW, &lock);
  }

  numEW++;
  printf("Car (%d) entered tunnel in EW direction... %d %d %d\n", tid, numEW, isAmbulance, ambulancesWaiting);
  pthread_mutex_unlock(&lock); //unlock
                               
  sleep(EW_SPEED);

  pthread_mutex_lock(&lock); //lock
  numEW--;
  printf("Car (%d) exited tunnel in EW direction... %d %d %d\n", tid, numEW, isAmbulance, ambulancesWaiting);
  pthread_cond_signal(&keyEW);
  pthread_mutex_unlock(&lock); //unlock

  return 0;
}

void *wecar(void *arg)
{
  int tid = *(int *)arg;

  pthread_mutex_lock(&lock); //lock

  if (numWE >= 1){
    pthread_cond_wait(&keyWE, &lock);
  }

  numWE++;
  printf("Car (%d) entered tunnel in WE direction... %d %d %d\n", tid, numWE, isAmbulance, ambulancesWaiting);
  pthread_mutex_unlock(&lock); //unlock

  sleep(WE_SPEED);

  pthread_mutex_lock(&lock); //lock
  numWE--;
  printf("Car (%d) exited tunnel in WE direction... %d %d %d\n", tid, numWE, isAmbulance, ambulancesWaiting);
  pthread_cond_signal(&keyWE);
  pthread_mutex_unlock(&lock); //unlock

  return 0;
}

void *ambulance(void *arg)
{
  int tid = *(int*)arg;
  int direction = EW_DIRECTION;

  pthread_mutex_lock(&lock);

  if(direction == EW_DIRECTION) {

    if (numEW >= 3){
      ambulancesWaiting++;
      pthread_cond_wait(&keyEW, &lock);
      ambulancesWaiting--;
    }

    numEW++;
    printf("Ambulance %d entered the tunnel in EW direction %d %d %d\n", tid, numEW, isAmbulance, ambulancesWaiting);
  } else if (direction == WE_DIRECTION) {

    if (numWE >= 1){
      ambulancesWaiting++;
      pthread_cond_wait(&keyWE, &lock);
      ambulancesWaiting--;
    }

    numWE++;
    printf("Ambulance %d entered the tunnel in WE direction %d %d %d\n", tid, numWE, isAmbulance, ambulancesWaiting);
  }

  isAmbulance = 1;
  pthread_mutex_unlock(&lock);

  sleep(1); // ambulance is the fastest

  pthread_mutex_lock(&lock);
  isAmbulance = 0;

  if(direction == EW_DIRECTION) {
    numEW--;
    pthread_cond_signal(&keyEW);

    printf("Ambulance %d exited the tunnel in EW direction %d %d %d\n", tid, numEW, isAmbulance, ambulancesWaiting);
  } else if(direction == WE_DIRECTION) {
    numWE--;
    pthread_cond_signal(&keyWE);

    printf("Ambulance %d exited the tunnel in WE direction %d %d %d\n", tid, numEW, isAmbulance, ambulancesWaiting);
  }
  
  pthread_mutex_unlock(&lock);

  return 0;
}

int
main(int argc, char **argv)
{
  pthread_t threads[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int tids[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int i;
  srand(time(0));

  for(i = 0; i < NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ewcar, &tids[i]);
  }

  for(i = NUM_EW_CARS; i < NUM_WE_CARS + NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, wecar, &tids[i]);
  }

  for(i = NUM_EW_CARS + NUM_WE_CARS; i < TOTAL_THREADS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ambulance, &tids[i]);
    // make the ambulances arrive at random times
    sleep(rand() % 10);
  }

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everyting finished...\n");
  exit(EXIT_SUCCESS);
}
