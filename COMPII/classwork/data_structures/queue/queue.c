#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct queue
{
    int size;
    int capacity;
    int index_of_front;
    int* data;
};

typedef struct queue Queue;

QUEUE queue_init_default(void)
{
    Queue* pQ;
    pQ = (Queue*)malloc(sizeof(Queue));
    if(pQ != NULL)
    {
        pQ->size = 0;
        pQ->capacity = 10;
        pQ->index_of_front = 0;
        pQ->data = (int*)malloc(sizeof(int)*pQ->capacity);
        if(pQ->data == NULL)
        {
            free(pQ);
            return NULL;
        }
    }
    return pQ;
}

Boolean queue_is_empty(QUEUE hQ)
{
    Queue* pQ = (Queue*)hQ;
    
    return (Boolean)(pQ->size <= 0);
}

Status queue_insert(QUEUE hQ, int value)
{
    Queue* pQ = (Queue*)hQ;
    int* temp;
    int i;

    if(pQ->size >= pQ->capacity)
    {
        temp = (int*)malloc(sizeof(int)*pQ->capacity*2);
        if(temp == NULL)
        {
            return FAILURE;
        }
        for(i=0; i<pQ->size; i++)
        {
            temp[i] = pQ->data[(pQ->index_of_front + i) % pQ->capacity];
        }
        pQ->index_of_front = 0;
        free(pQ->data);
        pQ->data = temp;
        pQ->capacity *= 2;
    }
    pQ->data[pQ->index_of_front + pQ->size % pQ->capacity] = value;
    pQ->size++;
    return SUCCESS;
}

Status queue_service(QUEUE hQ)
{
    Queue* pQ = (Queue*)hQ;
    if(queue_is_empty(hQ))
    {
        return FAILURE;
    }
    pQ->index_of_front = (pQ->index_of_front + 1) % pQ->capacity;
    pQ->size--;
    return SUCCESS;
}

int queue_front(QUEUE hQ, Status* pStatus)
{
    Queue* pQ = (Queue*)hQ;
    if(queue_is_empty(hQ))
    {
        if(pStatus != NULL)
        {
            *pStatus = FAILURE;
        }
        return -1337;
    }
    if(pStatus != NULL)
    {
        *pStatus = SUCCESS;
    }
    Status status;
    queue_front(hQ, &status);
    queue_front(hQ, NULL);

    return pQ->data[pQ->index_of_front];
}

void queue_destroy(QUEUE* phQ)
{
    Queue* pQ = (Queue*)*phQ;
    free(pQ->data);
    free(pQ);
    *phQ = NULL;

}