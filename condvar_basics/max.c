#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int inSection = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t key = PTHREAD_COND_INITIALIZER;

void *thread(void *arg)
{
	char *letter = (char *)arg;
	printf("%c wants to enter the critical section\n", *letter);

  pthread_mutex_lock(&lock);

  printf("# threads in section %d\n", inSection); 

  if (inSection >= 3){
    pthread_cond_wait(&key, &lock);
  }
  else{
    inSection++;
    pthread_cond_signal(&key);
  }

  pthread_mutex_unlock(&lock);
  
	printf("%c is in the critical section\n", *letter);
	sleep(1);
	printf("%c has left the critical section\n", *letter);

  pthread_mutex_lock(&lock);
  inSection--;
  pthread_mutex_unlock(&lock);

	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[8];
	int i;
	char *letters = "abcdefgh";

	for(i = 0; i < 8; ++i) {
		pthread_create(&threads[i], NULL, thread, &letters[i]);

		if(i == 4)
			sleep(4);
	}

	for(i = 0; i < 8; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished...\n");

	return 0;
}
