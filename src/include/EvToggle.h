
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
    void          SetColor(uint16_t ColorOn, uint16_t ColorOff = CL_TOGGLE_OFF, uint16_t ColorKnob = CL_TOGGLE_KNOB);
    void          SetOnTouch(void (*OnTouch)(EvToggle *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvToggle *Sender, int32_t Value));

  protected:
    int16_t       mValue;
    color16       mColorOn;
    color16       mColorOff;
    color16       mColorKnob;
    bool          mSkipUp;
    int16_t       mPosKnob;
    int8_t        mDirKnob;
    bool          mTouchKnob;
    uint16_t      mTimerKnob;

    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvToggle *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvToggle *Sender, int32_t Value);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvToggle *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern uint32_t   millis_nz(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TOGGLE_H_ */
