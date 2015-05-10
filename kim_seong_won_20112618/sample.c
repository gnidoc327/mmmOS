#include <errno.h>
#include "sthread.h"
#include "sync.h"
#include <stdio.h>

int shared_mem = 0;
sthread_mutex_t mutex;


// lock mutex a few times and increment shared_mem 
int thread1(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);

 int i;
 for (i=0; i<=threadno; i++){


  printf("thread %d: want that critical section!\n", threadno);
  sthread_mutex_lock(&mutex);
  
  printf("thread %d: this critical section!\n", threadno);
  shared_mem++;
  sleep(1);
  sthread_mutex_unlock(&mutex);
  printf("thread %d: out to there.\n", threadno);

 }

 printf("thread %d: peace out.\n", threadno);

 return 0;
}


//test "recursiveness" of the mutex.  
int thread2(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);


 printf("thread %d: want that critical section.\n", threadno);
 sthread_mutex_lock(&mutex);
 printf("thread %d: get that critical section.\n", threadno);

 sleep(1);

 sthread_mutex_lock(&mutex);

 printf("thread %d: locked again\n", threadno);

 shared_mem++;
 sleep(1);


 sthread_mutex_unlock(&mutex);

 printf("thread %d: unlocked once. so step off.\n", threadno);

 sleep(1);

 sthread_mutex_unlock(&mutex);  

 printf("thread %d: peace out.\n", threadno);
}



//just holds the lock for a while
int thread3(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);


 sthread_mutex_lock(&mutex);
 printf("thread %d: Got the lock.  hanging out for a while.\n", threadno);

 sleep(5);

 sthread_mutex_unlock(&mutex);
 printf("thread %d: peace out.\n", threadno);
}


//tests trylock
int thread4(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);

 int i;
 for (i = 0; i < 10; i++){

  printf("thread %d: checking that lock out...\n", threadno);
  if (sthread_mutex_trylock(&mutex)){
   printf("thread %d: Damn!  Finally!\n", threadno);

   shared_mem++;
   sleep(1);

   sthread_mutex_unlock(&mutex);
   break;


  } else {
   printf("thread %d: whatever, didn't want it anyways\n", threadno);
  }
 }

 printf("thread %d: peace out.\n", threadno);
 return 0;
}


int main(int argc, char** argv){

 sthread_t threads[7];

 if (sthread_init() == -1){
  fprintf(stderr, "%s: sthread_init: %s\n", argv[0], strerror(errno));
  return 0;
 }


 int i;
 for (i=0; i<4; i++){
  if (sthread_create(&(threads[i]), thread1, (void *)(i+1)) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[4]), thread2, (void *)(5)) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[i]), thread3, (void *)6) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[i]), thread4, (void *)7) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  return 0;
 }
}
