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

const char * const EvNumInt::TypeName = "EvNumInt";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvNumInt** class.
 * 
 * A new **EvNumInt** class is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvNumInt**.
 * @param[in]  Top     The top position of the **EvNumInt**.
 * @param[in]  Width   The width of the **EvNumInt**.
 * @param[in]  Height  The height of the **EvNumInt**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvNumInt**. If nullptr, the default tag name is **"EvNumInt"**.
 * @param[in]  State   The initial state of the **EvNumInt**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvNumInt** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumInt  *EvNumInt::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvNumInt  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvNumInt *)EvObj::TryCreate(new EvNumInt(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->TextFont(25);
    obj->TextPadding(5, 0);
    obj->TextColor(CL_NUMINT_TEXT);
    obj->TextAlign(RIGHT_CENTER);
    obj->BgColor(CL_NUMINT_BG);
    obj->BdShape(FIXED_CORNERS);
    obj->SetValue(0);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvNumInt** class.
 * 
 * A new **EvNumInt** class is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvNumInt**.
 * @param[in]  Top     The top position of the **EvNumInt**.
 * @param[in]  *Src    The address pointer of the **EvNumInt** source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvNumInt**. If nullptr, the default tag name is "**"EvNumInt"**.
 * @param[in]  State   The initial state of the **EvNumInt**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvNumInt** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumInt  *EvNumInt::Create(int16_t Left, int16_t Top, int32_t Value, const EvNumInt *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvNumInt  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvNumInt *)EvObj::TryCreate(new EvNumInt(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mTurnOver = Src->mTurnOver;
    obj->mColorHold = Src->mColorHold;
    obj->mColorInc = Src->mColorInc;
    obj->mColorDec = Src->mColorDec;
    obj->mFormat = Src->mFormat;
    obj->mMin = Src->mMin;
    obj->mMax = Src->mMax;
    obj->mInc = Src->mInc;
    obj->printValue(obj->mValue = Value);
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumInt::EvNumInt(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mValue(-1),
  mMin(0),
  mMax(1000),
  mInc(0),
  mBusy(false),
  mSkipUp(false),
  mTurnOver(false),
  mColorHold(CL_NUMINT_HOLD),
  mColorInc(CL_NUMINT_INC),
  mColorDec(CL_NUMINT_DEC),
  mFormat("%ld"),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
  BdWidth(24);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the value of the NumInt.
 * 
 * @return     The value of the NumInt.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t     EvNumInt::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Increment the value of the NumInt.
 * 
 * If the Value is changed, the OnChange callback function is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 *
 * @param[in]  Inc     The increment value of the NumInt.
 * 
 * @return     true if the value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvNumInt::IncValue(int32_t Inc)
{
  int32_t   value = mValue + Inc;

  if (mTurnOver)
  {
    if (Inc < 0 && value < mMin)
      value += mMax - mMin + 1;
    else if (Inc > 0 && value > mMax)
      value -= mMax - mMin + 1;
  }

  return SetValue(value);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the value of the NumInt.
 * 
 * If the Value is changed, the OnChange callback function is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 *
 * @param[in]  Value   The value of the NumInt.
 * 
 * @return     true if the value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvNumInt::SetValue(int32_t Value)
{
  if (Value < mMin)
    Value = mMin;
  else if (Value > mMax)
    Value = mMax;

  if (mValue == Value || mFormat == nullptr || mBusy)
    return false;

  mBusy = true;
  printValue(mValue = Value);

  if (mOnChange != nullptr)
    mOnChange(this, Value);

  mBusy = false;
  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Set or Clear the TurnOver flag.
 * 
 * @param[in]  TurnOver   The TurnOver flag value.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::SetTurnOver(bool TurnOver)
{
  mTurnOver = TurnOver;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text output format.
 * 
 * Used the standard printf format. Default is "%ld".
 * 
 * @param[in]  *Format   The cstring output format (standard printf format).
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::SetFormat(const char *Format)
{
  mFormat = Format;
  printValue(mValue);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the valid range of value.
 * 
 * Min maybe smaller or larger than Max.
 * 
 * @param[in]  Min    The minimum range value.
 * @param[in]  Max    The maximum range value.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::SetRange(int32_t Min, int32_t Max)
{
  mMin = Min;
  mMax = Max;

  if (mValue < Min)
    SetValue(Min);
  else if (mValue > Max)
    SetValue(Max);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::SetColor(uint16_t ColorHold, uint16_t ColorInc, uint16_t ColorDec)
{
  if (mColorHold != ColorHold || mColorInc != ColorInc || mColorDec != ColorDec)
  {
    mColorHold = ColorHold;
    mColorInc = ColorInc;
    mColorDec = ColorDec;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvNumInt::SetOnTouch(void (*OnTouch)(EvNumInt *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvNumInt::SetOnChange(void (*OnChange)(EvNumInt *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::drawEvent(void)
{
  DrawText(0, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::touchEvent(const EvTouchEvent *Touch)
{
  uint16_t  delta;
  uint16_t  bdColor;
  int16_t   relX = Touch->x;
  int16_t   relY = Touch->y;

  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mInc = 0;
      mSkipUp = false;
      BdColor(relX < mWidth / 2 ? mColorDec : mColorInc);
      Touch->repeatTimer = 250;
      Touch->repeatDelay = 250;
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      if (!mSkipUp)
        break;

      if (relY > mHeight)
      {
        mInc = -1;
        bdColor = mColorDec;
        delta = ((relY - mHeight) > 75) ? 75 : relY - mHeight;
      }
      else if (relY < 0)
      {
        mInc = 1;
        bdColor = mColorInc;
        delta = (relY < -75) ? 75 : -relY;
      }
      else
      {
        mInc = 0;
        delta = 0;
        bdColor = mColorHold;
      }

      Touch->repeatDelay = 333 - (delta << 2);
      BdColor(bdColor);
      Touch->event = 0;
      break;

    case TOUCH_HOLD:
      BdColor(mColorHold);
      mSkipUp = true;
      Touch->event = 0;
      break;

    case TOUCH_REPEAT:
      IncValue(mInc);
      Touch->event = 0;
      break;

    case TOUCH_END:
      if (!mSkipUp && (uint16_t)relX <= mWidth && (uint16_t)relY <= mHeight)
        IncValue(relX < mWidth / 2 ? -1 : 1);

    case TOUCH_CANCEL:
      BdColor(CL_NOCOLOR);
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumInt::printValue(int32_t Value)
{
  char      str[80];

  snprintf(str, sizeof(str) - 1, mFormat, Value);
  TextLabel(str);
}
