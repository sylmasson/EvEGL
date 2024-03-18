
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     DISP_MAX        4
#define     FREQ_REFRESH    60
#define     PERIOD_REFRESH  (1000000L / FREQ_REFRESH)   // usec

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvEVE, public EvPanel, public EvSysFont
{
  friend class  EvObj;
  friend class  EvShell;

  protected:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, SPIClass *Spi = nullptr, uint32_t Baudrate = 30000000);

  public:
    virtual       ~EvDisplay();

    bool          Lock(void);
    bool          Unlock(void);
    bool          TryLock(void);
    void          KbdDelete(void);
    uint32_t      FrameNumber(void);
    void          Rotate(uint8_t Orientation);
    void          SetOnUpdate(void (*OnUpdate)(EvDisplay *Disp));
    void          SetOnTouch(void (*OnTouch)(EvObj *Obj, EvTouchEvent *Touch));

    static bool   Update(void);

  protected:
    void          update(void);

    uint8_t       mOrientation;
    uint8_t       mFrameCount;
    uint32_t      mTimeUsed;
    uint16_t      mSizeDL;
    uint16_t      mMaxDL;
    EvKbd         *mKbd;

  #if defined(ESP32)
    SemaphoreHandle_t   mMutex;
  #endif

  private:
    void          (*mOnUpdate)(EvDisplay *Disp);

  public:
    EvTouch       Touch;
    const uint8_t &Orientation = mOrientation;

  protected:
    static uint16_t   sDispCount;
    static uint16_t   sTraceFlags;
    static uint32_t   sFrameNumber;
    static uint32_t   sSecondTimer;
    static uint32_t   sUpdateTimer;
    static EvDisplay  *sDispList[DISP_MAX];

  public:
    static EvDisplay  *Create(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, SPIClass *Spi = nullptr, uint32_t Baudrate = 30000000);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
