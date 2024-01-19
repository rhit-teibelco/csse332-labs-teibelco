#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Constants in the code
#define FIRST_ROOM_CAPACITY 5
#define FIRST_ROOM_SERVICE_RATE 1

#define SECOND_ROOM_CAPACITY 2
#define SECOND_ROOM_SERVICE_RATE 2
#define WAITING_ROOM_CAPACITY 2

#define NUM_PEOPLE_ARRIVING 10

// capture the of customers that leave early
int num_left_customers = 0;
// TODO: Define other state of the world variables here
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t key1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t key2 = PTHREAD_COND_INITIALIZER;

int numRoom1 = 0;
int numRoom2 = 0;
int waiting2 = 0;
// TODO: Define your mutex locks and condition variables:

void *customer(void *arg)
{
  long int tid = (long int)arg;

  printf("[Customer %ld] Just arrived at first room...\n", tid);

  /*
  //HANDLE ROOM 1
  pthread_mutex_lock(&lock); //lock
  if (numRoom1 >= 5){
    pthread_cond_wait(&key1, &lock);
  }
  else{
    pthread_cond_broadcast(&key1);
  }

  numRoom1++;
  printf("numinfirst: %d\n", numRoom1);
  printf("[Customer %ld] Entered first room...\n", tid);
  pthread_mutex_unlock(&lock); //unlock

  sleep(FIRST_ROOM_SERVICE_RATE);

  pthread_mutex_lock(&lock); //lock
  printf("[Customer %ld] Left first room...\n", tid);
  numRoom1--;
  printf("numinfirst: %d\n", numRoom1);
  pthread_mutex_unlock(&lock);

  */

  //HANDLE ROOM 1
  pthread_mutex_lock(&lock); //lock

  if (numRoom1 >= 5){
    printf("[Customer %ld] Waiting...\n", tid);
    pthread_cond_wait(&key1, &lock);
  }

  numRoom1++;
  if (numRoom1 > 5){
    printf("\nROOM 1 CAPACITY ERROR\n");
  }

  printf("[Customer %ld] Entered first room...\n", tid);

  pthread_mutex_unlock(&lock); //unlock

  sleep(SECOND_ROOM_SERVICE_RATE);

  pthread_mutex_lock(&lock); //lock
                             
  numRoom1--;
  if (numRoom1 < 0){
    printf("\nROOM 1 CAPACITY ERROR\n");
  }

  printf("[Customer %ld] Left first room...\n", tid);

  pthread_cond_signal(&key1);

  pthread_mutex_unlock(&lock); //unlock


  //HANDLE WAITING ROOM 

  pthread_mutex_lock(&lock); //lock
  
  int doReturn = 0;
 
  if(waiting2 >= 2) 
  {
    doReturn = 1;

    num_left_customers++;

    printf("[Customer %ld] Left unhapy because waiting room is full...\n", tid);

  }
  else{
    doReturn = 0;
 
    waiting2++;
    if (waiting2 > 2){
      printf("WAITING CAPACITY ERROR\n");
    }

    printf("[Customer %ld] Joined the waiting room for second room...\n", tid);
  }

  pthread_mutex_unlock(&lock); //unlock
  
  //UNHAPPY RETURN
  
  if (doReturn){
    return 0;
  }

  //HANDLE ROOM 2
  pthread_mutex_lock(&lock); //lock

  if (numRoom2 >= 2){
    printf("[Customer %ld] Waiting...\n", tid);
    pthread_cond_wait(&key2, &lock);
  }

  waiting2--;
  if (waiting2 < 0){
    printf("\nWAITING ROOM CAPACITY ERROR\n");
  }

  numRoom2++;
  if (numRoom2 > 2){
    printf("\nROOM 2 CAPACITY ERROR\n");
  }

  printf("[Customer %ld] Entered second room...\n", tid);

  pthread_mutex_unlock(&lock); //unlock

  sleep(SECOND_ROOM_SERVICE_RATE);

  pthread_mutex_lock(&lock); //lock
                             
  numRoom2--;
  if (numRoom2 < 0){
    printf("\nROOM 2 CAPACITY ERROR\n");
  }

  printf("[Customer %ld] Left second room...\n", tid);

  pthread_cond_signal(&key2);

  pthread_mutex_unlock(&lock); //unlock
  
  return 0;
}

int
main(int argc, char **argv)
{
  int i;
  long int tids[NUM_PEOPLE_ARRIVING];
  pthread_t threads[NUM_PEOPLE_ARRIVING];
  srand(time(0));

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, customer, (void*)tids[i]);
    if(!(i % 2))
      sleep(rand() % 2);
  }

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everything finished: %d customers left unhappy...\n",
         num_left_customers);
  exit(EXIT_SUCCESS);
}
