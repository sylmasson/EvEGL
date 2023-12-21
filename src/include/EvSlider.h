
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
  public:
    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          SetDelay(uint16_t Msec);
    void          SetColor(uint16_t ColorLower, uint16_t ColorUpper, uint16_t ColorKnob);
    void          SetRange(int16_t Min, int16_t Max);
    void          SetOnTouch(void (*OnTouch)(EvSlider *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvSlider *Sender, int32_t Value));

  protected:
    EvSlider(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    int16_t       mMin;
    int16_t       mMax;
    int16_t       mInc;
    int16_t       mSetPoint;
    int16_t       mThickness;
    int16_t       mKnobSize;
    int16_t       mSliderWidth;
    int16_t       mSliderLength;
    uint32_t      mSliderDelay;     // usec
    uint16_t      mColorLower;
    uint16_t      mColorUpper;
    uint16_t      mColorKnob;
    bool          mTouchKnob;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSlider *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvSlider *Sender, int32_t Value);

    bool          setValue(int16_t Value);

  public:
    static EvSlider *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SLIDER_H_ */
