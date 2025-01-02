/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
