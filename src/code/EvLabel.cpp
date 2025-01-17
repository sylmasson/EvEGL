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

const char * const EvLabel::TypeName = "EvLabel";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Width   The width of **EvLabel**.
 * @param[in]  Height  The height of **EvLabel**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest)) != nullptr)
  {
    obj->TextLabel(obj->Tag);
    obj->TextColor(CL_LABEL_TEXT);
    obj->TextAlign(RIGHT_CENTER);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Label   The object **EvLabel**.
 * @param[in]  Font    The font size of **EvLabel**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, const char *Label, uint8_t Font, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, 0, 0, Dest->Disp, !Tag ? Label : Tag, State), Dest)) != nullptr)
  {
    obj->TextFont(Font);
    obj->TextLabel(Label);
    obj->TextColor(CL_LABEL_TEXT);
    obj->mHeight = 10 * ((obj->TextHeight(Font) + 5) / 10);
    obj->mWidth = obj->TextWidth(Label);
    obj->SetView();
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Label   The object **EvLabel**.
 * @param[in]  *Src    The address pointer of the **EvLabel**. source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, const char *Label, const EvLabel *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, !Tag ? Label : Tag, State), Dest)) != nullptr)
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
    if ((obj->mStyle.align & 3) == 0)
      obj->mWidth = obj->TextWidth(Label);
    obj->SetView();
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel::EvLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mTouchFlag(false),
  mOnTouch(nullptr)
{
}

/// @copydoc EvButton::SetOnTouch()

void        EvLabel::SetOnTouch(void (*OnTouch)(EvLabel *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::drawEvent(void)
{
  DrawText(0, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchFlag = true;
      Touch->event = 0;
      break;

    case TOUCH_CANCEL:
    case TOUCH_END:
      mTouchFlag = false;
      Touch->event = 0;
      break;
  }
}
