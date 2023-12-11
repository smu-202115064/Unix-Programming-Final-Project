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


// 한 번 생성한 노드는 재활용 하기위해 별도의 큐에 저장.
t_queue *disposed_node = NULL;


/**
 * @brief 새로운 큐를 만드는 함수.
 *
 * 내부적으로 `malloc()`을 호출하여 생성한다.
 *
 * @return t_queue* 새로 생성된 큐의 포인터를 반환한다.
 */
t_queue *queue_create() {
    t_queue *q = malloc(sizeof(t_queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}


void queue_push_node(t_queue *queue, t_queue_node *node) {
    if (queue->size > 0) {
        queue->rear->next = node;
    }
    queue->rear = node;
    queue->size++;
}


t_queue_node *queue_pop_node(t_queue *queue) {
    t_queue_node *node;
    if (queue->size == 0) {
        printf("empty queue\n");
        exit(1);
    }
    node = queue->front;
    queue->front = node->next;
    queue->size--;
    return node;
}

/**
 * @brief 새로운 큐 노드를 생성(하거나 재활용)한다.
 *
 * @param data 노드에 담길 데이터
 * @return t_queue_node*
 */
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


/**
 * @brief 큐에 데이터를 삽입
 *
 * @param queue 데이터를 삽입할 큐
 * @param data 삽입할 데이터
 */
void queue_push(t_queue *queue, int data) {
    t_queue_node *node = queue_create_node(data);
    queue_push_node(queue, node);
}


/**
 * @brief 큐에서 데이터를 추출
 *
 * @param queue 데이터를 뽑아낼 큐
 * @return int 추출한 데이터
 */
int queue_pop(t_queue *queue) {
    t_queue_node *node = queue_pop_node(queue);
    if (disposed_node == NULL) {
        disposed_node = queue_create();
    }
    queue_push_node(disposed_node, node);
    return node->data;
}
