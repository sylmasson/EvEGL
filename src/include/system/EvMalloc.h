
#ifndef     _EV_MALLOC_H_
#define     _EV_MALLOC_H_

// EvBmp constant

#define     RAW_DATA        0
#define     JPEG_DATA       1
#define     ZIP_DATA        2
#define     FMT_DATA_MAX    (ZIP_DATA + 1)

// EvMalloc constant

#define     EV_UNDEFINED    0
#define     EV_FONT         1
#define     EV_BMP          2
#define     EV_OBJ          3
#define     EV_VIDEO        4

#define     EV_MALLOC_ADDR  0x000000
#define     EV_MALLOC_SIZE  0x100000

#define     EV_MALLOC_MIN   64        // must be 32, 64, 128, 256 etc..

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct      EvFont;
class       EvObj;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvBmp
{
  uint8_t         Format;     // 0 = RAW, 1 = JPEG, 2 = ZIP
  uint8_t         Layout;     // Layout of Bitmap
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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvMalloc
{
  public:
    EvMalloc(void);
    ~EvMalloc(void);

    void          Free(const EvMem *Ptr);
    const EvMem   *Malloc(size_t Size, const void *Owner = NULL, uint8_t TypeId = EV_UNDEFINED);
    const EvMem   *Malloc(size_t Size, const EvFont *Owner);
    const EvMem   *Malloc(size_t Size, const EvBmp *Owner);
    const EvMem   *Malloc(size_t Size, const EvObj *Owner);
    const EvMem   *Realloc(const EvMem *Ptr, size_t Size);  // just resize, no copy
    const EvMem   *FindByTag(const char *Tag, uint8_t TypeId = EV_UNDEFINED);
    const EvMem   *FindByOwner(const void *Owner);
    const EvMem   *FindByAddr(uint32_t Addr);
    const EvMem   *FindById(uint16_t Id);
    const EvMem   *FindFirst(void);

  private:
    EvMem         *mFirst;
    uint16_t      mId;

    uint16_t      nextId(void);
    void          renewAllIds();
    void          memFree(EvMem *Ptr);
    EvMem         *memAlloc(size_t Size);
    EvMem         *memSplit(EvMem *Ptr, size_t Size, bool Bottom = false);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_MALLOC_H_ */
