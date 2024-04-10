
#include    <EvGUI.h>

#define     TEXT_COLOR      RGB555(  0,   0,   0)
#define     BG_COLOR        RGB555(255, 255, 255)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const EvFont         *Menio20;

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard Terminal.
 * 
 * A new standard Terminal is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the Terminal.
 * @param[in]  Top     The top position of the Terminal.
 * @param[in]  Width   The width of the Terminal.
 * @param[in]  Height  The height of the Terminal.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Terminal. If nullptr, the default tag name is "EvTerminal".
 * @param[in]  State   The initial state of the Terminal. Default is set to VISIBLE_OBJ.
 *
 * @return     EvTerminal address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTerminal  *EvTerminal::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTerminal *)EvObj::TryCreate(new EvTerminal(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTerminal" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTerminal::EvTerminal(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvTextBlock(Left, Top, Width, Height, Disp, Tag, State),
  mScrolling(0),
  MaxBufferSize(4096)
{
  WrapText(false);
  TextFont(17);
  TextPadding(7, 5);
  TextAlign(LEFT_CENTER);
  TextColor(TEXT_COLOR);
  BgColor(BG_COLOR);

  if (!(Cursor = EvTextCursor::Create(0, 0, 0, 0, this, nullptr, VISIBLE_DIS_OBJ)))
    abortCreate();
  else
  {
    Disp->UnloadFont(17);
    Disp->LoadFont(Menio20, 17);
    Cursor->SetStyle(CURSOR_SMOOTH);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTerminal::refreshEvent(void)
{
  if (IsModifiedText())
  {
    int16_t   lineHeight;
    int16_t   textHeight = TextHeight();
    int16_t   x, y, cursorWidth = TextCursorWidth();
    int16_t   extraLength = mLabel.length() - MaxBufferSize;

    if (extraLength > 0)
    {
      int16_t i = mLabel.indexOf('\n', extraLength);
      mLabel.remove(0, i < 0 ? extraLength : i + 1);
    }

    TextAlign(LEFT_CENTER);
    parseTextAsLines();

    if ((lineHeight = textHeight + mLineSpacing) < mMinLineHeight)
      lineHeight = mMinLineHeight;

    if ((y = mLinesCount - 1) < 0)
      x = y = 0;
    else
    {
      x = TextWidth(c_str(getLine(y)));
      y *= lineHeight;
    }

    Cursor->MoveTo(x + mStyle.padX, y + mStyle.padY + ((lineHeight - textHeight) >> 1));
    Cursor->ReSize(cursorWidth, textHeight);
    Cursor->BgColor(mStyle.color);

    if (!mScrolling || (mScrolling == 1 && !(mScrollBarY->Scrolling() & SCROLL_MOVING)))
    {
      mScrollBarX->SetValue(0, false);
      mScrollBarY->SetValue(32767, false);
      mScrolling = 0;
    }
  }
  else if (mScrolling == 2 && !(mScrollBarY->Scrolling() & SCROLL_TIMER))
  {
    mScrollBarX->ToBeginning();
    mScrollBarY->ToTheEnd();
    mScrolling = 1;
  }

  EvScrollBox::refreshEvent();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTerminal::touchEvent(const EvTouchEvent *Touch)
{
  if (Touch->event == TOUCH_MOVE)
    mScrolling = 2;

  EvTextBlock::touchEvent(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

