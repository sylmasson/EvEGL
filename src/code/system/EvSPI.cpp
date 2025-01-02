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

#include    <EvEGL.h>

#define     hostRead(addr)  hostTransaction((addr) & 0x3FFFFF)
#define     hostWrite(addr) hostTransaction(((addr) & 0x3FFFFF) | 0x800000)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvSPI::baudrate;
SPIClass    *EvSPI::hostSPI = nullptr;
EvSPI       *EvSPI::inUsed = nullptr;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSPI::hostSetup(uint8_t CS, uint8_t RST, uint32_t Baudrate, SPIClass *Spi)
{
  wrPtr = wrLast = 0;
  wrFree = RAM_CMD_EMPTY;
  pinMode(csPin = CS, OUTPUT);
  csDisable();

  if ((rstPin = RST) != 255)
  {
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, LOW);
    delay(20);
    digitalWrite(rstPin, HIGH);
    delay(20);
  }

  if (hostSPI == nullptr)
  {
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

void        EvSPI::wrDataDMA(uint32_t Addr, const uint8_t *Data, uint32_t Count, EventResponderRef EventDMA)
{
  digitalWrite(5, HIGH);

  hostWrite(Addr);
  TaskDMA.Start();
  hostAddr += Count;
  hostSPI->transfer(Data, nullptr, Count, EventDMA);
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
    EvErr->println("wrCmdBuf16: Misalignment");
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
    EvErr->println("wrCmdBuf32: Misalignment");
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

    if (wrFree < cnt)
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
  wrPtr = wrLast = 0;
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
  uint16_t  ptr = wrPtr & RAM_CMD_EMPTY;

  if (wrLast != ptr)
    wr16(REG_CMD_WRITE, wrLast = ptr);
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
  while (TaskDMA.Busy())
    yield();

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
