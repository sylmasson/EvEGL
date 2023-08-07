
#include    <EvGUI.h>

#define     COLOR_CHECK     RGB555(  0,   0, 160)
#define     COLOR_UNCHECK   RGB555(255, 255, 255)
#define     COLOR_BORDER    RGB555(160, 160, 160)
#define     COLOR_TEXT      RGB555(  0,   0,   0)
#define     COLOR_TEXT_2    RGB555(150, 150, 150)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard CheckBox.
 * 
 * A new CheckBox is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the CheckBox.
 * @param[in]  Top     The top position of the CheckBox.
 * @param[in]  Width   The width of the CheckBox.
 * @param[in]  Height  The height of the CheckBox.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the CheckBox. If NULL, the default tag name is "EvCheckBox".
 * @param[in]  State   The initial state of the CheckBox. Default is set to VISIBLE_OBJ.
 *
 * @return     EvCheckBox address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox  *EvCheckBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvCheckBox *)EvObj::TryCreate(new EvCheckBox(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvCheckBox" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvCheckBox::EvCheckBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mValue = false;
  mTouchBox = false;
  TextLabel("CheckBox");
  TextAlign(LEFT_CENTER);
  TextColor(COLOR_TEXT, COLOR_TEXT_2);
  SetColor(COLOR_CHECK, COLOR_UNCHECK, COLOR_BORDER);
  BdShape(RATIO_CORNERS);
  SetOnTouch(NULL);
  SetOnChange(NULL);
  SetValue(true);
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

  if (mOnChange != NULL)
    (*mOnChange)(this, mValue);

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
  if (mColorCheck != ColorCheck || mColorUncheck != ColorUncheck || mColorBorder != ColorBorder)
  {
    mColorCheck = ColorCheck;
    mColorUncheck = ColorUncheck;
    mColorBorder = ColorBorder;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvCheckBox::SetOnTouch(void (*OnTouch)(EvCheckBox *Sender, EvTouchEvent *Touch))
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
    FillRectangle(0, 0, size, size, mColorBorder, radius);
    Disp->ColorA(255 >> mTouchBox);
    if ((bd = size >> 2) < 16) bd = 16;
    if ((radius -= bd) < 16) radius = 16;
    FillRectangle2f(bd, bd, (size << 4) - (bd * 2), (size << 4) - (bd * 2), mColorUncheck, radius);
    Disp->ColorA(255);
  }
  else
  {
    Disp->ColorA(255 >> mTouchBox);
    FillRectangle(0, 0, size, size, mColorCheck, radius);
    Disp->ColorA(255);
    Disp->ColorRGB(mColorUncheck);
    Disp->Begin(LINE_STRIP);
    Disp->LineWidth(1 * size);
    Disp->Vertex2f(4 * size, 9 * size);
    Disp->Vertex2f(6 * size, 11 * size);
    Disp->Vertex2f(12 * size, 5 * size);
  }

  DrawText(size + 4, 0, mWidth - (size + 4), mHeight, c_str(Label), IsEnabled() ? mStyle.color : mStyle.color2);

  if (!IsEnabled())
  {
    Disp->ColorA(150);
    FillRectangle(0, 0, size, size, RGB555(230, 230, 230), radius);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvCheckBox::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      Touch->event = 0;
      mTouchBox = true;
      Modified();
      break;

    case TOUCH_END:
      SetValue(!mValue);

    case TOUCH_CANCEL:
      Touch->event = 0;
      mTouchBox = false;
      Modified();
      break;
  }
}
