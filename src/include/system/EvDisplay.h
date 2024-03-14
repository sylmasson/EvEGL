
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvEVE, public EvPanel, public EvSysFont
{
    friend class  EvObj;
    friend class  EvShell;

  public:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, SPIClass *Spi = nullptr, uint32_t Baudrate = 30000000);

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
    void          dispUpdate(void);
    void          touchUpdate(void);
    void          touchUpdate(EvTouchEvent *Touch, EvTouchPos TouchPos, uint32_t msec);

    uint8_t       mOrientation;
    uint8_t       mFrameCount;
    uint32_t      mTimeUsed;
    uint16_t      mSizeDL;
    uint16_t      mMaxDL;
    EvKbd         *mKbd;

    EvTouchEvent  mTouch[5];

  #if defined(ESP32)
    SemaphoreHandle_t   mMutex;
  #endif

  private:
    void          (*mOnUpdate)(EvDisplay *Disp);
    void          (*mOnTouch)(EvObj *Obj, EvTouchEvent *Touch);

  public:
    const uint8_t &Orientation = mOrientation;

  protected:
    static uint16_t   sDispCount;
    static uint16_t   sTraceFlags;
    static uint32_t   sFrameNumber;
    static uint32_t   sSecondTimer;
    static uint32_t   sUpdateTimer;
    static EvDisplay  *sDispList[DISP_MAX];
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
