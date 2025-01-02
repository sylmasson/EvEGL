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

const char * const EvCheckBox::TypeName = "EvCheckBox";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvCheckBox**.
 * 
 * A new **EvCheckBox** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvCheckBox**.
 * @param[in]  Top     The top position of the **EvCheckBox**.
 * @param[in]  Width   The width of the **EvCheckBox**.
 * @param[in]  Height  The height of the **EvCheckBox**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvCheckBox**. If nullptr, the default tag name is **"EvCheckBox"**.
 * @param[in]  State   The initial state of the **EvCheckBox**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvCheckBox** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox  *EvCheckBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvCheckBox  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvCheckBox *)EvObj::TryCreate(new EvCheckBox(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->TextLabel(obj->Tag);
    obj->TextColor(CL_CHECKBOX_TEXT, CL_CHECKBOX_TEXT2);
    obj->BdShape(RATIO_CORNERS);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvCheckBox**.
 * 
 * A new **EvCheckBox** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvCheckBox**.
 * @param[in]  Top     The top position of the **EvCheckBox**.
 * @param[in]  Label   The Label of the **EvCheckBox**.
 * @param[in]  Font    The font size of the Label.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvCheckBox**. If nullptr, the default tag name is **"EvCheckBox"**.
 * @param[in]  State   The initial state of the **EvCheckBox**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvCheckBox** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox  *EvCheckBox::Create(int16_t Left, int16_t Top, const char *Label, uint8_t Font, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvCheckBox  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvCheckBox *)EvObj::TryCreate(new EvCheckBox(Left, Top, 0, 0, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->TextFont(Font);
    obj->TextLabel(Label);
    obj->TextPadding(5, 0);
    obj->TextColor(CL_CHECKBOX_TEXT, CL_CHECKBOX_TEXT2);
    obj->mHeight = 10 * ((obj->TextHeight(Font) + 5) / 10);
    obj->mWidth = obj->TextWidth(Label) + obj->mHeight + obj->mStyle.padX + 10;
    obj->BdShape(RATIO_CORNERS);
    obj->SetView();
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard **EvCheckBox**.
 * 
 * A new **EvCheckBox** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvCheckBox**.
 * @param[in]  Top     The top position of the **EvCheckBox**.
 * @param[in]  Label   The Label of the **EvCheckBox**.
 * @param[in]  *Src    The address pointer of the **EvCheckBox** source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvCheckBox**. If nullptr, the default tag name is **"EvCheckBox"**.
 * @param[in]  State   The initial state of the **EvCheckBox**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvCheckBox** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox  *EvCheckBox::Create(int16_t Left, int16_t Top, const char *Label, const EvCheckBox *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvCheckBox  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvCheckBox *)EvObj::TryCreate(new EvCheckBox(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, Tag, State), Dest)) != nullptr)
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
    obj->mColorCheck = Src->mColorCheck;
    obj->mColorUncheck = Src->mColorUncheck;
    obj->mColorBorder = Src->mColorBorder;
    obj->mWidth = obj->TextWidth(Label) + obj->mHeight + obj->mStyle.padX + 10;
    obj->SetView();
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox::EvCheckBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mValue(true),
  mColorCheck(CL_CHECKBOX_TRUE),
  mColorUncheck(CL_CHECKBOX_FALSE),
  mColorBorder(CL_CHECKBOX_BD),
  mTouchFlag(false),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the state value of the CheckBox.
 * 
 * @return     true if is checked and false if is unchecked.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvCheckBox::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the state value of the CheckBox.
 * 
 * If the Value is changed, the OnChange callback function is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 *
 * @param[in]  Value   The state value of the CheckBox. Maybe true or false.
 * 
 * @return     true if the state value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvCheckBox::SetValue(int16_t Value)
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
 * @brief      Sets the colors of the CheckBox.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorCheck     The color of the CheckBox when checked.
 * @param[in]  ColorUncheck   The color of the CheckBox when unchecked.
 * @param[in]  ColorBorder    The border color of the CheckBox.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvCheckBox::SetColor(uint16_t ColorCheck, uint16_t ColorUncheck, uint16_t ColorBorder)
{
  if (mColorCheck.Set(ColorCheck) | mColorUncheck.Set(ColorUncheck) | mColorBorder.Set(ColorBorder))
    Modified();
}

/// @copydoc EvButton::SetOnTouch()

void        EvCheckBox::SetOnTouch(void (*OnTouch)(EvCheckBox *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvCheckBox::SetOnChange(void (*OnChange)(EvCheckBox *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvCheckBox::drawEvent(void)
{
  uint16_t  size = mHeight;
  int16_t   bd, radius = mBdRadius;

  if (!mValue)
  {
    FillRectangle(0, 0, size, size, mColorBorder.Get(), radius);
    Disp->ColorA(255 >> mTouchFlag);
    if ((bd = size >> 2) < 16) bd = 16;
    if ((radius -= bd) < 16) radius = 16;
    FillRectangle2f(bd, bd, (size << 4) - (bd * 2), (size << 4) - (bd * 2), mColorUncheck.Get(), radius);
    Disp->ColorA(255);
  }
  else
  {
    Disp->ColorA(255 >> mTouchFlag);
    FillRectangle(0, 0, size, size, mColorCheck.Get(), radius);
    Disp->ColorA(255);
    Disp->ColorRGB(mStyle.color2.Get());
    Disp->Begin(LINE_STRIP);
    Disp->LineWidth(1 * size);
    Disp->Vertex2f(4 * size, 9 * size);
    Disp->Vertex2f(6 * size, 11 * size);
    Disp->Vertex2f(12 * size, 5 * size);
  }

  DrawText(size + 4, 0, mWidth - (size + 4), mHeight, c_str(Label), IsEnabled() ? mStyle.color.Get() : CL_GRAY);

  if (!IsEnabled())
  {
    Disp->ColorA(150);
    FillRectangle(0, 0, size, size, CL_WHITE_SMOKE, radius);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvCheckBox::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchFlag = true;
      Touch->event = 0;
      Modified();
      break;

    case TOUCH_END:
      SetValue(!mValue);

    case TOUCH_CANCEL:
      mTouchFlag = false;
      Touch->event = 0;
      Modified();
      break;
  }
}
