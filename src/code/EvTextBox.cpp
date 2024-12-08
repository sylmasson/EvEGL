
#include    <EvGUI.h>

#define     ClrEditedText() (mFlags &= ~1)
#define     ClrMoveCursor() (mFlags &= ~2)
#define     ClrMoveToWord() (mFlags &= ~4)
#define     ClrTouchBox()   (mFlags &= ~8)

#define     SetEditedText() (mFlags |= 1)
#define     SetMoveCursor() (mFlags |= 2)
#define     SetMoveToWord() (mFlags |= 4)
#define     SetTouchBox()   (mFlags |= 8)

#define     IsEditedText()  (mFlags & 1)
#define     IsMoveCursor()  (mFlags & 2)
#define     IsMoveToWord()  (mFlags & 4)
#define     IsTouchBox()    (mFlags & 8)

const char * const EvTextBox::TypeName = "EvTextBox";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard TextBox.
 * 
 * A new standard TextBox is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the TextBox.
 * @param[in]  Top     The top position of the TextBox.
 * @param[in]  Width   The width of the TextBox.
 * @param[in]  Height  The height of the TextBox.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the TextBox. If nullptr, the default tag name is "EvTextBox".
 * @param[in]  State   The initial state of the TextBox. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     EvTextBox address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox   *EvTextBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTextBox *)EvObj::TryCreate(new EvTextBox(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox::EvTextBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvPanel(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mFlags(0),
  mMaxLength(0),
  mColorBd(CL_TEXTBOX_BD),
  mColorBdFocus(CL_TEXTBOX_FOCUS),
  mColorTextSelect(CL_TEXTBOX_SELECT),
  mCursorIndex(0),
  mSelectBegin(0),
  mSelectCount(0),
  mOnTouch(nullptr),
  mOnChange(nullptr),
  mOnReturn(nullptr),
  mOnFilter(nullptr),
  mOnSetKbdFocus(nullptr),
  mOnLostKbdFocus(nullptr),
  SelectAllOnSetKbdFocus(false)
{
  TextClear();
  TextFont(26);
  TextPadding(10, 0);
  TextColor(CL_TEXTBOX_TEXT);
  BgColor(CL_TEXTBOX_BG);
  BdColor(CL_TEXTBOX_BD);
  BdShape(FIXED_CORNERS);
  BdWidth(24);

  if ((Cursor = EvTextCursor::Create(0, 0, 0, 0, this, nullptr, DISABLED_OBJ | FIXED_OBJ)) == nullptr)
    abortCreate();
  else
  {
    Cursor->BgColor(CL_TEXTBOX_CURSOR);
    Cursor->SetStyle(CURSOR_SMOOTH);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void       EvTextBox::Unselect(void)
{
  mSelectBegin = 0;
  mSelectCount = 0;
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SelectAll(void)
{
  mSelectBegin = 0;
  mSelectCount = mLabel.length();
  mCursorIndex = mSelectCount;
  SetMoveCursor();
  Modified();
}

/// @copydoc EvObj::TextClear()

void        EvTextBox::TextClear(void)
{
  Unselect();
  mLabel = "";
  mPanelOffsetX = 0;
  mCursorIndex = 0;
  SetEditedText();
}

/// @copydoc EvObj::TextLabel()

void        EvTextBox::TextLabel(const char *Label)
{
  TextClear();
  write((const uint8_t *)Label, strlen(Label));
  mCursorIndex = 0;
  mPanelOffsetX = 0;
}

/// @copydoc EvObj::TextLabel()

void        EvTextBox::TextLabel(const String &Label)
{
  TextLabel(c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetMaxLength(uint16_t MaxLength)
{
  if (MaxLength > 0 && MaxLength < mLabel.length())
  {
    Unselect();
    SetEditedText();
    mLabel.remove(MaxLength);
    mCursorIndex = mLabel.length();
  }

  mMaxLength = MaxLength;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetColor(uint16_t ColorBd, uint16_t ColorBdFocus, uint16_t ColorTextSelect)
{
  if (mColorBd.Set(ColorBd) | mColorBdFocus.Set(ColorBdFocus) | mColorTextSelect.Set(ColorTextSelect))
    Modified();
}

/// @copydoc EvButton::SetOnTouch()

void        EvTextBox::SetOnTouch(void (*OnTouch)(EvTextBox *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvTextBox::SetOnChange(void (*OnChange)(EvTextBox *Sender, const String &Str))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetOnReturn(void (*OnReturn)(EvTextBox *Sender, const String &Str))
{
  mOnReturn = OnReturn;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetOnFilter(int (*OnFilter)(EvTextBox *Sender, const uint8_t C))
{
  mOnFilter = OnFilter;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetOnKbdFocus(void (*OnSetKbdFocus)(EvTextBox *Sender), void (*OnLostKbdFocus)(EvTextBox *Sender))
{
  mOnSetKbdFocus = OnSetKbdFocus;
  mOnLostKbdFocus = OnLostKbdFocus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox   &EvTextBox::operator+=(const char C)
{
  write((const uint8_t *)&C, 1);
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox   &EvTextBox::operator+=(const char *Str)
{
  write((const uint8_t *)Str, strlen(Str));
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox   &EvTextBox::operator+=(const String &Str)
{
  write((const uint8_t *)c_str(Str), Str.length());
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t      EvTextBox::WriteKey(uint8_t Key, uint8_t Layout, uint8_t ShiftKey, bool DoubleTouch)
{
  int16_t     nextLayout = -1;
  const char  *str = c_str(mLabel);

  switch (Key)
  {
    case ' ':
      if (DoubleTouch && mCursorIndex >= 2 && str[mCursorIndex - 1] == ' ' && str[mCursorIndex - 2] != ' ' && str[mCursorIndex - 2] != '.')
      {
        mLabel.setCharAt(mCursorIndex - 1, '.');

        if (ShiftKey < 2)
          nextLayout = LAYOUT_SHIFT;
      }
      else if (Layout != LAYOUT_ALPHA)
        nextLayout = LAYOUT_ALPHA;
      else if (ShiftKey <= 1 && mCursorIndex > 0 && str[mCursorIndex - 1] == '.')
        nextLayout = LAYOUT_SHIFT;
      break;

    case '\b':
      if (ShiftKey <= 1)
      {
        if (mCursorIndex == 1)
          nextLayout = LAYOUT_SHIFT;
        else if (Layout == LAYOUT_ALPHA && mCursorIndex > 0)
          nextLayout = isupper(str[mCursorIndex - 1]) ? LAYOUT_SHIFT : LAYOUT_ALPHA;
      }
      break;

    case '\'':
      if (Layout != LAYOUT_ALPHA)
        nextLayout = LAYOUT_ALPHA;
      break;

    case '\n':
      if (mOnReturn != nullptr)
        mOnReturn(this, mLabel);

      LostKbdFocus();
      break;

    default:
      if (ShiftKey == 1 && isupper(Key))
        nextLayout = LAYOUT_ALPHA;
      break;
  }

  write((const uint8_t *)&Key, 1);
  return nextLayout;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t      EvTextBox::write(uint8_t C)
{
  return write((const uint8_t *)&C, 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t      EvTextBox::write(const uint8_t *Buffer, size_t Count)
{
  int       c;
  uint16_t  len;
  char      *buf;

  Cursor->SetStyle(CURSOR_SOLID);

  for (size_t i = 0; i < Count; i++)
    if ((c = filter(*Buffer++)) == '\b')
    {
      if (mSelectCount > 0)
      {
        mLabel.remove(mCursorIndex = mSelectBegin, mSelectCount);
        SetEditedText();
        moveCursor();
        Unselect();
      }
      else if (mCursorIndex > 0)
      {
        mLabel.remove(--mCursorIndex, 1);
        SetEditedText();
      }
    }
    else if (c >= ' ' && c <= '~')
    {
      if (mSelectCount > 0)
      {
        mLabel.remove(mCursorIndex = mSelectBegin, mSelectCount);
        SetEditedText();
        moveCursor();
        Unselect();
      }

      if ((len = mLabel.length()) < mMaxLength || mMaxLength == 0)
      {
        mLabel.concat((const char)c);
        SetEditedText();

        if (mCursorIndex >= len || (uint16_t)mLabel.length() != len + 1)
          mCursorIndex = mLabel.length();
        else
        {
          buf = (char *)c_str(mLabel) + mCursorIndex;
          memmove(buf + 1, buf, len - mCursorIndex);
          mCursorIndex++;
          *buf = c;
        }
      }
    }

  Cursor->SetStyle(CURSOR_SMOOTH);
  return Count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::selectWord(const EvTouchEvent *Touch)
{
  int16_t   c, x = textLeft() + mPanelOffsetX;
  uint16_t  ind = 0, length = mLabel.length();

  Unselect();
  SetMoveCursor();
  mCursorIndex = 0;

  if (Touch->x >= x)
  {
    do
    {
      for (; ind < length && mLabel.charAt(ind) == ' '; ind++)
        if ((x += TextWidth(' ')) >= Touch->x)
        {
          mSelectCount = ind - mSelectBegin;
          mCursorIndex = ind;
          return;
        }

      for (mSelectBegin = ind, mSelectCount = 0; ind < length && (c = mLabel.charAt(ind)) != ' '; mSelectCount++, ind++)
        x += TextWidth(c);
    }
    while (Touch->x > x && ind < length);

    mCursorIndex = ind;

    if (Touch->x <= x)
      return;
  }

  SelectAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::moveToWord(const EvTouchEvent *Touch)
{
  int16_t   c, x = textLeft() + mPanelOffsetX;
  uint16_t  ind = 0, length = mLabel.length();

  Unselect();
  SetMoveCursor();
  mCursorIndex = 0;

  if (Touch->x >= x)
  {
    do
    {
      for (; ind < length && mLabel.charAt(ind) == ' '; ind++)
        if ((x += TextWidth(' ')) >= Touch->x)
        {
          mCursorIndex = ind;
          return;
        }

      for (; ind < length && (c = mLabel.charAt(ind)) != ' '; ind++)
        x += TextWidth(c);
    }
    while (Touch->x > x && ind < length);

    mCursorIndex = ind;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::moveToChar(const EvTouchEvent *Touch)
{
  int16_t   x = textLeft() + mPanelOffsetX;
  uint16_t  length = mLabel.length();

  if (mSelectCount)
    Unselect();

  for (mCursorIndex = 0; mCursorIndex < length && Touch->x > x; mCursorIndex++)
    x += TextWidth(mLabel.charAt(mCursorIndex));

  SetMoveCursor();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::moveCursor(void)
{
  int16_t   x, y, w;
  int16_t   textHeight = TextHeight();
  int16_t   cursorWidth = TextCursorWidth();
  int16_t   cursorHeight = textHeight;

  if (IsTouchBox())
  { // 50% oversize cursor
    cursorWidth += ((cursorWidth == 1) ? 1 : cursorWidth / 2);
    cursorHeight = (textHeight * 3) / 2;
  }

  w = TextWidth(c_str(mLabel), mStyle.font, mCursorIndex);
  y = textTop() - ((cursorHeight - textHeight) / 2);
  x = textLeft() + w;

  if (x + mPanelOffsetX < mStyle.padX)
    mPanelOffsetX = -w;
  else if (x + mPanelOffsetX > mWidth - mStyle.padX)
    mPanelOffsetX = mWidth - w - (mStyle.padX * 2) - 1;

  Cursor->MoveTo(x + mPanelOffsetX, y);
  Cursor->ReSize(cursorWidth, cursorHeight);
  ClrMoveCursor();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvTextBox::textLeft(void)
{
  int16_t   x;

  switch (mStyle.align & 3)
  {
    case 3:
    case 0: x = mStyle.padX; break;
    case 1: x = ((mWidth - TextWidth(c_str(mLabel)) + 1) / 2); break;
    case 2: x = mWidth - TextWidth(c_str(mLabel)) - mStyle.padX - 1; break;
  }

  return x;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvTextBox::textTop(void)
{
  int16_t   y;

  switch ((mStyle.align >> 2) & 3)
  {
    case 0: y = mStyle.padY; break;
    case 1: y = ((mHeight - TextHeight()) / 2); break;
    case 2:
    case 3: y = mHeight - TextHeight() - mStyle.padY - 1; break;
  }

  return y;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvTextBox::filter(uint8_t C)
{
  if (mOnFilter != nullptr)
    return mOnFilter(this, C);

  return C;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::drawEvent(void)
{
  if (mSelectCount > 0)
  {
    char      *str = (char *)c_str(mLabel);
    int16_t   x = textLeft() + TextWidth(str, mStyle.font, mSelectBegin);
    int16_t   w = TextWidth(str + mSelectBegin, mStyle.font, mSelectCount);

    FillRectangle(x + mPanelOffsetX, textTop(), w, TextHeight(), mColorTextSelect.Get());
  }

  DrawText(mPanelOffsetX, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::resizeEvent(void)
{
  SetMoveCursor();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::refreshEvent(void)
{
  if (IsModifiedText())
    SetMoveCursor();

  if (IsEditedText())
  {
    if (mOnChange != nullptr)
      mOnChange(this, mLabel);

    SetMoveCursor();
    Modified();
  }

  if (IsMoveCursor())
    moveCursor();

  ClrEditedText();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::setKbdFocusEvent(void)
{
  Cursor->Show();
  BdColor(mColorBdFocus.Raw());
  mAlign = mStyle.align;
  TextAlign((mAlign & ~3) | ALIGNMENT_LOCK);

  if (mOnSetKbdFocus != nullptr)
    mOnSetKbdFocus(this);

  if (SelectAllOnSetKbdFocus)
    SelectAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::lostKbdFocusEvent(void)
{
  Unselect();
  Cursor->Hide();
  BdColor(mColorBd.Raw());
  TextAlign(mAlign);
  mCursorIndex = 0;
  mPanelOffsetX = 0;

  if (mOnLostKbdFocus != nullptr)
    mOnLostKbdFocus(this);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      ClrTouchBox();
      SetMoveToWord();
      Cursor->SetStyle(CURSOR_SOLID);
      Touch->repeatTimer = 200;
      Touch->repeatDelay = 40;
      Touch->event = 0;
      break;

    case TOUCH_HOLD:
      SetKbdFocus();
      SetTouchBox();

    case TOUCH_MOVE:
      if (IsTouchBox())
      {
        if (Touch->x > mStyle.padX && Touch->x < (mWidth - mStyle.padX))
          moveToChar(Touch);

        ClrMoveToWord();
        Touch->event = 0;
      }
      break;

    case TOUCH_REPEAT:
      if (Touch->x < 0 && mCursorIndex > 0)
      {
        mCursorIndex--;
        SetMoveCursor();
        Modified();
      }
      else if (Touch->x > mWidth && mCursorIndex < mLabel.length())
      {
        mCursorIndex++;
        SetMoveCursor();
        Modified();
      }

      Touch->event = 0;
      break;

    case TOUCH_DOUBLE:
      selectWord(Touch);
      ClrMoveToWord();
      Touch->event = 0;
      break;

    case TOUCH_END:
      if (IsMoveToWord())
        moveToWord(Touch);

      SetKbdFocus();

    case TOUCH_CANCEL:
      Cursor->SetStyle(CURSOR_SMOOTH);
      ClrTouchBox();
      moveCursor();
      Touch->event = 0;
      break;
  }
}
