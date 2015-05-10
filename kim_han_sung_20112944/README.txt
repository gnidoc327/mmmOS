Machine Problem 2 : Synchronization and Threads in User Space

Team name : Manner Maketh Man

---------------------------------------------------------------

Member : kim han sung(20112944), kim seong won(20112618)

Environment - ubuntu 14.04 LTS

Executable, Special compiling instructions or Anything 

---------------------------------------------------------------

-Executable thing

sthread_mutex_init(sthread_mutex_t *mutex);

sthread_mutex_destroy(sthread_mutex_t *mutex);

sthread_mutex_lock(sthread_mutex_t *mutex);

sthread_mutex_trylock(sthread_mutex_t *mutex);

sthread_mutex_unlock(sthread_mutex_t *mutex);


-Special Compiling

queue // for sthread

- Anything

MakeFile // this makefile is not working
sample.c // mutex test code
