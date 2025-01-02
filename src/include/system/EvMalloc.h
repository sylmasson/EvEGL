/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef     _EV_MALLOC_H_
#define     _EV_MALLOC_H_

// EvBmp constant

#define     RAW_DATA        0
#define     JPEG_DATA       1
#define     ZIP_DATA        2
#define     PNG_DATA        3
#define     BMP_MALLOC      (1 << 3)

// EvMalloc constant

#define     EV_UNDEFINED    0
#define     EV_FONT         1
#define     EV_BMP          2
#define     EV_OBJ          3

#define     EV_MALLOC_ADDR  0x000000
#define     EV_MALLOC_SIZE  0x100000

#define     EV_MALLOC_MIN   64        // must be 32, 64, 128, 256 etc..

#define     ID_DEL_MAX      128

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct      EvFont;
class       EvObj;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvBmp
{
  uint8_t         Format;     // 0 = RAW, 1 = JPEG, 2 = ZIP, 3 = PNG
  uint8_t         Layout;     // Layout of Bitmap (31-44 must be translate to 37808-37821 EXT_FORMAT)
  uint16_t        Width;      // Width of Bitmap
  uint16_t        Height;     // Height of Bitmap
  uint16_t        PalSize;    // Palette size (Included at start of compressed data)
  uint32_t        BmpSize;    // Size of Bitmap
  uint32_t        DataSize;   // Size of all compressed data
  const uint8_t   *Data;      // Compressed data pointer
  const char      *Tag;       // C string Tag pointer
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvMem is a linked list struct used by EvMalloc class to manage
 *            the dynamic allocation of the RAM_G memory.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvMem
{
  EvMem           *next;        ///< Next block allocated or free.
  EvMem           *prev;        ///< Previous block allocated or free.
  uint32_t        used;         ///< Used bytes of allocated block in RAM_G. If zero, the block is free.
  uint32_t        size;         ///< Size of the allocated block in RAM_G.
  uint32_t        addr;         ///< Address of the allocated block in RAM_G.
  const void      *owner;       ///< Owner pointer of the allocated block.
  uint8_t         typeId;       ///< Type Id of the owner pointer.
  uint8_t         count;        ///< The EvFont or EvBmp count in use.
  int16_t         startDL;      ///< Start location in DL. Otherwise equal to -1.
  uint16_t        id;           ///< Id number of the allocated block.
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvMalloc class provide all functions required to manage the
 *            dynamic allocation of the RAM_G memory.
 *
 * The Free, Malloc and Realloc functions were developed in a similar way to the
 * standard C functions.
 * 
 * @note      However, that Realloc only resizes or relocates a memory block.
 * The new block must be considered as uninitialized unlike the standard C function.
 * 
 * @note      A memory block of 42K is automatically allocated by default at 0xF5800.
 * This buffer is necessary for the PNG decoding process. The BufferPNG public variable
 * can be used to free this buffer if the PNG decoding process is never used.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvMalloc
{
  public:
    EvMalloc(void);
    virtual       ~EvMalloc(void);

    void          Free(const EvMem *Ptr);
    const EvMem   *Malloc(size_t Size, const void *Owner = nullptr, uint8_t TypeId = EV_UNDEFINED);
    const EvMem   *Malloc(size_t Size, const EvFont *Owner);
    const EvMem   *Malloc(size_t Size, const EvBmp *Owner);
    const EvMem   *Malloc(size_t Size, const EvObj *Owner);
    const EvMem   *Realloc(const EvMem *Ptr, size_t Size);  // just resize, no copy
    const EvMem   *FindByTag(const char *Tag, uint8_t TypeId = EV_UNDEFINED);
    const EvMem   *FindByOwner(const void *Owner);
    const EvMem   *FindByAddr(uint32_t Addr);
    const EvMem   *FindById(uint16_t Id);
    const EvMem   *FindFirst(void);

    const EvMem   *BufferPNG; // Can be freed if no PNG decoding process is used
    const EvMem   *BufferDMA;

  private:
    EvMem         *mFirst;
    uint16_t      mId;
    uint16_t      mIdDelCnt;
    uint16_t      mIdDel[ID_DEL_MAX];

    uint16_t      nextId(void);
    void          renewAllIds();
    void          memFree(EvMem *Ptr);
    EvMem         *memAlloc(size_t Size);
    EvMem         *memSplit(EvMem *Ptr, size_t Size, bool Bottom = false);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_MALLOC_H_ */
