#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"


// 한 번 생성한 노드는 재활용 하기위해 별도의 큐에 저장.
t_queue *disposed_node = NULL;


void queue_push_node(t_queue *queue, t_queue_node *node) {
    if (queue->size > 0) {
        queue->rear->next = node;
    }
    queue->rear = node;
    queue->size++;
}


t_queue_node *queue_pop_node(t_queue *queue) {
    t_queue_node *node = queue->front;
    queue->front = node->next;
    queue->size--;
    return node;
}


t_queue_node *queue_create_node(int data) {
    t_queue_node *node;
    if (disposed_node != NULL && disposed_node->size > 0) {
        // 기존에 쓸모를 다하여 버려진 노드를 회수하여 재활용.
        node = queue_pop_node(disposed_node);
    } else {
        // 새로운 공간을 할당하여 생성.
        node = malloc(sizeof(t_queue_node));
    }
    node->data = data;
    node->next = NULL;
    return node;
}


t_queue *queue_create() {
    t_queue *q = malloc(sizeof(t_queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}


void queue_push(t_queue *queue, const int data) {
    t_queue_node *node = queue_create_node(data);
    queue_push_node(queue, node);
}


int queue_pop(t_queue *queue) {
    if (queue->size == 0) {
        printf("empty queue\n");
        exit(EXIT_FAILURE);
    }
    t_queue_node *node = queue_pop_node(queue);
    if (disposed_node == NULL) {
        disposed_node = queue_create();
    }
    queue_push_node(disposed_node, node);
    return node->data;
}


void queue_remove(t_queue *queue, const int data) {
    for (int i = 0; i < queue->size; i++) {
        if (queue_front(queue) == data) {
            queue_pop(queue);
            return;
        }
        queue_push(queue, queue_pop(queue));
    }
}


int queue_front(const t_queue *queue) {
    if (queue->size == 0) {
        printf("empty queue\n");
        exit(EXIT_FAILURE);
    }
    return queue->front->data;
}


bool queue_contains(const t_queue *queue, const int data) {
    t_queue_node *node = queue->front;
    while (node != NULL) {
        if (node->data == data) {
            return true;
        }
        node = node->next;
    }
    return false;
}