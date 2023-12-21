
#ifndef     _EV_TOUCHCAL_H_
#define     _EV_TOUCHCAL_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTouchCal : public EvObj
{
  public:
    EvTouchCal(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | SYSTEM_OBJ);
    ~EvTouchCal(void);

    void          SetOnExit(void (*mOnExit)(EvTouchCal *Sender, bool Save));
    void          SetText(const char *Text[]);

  protected:
    uint8_t       mInd;
    uint8_t       mPointSize;
    uint8_t       mTargetSize;
    uint8_t       mTouchTag;
    uint16_t      mTimer;
    int16_t       mTouchX[4];
    int16_t       mTouchY[4];
    int16_t       mTargetX[4];
    int16_t       mTargetY[4];
    EvTouchPos    mtouchRawPos;
    const char    **mText;

    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnExit)(EvTouchCal *Sender, bool Save);

  public:
    int32_t       Matrix[6];

    static EvTouchCal *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | SYSTEM_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TOUCHCAL_H_ */
