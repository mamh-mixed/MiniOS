#ifndef __QUEUE_H__
#define __QUEUE_H__ 0

#include <stddef.h>
#include <type.h>
#include <memory.h>
#include <sync.h>

void _queueInit(Queue *queue, Uint32 capacity, Bool isSync);

Bool _queueIsFull(Queue *queue, Bool isSync);

Bool _queueIsEmpty(Queue *queue, Bool isSync);

Uint32 _queueGetLength(Queue *queue, Bool isSync);

void *_queueGetFront(Queue *queue, Bool isSync);

void _queuePop(Queue *queue, Bool isSync);

void _queuePush(Queue *queue, void *value, Bool isSync);

void _queueClear(Queue *queue, Bool isSync);

void _queueDestory(Queue* queue, Bool isSync);


void queueSyncInit(Queue *queue, Uint32 capacity);

Bool queueSyncIsFull(Queue *queue);

Bool queueSyncIsEmpty(Queue *queue);

Uint32 queueSyncGetLength(Queue *queue);

void *queueSyncGetFront(Queue *queue);

void queueSyncPop(Queue *queue);

void queueSyncPush(Queue *queue, void *value);

void queueSyncClear(Queue *queue);

void queueSyncDestory(Queue* queue);


void queueNoSyncInit(Queue *queue, Uint32 capacity);

Bool queueNoSyncIsFull(Queue *queue);

Bool queueNoSyncIsEmpty(Queue *queue);

Uint32 queueNoSyncGetLength(Queue *queue);

void *queueNoSyncGetFront(Queue *queue);

void queueNoSyncPop(Queue *queue);

void queueNoSyncPush(Queue *queue, void *value);

void queueNoSyncClear(Queue *queue);

void queueNoSyncDestory(Queue* queue);

#endif