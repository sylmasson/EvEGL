
#ifndef     _EV_VUBAR_H_
#define     _EV_VUBAR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvVuBar : public EvObj
{
  public:
    bool          SetValue(int16_t Value);
    void          SetPeakDetect(bool Enabled);
    void          SetOnTouch(void (*OnTouch)(EvVuBar *Sender, EvTouchEvent *Touch));
    void          SetColor(uint32_t Space, uint32_t Off, uint32_t Low = 0xFF00C000, uint32_t Med = 0xFFC0C000, uint32_t High = 0xFFFF0000);
    bool          SetFormat(uint8_t Low, uint8_t Med, uint8_t High, uint8_t SpaceWidth = 4);

  protected:
    EvVuBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    int16_t       mDotValue;
    int16_t       mPeakValue;
    bool          mVertical;
    uint8_t       mDotCnt[3];
    uint16_t      mDotCount;
    uint16_t      mSpWidth;    // '1/256'
    uint16_t      mDotWidth;   // '1/256'
    uint32_t      mPeakTimer;
    uint32_t      mSpColor;
    uint32_t      mOffColor;
    uint32_t      mDotColor[3];

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvVuBar *Sender, EvTouchEvent *Touch);

    void          resize();

  public:
    const int16_t &Value = mValue;

    static EvVuBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_VUBAR_H_ */
