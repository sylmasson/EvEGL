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

const char * const EvTextCursor::TypeName = "EvTextCursor";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard Text Cursor.
 * 
 * A new standard Text Cursor is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the Text Cursor.
 * @param[in]  Top     The top position of the Text Cursor.
 * @param[in]  Width   The width of the Text Cursor.
 * @param[in]  Height  The height of the Text Cursor.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Text Cursor. If nullptr, the default tag name is "EvTextCursor".
 * @param[in]  State   The initial state of the Text Cursor. Default is set to VISIBLE_OBJ.
 *
 * @return     EvTextCursor address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextCursor    *EvTextCursor::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTextCursor *)EvObj::TryCreate(new EvTextCursor(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextCursor::EvTextCursor(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mCursorState(CURSOR_SOLID),
  mCursorTimer(millis())
{
  SetOpacity(128 << 1);
  SetStyle(CURSOR_SMOOTH);
  BdShape(ROUND_CORNERS);
  BgColor(CL_TEXTCURSOR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvTextCursor::Style(void)
{
  return mCursorState & 3;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvTextCursor::SetStyle(uint8_t CursorStyle)
{
  uint8_t   style = CursorStyle & 3;
  uint8_t   state = mCursorState & 3;

  if (style != state)
    switch (style)
    {
      case CURSOR_NONE:
        mCursorState = CURSOR_NONE;
        SetOpacity(0);
        break;

      case CURSOR_SOLID:
        mCursorState = CURSOR_SOLID;
        SetOpacity(128 << 1);
        break;

      case CURSOR_BLINK:
      case CURSOR_SMOOTH:
        if (state == CURSOR_NONE)
        {
          mCursorState = style;
          mCursorTimer = millis();
        }
        else if (state == CURSOR_SOLID)
        {
          mCursorState = (2 << 2) + style;
          mCursorTimer = millis();
        }
        break;
    }

  return mCursorState & 3;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTextCursor::refreshEvent(void)
{
  uint16_t  msec, timer;
  uint8_t   style = mCursorState & 3;

  if (style == CURSOR_NONE || style == CURSOR_SOLID)
    return;

  msec = millis();
  timer = msec - mCursorTimer;

  switch ((mCursorState >> 2) & 3)
  {
    case 0:
    case 2:
      if (timer >= 500 - 128)
      {
        mCursorTimer = msec;
        mCursorState += 4;
      }
      break;

    case 1:
      if (timer >= 128)
      {
        mCursorTimer = msec;
        mCursorState += 4;
      }

      SetOpacity(style == CURSOR_SMOOTH ? (timer << 1) : 0);
      break;

    case 3:
      if (timer >= 128)
      {
        mCursorTimer = msec;
        mCursorState &= 3;
        timer = 128; 
      }

      SetOpacity(style == CURSOR_SMOOTH ? ((128 - timer) << 1) : (128 << 1));
      break;
  }
}
