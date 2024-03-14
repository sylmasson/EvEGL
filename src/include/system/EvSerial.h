
#ifndef     _EV_SERIAL_H_
#define     _EV_SERIAL_H_

class DummySerial : public Stream
{
  public:
    virtual int       available(void);
    virtual int       peek(void);
    virtual int       read(void);
    virtual size_t    write(uint8_t C);
    virtual int       availableForWrite(void);
};

extern Stream         *EvIn;
extern Stream         *EvOut;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SERIAL_H_ */
