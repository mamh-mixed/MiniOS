#include <linklist.h>

/**
 * 链表初始化.
 */ 
void linkListInit(LinkList* list)
{
    ASSERT(list != NULL);
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.next = NULL;
    list->tail.prev = &list->head;
}

/**
 * 在before前插入节点elem.
 */ 
void linkListInsertBefore(LinkListItem* before, LinkListItem* elem) {
    ASSERT(before != NULL);
    ASSERT(elem != NULL);

    InterruptStatus oldStatus = interruptDisable();

    before->prev->next = elem;
    elem->prev = before->prev;
    elem->next = before;

    before->prev = elem;

    interruptSetStatus(oldStatus);
}

/**
 * 将给定的元素添加至链表队首.
 */ 
void linkListPush(LinkList *list, LinkListItem* elem) {
    ASSERT(list != NULL);
    ASSERT(elem != NULL);
    linkListInsertBefore(list->head.next, elem);
}

/**
 * 将给定的元素添加至链表队尾.
 */ 
void linkListAppend(LinkList *list, LinkListItem* elem) {
    ASSERT(list != NULL);
    ASSERT(elem != NULL);
    linkListInsertBefore(&list->tail, elem);
}

/**
 * 将指定的元素从其链表中脱离.
 */ 
void linkListRemove(LinkListItem* elem) {
    ASSERT(elem != NULL);

    InterruptStatus oldStatus = interruptDisable();

    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    interruptSetStatus(oldStatus);
}

/**
 * 弹出第一个元素.
 */ 
LinkListItem* linkListPop(LinkList* list) {
    ASSERT(list != NULL);

    LinkListItem* result = list->head.next;

    linkListRemove(result);

    return result;
}

/**
 * 链表查找.
 */ 
int linkListFind(LinkList* list, LinkListItem* elem) {
    ASSERT(list != NULL);
    ASSERT(elem != NULL);

    LinkListItem* e = list->head.next;

    while (e != &list->tail) {
        if (e == elem) {
            return 1;
        }
        e = e->next;
    }
    
    return 0;
}

int linkListEmpty(LinkList* list) {
    ASSERT(list != NULL);
    return (list->head.next == &list->tail ? 1 : 0);
}

Uint32 linkListLength(LinkList* list) {
    ASSERT(list != NULL);

    LinkListItem* e = list->head.next;
    Uint32 length = 0;

    while (e != &list->tail) {
        ++length;
        e = e->next;
    }
    
    return length;
}

/**
 * 遍历链表，将链表元素和参数arg传给函数func，如果返回1，那么返回当前元素.
 */ 
LinkListItem* linkListTraversal(LinkList* list, function func, int arg) {
    ASSERT(list != NULL);
    ASSERT(func != NULL);

    if (linkListEmpty(list)) {
        return NULL;
    }

    LinkListItem* e = list->head.next;
     while (e != &list->tail) {
        if (func(e, arg)) {
            return e;
        }
        e = e->next;
    }

    return NULL;
}