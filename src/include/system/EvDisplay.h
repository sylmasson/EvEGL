
#ifndef     _EV_DISPLAY_H_
#define     _EV_DISPLAY_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     DISP_MAX        4
#define     FREQ_REFRESH    60
#define     PERIOD_REFRESH  (1000000L / FREQ_REFRESH)   // usec

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDisplay : public EvPanel, public EvEVE, public EvSysFont
{
  friend class  EvObj;
  friend class  EvShell;

  protected:
    EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, uint32_t Baudrate = 30000000, SPIClass *Spi = nullptr);

  public:
    bool          Lock(void);
    bool          Unlock(void);
    bool          TryLock(void);
    uint32_t      FrameNumber(void);
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
    static uint32_t   sFrameNumber;
    static uint32_t   sSecondTimer;
    static uint32_t   sUpdateTimer;
    static EvDisplay  *sDispList[DISP_MAX];

  public:
    static EvDisplay  *Create(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST = 255, uint32_t Baudrate = 30000000, SPIClass *Spi = nullptr);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_DISPLAY_H_ */
