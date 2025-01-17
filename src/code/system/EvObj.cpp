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

//#define     VERBOSE

const char * const EvObj::TypeName = "EvObj";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj::EvObj(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  mStatus(State),
  mOpacity(256),
  mLeft(Left),
  mTop(Top),
  mWidth(Width),
  mHeight(Height),
  mTouchCnt(0),
  mTouchMax(1),
  mBgColor(CL_NOCOLOR),
  mBgColorA(0),
  mBdShape(SQUARE_CORNERS),
  mBdRadius(0),
  mBdWidth(0),
  mBdColor(CL_NOCOLOR),
  mLabel(""),
  mOwner(nullptr),
  mCache(nullptr),
  Disp(Disp),
  Tag(!Tag ? TypeName : Tag),
  TagId(0)
{
  mStyle.font = 25;
  mStyle.align = LEFT_CENTER;
  mStyle.padX = 0;
  mStyle.padY = 0;
  TextColor(CL_DEFAULT_TEXT, CL_REVERSE_TEXT);
  ModifiedText();
  SetView();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj::~EvObj(void)
{
  #ifdef VERBOSE
    char    str[32];

    snprintf(str, sizeof(str) - 1, "~EvObj: %p ", this);
    EvOut->print(str);
    DisplayTagList(EvOut);
    EvOut->println();
  #endif

  if (IsOnKbdFocus())
    LostKbdFocus();

  if (mOwner != nullptr)
    mOwner->RemoveObj(this);

  Disp->Touch.WaitForRelease(this);
  EvEditor::DestroyedObj(this);

  if (mCache != nullptr)
    Disp->RAM_G.Free(mCache);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the left position of the Object.
 * 
 * @return     The left position of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::Left(void)
{
  return mLeft;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the top position of the Object.
 * 
 * @return     The top position of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::Top(void)
{
  return mTop;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the width of the Object.
 * 
 * @return     The width of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvObj::Width(void)
{
  return mWidth;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the height of the Object.
 * 
 * @return     The height of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvObj::Height(void)
{
  return mHeight;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the border style of the Object.
 * 
 * @return     The border style of the Object.
 * 
 * @see        BdShape()
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvObj::Shape(void)
{
  return mBdShape;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Enable the touch event on the Object.
 * 
 * When creating a new Object, the touch event is enabled by default.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Enable(void)
{
  if (mStatus & DISABLED_OBJ)
  {
    mStatus &= ~DISABLED_OBJ;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Disable the touch event on the Object.
 * 
 * When creating a new Object, the touch event is enabled by default.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Disable(void)
{
  if (!(mStatus & DISABLED_OBJ))
  {
    mStatus |= DISABLED_OBJ;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Delete the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Delete(void)
{
  delete this;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the Object as modified.
 * 
 * Notify that the Object is modified for the next drawEvent.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Modified(void)
{
  mStatus |= MODIFIED_OBJ;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the Text Label of the Object as modified.
 * 
 * Notify that the Text Label of the Object is modified for the next refreshEvent
 * and notify that the Object is modified for the next drawEvent.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::ModifiedText(void)
{
  mStatus |= (MODIFIED_OBJ | MODIF_TEXT_OBJ);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the Object is visible.
 * 
 * @return     true if visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsVisible(void)
{
  return (mStatus & VISIBLE_OBJ) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the Object is viewable.
 * 
 * @return     true if viewable.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsViewable(void)
{
  return ((mStatus & VISIBLE_OBJ) != 0 && mView.w > 0) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the Object is enabled.
 * 
 * @return     true if enabled.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsEnabled(void)
{
  return (mStatus & DISABLED_OBJ) ? false : true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the Object is modified.
 * 
 * @return     true if modified.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsModified(void)
{
  return (mStatus & MODIFIED_OBJ) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the Text Label of the Object is modified.
 * 
 * @return     true if modified.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsModifiedText(void)
{
  return (mStatus & MODIF_TEXT_OBJ) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if is a control Object.
 * 
 * Allows to know if the Object should keep control of touch event.
 * For example, the EvScrollBar class requires this option.
 * 
 * @return     true if is a control Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsControlObj(void)
{
  return (mStatus & CONTROL_OBJ) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if is a multi-touch Object.
 * 
 * Used to find out if the object allows the use of multi-touch events.
 * 
 * @return     true if is a multi-touch Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsMultiTouchObj(void)
{
  return (mTouchMax > 1) ? true : false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the border style of the Object.
 * 
 * @param[in]  Shape    The border style of the Object.
 * 
 * Five choices of borders style are possible:
 * 
 * @tableofcontents
 *     Shape      |                         Description
 * -------------- | -----------------------------------------------------------------
 * ROUND_CORNERS  | Corners with the largest possible radius.
 * RATIO_CORNERS  | Corners with a proportional radius to the Object size.
 * FIXED_CORNERS  | Corners with a fixed radius according to the display resolution.
 * SQUARE_CORNERS | Corners defined as squares.
 * USER_CORNERS   | Corners defined with BdRadius().
 * 
 * @see        BdRadius()
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BdShape(uint8_t Shape)
{
  uint16_t  radius;

  switch (Shape & 7)
  {
    case ROUND_CORNERS:  radius = min(mWidth, mHeight) << 3; break; // radius = mWidth << 4; break;
    case RATIO_CORNERS:  radius = min(mWidth, mHeight) << 2; break; // (mHeight < mWidth) ? mHeight << 2 : mWidth << 2; break;
    case FIXED_CORNERS:  radius = (Disp->Width() >= 800) ? 6 << 4 : 4 << 4; break;
    case SQUARE_CORNERS: radius = 0; break;
    case USER_CORNERS:   radius = mBdRadius; break;
    default: return;
  }

  if (mBdRadius != radius || mBdShape != Shape)
  {
    mBdShape = Shape;
    mBdRadius = radius;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the border radius of the Object.
 * 
 * @param[in]  Radius   The border radius of the Object.
 * 
 * Radius is allways interpreted in 1/16 of pixel.
 * 
 * @note       This function overwrites the border style defined by BdShape().
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BdRadius(uint16_t Radius)
{
  if (mBdRadius != Radius || mBdShape != USER_CORNERS)
  {
    mBdShape = USER_CORNERS;
    mBdRadius = Radius;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the border width of the Object.
 * 
 * @param[in]  Width    The border width of the Object.
 * 
 * Width is allways interpreted in 1/16 of pixel. The default width is 0.
 * 
 * @see        BdColor
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BdWidth(uint16_t Width)
{
  if (mBdWidth != Width)
  {
    mBdWidth = Width;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the border color of the Object.
 * 
 * @param[in]  Color    The border color of the Object.
 * 
 * Color are defined in RGB555 format. The default is CL_NOCOLOR.
 * 
 * A border color is drawing only if BdWidth has been defined.
 * 
 * @see        BdWidth
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BdColor(uint16_t Color)
{
  if (mBdColor.Set(Color))
    Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the background color of the Object.
 * 
 * @param[in]  Color    The background color of the Object.
 * 
 * Color are defined in RGB555 format. The default is CL_NOCOLOR.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BgColor(uint16_t Color)
{
  if (mBgColor.Set(Color) | (mBgColorA != 0xFF))
  {
    mBgColorA = 0xFF;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the background color of the Object.
 * 
 * @param[in]  Color    The background color of the Object.
 * @param[in]  Aplha    The background colorA of the Object..
 * 
 * Color are defined in RGB555 format. The default is CL_NOCOLOR.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BgColor(uint16_t Color, uint8_t Aplha)
{
  if (mBgColor.Set(Color) | (mBgColorA != Aplha))
  {
    mBgColorA = Aplha;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Move the Object by offset value.
 * 
 * @param[in]  X     The horizontal offset value.
 * @param[in]  Y     The verticel offset value.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::MoveRel(int16_t X, int16_t Y)
{
  if (X || Y)
  {
    mLeft += X;
    mTop += Y;
    SetView();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Move the Object to the absolute positions.
 * 
 * @param[in]  Left   The new left position of the Object.
 * @param[in]  Top    The new Top position of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::MoveTo(int16_t Left, int16_t Top)
{
  if (mLeft != Left || mTop != Top)
  {
    mLeft = Left;
    mTop = Top;
    SetView();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Move the Object to another EvPanel at the absolute positions.
 * 
 * @param[in]  Dest   The destination EvPanel address pointer.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::MoveTo(EvPanel *Dest)
{
  if (Dest != mOwner && Dest != nullptr)
  {
    if (mOwner != nullptr)
      mOwner->RemoveObj(this);

    mOwner = nullptr;
    SetDisplay(Dest->Disp);
    Dest->AddObj(this, Tag);
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Resize the Object.
 * 
 * @param[in]  Width   The new width of the Object.
 * @param[in]  Height  The new height of the Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::ReSize(uint16_t Width, uint16_t Height)
{
  if (mWidth != Width || mHeight != Height)
  {
    mWidth = Width;
    mHeight = Height;
    BdShape(mBdShape);
    Modified();
    SetView();
    resizeEvent();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Align the object to its owner.
 * 
 * @param[in]  Align   The Object alignment.
 * @param[in]  PadX    The horizontal Object padding.
 * @param[in]  PadY    The vertical Object padding.
 * 
 * Ten choices are possible:
 * 
 * @tableofcontents
 * |     Align      |
 * | -------------- |
 * | LEFT_TOP       |
 * | CENTER_TOP     |
 * | RIGHT_TOP      |
 * | LEFT_CENTER    |
 * | CENTER_CENTER  |
 * | RIGHT_CENTER   |
 * | LEFT_BOTTOM    |
 * | CENTER_BOTTOM  |
 * | RIGHT_BOTTOM   |
 * | CENTER         |
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::OwnerAlign(uint8_t Align, int16_t PadX, int16_t PadY)
{
  if (mOwner != nullptr)
  {
    int16_t   x, y;

    switch ((Align >> 2) & 3)
    {
      case 0: y = PadY; break;
      case 1: y = (mOwner->mHeight - mHeight) / 2; break;
      case 2: y = mOwner->mHeight - mHeight - PadY; break;
      case 3: y = mTop; break;
    }

    switch (Align & 3)
    {
      case 0: x = PadX; break;
      case 1: x = (mOwner->mWidth - mWidth) / 2; break;
      case 2: x = mOwner->mWidth - mWidth - PadX; break;
      case 3: x = mLeft;
    }

    MoveTo(x, y);
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the EvPanel owner of the Object.
 * 
 * @param[in]  Level    The owner's back level.
 * 
 * @return     Owner's EvPanel address pointer if one exists, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel     *EvObj::GetOwner(uint16_t Level)
{
  EvPanel   *owner = mOwner;

  while (--Level && owner != nullptr)
    owner = owner->mOwner;

  return owner;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the EvPanel owner of the Object.
 * 
 * @param[in]  Owner    The owner's EvPanel address pointer.
 * 
 * Normally, the owner is set automatically when creating an Object.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::SetOwner(EvPanel *Owner)
{
  mOwner = Owner;
  SetView();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Move the Object to the foreground of the screen.
 * 
 * @param[in]  AllOwner   Recursive on all owners.
 * 
 * If AllOwner is true, all of its owners are also set to the foreground while preserving the hierarchy.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::ToFront(bool AllOwner)
{
  if (mOwner != nullptr)
    mOwner->SetToFront(this, AllOwner);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Set the keyboard focus on the current Object.
 * 
 * The keyboard focus is implemented on any EvObj with inheritance of the Print class.
 * You can use any method of the Print class to send characters to the focused object.
 * For example, the keyboard sends the keys with the Print class method to the focused object.
 * 
 * @see        LostKbdFocus, GetKbdFocus, IsOnKbdFocus
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::SetKbdFocus(void)
{
  EvKbd     *kbd = Disp->Kbd;

  if (kbd->FocusObj != this)
  {
    if (kbd->FocusObj != nullptr)
      kbd->FocusObj->lostKbdFocusEvent();

    kbd->FocusObj = this;
    kbd->SetLayout(LAYOUT_SHIFT);
    kbd->Open();
    setKbdFocusEvent();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Lost keyboard focus of current Object.
 * 
 * @see        SetKbdFocus, GetKbdFocus, IsOnKbdFocus
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::LostKbdFocus(void)
{
  EvKbd     *kbd = Disp->Kbd;

  if (kbd->FocusObj != nullptr)
  {
    kbd->FocusObj->lostKbdFocusEvent();
    kbd->FocusObj = nullptr;
    kbd->Close();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Gets who has the keyboard focus.
 * 
 * @return     Current object that has the keyboard focus.
 *
 * @see        SetKbdFocus, LostKbdFocus, IsOnKbdFocus
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvObj::GetKbdFocus(void)
{
  return Disp->Kbd->FocusObj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the keyboard focus is on the current Object.
 * 
 * @return     True if the keyboard focus is on the current Object, otherwise returns False.
 *
 * @see        SetKbdFocus, LostKbdFocus, GetKbdFocus
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::IsOnKbdFocus(void)
{
  return GetKbdFocus() == this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvObj::TouchCnt(void)
{
  return mTouchCnt;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvObj::TouchMax(void)
{
  return mTouchMax;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvObj::TouchMax(uint16_t Max)
{
  return mTouchMax = (Max > 5) ? 5 : Max;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the Object as visible.
 * 
 * If the Object is not visible, change it to visible Object.
 * 
 * By default, a new Object is set as visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Show(void)
{
  if (!(mStatus & VISIBLE_OBJ))
  {
    mStatus |= VISIBLE_OBJ;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the Object as not visible.
 * 
 * If the Object is visible, change it to not visible Object.
 * 
 * By default, a new Object is set as visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Hide(void)
{
  if (mStatus & VISIBLE_OBJ)
  {
    mStatus &= ~VISIBLE_OBJ;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Clear the display list cache of the Opject.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::ClearCache(void)
{
  if (mCache)
  {
    Disp->RAM_G.Free(mCache);
    mCache = nullptr;
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the opacity of the Object.
 * 
 * @param[in]  Opacity   The opacity of the Object. Can be from 0 to 256.
 * 
 * @li  0 for full transparency.
 * @li  256 for a full opacity.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::SetOpacity(uint16_t Opacity)
{
  if (Opacity > 256)
    Opacity = 256;

  if (mOpacity != Opacity)
  {
    mOpacity = Opacity;
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the label of the Object to an empty string.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextClear(void)
{
  TextLabel("");
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text font of the Object.
 * 
 * @param[in]  Font   The text font of the Object.
 * 
 * Must be a valid display font number.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextFont(uint8_t Font)
{
  if (Font != 0 && Font <= LAST_FONT && mStyle.font != Font)
  {
    mStyle.font = Font;
    ModifiedText();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text alignment of the Object.
 * 
 * @param[in]  Align    The text alignment of the Object.
 * 
 * Thirteen choices are possible:
 * 
 * @tableofcontents
 * |     Align      |
 * | -------------- |
 * | LEFT_TOP       |
 * | CENTER_TOP     |
 * | RIGHT_TOP      |
 * | JUSTIFY_TOP    |
 * | LEFT_CENTER    |
 * | CENTER_CENTER  |
 * | RIGHT_CENTER   |
 * | JUSTIFY_CENTER |
 * | LEFT_BOTTOM    |
 * | CENTER_BOTTOM  |
 * | RIGHT_BOTTOM   |
 * | JUSTIFY_BOTTOM |
 * | CENTER         |
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextAlign(uint8_t Align)
{
  if (mStyle.align != Align)
  {
    mStyle.align = Align;
    ModifiedText();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text padding of the Object.
 * 
 * @param[in]  X   The horizontal text padding of the Object.
 * @param[in]  Y   The vertical text padding of the Object.
 * 
 * When CENTER alignment element is defined, the text padding as no effect.
 * 
 * @see        TextAlign
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextPadding(int8_t X, int8_t Y)
{
  if (mStyle.padX != X || mStyle.padY != Y)
  {
    mStyle.padX = X;
    mStyle.padY = Y;
    ModifiedText();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets only the normal text color of the Object.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  Color      Normal text color.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextColor(uint16_t Color)
{
  if (mStyle.color.Set(Color))
    Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text colors of the Object.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  Color      Normal text color.
 * @param[in]  Color2     Second text color option.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextColor(uint16_t Color, uint16_t Color2)
{
  if (mStyle.color.Set(Color) | mStyle.color2.Set(Color2))
    Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the label of the Object.
 * 
 * @param[in]  Label  The C string label of the Object.
 * 
 * The text label is available, if it wants, by a child class of EvObj.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextLabel(const char *Label)
{
  mLabel = (Label == nullptr) ? "" : Label;
  ModifiedText();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the label of the Object.
 * 
 * @param[in]  Label  The String label of the Object.
 * 
 * The text label is available, if it wants, by a child class of EvObj.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TextLabel(const String &Label)
{
  mLabel = Label;
  ModifiedText();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the text height of the font in pixels.
 *
 * @param[in]  Font   The text font. If not specify the default font is the object's font.
 *
 * @return     The text height value in pixels.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::TextHeight(uint8_t Font)
{
  const EvFont  *fnt;

  if (Font == 0)
    Font = mStyle.font;

  if (Font <= LAST_FONT && (fnt = Disp->SystemFont[Font]) != nullptr)
    return fnt->Height;

  return 0;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the width of a character in specific font in pixels.
 *
 * @param[in]  C      The character to be evaluated.
 * @param[in]  Font   The text font. If not specify the default font is the object's font.
 *
 * @return     The width value of a character in pixels.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::TextWidth(const char C, uint8_t Font)
{
  const EvFont  *fnt;
  int16_t       c = C;

  if (Font == 0)
    Font = mStyle.font;

  if (Font <= LAST_FONT && (fnt = Disp->SystemFont[Font]) != nullptr && c >= fnt->CharBeg && c < (fnt->CharBeg + fnt->CharCnt))
    return fnt->Width[c];

  return 0;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the width of a String in specific font in pixels.
 *
 * @param[in]  Str    The String to be evaluated.
 * @param[in]  Font   The text font. If not specify the default font is the object's font.
 * @param[in]  Count  The maximum of characters. Default is -1 limiting at 65535 characters.
 *
 * @return     The width value of a String in pixels.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::TextWidth(String &Str, uint8_t Font, int16_t Count)
{
  return TextWidth(c_str(Str), Font, Count);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the width of a C string in specific font in pixels.
 *
 * @param[in]  Str    The C string to be evaluated.
 * @param[in]  Font   The text font. If not specify the default font is the object's font.
 * @param[in]  Count  The maximum of characters. Default is -1 limiting at 65535 characters.
 *
 * @return     The width value of a C string in pixels.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::TextWidth(const char *Str, uint8_t Font, int16_t Count)
{
  const EvFont  *fnt;
  int16_t       c, width = 0;

  if (Font == 0)
    Font = mStyle.font;

  if (Str != nullptr && Font <= LAST_FONT && (fnt = Disp->SystemFont[Font]) != nullptr)
    for (; (c = *Str) != 0 && Count != 0; Str++, Count--)
      if (c >= fnt->CharBeg && c < (fnt->CharBeg + fnt->CharCnt))
        width += fnt->Width[c];

  return width;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the cursor width of the font in pixels.
 *
 * @param[in]  Font   The text font. If not specify the default font is the object's font.
 *
 * @return     The cursor width value in pixels. Minimum is 1 pixel.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvObj::TextCursorWidth(uint8_t Font)
{
  const EvFont  *fnt;

  if (Font == 0)
    Font = mStyle.font;

  if (Font <= LAST_FONT && (fnt = Disp->SystemFont[Font]) != nullptr)
    return fnt->CurWidth;

  return 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str)
{
  return DrawText(Left, Top, Width, Height, Str, mStyle.color.Get(), mStyle.font, mStyle.align, mStyle.padX, mStyle.padY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str, uint16_t Color)
{
  return DrawText(Left, Top, Width, Height, Str, Color, mStyle.font, mStyle.align, mStyle.padX, mStyle.padY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str, uint16_t Color, uint8_t Font, uint8_t Align, int16_t PadX, int16_t PadY)
{
  int32_t   x = 0;
  int16_t   i, c, y;
  int16_t   textHeight = TextHeight(Font);
  int16_t   textWidth = TextWidth(Str, Font);
  int16_t   spCount, spWidth = TextWidth(' ', Font) << 4;

  if (Width > 0 && Height > 0 && (Color & 0x8000) != 0 && textWidth > 0 && textHeight > 0)
  {
    switch ((Align >> 2) & 3)
    {
      case 0: y = Top + PadY; break;
      case 1: y = Top + ((Height - textHeight) / 2); break;
      case 2:
      case 3: y = Top + Height - textHeight - PadY - 1; break;
    }

    switch (Align & 3)
    {
      case 0: x = Left + PadX; break;
      case 1: x = Left + ((Width - textWidth + 1) / 2); break;
      case 2: x = Left + Width - textWidth - PadX - 1; break;
      case 3:
        for (i = 0, spCount = 0; (c = Str[i]) != 0 && c != '\n'; i++)
          if (c == ' ')
            spCount++;

        x = Left + PadX;
        i = Width - textWidth - (PadX * 2) - 1;

        if (i > 0 && spCount > 0 && c != '\n')
          spWidth += (i << 4) / spCount;
    }

    Disp->ColorRGB(Color);
    Disp->Begin(BITMAPS);
    x <<= 4;

    while ((c = *Str++) != 0 && c != '\n')
    {
      if (c == ' ')
        x += spWidth;
      else if ((textWidth = TextWidth(c, Font) << 4) > 0)
      {
        if (x + textWidth >= 0 && x < (mWidth << 4))
        {
          if ((uint16_t)x + 8 < (512 << 4) && (uint16_t)y < 512)
            Disp->Vertex2ii((x + 8) >> 4, y, Font, c);
          else
          {
            Disp->BitmapHandle(Font);
            Disp->Cell(c);
            Disp->Vertex2f(x, y << 4);
          }
        }

        x += textWidth;
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawCircle(int16_t X, int16_t Y, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth)
{
  uint16_t  r = Radius - 8;

  FillRectangle2f((X << 4) - r, (Y << 4) - r, r << 1, r << 1, 0, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawCircle2f(int16_t X, int16_t Y, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth)
{
  uint16_t  r = Radius - 8;

  FillRectangle2f(X - r, Y - r, r << 1, r << 1, 0, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::FillCircle(int16_t X, int16_t Y, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth, uint16_t BdColor)
{
  uint16_t  r = Radius - 8;

  FillRectangle2f((X << 4) - r, (Y << 4) - r, r << 1, r << 1, FillColor, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::FillCircle2f(int16_t X, int16_t Y, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth, uint16_t BdColor)
{
  uint16_t  r = Radius - 8;

  FillRectangle2f(X - r, Y - r, r << 1, r << 1, FillColor, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawRectangle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth)
{
  FillRectangle2f(Left << 4, Top << 4, Width << 4, Height << 4, 0, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DrawRectangle2f(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth)
{
  FillRectangle2f(Left, Top, Width, Height, 0, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::FillRectangle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth, uint16_t BdColor)
{
  FillRectangle2f(Left << 4, Top << 4, Width << 4, Height << 4, FillColor, Radius, BdWidth, BdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::FillRectangle2f(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth, uint16_t BdColor)
{
  int16_t   x1, y1, x2, y2, r1, r2, offset = 0;

  r1 = Radius;
  x2 = (x1 = Left) + (Width -= 16);
  y2 = (y1 = Top) + (Height -= 16);

  if (r1 > (Width >>= 1)) r1 = Width;
  if (r1 > (Height >>= 1)) r1 = Height;

  x1 += r1;
  y1 += r1;
  x2 -= r1;
  y2 -= r1;

  if (r1 < 16) r1 = 16;

  if (!(FillColor & 0x8000))
  {
    if (!BdWidth || !(BdColor & 0x8000))
      return;

    if (BdWidth >= Width || BdWidth >= Height)
    {
      Disp->ColorRGB(BdColor);
      Disp->LineWidth(r1);
      Disp->Begin(RECTS);
      Disp->Vertex2f(x1, y1);
      Disp->Vertex2f(x2, y2);
    }
    else
    {
      if ((r2 = r1 - BdWidth) < 16)
      {
        offset = 16 - r2;
        r2 = 16;
      }

      Disp->SaveContext();
      Disp->ColorMask(0, 0, 0, 1);
      Disp->BlendFunc(ONE, ZERO);
      Disp->ColorRGB(BdColor);
      Disp->LineWidth(r1);
      Disp->Begin(RECTS);
      Disp->Vertex2f(x1, y1);
      Disp->Vertex2f(x2, y2);

      Disp->BlendFunc(ZERO, ONE_MINUS_SRC_ALPHA);
      Disp->Opacity(OPACITY_MAX);
      Disp->ColorA(255);
      Disp->LineWidth(r2);
      Disp->Vertex2f(x1 + offset, y1 + offset);
      Disp->Vertex2f(x2 - offset, y2 - offset);

      Disp->ColorMask(1, 1, 1, 0);
      Disp->BlendFunc(DST_ALPHA, ONE_MINUS_DST_ALPHA);
      Disp->LineWidth(r1);
      Disp->Vertex2f(x1, y1);
      Disp->Vertex2f(x2, y2);
      Disp->RestoreContext();
    }
  }
  else
  {
    if (!BdWidth || !(BdColor & 0x8000) || BdColor == FillColor)
    {
      Disp->ColorRGB(FillColor);
      Disp->LineWidth(r1);
      Disp->Begin(RECTS);
      Disp->Vertex2f(x1, y1);
      Disp->Vertex2f(x2, y2);
    }
    else
    {
      Disp->ColorRGB(BdColor);
      Disp->LineWidth(r1);
      Disp->Begin(RECTS);
      Disp->Vertex2f(x1, y1);
      Disp->Vertex2f(x2, y2);

      if (BdWidth < Disp->Width() && BdWidth < Disp->Height())
      {
        if ((r2 = r1 - BdWidth) < 16)
        {
          offset = 16 - r2;
          r2 = 16;
        }

        Disp->ColorRGB(FillColor);
        Disp->LineWidth(r2);
        Disp->Vertex2f(x1 + offset, y1 + offset);
        Disp->Vertex2f(x2 - offset, y2 - offset);
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::BeginFunction(uint16_t &Label, uint16_t CmdSize)
{
  Label = (Disp->ReadDL() >> 2) + 1;
  Disp->Jump(Label + CmdSize + 3);
  Disp->SaveContext();
  Disp->ClearContext();
  Disp->ClearPrimitive();
  mStatus |= FUNCT_USED_OBJ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::EndFunction(uint16_t Label, uint16_t CmdSize)
{
  if (!(mStatus & FUNCT_USED_OBJ))
  {
    EvErr->print("\nEndFunction() Error: BeginFunction() was not called in ");
    DisplayTagList(EvErr);
  }
  else
  {
    uint16_t  size;

    Disp->RestoreContext();
    Disp->Return();
    size = (Disp->ReadDL() >> 2) - Label - 3;

    if (CmdSize != size)
    {
      char    str[80];

      snprintf(str, sizeof(str) - 1, "\nEndFunction() Error: CmdSize Expected=%u, Generated=%u in ", CmdSize, size);
      EvErr->print(str);
      DisplayTagList(EvErr);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::DisplayTagList(Stream *Out)
{
  EvObj     *obj;

  for (obj = this; obj != nullptr; )
  {
    Out->print('[');
    Out->print(obj->Tag == nullptr ? "nullptr" : obj->Tag);
    Out->print(']');

    if ((obj = obj->mOwner) == nullptr)
      break;

    Out->print("->");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvObj::TouchStart(void)
{
  if (mTouchCnt < mTouchMax)
  {
    mTouchCnt++;
    return true;
  }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TouchEnd(void)
{
  if (mTouchCnt > 0)
    mTouchCnt--;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::TouchUpdate(const EvTouchEvent *Touch)
{
  static const char   *nameEvent[7] = {"START", "END", "MOVE", "HOLD", "REPEAT", "DOUBLE", "CANCEL"};

  if (Touch->event)
  {
    if ((Disp->sTraceFlags & TRACE_TOUCH) != 0 && Touch->event != TOUCH_REPEAT && (!(mStatus & CONTROL_OBJ) || Touch->obj == nullptr))
    {
      char    str[80];

      snprintf(str, sizeof(str) - 1, "\n[%s] (%5lums) %3d TP%u %-7s", Disp->Tag, Touch->timer, Touch->tag, Touch->id, nameEvent[Touch->event-1]);
      EvOut->print(str);
      snprintf(str, sizeof(str) - 1, "xy(%3d,%4d)  ", Touch->x, Touch->y);
      EvOut->print(str);

      if (Touch->event == TOUCH_START)
        snprintf(str, sizeof(str) - 1, "abs(%3u,%4u)  ", Touch->abs.x, Touch->abs.y);
      else
        snprintf(str, sizeof(str) - 1, "move(%3d,%3d)  ", Touch->move.x, Touch->move.y);

      EvOut->print(str);
      DisplayTagList(EvOut);
    }

    if (Touch->event == TOUCH_DOUBLE)
    {
      if (!(mStatus & (SYSTEM_OBJ | FIXED_OBJ)))
        EvEditor::SelectObj(this);
      else if (this == Disp)
        EvEditor::SelectObj(nullptr);
    }

    if (IsEnabled())
      touchEvent(Touch);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvObj::Touching(const EvTouchEvent *Touch)
{
  int16_t   x, y, pad = 12;

  x = Touch->abs.x;
  y = Touch->abs.y;

  return ((mStatus & (VISIBLE_OBJ | DISABLED_OBJ)) == VISIBLE_OBJ && x >= mView.x1-pad && x <= mView.x2+pad && y >= mView.y1-pad && y <= mView.y2+pad) ? this : nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::SetDisplay(EvDisplay *Disp)
{
  if (EvObj::Disp != Disp)
  {
    if (mCache != nullptr)
    {
      EvObj::Disp->RAM_G.Free(mCache);
      mCache = nullptr;
    }

    EvObj::Disp = Disp;
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::SetView(void)
{
  int16_t   x, y;
  EvView    p = mView;
  EvView    &v = mView;

  if (mOwner != nullptr)
  {
    v = mOwner->mView;

    if (mStatus & FLOATING_OBJ)
    {
      v.x1 = v.y1 = 0;
      v.x2 = Disp->Width() - 1;
      v.y2 = Disp->Height() - 1;
    }

    if (!(mStatus & FIXED_OBJ))
    {
      v.ox += mOwner->OffsetX();
      v.oy += mOwner->OffsetY();
    }
  }
  else
  {
    v.x1 = v.ox = 0;
    v.y1 = v.oy = 0;
    v.x2 = Disp->Width() - 1;
    v.y2 = Disp->Height() - 1;
  }

  x = v.ox += mLeft;
  y = v.oy += mTop;

  if (x > v.x1)
    v.x1 = x;

  if ((x += mWidth - 1) < v.x2)
    v.x2 = x;

  if (y > v.y1)
    v.y1 = y;

  if ((y += mHeight - 1) < v.y2)
    v.y2 = y;

  if ((v.w = v.x2 - v.x1 + 1) < 0 || (v.h = v.y2 - v.y1 + 1) < 0)
    v.w = v.h = 0;

  if (!(mStatus & (MODIFIED_OBJ | MOVED_OBJ)))
    if (p.ox != v.ox || p.oy != v.oy || p.x1 != v.x1 || p.y1 != v.y1 || p.w != v.w || p.h != v.h)
      mStatus |= MOVED_OBJ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Refresh(void)
{
  refreshEvent();
  mStatus &= ~MODIF_TEXT_OBJ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::Draw(void)
{
  if (!(mStatus & VISIBLE_OBJ) || mView.w <= 0)
  {
    if (mCache != nullptr)
    {
      Disp->RAM_G.Free(mCache);
      mCache = nullptr;
    }
  }
  else
  {
    if (mCache != nullptr && !(mStatus & (MODIFIED_OBJ | FUNCT_USED_OBJ)))
    {
      if (mStatus & MOVED_OBJ)
      {  // if only moved, overwrite commands VertexTranslate and Scissor
        Disp->wr32(mCache->addr + 4, EV_VERTEX_TRANSLATE_X(mView.ox << 4));
        Disp->wr32(mCache->addr + 8, EV_VERTEX_TRANSLATE_Y(mView.oy << 4));
        Disp->wr32(mCache->addr + 12, EV_SCISSOR_XY(mView.x1, mView.y1));
        Disp->wr32(mCache->addr + 16, EV_SCISSOR_SIZE(mView.w, mView.h));
        mStatus &= ~MOVED_OBJ;
      }

      Disp->CmdAppend(mCache->addr, mCache->used);
      Disp->wrCmdBufUpdate();
    }
    else
    {
      uint16_t    sizeDL, startDL = Disp->ReadDL();

      Disp->SaveContext();
      Disp->VertexTranslateX(mView.ox << 4);
      Disp->VertexTranslateY(mView.oy << 4);
      Disp->ScissorXY(mView.x1, mView.y1);
      Disp->ScissorSize(mView.w, mView.h);
      Disp->Opacity(mOpacity);
      Disp->ClearPrimitive();

      if (mBgColor.Get() != 0 && Disp->ColorA(mBgColorA) != 0)
        FillRectangle2f(0, 0, mWidth << 4, mHeight << 4, mBgColor.Get(), mBdRadius);

      if (Disp->ColorA(255) != 0)
      {
        mStatus &= ~(MODIFIED_OBJ | MOVED_OBJ | FUNCT_USED_OBJ);
        drawEvent();

        if (mBdWidth)
        {
          Disp->TagMask(0);
          FillRectangle2f(0, 0, mWidth << 4, mHeight << 4, 0, mBdRadius, mBdWidth, mBdColor.Get());
        }

        if ((mStatus & (DISABLED_OBJ | FILTER_DIS_OBJ)) == (DISABLED_OBJ | FILTER_DIS_OBJ))
        {
          Disp->ColorA(150);
          FillRectangle2f(0, 0, mWidth << 4, mHeight << 4, RGB555(230, 230, 230), mBdRadius);
        }
      }

      Disp->RestoreContext();
      Disp->wrCmdBufUpdate();
      sizeDL = Disp->ReadDL() - startDL;
      mCache = (mCache == nullptr) ? Disp->RAM_G.Malloc(sizeDL, this) : Disp->RAM_G.Realloc(mCache, sizeDL);

      if (mCache != nullptr)
      {
        Disp->CmdMemCpy(mCache->addr, (((EvMem *)mCache)->startDL = startDL) + RAM_DL, sizeDL);
        Disp->wrCmdBufUpdate();
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvObj::abortCreate(void)
{
  mStatus |= ABORT_OBJ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvObj::TryCreate(EvObj *Obj, EvPanel *Dest)
{
  if (Dest == nullptr)
    EvErr->println("TryCreate: Dest is nullptr");
  else if (Obj != nullptr)
  {
    #ifdef VERBOSE
      char    str[80];

      snprintf(str, sizeof(str) - 1, "TryCreate: %p \"%s\"", Obj, Obj->Tag);
      EvOut->print(str);
    #endif

    if (!(Obj->mStatus & ABORT_OBJ) && Dest->AddObj(Obj) != nullptr)
    {
      #ifdef VERBOSE
        EvOut->println(" Ok");
      #endif
      return Obj;
    }

    #ifdef VERBOSE
      EvOut->println(" Abort");
    #endif

    delete Obj;
  }

  return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
