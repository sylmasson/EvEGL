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

#include    <EvEGL.h>

#define     KNOB_TAG        100
#define     SLIDER_DELAY    150

const char * const EvSlider::TypeName = "EvSlider";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvSlider**.
 * 
 * A new **EvSlider** is created at the specified size and relative position
 * of its owner Dest.
 *
 * @note       If the height is greater than the width, the **EvSlider** works vertically.
 *             
 * @param[in]  Left    The left position of the **EvSlider**.
 * @param[in]  Top     The top position of the **EvSlider**.
 * @param[in]  Width   The width of the **EvSlider**.
 * @param[in]  Height  The height of the **EvSlider**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvSlider**. If nullptr, the default tag name is **"EvSlider"**.
 * @param[in]  State   The initial state of the **EvSlider**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvSlider** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSlider    *EvSlider::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvSlider  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvSlider *)EvObj::TryCreate(new EvSlider(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->BdShape(ROUND_CORNERS);
    obj->resizeEvent();
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvSlider**.
 * 
 * A new **EvSlider** is created at the specified size and relative position
 * of its owner Dest.
 *
 * @note       If the height is greater than the width, the **EvSlider** works vertically.
 *             
 * @param[in]  Left    The left position of the **EvSlider**.
 * @param[in]  Top     The top position of the **EvSlider**.
 * @param[in]  Value   The value of the **EvSlider**.
 * @param[in]  *Src    The address pointer of the **EvSlider** source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvSlider**. If nullptr, the default tag name is **"EvSlider"**.
 * @param[in]  State   The initial state of the **EvSlider**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvSlider** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSlider    *EvSlider::Create(int16_t Left, int16_t Top, int32_t Value, const EvSlider *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvSlider  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvSlider *)EvObj::TryCreate(new EvSlider(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mBdWidth = Src->mBdWidth;
    obj->mBdColor = Src->mBdColor;
    obj->mSliderDelay = Src->mSliderDelay;
    obj->mColorLower = Src->mColorLower;
    obj->mColorUpper = Src->mColorUpper;
    obj->mColorKnob = Src->mColorKnob;
    obj->mMin = Src->mMin;
    obj->mMax = Src->mMax;
    obj->resizeEvent();
    obj->setValue(obj->mSetPoint = Value);
    obj->mValue = obj->mSetPoint;
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSlider::EvSlider(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mValue(0),
  mMin(0),
  mMax(100),
  mSetPoint(0),
  mSliderDelay(SLIDER_DELAY * 1000L),
  mColorLower(CL_SLIDER_LOWER),
  mColorUpper(CL_SLIDER_UPPER),
  mColorKnob(CL_SLIDER_KNOB),
  mTouchKnob(false),
  mBusy(false),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the position value of the Slider.
 * 
 * @return     The position value of the Slider.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t     EvSlider::Value(void)
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

bool        EvSlider::SetValue(int32_t Value)
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
  if (mColorLower.Set(ColorLower) | mColorUpper.Set(ColorUpper) | mColorKnob.Set(ColorKnob))
    Modified();
}

/// @copydoc EvNumInt::SetRange()

void        EvSlider::SetRange(int32_t Min, int32_t Max)
{
  if (Min != Max)
  {
    mMin = Min;
    mMax = Max;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvSlider::SetOnTouch(void (*OnTouch)(EvSlider *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvSlider::SetOnChange(void (*OnChange)(EvSlider *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::drawEvent(void)
{
  int32_t   knobPos;
  int16_t   x, y, border, radius;

  mKnobSize = (mBdShape == ROUND_CORNERS) ? mSliderWidth : ((mSliderWidth * 2) / 3);
  knobPos = ((mValue - mMin) * (mSliderLength - mKnobSize)) / (mMax - mMin);
  x = y = (mSliderWidth - mThickness) >> 1;
  radius = mBdRadius >> 1;
  Disp->Clear(0, 1, 1);
  Disp->StencilOp(INCR, INCR);

  if ((border = mSliderWidth >> 6) < 24)
    border = 24;

  if (mWidth >= mHeight)
  {
    FillRectangle2f(8, y, (mWidth - 1) << 4, mThickness, mColorUpper.Get(), radius);
    FillRectangle2f(8, y, knobPos + (mKnobSize >> 1), mThickness, mColorLower.Get(), radius);
    Disp->TagValue(KNOB_TAG);
    FillRectangle2f(knobPos, 0, mKnobSize, mSliderWidth, mColorKnob.Get(), mBdRadius, border, mColorUpper.Get());
  }
  else
  {
    y = (mHeight << 4) - knobPos - (mKnobSize >> 1);
    FillRectangle2f(x, 8, mThickness, (mHeight - 1) << 4, mColorUpper.Get(), radius);
    FillRectangle2f(x, y, mThickness, ((mHeight - 1) << 4) - y, mColorLower.Get(), radius);
    Disp->TagValue(KNOB_TAG);
    FillRectangle2f(0, y - (mKnobSize >> 1), mSliderWidth, mKnobSize, mColorKnob.Get(), mBdRadius, border, mColorUpper.Get());
  }

  if (!IsEnabled())
  {
    Disp->ColorA(150);
    Disp->StencilFunc(GREATER, 0, 255);
    FillRectangle(0, 0, mWidth, mHeight, CL_WHITE_SMOKE, mBdRadius);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::resizeEvent(void)
{
  if (mWidth >= mHeight)
  {
    mThickness = mHeight * 3;
    mSliderWidth = mHeight << 4;
    mSliderLength = mWidth << 4;
  }
  else
  {
    mThickness = mWidth * 3;
    mSliderWidth = mWidth << 4;
    mSliderLength = mHeight << 4;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSlider::refreshEvent(void)
{
  int32_t   value = mValue;

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

void        EvSlider::touchEvent(const EvTouchEvent *Touch)
{
  int32_t   value, knobPos, sliderRange;

  sliderRange = mSliderLength - mKnobSize;
  knobPos = ((mWidth >= mHeight ? Touch->x : mHeight - Touch->y) << 4) - (mKnobSize >> 1);
  knobPos = (knobPos < 0) ? 0 : ((knobPos > sliderRange) ? sliderRange : knobPos);
  value = (knobPos * (mMax - mMin)) / (sliderRange >> 4);
  value = mMin + ((value + 8) >> 4);

  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

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

    case TOUCH_HOLD:
      mTouchKnob = true;

    case TOUCH_END:
      if (mSliderDelay < EvDisplay::FrameTime())
        SetValue(value);
      else
      {
        mSetPoint = value;

        if ((mInc = (abs(mSetPoint - mValue) * EvDisplay::FrameTime()) / mSliderDelay) < 1)
          mInc = 1;
      }

    case TOUCH_CANCEL:
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSlider::setValue(int32_t Value)
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

  if (mValue == Value || mBusy)
    return false;

  mValue = Value;
  mBusy = true;
  Modified();

  if (mOnChange != nullptr)
    mOnChange(this, mValue);

  mBusy = false;
  return true;
}
