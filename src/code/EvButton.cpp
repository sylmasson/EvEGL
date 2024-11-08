
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvButton**.
 * 
 * A new **EvButton** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvButton**.
 * @param[in]  Top     The top position of the **EvButton**.
 * @param[in]  Width   The width of the **EvButton**.
 * @param[in]  Height  The height of the **EvButton**.
 * @param[out] *Dest   The destination **EvPanel** address pointer.
 * @param[in]  Tag     The tag name of the **EvButton**. If nullptr, the default tag name is **"EvButton"**.
 * @param[in]  State   The initial state of the **EvButton**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvButton** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton    *EvButton::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvButton  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvButton *)EvObj::TryCreate(new EvButton(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvButton" : Tag, State), Dest)) != nullptr)
  {
    obj->TextAlign(CENTER);
    obj->TextLabel(obj->Tag);
    obj->TextColor(CL_BUTTON_TEXT, CL_BUTTON_TEXT_DOWN);
    obj->BdShape(FIXED_CORNERS | SHADOW);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvButton**.
 * 
 * A new **EvButton** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvButton**.
 * @param[in]  Top     The top position of the **EvButton**.
 * @param[in]  Label   The Label of the **EvButton**.
 * @param[in]  Font    The font size of the Label.
 * @param[out] *Dest   The destination **EvPanel** address pointer.
 * @param[in]  Tag     The tag name of the **EvButton**. If nullptr, the default tag name is **"EvButton"**.
 * @param[in]  State   The initial state of the **EvButton**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvButton** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton    *EvButton::Create(int16_t Left, int16_t Top, const char *Label, uint8_t Font, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvButton  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvButton *)EvObj::TryCreate(new EvButton(Left, Top, 0, 0, Dest->Disp, !Tag ? "EvButton" : Tag, State), Dest)) != nullptr)
  {
    uint16_t  w;

    obj->TextFont(Font);
    obj->TextLabel(Label);
    obj->TextAlign(CENTER);
    obj->TextColor(CL_BUTTON_TEXT, CL_BUTTON_TEXT_DOWN);
    obj->mHeight = 10 * ((obj->TextHeight(Font) * 2) / 10);
    obj->mWidth = (obj->mHeight * 5) / 2;
    if ((w = obj->TextWidth(Label)) > obj->mWidth - 20)
      obj->mWidth = (10 * ((w + 5) / 10)) + 20;
    obj->BdShape(FIXED_CORNERS | SHADOW);
    obj->SetView();
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvButton**.
 * 
 * A new **EvButton** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvButton**.
 * @param[in]  Top     The top position of the **EvButton**.
 * @param[in]  Label   The Label of the **EvButton**.
 * @param[in]  *Src    The address pointer of the **EvButton** source model.
 * @param[out] *Dest   The destination **EvPanel** address pointer.
 * @param[in]  Tag     The tag name of the **EvButton**. If nullptr, the default tag name is **"EvButton"**.
 * @param[in]  State   The initial state of the **EvButton**. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     **EvButton** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton    *EvButton::Create(int16_t Left, int16_t Top, const char *Label, const EvButton *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvButton  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvButton *)EvObj::TryCreate(new EvButton(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, !Tag ? "EvButton" : Tag, State), Dest)) != nullptr)
  {
    obj->TextLabel(Label);
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mBdWidth = Src->mBdWidth;
    obj->mBdColor = Src->mBdColor;
    obj->mColorUp = Src->mColorUp;
    obj->mColorDown = Src->mColorDown;
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvButton::EvButton(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mValue(0),
  mColorUp(CL_BUTTON_FACE),
  mColorDown(CL_BUTTON_FACE_DOWN),
  mTouchFlag(false),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
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

  if (mOnChange != nullptr)
    mOnChange(this, mValue);

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
  if (mColorUp.Set(ColorUp) | mColorDown.Set(ColorDown))
    Modified();
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

void        EvButton::SetOnTouch(void (*OnTouch)(EvButton *Sender, const EvTouchEvent *Touch))
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

  if (mBdShape & SHADOW)
  {
    h -= (y = Disp->Width() / 400);

    if (!mValue)
    {
      Disp->ColorA(100);
      FillRectangle(0, y, w, h, CL_BLACK, mBdRadius);
      Disp->ColorA(255);
      y = 0;
    }
  }

  FillRectangle(0, y, w, h, mValue ? mColorDown.Get() : mColorUp.Get(), mBdRadius);
  DrawText(0, y, mWidth, h, c_str(Label), mValue ? mStyle.color2.Get() : mStyle.color.Get());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvButton::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchFlag = true;
      Touch->event = 0;
      SetValue(true);
      break;

    case TOUCH_CANCEL:
    case TOUCH_END:
      mTouchFlag = false;
      Touch->event = 0;
      SetValue(false);
      break;
  }
}
