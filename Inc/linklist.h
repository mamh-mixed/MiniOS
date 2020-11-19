#ifndef __LINK_LIST_H__

#define __LINK_LIST_H__ 0

#include <stddef.h>
#include <type.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <debug.h>
#include <interrupt.h>

void _linkListInit(LinkList *list, Bool isSync);

Uint32 _linkListGetLength(LinkList *list, Bool isSync);

void _linkListPush(LinkList *list, void* data, Bool isSync);

void _linkListPop(LinkList *list, Bool isSync);

void _linkListDelete(LinkList *list, LinkListItem *item, Bool isSync);

LinkListItem *_linkListFind(LinkList *list, LinkListScanCallBack callback, void *arg, Bool isSync);

LinkListItem *_linkListGetNext(LinkList *list, Bool isSync);

void _linkListDestroy(LinkList *list, Bool isSync);



void linkListSyncInit(LinkList *list);

Uint32 linkListSyncGetLength(LinkList *list);

void linkListSyncPush(LinkList *list, void* data);

void linkListSyncPop(LinkList *list);

void linkListSyncDelete(LinkList *list, LinkListItem *item);

LinkListItem *linkListSyncFind(LinkList *list, LinkListScanCallBack callback, void *arg);

LinkListItem *linkListSyncGetNext(LinkList *list);

void linkListSyncDestroy(LinkList *list);


void linkListNoSyncInit(LinkList *list);

Uint32 linkListNoSyncGetLength(LinkList *list);

void linkListNoSyncPush(LinkList *list, void* data);

void linkListNoSyncPop(LinkList *list);

void linkListNoSyncDelete(LinkList *list, LinkListItem *item);

LinkListItem *linkListNoSyncFind(LinkList *list, LinkListScanCallBack callback, void *arg);

LinkListItem *linkListNoSyncGetNext(LinkList *list);

void linkListNoSyncDestroy(LinkList *list);

#endif