/* Copyright 2021 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**
  Imagine a group of friends are getting together to play music, but
  they are arriving at different times.  Arriving can happen at any
  time (e.g. when some other friends are playing).

  There are 3 different kinds of friends - drummers, singers, and
  guitarists.  It takes one of each kind to make a band, plus only
  1 band can be playing at once.  Once those conditions are met, the
  players can start playing and stop playing in any order.  However,
  all 3 players must stop playing before a new set of 3 can start
  playing.

  Example output:

  drummer arrived
  drummer arrived
  guitarist arrived
  guitarist arrived
  singer arrived
  drummer playing
  guitarist playing
  singer playing
  singer arrived
  singer arrived
  drummer arrived
  guitarist arrived
  drummer finished playing
  guitarist finished playing
  singer finished playing
  singer playing
  guitarist playing
  drummer playing
  singer finished playing
  guitarist finished playing
  drummer finished playing
  guitarist playing
  drummer playing
  singer playing
  guitarist finished playing
  drummer finished playing
  singer finished playing
  Everything finished.


 **/

int DRUM = 0;
int SING = 1;
int GUIT = 2;
int NONE = 0;
char drummer[] = "drummer";
char singer[] = "singer";
char guitarist[] = "guitarist";

char* names[] = {drummer, singer, guitarist};
int drummersReady = 0;
int singersReady = 0;
int guitaristsReady = 0;
int bandDone = 0;
int bandDrummer;
int bandSinger;
int bandGuitarist;
int idmkr = 1;

pthread_cond_t keyDone = PTHREAD_COND_INITIALIZER;
pthread_cond_t keyS = PTHREAD_COND_INITIALIZER;
pthread_cond_t keyG = PTHREAD_COND_INITIALIZER;
pthread_cond_t keyBand = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

// because the code is similar, we'll just have one kind of thread
// and we'll pass its kind as a parameter

void helper(int id, int kind){
  if (kind == DRUM){
    if (bandDrummer == NONE){
      bandDrummer = id;
      return;
    }
  }
  else if (kind == SING){
    if (bandSinger == NONE){
      bandSinger = id;
      return;
    }
  }
  else if (kind == GUIT){
    if (bandGuitarist == NONE){
      bandGuitarist = id;
      return;
    }
  }

  pthread_cond_wait(&keyDone, &lock);
  helper(id, kind);
 
}
void* friend(void * kind_ptr) {
  int kind = *((int*) kind_ptr);

  printf("%s arrived\n", names[kind]);
  pthread_mutex_lock(&lock); // lock
  //////////////////////////////////
  
  int id = idmkr;
  idmkr++;

  helper(id, kind); 
  
  if (bandDrummer == NONE || bandSinger == NONE || bandGuitarist == NONE){
    pthread_cond_wait(&keyBand, &lock);
  }
  else{
    pthread_cond_broadcast(&keyBand);
  }

  printf("%s playing\n", names[kind]);

  //////////////////////////////////////
  pthread_mutex_unlock(&lock); // unlock              
  sleep(1); 
  pthread_mutex_lock(&lock); // lock
  //////////////////////////////////
                             
  printf("%s finished playing\n", names[kind]);

  if (kind == DRUM){
    bandDrummer = NONE;
  }
  else if (kind == SING){
    bandSinger = NONE;
  }
  else if (kind == GUIT){
    bandGuitarist = NONE;
  }

  if (bandDrummer == NONE && bandSinger == NONE && bandGuitarist == NONE){
    pthread_cond_broadcast(&keyDone);
  }

  //////////////////////////////////////
  pthread_mutex_unlock(&lock); // unlock
  return NULL;
}


pthread_t friends[100];
int friend_count = 0;

void create_friend(int* kind) {
  pthread_create(&friends[friend_count], NULL, friend, kind);
  friend_count++;
}

int main(int argc, char **argv) {

  create_friend(&DRUM);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&GUIT);
  sleep(1);
  create_friend(&SING);
  create_friend(&SING);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&SING);

  // all threads must be created by this point
  // note if you didn't create an equal number of each, we'll be stuck forever
  for (int i = 0; i < friend_count; i++) {
    pthread_join(friends[i], NULL);
  }

  printf("Everything finished.\n");

}
