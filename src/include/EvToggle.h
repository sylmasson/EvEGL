
#ifndef     _EV_TOGGLE_H_
#define     _EV_TOGGLE_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvToggle class is used to create a standard Toggle switch.
 *
 * By default, the toggle switch is set to Off with ROUND_CORNERS shape.
 * 
 * @par Example
 * ~~~
 * EvToggle *myToggle = EvToggle::Create(100, 100, 50, 30, myPanel, "MyToggle");
 * myToggle->SetOnChange(MyToggleChangeEvent);
 * myToggle->BdShape(RATIO_CORNERS);
 * myToggle->SetValue(true);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvToggle : public EvObj
{
  protected:
    EvToggle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          SetColor(uint16_t ColorOn, uint16_t ColorOff, uint16_t ColorKnob);
    void          SetOnTouch(void (*OnTouch)(EvToggle *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvToggle *Sender, int32_t Value));

  protected:
    int16_t       mValue;
    uint16_t      mColorOn;
    uint16_t      mColorOff;
    uint16_t      mColorKnob;
    bool          mSkipUp;
    int16_t       mPosKnob;
    int8_t        mDirKnob;
    bool          mTouchKnob;
    uint16_t      mTimerKnob;

    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvToggle *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvToggle *Sender, int32_t Value);

  public:
    static EvToggle *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern uint32_t   millis_nz(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TOGGLE_H_ */
