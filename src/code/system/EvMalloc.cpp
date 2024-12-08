
#include    <EvGUI.h>

#define     BOTTOM_SIZE_MIN (128 * 1024)
#define     BLOCK_SIZE(x)   (((x) + (EV_MALLOC_MIN-1)) & ~(EV_MALLOC_MIN-1))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMalloc::EvMalloc(void) :
  mId(1),
  mIdDelCnt(0)
{
  if ((mFirst = new EvMem) != nullptr)
  {
    mFirst->id = 0;
    mFirst->used = 0;
    mFirst->owner = nullptr;
    mFirst->typeId = 0;
    mFirst->count = 0;
    mFirst->startDL = -1;
    mFirst->size = EV_MALLOC_SIZE;
    mFirst->addr = EV_MALLOC_ADDR;
    mFirst->next = mFirst->prev = nullptr;
  }

  BufferPNG = Malloc(42 * 1024, "PNG decoding process buffer");   // Allocate at 0xF5800
  BufferDMA = Malloc(64 * 1024, "DMA buffer used by MEDIAFIFO");  // Allocate at 0xE5800
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMalloc::~EvMalloc(void)
{
  for (EvMem *ptr = mFirst; ptr != nullptr; )
  {
    EvMem *tmp = ptr->next;
    delete ptr;
    ptr = tmp;
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Deallocate a RAM_G memory block.
 *
 * @param[in]  Ptr     EvMem pointer of the block to deallocate. If null, the function does nothing.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvMalloc::Free(const EvMem *Ptr)
{
  EvMem     *ptr = (EvMem *)Ptr;

  if (ptr != nullptr)
  {
    for (EvMem *p = mFirst; p != ptr; p = p->next)
      if (p == nullptr)
      {
        EvErr->printf("Error: EvMem pointer (%p) being freed was not allocated\n", ptr);
        return;
      }

    if (mIdDelCnt < ID_DEL_MAX)
      mIdDel[mIdDelCnt++] = ptr->id;

    memFree(ptr);
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Allocate an uninitialized RAM_G memory block.
 *
 * @param[in]  Size    Size of the memory block, in bytes.
 * @param[in]  Owner   Owner pointer of the allocated block. Default is nullptr.
 * @param[in]  TypeId  Type Id of the Owner pointer. Default is EV_UNDEFINED;
 * 
 * @return     EvMem struct pointer of the block on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Malloc(size_t Size, const void *Owner, uint8_t TypeId)
{
  EvMem     *ptr = nullptr;

  if (Size > 0)
  {
    uint32_t  used, size;

    if ((size = BLOCK_SIZE(used = Size)) < EV_MALLOC_MIN)
      size = EV_MALLOC_MIN;

    if ((ptr = memAlloc(size)) != nullptr)
    {
      ptr->used = used;
      ptr->owner = Owner;
      ptr->typeId = TypeId;

      if (mId == 0)
        renewAllIds();
      else
        ptr->id = nextId();
    }
  }

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Allocate an uninitialized RAM_G memory block.
 *
 * @param[in]  Size    Size of the memory block, in bytes.
 * @param[in]  Owner   Owner pointer of the allocated EV_FONT block.
 * 
 * @return     EvMem struct pointer of the block on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Malloc(size_t Size, const EvFont *Owner)
{
  return Malloc(Size, Owner, EV_FONT);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Allocate an uninitialized RAM_G memory block.
 *
 * @param[in]  Size    Size of the memory block, in bytes.
 * @param[in]  Owner   Owner pointer of the allocated EV_BMP block.
 * 
 * @return     EvMem struct pointer of the block on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Malloc(size_t Size, const EvBmp *Owner)
{
  return Malloc(Size, Owner, EV_BMP);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Allocate an uninitialized RAM_G memory block.
 *
 * @param[in]  Size    Size of the memory block, in bytes.
 * @param[in]  Owner   Owner pointer of the allocated EV_OBJ block.
 * 
 * @return     EvMem struct pointer of the block on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Malloc(size_t Size, const EvObj *Owner)
{
  return Malloc(Size, Owner, EV_OBJ);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Change the size of the RAM_G memory block.
 * 
 * @note       The new block must be considered as uninitialized
 *
 * @param[in]  Ptr     EvMem pointer of the block to reallocate. If nullptr, the function does nothing.
 * @param[in]  Size    New size of the memory block, in bytes. If size is zero, the block is deallocated.
 * 
 * @return     The same EvMem struct pointer of the block on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Realloc(const EvMem *Ptr, size_t Size)
{
  EvMem     *ptr = (EvMem *)Ptr;

  if (Size == 0)
  {
    Free(ptr);
    return nullptr;
  }

  if (ptr != nullptr && ptr->used > 0)
  {
    uint32_t  used, size;

    if ((size = BLOCK_SIZE(used = Size)) < EV_MALLOC_MIN)
      size = EV_MALLOC_MIN;

    if (size <= ptr->size)
    { // decrease the size of the block
      if (size == ptr->size || (ptr = memSplit(ptr, size, size >= BOTTOM_SIZE_MIN)) != nullptr)
        ptr->used = used;
    }
    else
    { // increase the size of the block
      EvMem     *next = ptr->next;
      uint32_t  missing = size - ptr->size;

      if (next != nullptr && next->used == 0 && next->size >= missing)
      { // next block is free and big enough to increase
        if ((next->size - missing) < EV_MALLOC_MIN)
        { // add all next free block
          ptr->size += next->size;
          ptr->used = used;

          if ((ptr->next = next->next) != nullptr)
            ptr->next->prev = ptr;

          delete next;
        }
        else
        { // add part of the next free block
          next->size -= missing;
          next->addr += missing;
          ptr->size = size;
          ptr->used = used;
        }
      }
      else
      { // must be moved completely
        uint16_t   id = ptr->id;
        const void *owner = ptr->owner;
        uint8_t    typeId = ptr->typeId;
        uint8_t    count = ptr->count;

        memFree(ptr);

        if ((ptr = memAlloc(size)) != nullptr)
        { // set same typeId, owner, count and id
          ptr->typeId = typeId;
          ptr->owner = owner;
          ptr->count = count;
          ptr->used = used;
          ptr->id = id;
        }
      }
    }
  }

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with Owner->Tag pointer.
 *
 * @param[in]  Tag     Valid Owner->Tag pointer of an already allocated RAM_G memory block.
 * @param[in]  TypeId  Type Id of the Owner pointer. Default is EV_UNDEFINED;
 * 
 * @return     EvMem struct pointer associated with the Owner->Tag pointer on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByTag(const char *Tag, uint8_t TypeId)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != nullptr; ptr = ptr->next)
    if (ptr->used != 0 && ptr->owner != nullptr && (TypeId == EV_UNDEFINED || TypeId == ptr->typeId))
    {
      const char  *tag = nullptr;

      switch (ptr->typeId)
      {
        case EV_UNDEFINED: tag = (const char *)(ptr->owner); break;
        case EV_FONT: tag = ((const EvFont *)(ptr->owner))->Tag; break;
        case EV_BMP: tag = ((const EvBmp *)(ptr->owner))->Tag; break;
        case EV_OBJ: tag = ((const EvObj *)(ptr->owner))->Tag; break;
      }

      if (tag && strcmp(tag, Tag) == 0)
        break;
    }

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with Owner pointer.
 *
 * @param[in]  Owner   Valid Owner pointer of an already allocated RAM_G memory block.
 * 
 * @return     EvMem struct pointer associated with the Owner pointer on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByOwner(const void *Owner)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != nullptr; ptr = ptr->next)
    if (ptr->used != 0 && ptr->owner == Owner)
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with the Addr address.
 *
 * @param[in]  Addr    Valid address in RAM_G memory already allocated.
 * 
 * @return     EvMem struct pointer associated with the Addr address on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByAddr(uint32_t Addr)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != nullptr; ptr = ptr->next)
    if (Addr >= ptr->addr && Addr < (ptr->addr + ptr->used))
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with the parameter Id.
 *
 * @param[in]  Id      Valid Id of an already allocated RAM_G memory block.
 * 
 * @return     EvMem struct pointer associated with the parameter Id on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindById(uint16_t Id)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != nullptr; ptr = ptr->next)
    if (ptr->used != 0 && ptr->id == Id)
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the first EvMem struct.
 *
 * @return     First EvMem struct pointer on success, otherwise returns nullptr.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindFirst(void)
{
  return mFirst;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvMalloc::nextId(void)
{
  uint16_t  id = mId;

  if (mIdDelCnt > 0)
    id = mIdDel[--mIdDelCnt];
  else if (++mId == 1000)
    mId = 0;

  return id;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvMalloc::renewAllIds()
{
  EvMem     *ptr;

  for (ptr = mFirst, mIdDelCnt = 0, mId = 1; ptr != nullptr; ptr = ptr->next)
    if (ptr->used != 0)
      ptr->id = mId++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvMalloc::memFree(EvMem *Ptr)
{
  EvMem     *prev, *next, *ptr = Ptr;

  ptr->id = 0;
  ptr->used = 0;
  ptr->owner = nullptr;
  ptr->typeId = 0;
  ptr->count = 0;
  ptr->startDL = -1;

  if ((next = ptr->next) != nullptr && next->used == 0)
  {
    ptr->size += next->size;

    if ((ptr->next = next->next) != nullptr)
      ptr->next->prev = ptr;

    delete next;
  }

  if ((prev = ptr->prev) != nullptr && prev->used == 0)
  {
    prev->size += ptr->size;

    if ((prev->next = ptr->next) != nullptr)
      prev->next->prev = prev;

    delete ptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMem       *EvMalloc::memAlloc(size_t Size)
{
  EvMem     *ptr = nullptr;

  for (EvMem *p = mFirst; p != nullptr; p = p->next)
    if (p->used == 0 && Size <= p->size && (ptr == nullptr || p->size < ptr->size))
      ptr = p;

  if (ptr != nullptr && (ptr = memSplit(ptr, Size, Size >= BOTTOM_SIZE_MIN)) != nullptr)
    ptr->used = ptr->size;

  return ptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMem       *EvMalloc::memSplit(EvMem *Ptr, size_t Size, bool Bottom)
{
  EvMem     *ptr = Ptr;
  EvMem     *next = ptr->next;
  EvMem     *prev = ptr->prev;
  uint32_t  free = ptr->size - Size;

  if (prev != nullptr && prev->used == 0)
  { // previous block is free
    prev->size += free;
    ptr->addr += free;
    ptr->size -= free;
  }
  else if (next != nullptr && next->used == 0)
  { // next block is free
    next->size += free;
    next->addr -= free;
    ptr->size -= free;
  }
  else if (free >= EV_MALLOC_MIN)
  { // next and/or previous block is nullptr or not free
    if ((next = new EvMem) == nullptr)
      return nullptr;

    if (!Bottom)
    { // Allocate memory block at top and free memory block at bottom (Default allocation)
      *next = *ptr;
      next->next = ptr->next;

      ptr->id = 0;
      ptr->used = 0;
      ptr->owner = nullptr;
      ptr->typeId = 0;
      ptr->count = 0;
      ptr->startDL = -1;
      ptr->size = free;

      if ((next->next = ptr->next) != nullptr)
        next->next->prev = next;

      ptr->next = next;
      next->addr += free;
      next->size -= free;
      next->prev = ptr;
      ptr = next;
    }
    else
    { // Allocate memory block at bottom and free memory block at top
      next->id = 0;
      next->used = 0;
      next->owner = nullptr;
      next->typeId = 0;
      next->count = 0;
      next->startDL = -1;
      next->size = free;
      next->addr = ptr->addr + Size;
      next->prev = ptr;

      if ((next->next = ptr->next) != nullptr)
        next->next->prev = next;

      ptr->size -= free;
      ptr->next = next;
    }
  }

  ptr->used = ptr->size;
  return ptr;
}
