
#include    <EvGUI.h>

#define     COLOR_UP        RGB555(210, 210, 210)
#define     COLOR_DOWN      RGB555(170, 170, 170)
#define     TEXT_UP         RGB555(0, 0, 0)
#define     TEXT_DOWN       RGB555(0, 0, 0)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvButton**.
 * 
 * A new **EvButton** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvButton**.
 * @param[in]  Top     The top position of the Button.
 * @param[in]  Width   The width of the **EvButton**.
 * @param[in]  Height  The height of the **EvButton**.
 * @param[out] *Dest   The destination **EvPanel** address pointer.
 * @param[in]  Tag     The tag name of the **EvButton**. If NULL, the default tag name is **"EvButton"**.
 * @param[in]  State   The initial state of the **EvButton**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvButton** address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton    *EvButton::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvButton *)EvObj::TryCreate(new EvButton(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvButton" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton::EvButton(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mValue = false;
  TextColor(TEXT_UP, TEXT_DOWN);
  SetColor(COLOR_UP, COLOR_DOWN);
  BdShape(FIXED_CORNERS | SHADOW);
  SetOnTouch(NULL);
  SetOnChange(NULL);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the state value of the **EvButton**.
 * 
 * @return     **true** if is pressed and **false** if is released.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvButton::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the state value of the **EvButton**.
 * 
 * If the Value is changed, the **OnChange** callback function is called.
 * 
 * Define **OnChange** callback function with **SetOnChange** function.
 *
 * @param[in]  Value   The state value of the **EvButton**. Maybe true or false.
 * 
 * @return     true if the state value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvButton::SetValue(int16_t Value)
{
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
 * @brief      Sets the colors of the **EvButton**.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorUp    The color of the **EvButton** when released.
 * @param[in]  ColorDown  The color of the **EvButton** when pressed.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::SetColor(uint16_t ColorUp, uint16_t ColorDown)
{
  if (mColorUp != ColorUp || mColorDown != ColorDown)
  {
    mColorUp = ColorUp;
    mColorDown = ColorDown;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the **OnTouch** callback function.
 * 
 * If is set, this callback function is called first by the **touchEvent** function.
 * 
 * When the function is called, two parameters are sent to it. **Sender** is used
 * to access member variables and functions. **Touch** gives access to the variables
 * of the **EvtouchEvent** struct.
 * 
 * @param[in]  OnTouch  The **OnTouch** static callback function pointer.
 *
 * @see        **EvTouchEvent**
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::SetOnTouch(void (*OnTouch)(EvButton *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the **OnChange** callback function.
 * 
 * If is set, this callback function is called on each Value change.
 * 
 * When the function is called, two parameters are sent to it. **Sender** is used
 * to access member variables and functions. The second is the new **Value**.
 *
 * @param[in]  OnChange  The static **OnChange** callback function pointer.
 * 
 * @see        **SetValue**
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::SetOnChange(void (*OnChange)(EvButton *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::drawEvent(void)
{
  int16_t   y = 0, w = mWidth, h = mHeight;
  uint16_t  colorButton = mColorUp, colorText = mStyle.color;

  if (mValue)
  {
    colorButton = mColorDown;
    colorText = mStyle.color2;
  }

  if (mBdShape & SHADOW)
  {
    h -= (y = Disp->Width() / 400);

    if (!mValue)
    {
      Disp->ColorA(100);
      FillRectangle(0, y, w, h, RGB555(0, 0, 0), mBdRadius);
      Disp->ColorA(255);
      y = 0;
    }
  }

  FillRectangle(0, y, w, h, colorButton, mBdRadius);
  DrawText(0, y, mWidth, h, c_str(Label), colorText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_CANCEL:
    case TOUCH_START:
    case TOUCH_END:
      SetValue(Touch->event == TOUCH_START);
      Touch->event = 0;
      break;
  }
}
