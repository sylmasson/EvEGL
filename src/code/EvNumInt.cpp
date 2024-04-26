
#include    <EvGUI.h>

#define     BD_SEL_COLOR    RGB555(  0,   0, 160)
#define     BD_INC_COLOR    RGB555(  0, 200, 100)
#define     BD_DEC_COLOR    RGB555(230,   0,   0)

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
  EvNumInt  *num = nullptr;

  if (Dest != nullptr && (num = (EvNumInt *)EvObj::TryCreate(new EvNumInt(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvNumInt" : Tag, State), Dest)) != nullptr)
  {
    num->TextFont(25);
    num->TextPadding(5, 0);
    num->TextAlign(RIGHT_CENTER);
    num->BdShape(FIXED_CORNERS);
    num->SetValue(0);
  }

  return num;
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
 * @param[in]  Tag     The tag name of the **EvNumInt**. If nullptr, the default tag name is "**EvNumInt"**.
 * @param[in]  State   The initial state of the **EvNumInt**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvNumInt** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumInt  *EvNumInt::Create(int16_t Left, int16_t Top, int32_t Value, const EvNumInt *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvNumInt  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvNumInt *)EvObj::TryCreate(new EvNumInt(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, !Tag ? "EvNumInt" : Tag, State), Dest)) != nullptr)
  {
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mBdWidth = Src->mBdWidth;
    obj->mBdColor = Src->mBdColor;
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
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mValue(-1),
  mMin(0),
  mMax(1000),
  mInc(0),
  mSkipUp(false),
  mFormat("%ld"),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
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

  if (mValue == Value || mFormat == nullptr)
    return false;

  printValue(mValue = Value);

  if (mOnChange != nullptr)
    mOnChange(this, Value);

  return true;
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
      BdWidth(24);
      BdColor(relX < mWidth / 2 ? BD_DEC_COLOR : BD_INC_COLOR);
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
        bdColor = BD_DEC_COLOR;
        delta = ((relY - mHeight) > 25) ? 25 : relY - mHeight;
      }
      else if (relY < 0)
      {
        mInc = 1;
        bdColor = BD_INC_COLOR;
        delta = (relY < -25) ? 25 : -relY;
      }
      else
      {
        mInc = 0;
        delta = 0;
        bdColor = BD_SEL_COLOR;
      }

/*      if (relX < 0)
      {
        mInc = -1;
        bdColor = BD_DEC_COLOR;
        delta = (relX <= -25) ? 25 : -relX;
      }
      else if (relX > mWidth)
      {
        mInc = 1;
        bdColor = BD_INC_COLOR;
        delta = ((relX - mWidth) > 25) ? 25 : relX - mWidth;
      }
      else
      {
        mInc = 0;
        delta = 0;
        bdColor = BD_SEL_COLOR;
      } */

      if (delta == 25)
        mInc *= 2;

      Touch->repeatDelay = 250 - (delta << 3);
      BdColor(bdColor);
      Touch->event = 0;
      break;

    case TOUCH_HOLD:
      BdColor(BD_SEL_COLOR);
      mSkipUp = true;
      Touch->event = 0;
      break;

    case TOUCH_REPEAT:
      SetValue(mValue + mInc);
      Touch->event = 0;
      break;

    case TOUCH_END:
      if (!mSkipUp && (uint16_t)relX <= mWidth && (uint16_t)relY <= mHeight)
        SetValue(mValue + (relX < mWidth / 2 ? -1 : 1));

    case TOUCH_CANCEL:
      BdWidth(0);
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
