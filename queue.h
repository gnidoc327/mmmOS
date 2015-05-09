#ifndef _ST_QUEUE_
#define _ST_QUEUE_

#include "sthread.h"
#include <stdlib.h>

struct st_queue {
     struct st_node* first;
	 struct st_node* last;
};

struct st_node {
     struct st_node* next;
	 sthread_t st;
};

void stq_init(struct st_queue* queue);

void stq_enqueue(struct st_queue* queue, sthread_t t);

sthread_t stq_dequeue(struct st_queue* queue);

#endif
