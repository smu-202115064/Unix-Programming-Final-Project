#ifndef	_QUEUE_H
#define	_QUEUE_H 1

#include <stdio.h>
#include <stdlib.h>


typedef struct queue_node {
    int data;
    struct queue_node *next;
} t_queue_node;


typedef struct queue {
    struct queue_node *front;
    struct queue_node *rear;
    size_t size;
} t_queue;


void queue_init(t_queue *queue);


/**
 * @brief 큐에 데이터를 삽입
 *
 * @param queue 데이터를 삽입할 큐
 * @param data 삽입할 데이터
 */
void queue_push(t_queue *queue, const int data);


/**
 * @brief 큐에서 데이터를 추출
 *
 * @param queue 데이터를 뽑아낼 큐
 * @return int 추출한 데이터
 */
int queue_pop(t_queue *queue);


void queue_remove(t_queue *queue, const int data);


int queue_front(const t_queue *queue);


bool queue_contains(const t_queue *queue, const int data);


void queue_print(const t_queue *queue);


#endif
