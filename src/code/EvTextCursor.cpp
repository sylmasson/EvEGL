
#include    <EvGUI.h>

#define     CURSOR_COLOR    RGB555(  0,   0, 160)

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
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Text Cursor. If NULL, the default tag name is "EvTextCursor".
 * @param[in]  State   The initial state of the Text Cursor. Default is set to VISIBLE_OBJ.
 *
 * @return     EvTextCursor address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextCursor    *EvTextCursor::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvTextCursor *)EvObj::TryCreate(new EvTextCursor(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTextCursor" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTextCursor::EvTextCursor(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mCursorState = CURSOR_SOLID;
  SetOpacity(128 << 1);
  Style(CURSOR_SMOOTH);
  BdShape(ROUND_CORNERS);
  BgColor(CURSOR_COLOR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvTextCursor::Style(void)
{
  return mCursorState & 3;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvTextCursor::Style(uint8_t CursorStyle)
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
