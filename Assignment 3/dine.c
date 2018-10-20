#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void* funcPhil(void *arg);
int philN = 0, philE = 0;
pthread_mutex_t stateC [1000];


int main (int argc, char *argv[]) {

  int cond;
  int Identity[1000];

  pthread_t th[1000];
  //Getting input
  philN = atoi(argv[1]);
  philE = atoi(argv[2]);

  //error checking
  if (argc != 3 || philN < 2 || philE > 1000 || philE < 1)
  {

    printf("Please Retry command ./dine with only 2 arguments for # of Philosphers and # of times to eat");

  }

  else
  {

    for (int x =0; x < philN; x++) {
      //Loading each thread
      Identity[x] = x;
      cond = pthread_create(&(th[x]), NULL, &funcPhil, &(Identity[x]));

      //Mutex Initialization
      pthread_mutex_init(&stateC[x], NULL);
    }

    //Join and Destroy Threads.

    for (int p = 0; p < philN; p++) cond = pthread_join(th[p], NULL);
    for (int r = 0; r < philN; r++) cond = pthread_mutex_destroy(&stateC[r]);

    printf("All %d Philosophers have eaten %d time(s)\n", philN, philE);
  }

  return 0;

}

void* funcPhil(void *arg) {

  int low =0, count =0;
  int set = *((int*) arg);
  int temp = 0;
  int temp2 = 0;
  temp = philN - 1;
  low = set - 1;

  if (low == -1) low = temp;

  // Loop for the amount they have to eat.

  while (count < philE) {

    //Lock
    temp2 = set + 1;
    printf("Philospher %d Thinking...\n", temp2);
    pthread_mutex_lock(&stateC[set]);
    pthread_mutex_lock(&stateC[low]);


    //Unlock
    sleep(1.5);
    printf("Philospher %d Eating...\n", temp2);
    pthread_mutex_unlock(&stateC[set]);
    pthread_mutex_unlock(&stateC[low]);
    printf("Philospher %d Finished\n", temp2);

    count++;
  }
}
