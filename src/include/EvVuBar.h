
#ifndef     _EV_VUBAR_H_
#define     _EV_VUBAR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvVuBar : public EvObj
{
  protected:
    EvVuBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    bool          SetValue(int16_t Value);
    void          SetPeakDetect(bool Enabled);
    void          SetOnTouch(void (*OnTouch)(EvVuBar *Sender, const EvTouchEvent *Touch));
    void          SetColor(uint16_t OffColor, uint16_t Low = RGB555(0, 192, 0), uint16_t Med = RGB555(192, 192, 0), uint16_t High = RGB555(255, 0, 0));
    bool          SetFormat(uint8_t Low, uint8_t Med, uint8_t High, uint8_t SpaceWidth = 4);

  protected:
    int16_t       mValue;
    int16_t       mDotValue;
    int16_t       mPeakValue;
    bool          mVertical;
    uint8_t       mDotCnt[3];
    uint16_t      mDotCount;
    uint16_t      mSpWidth;    // '1/256'
    uint16_t      mDotWidth;   // '1/256'
    uint16_t      mPeakTimer;
    color16       mOffColor;
    color16       mDotColor[3];

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvVuBar *Sender, const EvTouchEvent *Touch);

    void          resize();

  public:
    const int16_t &Value = mValue;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvVuBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_VUBAR_H_ */
