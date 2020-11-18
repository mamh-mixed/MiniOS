#include <queue.h>

void _queueInit(Queue *queue, Uint32 capacity, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreInit(&queue->semaphore, 1);
    }

    queue->buf = (void **)sysMalloc((capacity + 1) * sizeof(void *));
    ASSERT(queue->buf != NULL);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
}

Bool _queueIsFull(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreGet(&queue->semaphore);
    }
    Bool isFull = (queue->front == ((queue->rear + 1) % (queue->capacity + 1))) ? TRUE : FALSE;
    if (isSync)
    {
        semaphoreRelease(&queue->semaphore);
    }
    return isFull;
}

Bool _queueIsEmpty(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreGet(&queue->semaphore);
    }
    Bool isEmpty = queue->front == queue->rear ? TRUE : FALSE;
    if (isSync)
    {
        semaphoreRelease(&queue->semaphore);
    }
    return isEmpty;
}

Uint32 _queueGetLength(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreGet(&queue->semaphore);
    }
    Uint32 length = (queue->rear - queue->front + (queue->capacity + 1)) % (queue->capacity + 1);
    if (isSync)
    {
        semaphoreRelease(&queue->semaphore);
    }
    return length;
}

void *_queueGetFront(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (!_queueIsEmpty(queue, isSync))
    {
        if (isSync)
        {
            semaphoreGet(&queue->semaphore);
        }
        void *value = queue->buf[queue->front];
        if (isSync)
        {
            semaphoreRelease(&queue->semaphore);
        }
        return value;
    }
    return NULL;
}

void _queuePop(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (!_queueIsEmpty(queue, isSync))
    {
        if (isSync)
        {
            semaphoreGet(&queue->semaphore);
        }
        queue->front = (queue->front + 1) % (queue->capacity + 1);
        if (isSync)
        {
            semaphoreRelease(&queue->semaphore);
        }
    }
}

void _queuePush(Queue *queue, void *value, Bool isSync)
{
    ASSERT(queue != NULL);
    if (!_queueIsFull(queue, isSync))
    {
        if (isSync)
        {
            semaphoreGet(&queue->semaphore);
        }
        queue->buf[queue->rear] = value;
        queue->rear = (queue->rear + 1) % (queue->capacity + 1);
        if (isSync)
        {
            semaphoreRelease(&queue->semaphore);
        }
    }
}

void _queueClear(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreGet(&queue->semaphore);
    }
    queue->front = 0;
    queue->rear = 0;
    if (isSync)
    {
        semaphoreRelease(&queue->semaphore);
    }
}

void _queueDestory(Queue *queue, Bool isSync)
{
    ASSERT(queue != NULL);
    if (isSync)
    {
        semaphoreGet(&queue->semaphore);
    }
    sysFree(queue->buf);
    if (isSync)
    {
        semaphoreRelease(&queue->semaphore);
    }
}

void queueSyncInit(Queue *queue, Uint32 capacity)
{
    _queueInit(queue, capacity, TRUE);
}

Bool queueSyncIsFull(Queue *queue)
{
    return _queueIsFull(queue,TRUE);
}

Bool queueSyncIsEmpty(Queue *queue)
{
    return _queueIsEmpty(queue, TRUE);
}

Uint32 queueSyncGetLength(Queue *queue)
{
    return _queueGetLength(queue, TRUE);
}

void *queueSyncGetFront(Queue *queue)
{
    return _queueGetFront(queue, TRUE);
}

void queueSyncPop(Queue *queue)
{
    _queuePop(queue, TRUE);
}

void queueSyncPush(Queue *queue, void *value)
{
    _queuePush(queue, value, TRUE);
}

void queueSyncClear(Queue *queue)
{
    _queueClear(queue, TRUE);
}

void queueSyncDestory(Queue* queue)
{
    _queueDestory(queue, TRUE);
}

void queueNoSyncInit(Queue *queue, Uint32 capacity)
{
    _queueInit(queue, capacity, FALSE);
}

Bool queueNoSyncIsFull(Queue *queue)
{
    return _queueIsFull(queue,FALSE);
}

Bool queueNoSyncIsEmpty(Queue *queue)
{
    return _queueIsEmpty(queue, FALSE);
}

Uint32 queueNoSyncGetLength(Queue *queue)
{
    return _queueGetLength(queue, FALSE);
}

void *queueNoSyncGetFront(Queue *queue)
{
    return _queueGetFront(queue, FALSE);
}

void queueNoSyncPop(Queue *queue)
{
    _queuePop(queue, FALSE);
}

void queueNoSyncPush(Queue *queue, void *value)
{
    _queuePush(queue, value, FALSE);
}

void queueNoSyncClear(Queue *queue)
{
    _queueClear(queue, FALSE);
}

void queueNoSyncDestory(Queue* queue)
{
    _queueDestory(queue, FALSE);
}