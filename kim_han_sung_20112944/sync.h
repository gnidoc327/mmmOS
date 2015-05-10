/*
 * kim han sung, etc.
 *
 * sync.h
 */

#ifndef _STHREAD_SYNC_H_
#define _STHREAD_SYNC_H_


struct sthread_mutex_struct {
	int count;
	struct queue *queue;
	sthread_t current;
};

typedef struct sthread_mutex_struct sthread_mutex_t;

int sthread_mutex_init(sthread_mutex_t *mutex);
int sthread_mutex_destroy(sthread_mutex_t *mutex);
int sthread_mutex_lock(sthread_mutex_t *mutex);
int sthread_mutex_trylock(sthread_mutex_t *mutex);
int sthread_mutex_unlock(sthread_mutex_t *mutex);

#endif
