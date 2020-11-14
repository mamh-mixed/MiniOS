#include <bitmap.h>

void bitMapInit(BitMap *bitMap, Byte *bits, Uint32 byteLength)
{
    ASSERT(bitMap != NULL);
    ASSERT(bits != NULL);
    bitMap->bits = bits;
    bitMap->byteLength = byteLength;
    memset(bits, 0, byteLength);
}

Bool bitMapTest(const BitMap *bitMap, Uint32 bitIndex)
{
    ASSERT(bitMap != NULL);
    Uint32 index = bitIndex / 8;
    return (bitMap->bits[index]) & (0x00000001 << (bitIndex % 8));
}

int bitMapScanAndSet(BitMap *bitMap, Uint32 bitLength)
{
    ASSERT(bitMap != NULL);
    Int32 byteIndex = 0;

    // 寻找到第一个不全为 0 的 Byte
    while ((bitMap->bits[byteIndex] == 0xff) && byteIndex < bitMap->byteLength)
    {
        byteIndex++;
    }

    // 寻找当前字节内第一个不为 0 的 bit
    Int32 bitIndexInByte = 0;
    while ((bitMap->bits[byteIndex] & (0x00000001 << bitIndexInByte)) != 0)
    {
        bitIndexInByte++;
    }

    Int32 bitIndex = byteIndex * 8 + bitIndexInByte;
    Int32 leftBitIndex = bitIndex, rightBitIndex = bitIndex, limit = bitMap->byteLength * 8;
    Int32 count = 1;
    while (leftBitIndex < limit && rightBitIndex < limit)
    {
        if (count < bitLength)
        {
            if (!bitMapTest(bitMap, rightBitIndex))
            {
                ++rightBitIndex;
                ++count;
            }
            else
            {
                ++rightBitIndex;
                leftBitIndex = rightBitIndex;
                count = 0;
            }
        }
        else
        {
            break;
        }
    }

    if (count == bitLength)
    {
        bitMapSetBitRange(bitMap, leftBitIndex, count);
        return leftBitIndex;
    }
    return -1;
}

void bitMapSetBit(BitMap *bitMap, Uint32 bitIndex)
{
    ASSERT(bitMap != NULL);
    Uint32 index = bitIndex / 8;
    bitMap->bits[index] |= 0x00000001 << (bitIndex % 8);
}

void bitMapSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count)
{
    ASSERT(bitMap != NULL);
    for (Uint32 i = 0; i < count; i++)
    {
        bitMapSetBit(bitMap, bitIndex + i);
    }
}

void bitMapUnSetBit(BitMap *bitMap, Uint32 bitIndex)
{
    ASSERT(bitMap != NULL);
    Uint32 index = bitIndex / 8;
    bitMap->bits[index] &= ~(0x00000001 << (bitIndex % 8));
}

void bitMapUnSetBitRange(BitMap *bitMap, Uint32 bitIndex, Uint32 count)
{
    ASSERT(bitMap != NULL);
    for (Uint32 i = 0; i < count; i++)
    {
        bitMapUnSetBit(bitMap, bitIndex + i);
    }
}

void bitMapClear(BitMap *bitMap)
{
    ASSERT(bitMap != NULL);
    memset(bitMap->bits, 0, bitMap->byteLength);
}
