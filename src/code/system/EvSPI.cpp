
#include    <include/system/EvEVE.h>

#define     hostRead(addr)  hostTransaction((addr) & 0x3FFFFF)
#define     hostWrite(addr) hostTransaction(((addr) & 0x3FFFFF) | 0x800000)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvSPI::rstPin;
uint32_t    EvSPI::baudrate;
SPIClass    *EvSPI::hostSPI = nullptr;
EvSPI       *EvSPI::inUsed = nullptr;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::hostSetup(uint8_t CS, int16_t RST, SPIClass *Spi, uint32_t Baudrate)
{
  wrPtr = 0;
  wrFree = RAM_CMD_EMPTY;
  pinMode(csPin = CS, OUTPUT);
  csDisable();

  if (hostSPI == nullptr)
  {
    if ((rstPin = RST) >= 0)
    {
      pinMode(rstPin, OUTPUT);
      digitalWrite(rstPin, LOW);
      delay(20);
      digitalWrite(rstPin, HIGH);
      delay(20);
    }

    hostSPI = (Spi != nullptr) ? Spi : &SPI;
    baudrate = Baudrate;
    hostSPI->begin();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::hostCommand(uint8_t Cmd, uint8_t Parameter)
{
  hostTransaction(((uint32_t)Cmd << 16) | ((uint32_t)Parameter << 8));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::hostRequestSPI(void)
{
  if (inUsed != nullptr)
  {
    inUsed->csDisable();
    hostSPI->endTransaction();
    inUsed = nullptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvSPI::rd8(uint32_t Addr)
{
  uint8_t   data;

  hostRead(Addr);
  data = hostSPI->transfer(0);
  hostAddr++;
  return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvSPI::rd16(uint32_t Addr)
{
  uint16_t  data = 0;

  hostRead(Addr);
  hostSPI->transfer((uint8_t *)&data, 2);
  hostAddr += 2;
  return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvSPI::rd32(uint32_t Addr)
{
  uint32_t  data = 0;

  hostRead(Addr);
  hostSPI->transfer((uint8_t *)&data, 4);
  hostAddr += 4;
  return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::rdData(uint32_t Addr, uint8_t *Data, uint32_t Count)
{
  hostRead(Addr);
  memset(Data, 0, Count);
  hostSPI->transfer(Data, Count);
  hostAddr += Count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wr8(uint32_t Addr, uint8_t Data)
{
  hostWrite(Addr);
  hostSPI->transfer(Data);
  hostAddr++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wr16(uint32_t Addr, uint16_t Data)
{
  hostWrite(Addr);
  hostSPI->transfer((uint8_t *)&Data, 2);
  hostAddr += 2;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wr32(uint32_t Addr, uint32_t Data)
{
  hostWrite(Addr);
  hostSPI->transfer((uint8_t *)&Data, 4);
  hostAddr += 4;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrData(uint32_t Addr, const uint8_t *Data, uint32_t Count)
{
  uint16_t  cnt;
  uint8_t   buffer[128];

  hostWrite(Addr);
  hostAddr += Count;

  while (Count > 0)
  {
    cnt = (Count < sizeof(buffer)) ? Count : sizeof(buffer);
    memcpy(buffer, Data, cnt);
    hostSPI->transfer(buffer, cnt);
    Count -= cnt;
    Data += cnt;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBuf8(uint8_t Data)
{
  if (wrFree < 1)
    wrCmdBufFreeSpace(1);

  wr8(RAM_CMD + wrPtr, Data);
  wrPtr = (wrPtr + 1) & 0xFFF;
  wrFree -= 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBuf16(uint16_t Data)
{
  if (wrFree < 2)
    wrCmdBufFreeSpace(2);

  if (wrPtr & 1)
    Serial.println("wrCmdBuf16: Misalignment");
  else
  {
    wr16(RAM_CMD + wrPtr, Data);
    wrPtr = (wrPtr + 2) & 0xFFF;
    wrFree -= 2;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBuf32(uint32_t Data)
{
  if (wrFree < 4)
    wrCmdBufFreeSpace(4);

  if (wrPtr & 3)
    Serial.println("wrCmdBuf32: Misalignment");
  else
  {
    wr32(RAM_CMD + wrPtr, Data);
    wrPtr = (wrPtr + 4) & 0xFFF;
    wrFree -= 4;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBufData(const uint8_t *Data, uint32_t Count)
{
  while (Count > 0)
  {
    uint16_t  cnt = (Count < 256) ? Count : 256;

    if (wrPtr + cnt > RAM_CMD_SIZE)
      cnt = RAM_CMD_SIZE - wrPtr;

    wrCmdBufFreeSpace(cnt);
    wrData(RAM_CMD + wrPtr, Data, cnt);
    wrPtr = (wrPtr + cnt) & 0xFFF;
    wrFree -= cnt;
    Count -= cnt;
    Data += cnt;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBufAlign(void)
{
  uint32_t  data = 0;

  if (wrPtr & 3)
    wrCmdBufData((uint8_t *)&data, 4 - (wrPtr & 3));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBufClear(void)
{
  wrPtr = 0;
  wrFree = RAM_CMD_EMPTY;
  wr8(REG_CPURESET, 1);
  wr16(REG_CMD_WRITE, 0);
  wr16(REG_CMD_READ, 0);
  wr8(REG_CPURESET, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBufFlush(void)
{
  wrCmdBufFreeSpace(RAM_CMD_EMPTY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSPI::wrCmdBufEmpty(void)
{
  wrCmdBufUpdate();

  return (wrCmdBufFreeSpace() == RAM_CMD_EMPTY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::wrCmdBufUpdate(void)
{
  wr16(REG_CMD_WRITE, wrPtr & RAM_CMD_EMPTY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvSPI::wrCmdBufFreeSpace(void)
{
  return (wrFree = RAM_CMD_EMPTY - ((wrPtr - rd16(REG_CMD_READ)) & 0xFFF));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvSPI::wrCmdBufFreeSpace(uint16_t FreeSpace)
{
  wrCmdBufUpdate();

  if (wrFree < FreeSpace)
    while (wrCmdBufFreeSpace() < FreeSpace)
      yield();

  return wrFree;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::csEnable()
{
  digitalWrite(csPin, LOW);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::csDisable()
{
  digitalWrite(csPin, HIGH);
  hostAddr = 0xFFFFFFFF;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::hostTransaction(uint32_t Addr)
{
  if (inUsed != this)
  {
    if (!inUsed)
      hostSPI->beginTransaction(SPISettings(baudrate, MSBFIRST, SPI_MODE0));
    else
      inUsed->csDisable();

    inUsed = this;
  }

  if (Addr != hostAddr)
  {
    uint8_t data[4], cnt = 3;

    csDisable();
    data[0] = Addr >> 16;
    data[1] = Addr >> 8;
    data[2] = Addr;
    data[3] = 0;
    csEnable();

    switch (data[0] >> 6)
    {
      case 0:  cnt++;   // Add dummy byte for Host Memory Read
      case 2:  hostSPI->transfer(data, cnt); hostAddr = Addr; break;
      default: csDisable(); break;
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
