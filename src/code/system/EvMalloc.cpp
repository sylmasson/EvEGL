
#include    <EvGUI.h>

#define     align32(x)      (((x) + 31) & ~31)  // 32 bytes alignment

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMalloc::EvMalloc(void)
{
  mId = 1;

  if ((mFirst = new EvMem) != NULL)
  {
    mFirst->id = 0;
    mFirst->size = 0;
    mFirst->owner = NULL;
    mFirst->typeId = 0;
    mFirst->count = 0;
    mFirst->startDL = -1;
    mFirst->length = EV_MALLOC_SIZE;
    mFirst->addr = EV_MALLOC_ADDR;
    mFirst->next = mFirst->prev = NULL;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMalloc::~EvMalloc(void)
{
  for (EvMem *ptr = mFirst; ptr != NULL; )
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
  if (Ptr != NULL)
    memFree((EvMem *)Ptr);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Allocate an uninitialized RAM_G memory block.
 *
 * @param[in]  Size    Size of the memory block, in bytes.
 * @param[in]  Owner   Owner pointer of the allocated block. Default is NULL.
 * @param[in]  TypeId  Type Id of the Owner pointer. Default is EV_UNDEFINED;
 * 
 * @return     EvMem struct pointer of the block on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Malloc(size_t Size, const void *Owner, uint8_t TypeId)
{
  EvMem     *ptr = NULL;

  if (Size > 0 && (ptr = memAlloc(Size)) != NULL)
  {
    ptr->owner = Owner;
    ptr->typeId = TypeId;

    if (mId == 0)
      renewAllIds();
    else
      ptr->id = nextId();
  }

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Change the size of the RAM_G memory block.
 * 
 * @note       The new block must be considered as uninitialized
 *
 * @param[in]  Ptr     EvMem pointer of the block to reallocate. If NULL, the function does nothing.
 * @param[in]  Size    New size of the memory block, in bytes. If size is zero, the block is deallocated.
 * 
 * @return     The same EvMem struct pointer of the block on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::Realloc(const EvMem *Ptr, size_t Size)
{
  EvMem     *ptr = (EvMem *)Ptr;
  uint32_t  length = align32(Size);

  if (Size == 0)
  {
    Free(ptr);
    return NULL;
  }

  if (length < EV_MALLOC_MIN)
    length = EV_MALLOC_MIN;

  if (ptr != NULL && ptr->size > 0)
  {
    EvMem   *next = ptr->next;

    if (length <= ptr->length)
    { // decrease the size of the block
      if ((ptr->length - length) < EV_MALLOC_MIN && next != NULL && next->size > 0)
        ptr->size = Size;
      else
        ptr = memSplit(ptr, length, Size);
    }
    else
    { // increase the size of the block
      uint32_t  lengthMissing = length - ptr->length;

      if (next != NULL && next->size == 0 && next->length >= lengthMissing)
      { // next block is free and big enough to increase
        if ((next->length - lengthMissing) < EV_MALLOC_MIN)
        { // add all next free block
          ptr->length += next->length;
          ptr->size = Size;

          if ((ptr->next = next->next) != NULL)
            ptr->next->prev = ptr;

          delete next;
        }
        else
        { // add part of the next free block
          next->length -= lengthMissing;
          next->addr += lengthMissing;
          ptr->length = length;
          ptr->size = Size;
        }
      }
      else
      { // must be moved completely
        uint16_t   id = ptr->id;
        const void *owner = ptr->owner;
        uint8_t    typeId = ptr->typeId;
        uint8_t    count = ptr->count;

        memFree(ptr);

        if ((ptr = memAlloc(Size)) != NULL)
        { // set same typeId, owner, count and id
          ptr->typeId = typeId;
          ptr->owner = owner;
          ptr->count = count;
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
 * @return     EvMem struct pointer associated with the Owner->Tag pointer on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByTag(const char *Tag, uint8_t TypeId)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != NULL; ptr = ptr->next)
    if (ptr->size != 0 && ptr->owner != NULL && (TypeId == EV_UNDEFINED || TypeId == ptr->typeId))
    {
      const char  *tag = NULL;

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
 * @return     EvMem struct pointer associated with the Owner pointer on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByOwner(const void *Owner)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != NULL; ptr = ptr->next)
    if (ptr->size != 0 && ptr->owner == Owner)
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with the Addr address.
 *
 * @param[in]  Addr    Valid address in RAM_G memory already allocated.
 * 
 * @return     EvMem struct pointer associated with the Addr address on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindByAddr(uint32_t Addr)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != NULL; ptr = ptr->next)
    if (Addr >= ptr->addr && Addr < (ptr->addr + ptr->size))
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the EvMem struct associated with the parameter Id.
 *
 * @param[in]  Id      Valid Id of an already allocated RAM_G memory block.
 * 
 * @return     EvMem struct pointer associated with the parameter Id on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindById(uint16_t Id)
{
  EvMem     *ptr;

  for (ptr = mFirst; ptr != NULL; ptr = ptr->next)
    if (ptr->size != 0 && ptr->id == Id)
      break;

  return ptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Find the first EvMem struct.
 *
 * @return     First EvMem struct pointer on success, otherwise returns NULL.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvMalloc::FindFirst(void)
{
  return mFirst;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvMalloc::nextId(void)
{
  uint16_t  id = mId;

  if (++mId == 1000)
    mId = 0;

  return id;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvMalloc::renewAllIds()
{
  EvMem     *ptr;

  for (ptr = mFirst, mId = 1; ptr != NULL; ptr = ptr->next)
    if (ptr->size != 0)
      ptr->id = mId++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvMalloc::memFree(EvMem *Ptr)
{
  EvMem     *prev, *next, *ptr = Ptr;

  ptr->id = 0;
  ptr->size = 0;
  ptr->owner = NULL;
  ptr->typeId = 0;
  ptr->count = 0;
  ptr->startDL = -1;

  if ((next = ptr->next) != NULL && next->size == 0)
  {
    ptr->length += next->length;

    if ((ptr->next = next->next) != NULL)
      ptr->next->prev = ptr;

    delete next;
  }

  if ((prev = ptr->prev) != NULL && prev->size == 0)
  {
    prev->length += ptr->length;

    if ((prev->next = ptr->next) != NULL)
      prev->next->prev = prev;

    delete ptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMem       *EvMalloc::memAlloc(size_t Size)
{
  EvMem     *ptr = NULL;
  uint32_t  length = align32(Size);

  if (length < EV_MALLOC_MIN)
    length = EV_MALLOC_MIN;

  for (EvMem *p = mFirst; p != NULL; p = p->next)
  {
    if (p->size == 0 && length <= p->length)
      if (ptr == NULL || p->length < ptr->length)
        ptr = p;
  }

  if (ptr != NULL)
  {
    if ((ptr->length - length) < EV_MALLOC_MIN)
      ptr->size = Size;
    else
      ptr = memSplit(ptr, length, Size);
  }

  return ptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvMem       *EvMalloc::memSplit(EvMem *Ptr, size_t Length, size_t Size)
{
  EvMem     *ptr = Ptr;
  EvMem     *next = ptr->next;
  uint32_t  lengthFree = ptr->length - Length;

  if (next != NULL && next->size == 0)
  { // next block is free
    next->length += lengthFree;
    next->addr -= lengthFree;
  }
  else
  { // next block is not free
    if ((next = new EvMem) == NULL)
      return NULL;

    next->id = 0;
    next->size = 0;
    next->owner = NULL;
    next->typeId = 0;
    next->count = 0;
    next->startDL = -1;
    next->length = lengthFree;
    next->addr = ptr->addr + Length;
    next->prev = ptr;

    if ((next->next = ptr->next) != NULL)
      next->next->prev = next;

    ptr->next = next;
  }

  ptr->length = Length;
  ptr->size = Size;
  return ptr;
}
