#ifndef __BITMAP_H__

#define __BITMAP_H__ 0

#include <stddef.h>
#include <type.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>


void bitMapInit(BitMap *bitMap, Byte *bits, Uint32 byteLength);

// 判断当前位的真假
Bool bitMapTest(const BitMap *bitMap, Uint32 bitIndex);

// 找出连续的若干个为 0 的位并返回首个 0 的序号，如果没找到返回 -1
int bitMapScanAndSet(BitMap *bitMap, Uint32 bitLength);

void bitMapSetBit(BitMap *bitMap, Uint32 bitIndex);

// 连续置位若干个 bit
void bitMapSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count);

void bitMapUnSetBit(BitMap *bitMap, Uint32 bitIndex);

// 连续清空若干个 bit
void bitMapUnSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count);

void bitMapClear(BitMap *bitMap);

#endif