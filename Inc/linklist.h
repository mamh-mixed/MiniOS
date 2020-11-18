#ifndef __LINK_LIST_H__

#define __LINK_LIST_H__ 0

#include <stddef.h>
#include <type.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <debug.h>
#include <interrupt.h>

/**
 * 获取结构体内成员在结构体的偏移.
 */
#define offset(struct_type, member) (int)(&((struct_type *)0)->member)

#define elem2entry(struct_type, struct_member_name, elem_ptr) \
    (struct_type *)((int)elem_ptr - offset(struct_type, struct_member_name))


void linkListInit(LinkList *list);
void linkListInsertBefore(LinkListItem *before, LinkListItem *elem);
void linkListPush(LinkList *list, LinkListItem *elem);
void linkListAppend(LinkList *list, LinkListItem *elem);
void linkListRemove(LinkListItem *elem);
LinkListItem *linkListPop(LinkList *list);
int linkListFind(LinkList *list, LinkListItem *elem);
int linkListEmpty(LinkList *list);
Uint32 linkListLength(LinkList *list);
LinkListItem *linkListTraversal(LinkList *list, function func, int arg);

#endif