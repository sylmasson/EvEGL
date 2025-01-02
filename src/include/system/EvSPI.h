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

#ifndef     _EV_SPI_H_
#define     _EV_SPI_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSPI
{
  public:
    void          hostSetup(uint8_t CS, uint8_t RST = 255, uint32_t Baudrate = 16000000, SPIClass *Spi = nullptr);
    void          hostCommand(uint8_t Cmd, uint8_t Parameter = 0);
    void          hostRequestSPI(void);

    uint8_t       rd8(uint32_t Add);
    uint16_t      rd16(uint32_t Addr);
    uint32_t      rd32(uint32_t Addr);
    void          rdData(uint32_t Addr, uint8_t *Data, uint32_t Count);

    void          wr8(uint32_t Addr, uint8_t Data);
    void          wr16(uint32_t Addr, uint16_t Data);
    void          wr32(uint32_t Addr, uint32_t Data);
    void          wrData(uint32_t Addr, const uint8_t *Data, uint32_t Count);
    void          wrDataDMA(uint32_t Addr, const uint8_t *Data, uint32_t Count, EventResponderRef EventDMA);

    void          wrCmdBuf8(uint8_t Data);
    void          wrCmdBuf16(uint16_t Data);
    void          wrCmdBuf32(uint32_t Data);
    void          wrCmdBufData(const uint8_t *Data, uint32_t Count);

    void          wrCmdBufAlign(void);
    void          wrCmdBufClear(void);
    void          wrCmdBufFlush(void);
    bool          wrCmdBufEmpty(void);
    void          wrCmdBufUpdate(void);
    uint16_t      wrCmdBufFreeSpace(void);
    uint16_t      wrCmdBufFreeSpace(uint16_t FreeSpace);

  protected:
    uint8_t       rstPin;
    uint8_t       csPin;
    uint16_t      wrPtr;
    uint16_t      wrFree;
    uint16_t      wrLast;
    uint32_t      hostAddr;

    static uint32_t baudrate;
    static SPIClass *hostSPI;
    static EvSPI    *inUsed;

    void          csEnable(void);
    void          csDisable(void);
    void          hostTransaction(uint32_t Addr);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SPI_H_ */
