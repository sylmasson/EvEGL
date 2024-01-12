
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvEVE, public EvPanel, public EvSysFont
{
  public:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, SPIClass *Spi = nullptr, uint32_t Baudrate = 30000000);

    bool          Lock(void);
    bool          Unlock(void);
    bool          TryLock(void);
    void          KbdDelete(void);
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
    EvTouchEvent  mTouch[5];

  #if defined(ESP32)
    SemaphoreHandle_t   mMutex;
  #endif

  private:
    void          (*mOnUpdate)(EvDisplay *Disp);
    void          (*mOnTouch)(EvObj *Obj, EvTouchEvent *Touch);

  public:
    uint16_t      SizeDL;
    uint16_t      MaxDL;
    EvKbd         *Kbd;
    const uint8_t &Orientation = mOrientation;

    static uint16_t   sDispCount;
    static uint32_t   sFrameNumber;
    static uint16_t   sTraceFlags;
    static uint32_t   sSecondTimer;
    static uint32_t   sUpdateTimer;
    static EvDisplay  *sDispList[DISP_MAX];
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
