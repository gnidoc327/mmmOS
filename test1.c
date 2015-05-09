#include <errno.h>
#include "sthread.h"
#include "sync.h"
#include <stdio.h>

int shared_mem = 0;
sthread_mutex_t mutex;


/**
 *  * threadmain1
 *   *
 *    * get a lock on the mutex a few times and increment the param
 *     *
 *      */
int threadmain1(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);

 int i;
 for (i=0; i<=threadno; i++){


  printf("thread %d: want that critical section!\n", threadno);
  sthread_mutex_lock(&mutex);

  printf("thread %d: fucking loving this critical section!\n", threadno);
  shared_mem++;
  sleep(1);
  sthread_mutex_unlock(&mutex);
  printf("thread %d: outta there.\n", threadno);

 }

 printf("thread %d: peace out.\n", threadno);

 return 0;
}


/**
 *  * threadmain2
 *   *
 *    * test "recursiveness" of the mutex.  
 *     */
int threadmain2(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);


 printf("thread %d: want that critical section. \n", threadno);
 sthread_mutex_lock(&mutex);
 printf("thread %d: got that critical section.  oh yes i did!\n", threadno);

 sleep(1);

 sthread_mutex_lock(&mutex);

 printf("thread %d: locked that shit again! don't even doubt it!\n", threadno);

 shared_mem++;
 sleep(1);


 sthread_mutex_unlock(&mutex);

 printf("thread %d: unlocked once.  still got another though, so step off.\n", threadno);

 sleep(1);

 sthread_mutex_unlock(&mutex);  

 printf("thread %d: peace out.\n", threadno);
}



/**
 *  * just holds the lock for a while
 *   *
 *    */
int threadmain3(void *arg){
 int threadno = (int)arg;

 printf("thread %d: Starting!\n", threadno);


 sthread_mutex_lock(&mutex);
 printf("thread %d: Got the lock.  hanging out for a while.\n", threadno);

 sleep(5);

 sthread_mutex_unlock(&mutex);
 printf("thread %d: peace out.\n", threadno);
}


/**
 *  * tests trylock
 *   *
 *    */
int threadmain4(void *arg){
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
  if (sthread_create(&(threads[i]), threadmain1, (void *)(i+1)) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[4]), threadmain2, (void *)(5)) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[i]), threadmain3, (void *)6) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  if (sthread_create(&(threads[i]), threadmain4, (void *)7) == -1)
   fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  return 0;
 }
}
