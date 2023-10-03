
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvEVE, public EvPanel, public EvSysFont
{
  public:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, int16_t RTS = -1, SPIClass *Spi = NULL, uint32_t Baudrate = 16000000);

    bool          Lock(void);
    bool          Unlock(void);
    bool          TryLock(void);
    void          KbdDelete(void);
    void          Rotate(uint8_t Orientation);
    void          SetOnUpdate(void (*OnUpdate)(EvDisplay *Disp));
    void          SetOnTouch(void (*OnTouch)(EvObj *Obj, EvTouchEvent *Touch));

    void          Update(void);
    static bool   UpdateAll(void);

  protected:
    void          displayUpdate(void);
    void          touchUpdate(void);
    void          touchUpdatePtr(EvTouchEvent *Touch, EvTouchPos TouchPos, uint32_t msec);

    uint8_t       mOrientation;
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
    static uint16_t   sFrameCount;
    static uint16_t   sTraceFlags;
    static uint32_t   sSecondTimer;
    static uint32_t   sUpdateTimer;
    static EvDisplay  *sDispList[DISP_MAX];
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
