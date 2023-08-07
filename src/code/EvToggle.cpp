
#include    <EvGUI.h>

#define     KNOB_TAG        100
#define     SLIDE_DELAY     150

#define     COLOR_ON        RGB555(  0, 200, 100)
#define     COLOR_OFF       RGB555(210, 210, 210)
#define     COLOR_KNOB      RGB555(255, 255, 255)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Toggle switch.
 * 
 * A new Toggle switch is created at the specified size and relative position
 * of its owner Dest.
 *             
 * @param[in]  Left    The left position of the Toggle switch.
 * @param[in]  Top     The top position of the Toggle switch.
 * @param[in]  Width   The width of the Toggle switch.
 * @param[in]  Height  The height of the Toggle switch.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Toggle switch. If NULL, the default tag name is "EvToggle".
 * @param[in]  State   The initial state of the Toggle switch. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     EvToggle address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvToggle    *EvToggle::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvToggle *)EvObj::TryCreate(new EvToggle(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvToggle" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvToggle::EvToggle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mTouchKnob = false;
  SetColor(COLOR_ON, COLOR_OFF, COLOR_KNOB);
  BdShape(ROUND_CORNERS);
  SetOnTouch(NULL);
  SetOnChange(NULL);
  SetValue(mValue = false);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the state value of the Toggle switch.
 * 
 * @return     true if is On and false if is Off.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvToggle::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the state value of the Toggle switch.
 * 
 * If the Value is changed, the OnChange callback function is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 *
 * @param[in]  Value   The state value of the Toggle switch. Maybe true or false.
 * 
 * @return     true if the state value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvToggle::SetValue(int16_t Value)
{
  mDirKnob = 0;
  mTimerKnob = 0;

  if (Value)
  {
    mPosKnob = SLIDE_DELAY;
    BgColor(mColorOn);
  }
  else
  {
    mPosKnob = 0;
    BgColor(mColorOff);
  }

  if (mValue == Value)
    return false;

  mValue = Value;
  Modified();

  if (mOnChange != NULL)
    (*mOnChange)(this, mValue);

  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Toggle switch.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorOn      The color of the Toggle switch when is On.
 * @param[in]  ColorOff     The color of the Toggle switch when is Off.
 * @param[in]  ColorKnob    The color of the knob.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvToggle::SetColor(uint16_t ColorOn, uint16_t ColorOff, uint16_t ColorKnob)
{
  if (mColorOn != ColorOn || mColorOff != ColorOff || mColorKnob != ColorKnob)
  {
    mColorOn = ColorOn;
    mColorOff = ColorOff;
    mColorKnob = ColorKnob;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvToggle::SetOnTouch(void (*OnTouch)(EvToggle *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvToggle::SetOnChange(void (*OnChange)(EvToggle *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvToggle::drawEvent(void)
{
  int16_t   x, y, radius, border;

  if (mWidth >= mHeight)
  {
    y = radius = (mHeight - 1) << 3;
    x = radius + ((((mWidth - 1) << 4) - (radius * 2)) * mPosKnob / SLIDE_DELAY);
  }
  else
  {
    x = radius = (mWidth - 1) << 3;
    y = (mHeight - 1) << 4;
    y -= radius + ((y - (radius * 2)) * mPosKnob / SLIDE_DELAY);
  }

  border = (radius * 8) / 100; 
  Disp->Clear(0, 1, 1);
  Disp->TagValue(KNOB_TAG);
  Disp->ColorA(50);
  FillCircle2f(x, y + border, RGB555(0, 0, 0), radius - border);
  Disp->ColorA(255);
  FillCircle2f(x, y, mColorKnob, radius - (border * 2));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvToggle::refreshEvent(void)
{
  if (mTimerKnob != 0)
  {
    int16_t pos = millis();

    Modified();
    pos -= mTimerKnob;

    if (mDirKnob < 0)
    {
      if ((mPosKnob = SLIDE_DELAY - pos) <= 0)
        SetValue(false);
    }
    else if ((mPosKnob = pos) >= SLIDE_DELAY)
      SetValue(true);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvToggle::touchEvent(EvTouchEvent *Touch)
{
  int16_t   pos, toogleSize;

  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  if (mWidth >= mHeight)
  {
    pos = Touch->x;
    toogleSize = mWidth;
  }
  else
  {
    pos = mHeight - Touch->y;
    toogleSize = mHeight;
  }

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchKnob = (Touch->tag == KNOB_TAG);
      Touch->event = 0;
      mSkipUp = false;
      break;

    case TOUCH_END:
      if (!mSkipUp && pos > 0 && pos < toogleSize)
      {
        mTimerKnob = millis_nz();
        mDirKnob = mValue ? -1 : 1;
      }

      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      if (mTouchKnob)
      {
        Touch->event = 0;

        if (mTimerKnob == 0)
        {
          if (mValue && pos < (toogleSize * 4) / 10)
            mDirKnob = -1;
          else if (!mValue && pos > (toogleSize * 6) / 10)
            mDirKnob = 1;
          else break;

          mTimerKnob = millis_nz();
          mSkipUp = true;
        }
      }
      break;

    case TOUCH_CANCEL:
      Touch->event = 0;
      mSkipUp = true;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t      millis_nz(void)
{
  uint32_t    msec = millis();

  if (msec == 0)
    msec--;

  return msec;
}

