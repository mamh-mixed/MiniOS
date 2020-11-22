#include <linklist.h>

void _linkListInit(LinkList *list, Bool isSync)
{
    ASSERT(list != NULL);
    if (isSync)
    {
        semaphoreInit(&list->semaphore, 1);
    }
    list->head = NULL;
    list->tail = NULL;
    list->cur = NULL;
    list->length = 0;
}

Uint32 _linkListGetLength(LinkList *list, Bool isSync)
{
    ASSERT(list != NULL);
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }
    Uint32 length = list->length;
    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
    return length;
    ;
}

void _linkListPush(LinkList *list, void *data, Bool isSync)
{
    ASSERT(list != NULL);
    LinkListItem *p = (LinkListItem *)sysMalloc(sizeof(LinkListItem));
    ASSERT(p != NULL);
    Uint32 length = _linkListGetLength(list, isSync);
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }

    p->data = data;
    p->next = NULL;
    if (length != 0)
    {
        p->next = list->tail->next;
        p->prev = list->tail;
        list->tail->next = p;
        list->tail = p;
    }
    else
    {
        list->head = p;
        list->tail = p;
    }
    (list->length)++;

    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
}

void _linkListPop(LinkList *list, Bool isSync)
{
    ASSERT(list != NULL);
    Uint32 length = _linkListGetLength(list, isSync);
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }
    if (length != 0)
    {
        if (length == 1)
        {
            sysFree(list->head);
            list->head = NULL;
            list->tail = NULL;
        }
        else
        {
            LinkListItem *temp = list->tail;
            list->tail = list->tail->prev;
            list->tail->next = NULL;
            sysFree(temp);
        }
        (list->length)--;
    }
    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
}

void _linkListDelete(LinkList *list, LinkListItem *item, Bool isSync)
{
    ASSERT(list != NULL);
    ASSERT(item != NULL);
    Uint32 length = _linkListGetLength(list, isSync);
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }

    LinkListItem *prev = item->prev, *next = item->next;
    if (prev != NULL && next == NULL)
    {
        list->tail = prev;
        list->tail->next = NULL;
    }
    else if (prev == NULL && next != NULL)
    {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    else if (prev == NULL && next == NULL)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        prev->next = item->next;
        next->prev = prev;
    }

    (list->length)--;
    sysFree(item);

    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
}

LinkListItem *_linkListFind(LinkList *list, LinkListScanCallBack callback, void *arg, Bool isSync)
{
    LinkListItem *p = list->head;
    Uint32 length = _linkListGetLength(list, isSync);
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }

    while (p != NULL)
    {
        if (callback(p, arg) == TRUE)
        {
            break;
        }
    }

    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
    return p;
}

LinkListItem *_linkListGetNext(LinkList *list, Bool isSync)
{
    if (isSync)
    {
        semaphoreGet(&list->semaphore);
    }

    LinkListItem *ret = NULL;

    if (list->cur == NULL)
    {
        list->cur = list->head;
    }
    else
    {
        list->cur = list->cur->next;
    }
    ret = list->cur;

    if (isSync)
    {
        semaphoreRelease(&list->semaphore);
    }
    return ret;
}

void _linkListDestroy(LinkList *list, Bool isSync)
{
    LinkListItem *p = list->head;
    while (_linkListGetLength(list, isSync) != 0)
    {
        _linkListPop(list, isSync);
    }
}

void linkListSyncInit(LinkList *list)
{
    _linkListInit(list, TRUE);
}

Uint32 linkListSyncGetLength(LinkList *list)
{
    return _linkListGetLength(list, TRUE);
}

void linkListSyncPush(LinkList *list, void *data)
{
    _linkListPush(list, data, TRUE);
}

void linkListSyncPop(LinkList *list)
{
    _linkListPop(list, TRUE);
}

void linkListSyncDelete(LinkList *list, LinkListItem *item)
{
    _linkListDelete(list, item, TRUE);
}

LinkListItem *linkListSyncFind(LinkList *list, LinkListScanCallBack callback, void *arg)
{
    _linkListFind(list, callback, arg, TRUE);
}

LinkListItem *linkListSyncGetNext(LinkList *list)
{
    _linkListGetNext(list, TRUE);
}

void linkListSyncDestroy(LinkList *list)
{
    LinkListItem *p = list->head;
    while (_linkListGetLength(list, TRUE) != 0)
    {
        _linkListPop(list, TRUE);
    }
}

void linkListNoSyncInit(LinkList *list)
{
    _linkListInit(list, FALSE);
}

Uint32 linkListNoSyncGetLength(LinkList *list)
{
    return _linkListGetLength(list, FALSE);
}

void linkListNoSyncPush(LinkList *list, void *data)
{
    _linkListPush(list, data, FALSE);
}

void linkListNoSyncPop(LinkList *list)
{
    _linkListPop(list, FALSE);
}

void linkListNoSyncDelete(LinkList *list, LinkListItem *item)
{
    _linkListDelete(list, item, FALSE);
}

LinkListItem *linkListNoSyncFind(LinkList *list, LinkListScanCallBack callback, void *arg)
{
    _linkListFind(list, callback, arg, FALSE);
}

LinkListItem *linkListNoSyncGetNext(LinkList *list)
{
    _linkListGetNext(list, FALSE);
}

void linkListNoSyncDestroy(LinkList *list)
{
    LinkListItem *p = list->head;
    while (_linkListGetLength(list, FALSE) != 0)
    {
        _linkListPop(list, FALSE);
    }
}