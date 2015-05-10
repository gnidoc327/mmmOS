#include "queue.h"


void stq_init(struct st_queue* queue){
     queue->first = NULL;
	 queue->last = NULL;
}


void stq_enqueue(struct st_queue* queue, sthread_t t){

     struct st_node* node = malloc(sizeof(struct st_node));

	 node->st = t;
	 node->next = NULL;

	 if (queue->last == NULL){
	 	queue->last = node;
		queue->first = node;
	 } else {
		queue->last->next = node;
		queue->last = node;
	 }
}


sthread_t stq_dequeue(struct st_queue* queue){

     sthread_t returnNode = NULL;
	 struct st_node* stNode = NULL;

	 if (queue->first = NULL){
	 	return NULL;
	 } else {
		returnNode = queue->first->st;
		stNode = queue->first;
		queue->first = queue->first->next;
        if (queue->first == NULL){
			queue->last = NULL;
		}
		free(stNode);
		return returnNode;
	 }
}
