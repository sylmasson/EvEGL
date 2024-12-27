
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     DISP_MAX        4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvPanel, public EvEVE, public EvSysFont
{
  friend class  EvObj;
  friend class  EvShell;

  protected:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, uint32_t Baudrate = 30000000, SPIClass *Spi = nullptr);

  public:
    void          Rotate(uint8_t Orientation);
    void          SetOnUpdate(void (*OnUpdate)(EvDisplay *Sender));
    void          SetOnUpdateFPS(void (*OnUpdateFPS)(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL));
    void          SetOnTouch(void (*OnTouch)(EvObj *Sender, const EvTouchEvent *Touch));

    static bool   Update(void);

  protected:
    void          update(void);

    uint8_t       mOrientation;
    uint8_t       mFrameCount;
    uint32_t      mTimeUsed;
    uint16_t      mSizeDL;
    uint16_t      mMaxDL;

  private:
    void          (*mOnUpdate)(EvDisplay *Disp);
    void          (*mOnUpdateFPS)(EvDisplay *Disp, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL);

  public:
    EvKbd         *Kbd;
    EvTouch       Touch;
    const uint8_t &Orientation = mOrientation;

  protected:
    static uint16_t   sDispCount;
    static uint16_t   sTraceFlags;
    static uint32_t   sFrameRate;     // mHz
    static uint32_t   sFrameTime;     // usec
    static uint32_t   sFrameNumber;
    static uint32_t   sSecondTimer;   // usec
    static uint32_t   sUpdateTimer;   // usec
    static EvDisplay  *sDispList[DISP_MAX];

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static bool       SetFrameRate(float FrameRate);
    static uint32_t   FrameRate(void) { return sFrameRate; };
    static uint32_t   FrameTime(void) { return sFrameTime; };
    static uint32_t   FrameNumber(void) { return sFrameNumber; };
    static EvDisplay  *Create(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, uint32_t Baudrate = 30000000, SPIClass *Spi = nullptr);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
