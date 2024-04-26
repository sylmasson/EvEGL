
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

  private:
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
