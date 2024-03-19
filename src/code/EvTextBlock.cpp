
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard TextBlock.
 * 
 * A new standard TextBlock is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the TextBlock.
 * @param[in]  Top     The top position of the TextBlock.
 * @param[in]  Width   The width of the TextBlock.
 * @param[in]  Height  The height of the TextBlock.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the TextBlock. If nullptr, the default tag name is "EvTextBlock".
 * @param[in]  State   The initial state of the TextBlock. Default is set to VISIBLE_OBJ.
 *
 * @return     EvTextBlock address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBlock *EvTextBlock::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTextBlock *)EvObj::TryCreate(new EvTextBlock(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTextBlock" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBlock::EvTextBlock(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvScrollBox(Left, Top, Width, Height, Disp, Tag, State)
{
  mWrapText = true;
  mLineSpacing = 0;
  mLinesCount = 0;
  mLines = nullptr;
  mMaxWidth = 0;
  TextClear();
  TextFont(24);
  TextPadding(5, 0);
  TextAlign(LEFT_CENTER);
  TextColor(RGB555(0, 0, 0));
  BgColor(RGB555(255, 255, 255));
  SetOnTouch(nullptr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvTextBlock::WrapText(void)
{
  return mWrapText;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvTextBlock::WrapText(bool Wrap)
{
  if (Wrap != mWrapText)
  {
    mScrollBarX->SetValue(0);
    mScrollBarY->SetValue(0);
    mWrapText = Wrap;
    ModifiedText();
  }

  return mWrapText;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvTextBlock::LineSpacing(void)
{
  return mLineSpacing;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvTextBlock::LineSpacing(int Pixel)
{
  if (Pixel < -100)
    Pixel = -100;
  else if (Pixel > 100)
    Pixel = 100;

  if (Pixel != mLineSpacing)
  {
    mLineSpacing = Pixel;
    ModifiedText();
  }

  return mLineSpacing;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvTextBlock::LineSpacing(float Ratio)
{
  if (Ratio < 0.5)
    Ratio = 0.5;
  else if (Ratio > 2.0)
    Ratio = 2.0;

  return LineSpacing((int)(TextHeight() * (Ratio - 1.0)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String      EvTextBlock::GetLine(uint16_t Line)
{
  if (IsModifiedText())
    parseTextAsLines();

  return (Line < mLinesCount) ? getLine(Line) : "";
}

/// @copydoc EvButton::SetOnTouch()

void        EvTextBlock::SetOnTouch(void (*OnTouch)(EvTextBlock *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBlock &EvTextBlock::operator+=(const char C)
{
  write((const uint8_t *)&C, 1);
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBlock &EvTextBlock::operator+=(const char *Str)
{
  write((const uint8_t *)Str, strlen(Str));
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextBlock &EvTextBlock::operator+=(const String &Str)
{
  write((const uint8_t *)c_str(Str), Str.length());
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t      EvTextBlock::write(uint8_t C)
{
  return write((const uint8_t *)&C, 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t      EvTextBlock::write(const uint8_t *Buffer, size_t Count)
{
  mLabel.reserve(mLabel.length() + Count);

  for (size_t i = 0; i < Count; i++)
    mLabel.concat((const char)*Buffer++);

  ModifiedText();
  return Count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBlock::drawEvent(void)
{
  int       y, line, lineHeight;

  if ((lineHeight = TextHeight() + mLineSpacing) < mMinLineHeight)
    lineHeight = mMinLineHeight;

  line = -mOffsetY / lineHeight;

  for (y = mOffsetY + (line * lineHeight) + mStyle.padY; y < mHeight && line < mLinesCount; y += lineHeight, line++)
    if (line >= 0)
      DrawText(mOffsetX, y, mPageWidth, lineHeight, c_str(getLine(line)), mStyle.color, mStyle.font, (mStyle.align & 3) | 4, mStyle.padX, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBlock::resizeEvent(void)
{
  ModifiedText();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBlock::refreshEvent(void)
{
  if (IsModifiedText())
    parseTextAsLines();

  EvScrollBox::refreshEvent();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextBlock::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  EvScrollBox::touchEvent(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvTextBlock::parseTextAsLines(void)
{
  SubString *line;
  bool      startLine;
  int       lineHeight;
  uint16_t  index, length;
  int32_t   width, widthLimit;

  if (mLines)
    free(mLines);

  if (!mWrapText)
    widthLimit = 0x7FFFFFFF;
  else if ((widthLimit = mWidth - (mStyle.padX * 2)) < 0)
    widthLimit = 0;

  width = 0;
  mMaxWidth = 0;
  mLines = nullptr;
  mLinesCount = 0;
  startLine = true;
  length = mLabel.length();

  for (index = 0; index < length; )
  {
    int16_t   c;
    uint16_t  ind;
    uint16_t  spCount, wdCount;
    int32_t   spWidth, wdWidth;

    for (spCount = spWidth = 0, ind = index; ind < length && (c = mLabel.charAt(ind)) == ' '; spCount++, ind++)
      ;

    if (spCount)
      spWidth = spCount * TextWidth(' ', mStyle.font);

    for (wdCount = wdWidth = 0, ind = index + spCount, c = -1; ind < length && (c = mLabel.charAt(ind)) != ' ' && c != '\n'; wdCount++, ind++)
      wdWidth += TextWidth(c, mStyle.font);

    if (startLine || width + spWidth + wdWidth > widthLimit)
    {
      if ((mLines = (SubString *)realloc(mLines, ++mLinesCount * sizeof(SubString))) == nullptr)
      {
        mLinesCount = 0;
        mMaxWidth = 0;
        return false;
      }

      line = &mLines[mLinesCount - 1];

      if (startLine && (mStyle.align & 3) == 0)
      {
        width = spWidth + wdWidth;
        line->from = index;
      }
      else
      {
        width = wdWidth;
        line->from = index + spCount;
      }

      index += spCount + wdCount;
      startLine = false;
    }
    else if (wdWidth > 0 || c == '\n' || (mStyle.align & 3) == 0)
    {
      width += spWidth + wdWidth;
      index += spCount + wdCount;
    }

    if (width > mMaxWidth)
      mMaxWidth = width;

    if (c == '\n')
    {
      startLine = true;
      index++;
    }

    line->to = index;
  }

  if ((width = mMaxWidth + (mStyle.padX * 2)) < mWidth)
    width = mWidth;

  if ((lineHeight = TextHeight() + mLineSpacing) < mMinLineHeight)
    lineHeight = mMinLineHeight;

  SetPageSize(width, (mLinesCount * lineHeight) + (mStyle.padY * 2));
  mStatus &= ~MODIF_TEXT_OBJ;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String      EvTextBlock::getLine(uint16_t Line)
{
  String    str;
  SubString *line = &mLines[Line];

  str = Text.substring(line->from, line->to);
  return str;
}
