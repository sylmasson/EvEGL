
#include    <EvGUI.h>

#define     BG_COLOR        RGB555(255, 255, 255)
#define     BD_COLOR        RGB555(210, 210, 210)
#define     FOCUS_COLOR     RGB555(  0,  51, 153)
#define     TEXT_COLOR      RGB555(  0,   0,   0)
#define     CURSOR_COLOR    RGB555(  0,  51, 153)
#define     SELECT_COLOR    RGB555(179, 204, 255)

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
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the TextBox. If NULL, the default tag name is "EvTextBox".
 * @param[in]  State   The initial state of the TextBox. Default is set to VISIBLE_OBJ.
 *
 * @return     EvTextBox address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox   *EvTextBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvTextBox *)EvObj::TryCreate(new EvTextBox(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTextBox" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBox::EvTextBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  mFlags = 0;
  TextClear();
  TextFont(26);
  TextPadding(10, 0);
  TextAlign(LEFT_CENTER);
  TextColor(TEXT_COLOR);
  BgColor(BG_COLOR);
  BdColor(BD_COLOR);
  BdShape(FIXED_CORNERS);
  BdWidth(24);
  SetOnTouch(NULL);
  SetOnChange(NULL);
  SetOnFilter(NULL);
  SetOnFocus(NULL, NULL);
  SelectAllOnFocus = false;
  mCursorIndex = 0;
  mMaxLength = 0;

  if ((Cursor = EvTextCursor::Create(0, 0, 0, 0, this, NULL, DISABLED_OBJ | FIXED_OBJ)) == NULL)
    Abort();
  else
  {
    Cursor->BgColor(CURSOR_COLOR);
    Cursor->Style(CURSOR_SMOOTH);
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
  SetEditedText();
}

/// @copydoc EvObj::TextLabel()

void        EvTextBox::TextLabel(const char *Label)
{
  TextClear();
  write((const uint8_t *)Label, strlen(Label));
}

/// @copydoc EvObj::TextLabel()

void        EvTextBox::TextLabel(const String &Label)
{
  TextClear();
  write((const uint8_t *)c_str(Label), Label.length());
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

/// @copydoc EvButton::SetOnTouch()

void        EvTextBox::SetOnTouch(void (*OnTouch)(EvTextBox *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvTextBox::SetOnChange(void (*OnChange)(EvTextBox *Sender, const String &Str))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetOnFilter(int (*OnFilter)(EvTextBox *Sender, const uint8_t C))
{
  mOnFilter = OnFilter;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::SetOnFocus(void (*OnSetFocus)(EvTextBox *Sender), void (*OnLostFocus)(EvTextBox *Sender))
{
  mOnSetFocus = OnSetFocus;
  mOnLostFocus = OnLostFocus;
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

  Cursor->Style(CURSOR_SOLID);

  for (size_t i = 0; i < Count; i++)
    if ((c = filter(*Buffer++)) == '\b')
    {
      if (mSelectCount > 0)
      {
        mLabel.remove(mCursorIndex = mSelectBegin, mSelectCount);
        SetEditedText();
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

  Cursor->Style(CURSOR_SMOOTH);
  return Count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::selectWord(EvTouchEvent *Touch)
{
  int16_t   c, x = textLeft();
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

void        EvTextBox::moveToWord(EvTouchEvent *Touch)
{
  int16_t   c, x = textLeft();
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

void        EvTextBox::moveToChar(EvTouchEvent *Touch)
{
  int16_t   x = textLeft();
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
  int16_t   i, x, y;
  int16_t   textHeight = TextHeight();
  int16_t   cursorWidth = TextCursorWidth();
  int16_t   cursorHeight = textHeight;

  if (IsTouchBox())
  { // 50% oversize cursor
    cursorWidth += ((cursorWidth == 1) ? 1 : cursorWidth / 2);
    cursorHeight = (textHeight * 3) / 2;
  }

  for (i = 0, x = textLeft(); i < mCursorIndex; i++)
    x += TextWidth(mLabel.charAt(i));

  y = textTop() - ((cursorHeight - textHeight) / 2);

  Cursor->MoveTo(x, y);
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
  if (mOnFilter != NULL)
    return (*mOnFilter)(this, C);

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

    FillRectangle(x, textTop(), w, TextHeight(), SELECT_COLOR);
  }

  DrawText(0, 0, mWidth, mHeight, c_str(Label));
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
    if (mOnChange != NULL)
      (*mOnChange)(this, mLabel);

    SetMoveCursor();
    Modified();
  }

  if (IsMoveCursor())
    moveCursor();

  ClrEditedText();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::setFocusEvent(void)
{
  Cursor->Show();
  BdColor(FOCUS_COLOR);

  if (mOnSetFocus != NULL)
    (*mOnSetFocus)(this);

  if (SelectAllOnFocus)
    SelectAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::lostFocusEvent(void)
{
  Unselect();
  Cursor->Hide();
  BdColor(BD_COLOR);

  if (mOnLostFocus != NULL)
    (*mOnLostFocus)(this);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBox::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      ClrTouchBox();
      SetMoveToWord();
      Cursor->Style(CURSOR_SOLID);
      Touch->repeatTimer = 200;
      Touch->event = 0;
      break;

    case TOUCH_HOLD:
      SetFocus();
      SetTouchBox();

    case TOUCH_MOVE:
      if (IsTouchBox())
      {
        moveToChar(Touch);
        ClrMoveToWord();
        Touch->event = 0;
      }
      break;

    case TOUCH_DOUBLE:
      selectWord(Touch);
      ClrMoveToWord();
      Touch->event = 0;
      break;

    case TOUCH_END:
      if (IsMoveToWord())
        moveToWord(Touch);

      SetFocus();

    case TOUCH_CANCEL:
      Cursor->Style(CURSOR_SMOOTH);
      ClrTouchBox();
      moveCursor();
      Touch->event = 0;
      break;
  }
}
