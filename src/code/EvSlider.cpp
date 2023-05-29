
#include    <EvGUI.h>

#define     KNOB_TAG        100
#define     SLIDER_DELAY    150

#define     COLOR_LOWER     RGB555(  0,   0, 160)
#define     COLOR_UPPER     RGB555(210, 210, 210)
#define     COLOR_KNOB      RGB555(255, 255, 255)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Slider.
 * 
 * A new Slider is created at the specified size and relative position
 * of its owner Dest.
 *
 * @note       If the height is greater than the width, the Slider works vertically.
 *             
 * @param[in]  Left    The left position of the Slider.
 * @param[in]  Top     The top position of the Slider.
 * @param[in]  Width   The width of the Slider.
 * @param[in]  Height  The height of the Slider.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Slider. If NULL, the default tag name is "EvSlider".
 * @param[in]  State   The initial state of the Slider. Default is set to VISIBLE_OBJ.
 *
 * @return     EvSlider address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSlider    *EvSlider::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvSlider *)EvObj::TryCreate(new EvSlider(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvSlider" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSlider::EvSlider(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mTouchKnob = false;
  BdShape(ROUND_CORNERS);
  resize();
  SetDelay(SLIDER_DELAY);
  SetRange(0, 100);
  SetColor(COLOR_LOWER, COLOR_UPPER, COLOR_KNOB);
  SetOnTouch(NULL);
  SetOnChange(NULL);
  mSetPoint = mValue;
  SetValue(0);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the position value of the Slider.
 * 
 * @return     The position value of the Slider.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvSlider::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the position Value of the Slider.
 * 
 * If the Value is change, the callback function OnChange is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 * 
 * @param[in]  Value   The position Value of the Slider. Sets Min and Max value with SetRange.
 * 
 * @return     true if the position value is changed, otherwise returns false.
 * 
 * @see        SetOnChange, SetRange
 * 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSlider::SetValue(int16_t Value)
{
  if (mValue != mSetPoint)
    return false;

  return setValue(mSetPoint = Value);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the delay for the knob animation.
 * 
 * @param[in]  Msec   The delay in millisecond.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::SetDelay(uint16_t Msec)
{
  mSliderDelay = Msec * 1000L;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Slider.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorLower  The color on lower bar side.
 * @param[in]  ColorUpper  The color on upper bar side.
 * @param[in]  ColorKnob   The color of the knob.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::SetColor(uint16_t ColorLower, uint16_t ColorUpper, uint16_t ColorKnob)
{
  if (mColorLower != ColorLower || mColorUpper != ColorUpper || mColorKnob != ColorKnob)
  {
    mColorLower = ColorLower;
    mColorUpper = ColorUpper;
    mColorKnob = ColorKnob;
    Modified();
  }
}

/// @copydoc EvNumInt::SetRange()

void        EvSlider::SetRange(int16_t Min, int16_t Max)
{
  if (Min != Max)
  {
    mMin = Min;
    mMax = Max;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvSlider::SetOnTouch(void (*OnTouch)(EvSlider *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvSlider::SetOnChange(void (*OnChange)(EvSlider *Sender, int16_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::drawEvent(void)
{
  int16_t   x, y, border, radius, knobPos;

  mKnobSize = (mBdShape == ROUND_CORNERS) ? mSliderWidth : ((mSliderWidth * 2) / 3);
  knobPos = ((int32_t)(mValue - mMin) * (mSliderLength - mKnobSize)) / (mMax - mMin);
  x = y = (mSliderWidth - mThickness) >> 1;
  radius = mBdRadius >> 1;
  Disp->Clear(0, 1, 1);

  if ((border = mSliderWidth >> 6) < 24)
    border = 24;

  if (mWidth >= mHeight)
  {
    FillRectangle2f(8, y, (mWidth - 1) << 4, mThickness, mColorUpper, radius);
    FillRectangle2f(8, y, knobPos + (mKnobSize >> 1), mThickness, mColorLower, radius);
    Disp->TagValue(KNOB_TAG);
    FillRectangle2f(knobPos, 0, mKnobSize, mSliderWidth, mColorKnob, mBdRadius, border, mColorUpper);
  }
  else
  {
    y = (mHeight << 4) - knobPos - (mKnobSize >> 1);
    FillRectangle2f(x, 8, mThickness, (mHeight - 1) << 4, mColorUpper, radius);
    FillRectangle2f(x, y, mThickness, ((mHeight - 1) << 4) - y, mColorLower, radius);
    Disp->TagValue(KNOB_TAG);
    FillRectangle2f(0, y - (mKnobSize >> 1), mSliderWidth, mKnobSize, mColorKnob, mBdRadius, border, mColorUpper);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::resizeEvent(void)
{
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::refreshEvent(void)
{
  int16_t   value = mValue;

  if (value != mSetPoint)
  {
    if (mSetPoint > value)
    {
      if ((value += mInc) > mSetPoint)
        value = mSetPoint;
    }
    else
    {
      if ((value -= mInc) < mSetPoint)
        value = mSetPoint;
    }

    setValue(value);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::touchEvent(EvTouchEvent *Touch)
{
  int16_t   value, knobPos, sliderRange;

  sliderRange = mSliderLength - mKnobSize;
  knobPos = ((mWidth >= mHeight ? Touch->x : mHeight - Touch->y) << 4) - (mKnobSize >> 1);
  knobPos = (knobPos < 0) ? 0 : ((knobPos > sliderRange) ? sliderRange : knobPos);
  value = (knobPos * (mMax - mMin)) / (sliderRange >> 4);
  value = mMin + ((value + 8) >> 4);

  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchKnob = (Touch->tag == KNOB_TAG);
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      if (mTouchKnob)
      {
        Touch->event = 0;
        SetValue(value);
      }
      break;

    case TOUCH_END:
      if (mSliderDelay < PERIOD_REFRESH)
        SetValue(value);
      else
      {
        mSetPoint = value;

        if ((mInc = (abs(mSetPoint - mValue) * PERIOD_REFRESH) / mSliderDelay) < 1)
          mInc = 1;
      }

    case TOUCH_CANCEL:
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::resize(void)
{
  if (mWidth >= mHeight)
  {
    mThickness = mHeight << 2;
    mSliderWidth = mHeight << 4;
    mSliderLength = mWidth << 4;
  }
  else
  {
    mThickness = mWidth << 2;
    mSliderWidth = mWidth << 4;
    mSliderLength = mHeight << 4;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSlider::setValue(int16_t Value)
{
  if (mMax > mMin)
  {
    if (Value > mMax)
      mSetPoint = Value = mMax;
    else if (Value < mMin)
      mSetPoint = Value = mMin;
  }
  else
  {
    if (Value < mMax)
      mSetPoint = Value = mMax;
    else if (Value > mMin)
      mSetPoint = Value = mMin;
  }

  if (mValue == Value)
    return false;

  mValue = Value;
  Modified();

  if (mOnChange != NULL)
    (*mOnChange)(this, mValue);

  return true;
}
