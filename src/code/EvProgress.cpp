
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Progress Bar.
 * 
 * A new Progress Bar is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @note       If the height is greater than the width, the Progress Bar works vertically.
 *             
 * @param[in]  Left    The left position of the Progress Bar.
 * @param[in]  Top     The top position of the Progress Bar.
 * @param[in]  Width   The width of the Progress Bar.
 * @param[in]  Height  The height of the Progress Bar.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Progress Bar. If nullptr, the default tag name is "EvProgress".
 * @param[in]  State   The initial state of the Progress Bar. Default is set to VISIBLE_OBJ.
 *
 * @return     EvProgress address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvProgress  *EvProgress::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvProgress *)EvObj::TryCreate(new EvProgress(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvProgress" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvProgress::EvProgress(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mValue(-1),
  mColorLower(CL_PROGRESS_LOWER),
  mColorUpper(CL_PROGRESS_UPPER),
  mOnTouch(nullptr)
{
  TextAlign(CENTER);
  TextColor(CL_PROGRESS_TEXT);
  BgColor(CL_PROGRESS_UPPER);
  BdShape(ROUND_CORNERS);
  SetFormat("%.0f%%");
  SetValue(0);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the position value of the Progress Bar.
 * 
 * @return     The position value of the Progress Bar.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvProgress::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the position value of the Progress Bar.
 * 
 * @param[in]  Value   The position value of the Progress Bar.
 * 
 * @return     true if the position value is changed, otherwise returns false.
 * 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvProgress::SetValue(int16_t Value)
{
  Value = (Value > 1000) ? 1000 : ((Value < 0) ? 0 : Value);

  if (mValue == Value)
    return false;

  if (mFormat != nullptr)
  {
    char    str[80];

    snprintf(str, sizeof(str) - 1, mFormat, ((float)Value * mM) + mB);
    TextLabel(str);
  }

  mValue = Value;
  Modified();
  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Progress Bar.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorLower  The color of the lower side of the Progress Bar.
 * @param[in]  ColorUpper  The color of the upper side of the Progress Bar.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProgress::SetColor(uint16_t ColorLower, uint16_t ColorUpper)
{
  if (mColorLower.Set(ColorLower) | mColorUpper.Set(ColorUpper))
    Modified();

  BgColor(ColorUpper);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the text output format.
 * 
 * Used the standard printf format. Default is "%.0f%%".
 * 
 * @param[in]  *Format   The cstring output format (standard printf format).
 * @param[in]  M         The scale factor for the output display. M = 0.1 by default.
 * @param[in]  B         The offset added after scaling. B = 0 by default.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProgress::SetFormat(const char *Format, const float M, const float B)
{
  mM = M;
  mB = B;

  if ((mFormat = Format) != nullptr)
  {
    int16_t   value = mValue;

    mValue = -1;
    SetValue(value);
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvProgress::SetOnTouch(void (*OnTouch)(EvProgress *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProgress::drawEvent(void)
{
  uint16_t  length = ((int32_t)mValue * ((mWidth > mHeight) ? mWidth : mHeight)) / 1000;

  if (length > 0)
  {
    Disp->SaveContext();
    Disp->Clear(0, 1, 1);
    Disp->StencilOp(INCR, INCR);
    Disp->ColorMask(0, 0, 0, 0);

    if (mWidth >= mHeight)
      FillRectangle(0, 0, length, mHeight, mColorLower.Get(), 0);
    else
      FillRectangle(0, mHeight - length, mWidth, length, mColorLower.Get(), 0);

    Disp->StencilFunc(EQUAL, 1, 255);
    Disp->ColorMask(1, 1, 1, 1);
    FillRectangle(0, 0, mWidth, mHeight, mColorLower.Get(), mBdRadius);
    Disp->RestoreContext();
  }

  if (mFormat != nullptr)
    DrawText(0, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProgress::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}
