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

extern const EvFont         *Menio20;

const char * const EvTerminal::TypeName = "EvTerminal";

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
  return !Dest ? nullptr : (EvTerminal *)EvObj::TryCreate(new EvTerminal(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTerminal::EvTerminal(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvTextBlock(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mScrolling(0)
{
  if (!(Cursor = EvTextCursor::Create(0, 0, 0, 0, this, nullptr, VISIBLE_DIS_OBJ)))
  {
    abortCreate();
    return;
  }

  WrapText(false);
  TextFont(17);
  TextPadding(7, 5);
  TextColor(CL_TERMINAL_TEXT);
  BgColor(CL_TERMINAL_BG);
  BdShape(FIXED_CORNERS);
  SetBufferSize(4 * 1024);
  Disp->UnloadFont(17);
  Disp->LoadFont(Menio20, 17);
  Cursor->SetStyle(CURSOR_SMOOTH);
  Cursor->BgColor(CL_TERMINAL_TEXT);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTerminal::refreshEvent(void)
{
  if (IsModifiedText())
  {
    int16_t   lineHeight;
    int16_t   textHeight = TextHeight();
    int16_t   x, y, cursorWidth = TextCursorWidth();

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

