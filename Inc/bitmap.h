#ifndef BITMAP_H

#define BITMAP_H 0

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

typedef struct
{
    Byte *bits;
    Uint32 byteLength;
} BitMap;


void bitMapInit(BitMap *bitMap, Byte *bits, Uint32 byteLength);

Bool bitMapTest(const BitMap *bitMap, Uint32 bitIndex);

int bitMapScanAndSet(BitMap *bitMap, Uint32 bitLength);

void bitMapSetBit(BitMap *bitMap, Uint32 bitIndex);

void bitMapSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count);

void bitMapUnSetBit(BitMap *bitMap, Uint32 bitIndex);

void bitMapUnSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count);

void bitMapClear(BitMap *bitMap);

#endif