/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include "sthread.h"
#include "sync.h"
#include "stddef.h"
#include "stddef.h"
#include "stddef.h"

/*
 * Mutex routines
 */

int sthread_mutex_init(sthread_mutex_t *mutex){
	mutex->count = 0;
	stq_init(&mutex->q);
	mutex->current = NULL;

	return 0;
}

int sthread_mutex_destroy(sthread_mutex_t *mutex){
	return 0;
}

int sthread_mutex_lock(sthread_mutex_t *mutex){
	if( mutex->current == sthread_self() ){
		mutex->count++;
	}else if( !sthread_mutex_trylock(mutex) ){
 		stq_enqueue(&mutex->q, sthread_self());
		sthread_suspend();
	}

	return 0;
}

int sthread_mutex_trylock(sthread_mutex_t *mutex){
	if( mutex->current == NULL ){
		mutex->current = sthread_self();
		mutex->count = 1;
		return 1;
	}

	return 0;
}

int sthread_mutex_unlock(sthread_mutex_t *mutex){
	mutex->count--;
	if( mutex->count == 0 ){
		mutex->current = stq_dequeue(&mutex->q);
		mutex->count = 1;
		sthread_wake(mutex->current);
	}

	return 0;
}

