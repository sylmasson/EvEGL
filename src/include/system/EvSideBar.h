
#ifndef     _EV_SIDEBAR_H_
#define     _EV_SIDEBAR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSideBar : public EvPanel
{
  public:
    void          Open(void);
    void          Close(void);
    bool          IsOpen(void);
    void          SetPosition(void);
    void          SetPadding(int16_t PadX, int16_t PadY);
    void          Setup(uint8_t Type, bool OpenState = false, uint16_t Period = 220);
    void          Setup(int16_t PadX, int16_t PadY, uint8_t Type, bool OpenState = false, uint16_t Period = 220);

  protected:
    EvSideBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_DIS_OBJ);

    bool          mOpen;
    uint8_t       mType;
    int16_t       mPadX;
    int16_t       mPadY;
    int16_t       mShift;
    EvKinMove     mKinMotion;

    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);

  public:
    static EvSideBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SIDEBAR_H_ */
