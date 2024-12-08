
#ifndef     _EV_SLIDER_H_
#define     _EV_SLIDER_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvSlider class is used to create a standard Slider.
 * 
 * @par Example
 * ~~~
 * EvSlider *mySlider = EvSlider::Create(50, 50, 300, 50, myPanel, "MySlider");
 * mySlider->SetOnChange(MySliderChangeEvent);
 * mySlider->SetRange(-100, 100);
 * mySlider->SetValue(-30);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSlider : public EvObj
{
  protected:
    EvSlider(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int32_t       Value(void);
    bool          SetValue(int32_t Value);
    void          SetDelay(uint16_t Msec);
    void          SetColor(uint16_t ColorLower, uint16_t ColorUpper = CL_SLIDER_UPPER, uint16_t ColorKnob = CL_SLIDER_KNOB);
    void          SetRange(int32_t Min, int32_t Max);
    void          SetOnTouch(void (*OnTouch)(EvSlider *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvSlider *Sender, int32_t Value));

  protected:
    int32_t       mValue;
    int32_t       mMin;
    int32_t       mMax;
    int32_t       mInc;
    int32_t       mSetPoint;
    int16_t       mThickness;
    int16_t       mKnobSize;
    int16_t       mSliderWidth;
    int16_t       mSliderLength;
    uint32_t      mSliderDelay;     // usec
    color16       mColorLower;
    color16       mColorUpper;
    color16       mColorKnob;
    bool          mTouchKnob;
    bool          mBusy;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSlider *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvSlider *Sender, int32_t Value);

    bool          setValue(int32_t Value);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvSlider *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
    static EvSlider *Create(int16_t Left, int16_t Top, int32_t Value, const EvSlider *Src, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SLIDER_H_ */
