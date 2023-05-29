
#include    <EvGUI.h>
#include    <include/system/EvKeyboard.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//static EvKeyboard     *sKeyboard[2] = {&Keyboard480x230, &Keyboard1024x276};
static EvKeyboard     *sKeyboard[2] = {&Keyboard1077x276, &Keyboard600x252};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the virtual Keyboard "EvKbd".
 * 
 * A new virtual Keyboard is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the Keyboard.
 * @param[in]  Top     The top position of the Keyboard.
 * @param[in]  Width   The width of the Keyboard.
 * @param[in]  Height  The height of the Keyboard.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Keyboard. If NULL, the default tag name is "EvKbd".
 * @param[in]  State   The initial state of the Keyboard. Default is set to SYSTEM_OBJ.
 *
 * @return     EvKbd address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKbd       *EvKbd::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvKbd *)EvObj::TryCreate(new EvKbd(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvKbd" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKbd::EvKbd(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  mKey = 0;
  mLayout = 0;
  mPrevKey = 0;
  mShiftKey = 0;
  FocusObj = NULL;
  mKinMotion.Setup(200);
  SetOnTouch(NULL);
  TouchMax(2);
  Disable();

  if ((mOverKey = EvLabel::Create(0, 0, 0, 0, this, "KbdOverlay", DISABLED_OBJ | FLOAT_OBJ | SYSTEM_OBJ)) == NULL)
    Abort();
  else
  {
    mOverKey->TextPadding(0, 7);
    mOverKey->BdColor(OVER_KEY_BD_COLOR);
    mOverKey->BdWidth(24);
    SetKeyboard(Disp->Orientation & 2 ? 1 : 0);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::Open(void)
{
  if (!IsEnabled())
  {
    mKinMotion.Start(mKb->top - mTop);
    ToFront();
    Enable();
    Show();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::Close(void)
{
  if (IsEnabled())
  {
    Disable();
    mOverKey->Hide();
    mKinMotion.Start(Disp->Height() - mTop + 1);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::SetLayout(uint8_t Layout)
{
  uint8_t   layout = Layout & 0x3F;
  uint8_t   shiftkey = Layout >> 6;

  if (layout >= mKb->layoutCount)
    layout = LAYOUT_ALPHA;

  if (mLayout != layout || mShiftKey != shiftkey)
  {
    mShiftKey = shiftkey;
    mLayout = layout;
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::SetKeyboard(int Keyboard)
{
  SetKeyboard(sKeyboard[Keyboard]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::SetKeyboard(EvKeyboard *Keyboard)
{
  mKb = Keyboard;
  mKinMotion.Stop();
  BgColor(mKb->bgColor);
  BdRadius(mKb->bdRadius);
  ReSize(mKb->width, mKb->height);

  mKb->top = (int16_t)Disp->Height() - mKb->height;
  mKb->left = ((int16_t)Disp->Width() - (int16_t)mKb->width) / 2;
  MoveTo(mKb->left, IsEnabled() ? mKb->top : Disp->Height());
//  MoveTo(mKb->left, Disp->Height() - (IsEnabled() ? mKb->height : 0));
//  MoveTo(mKb->left, Disp->Height() - (IsEnabled() ? mKb->height : 0));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/// @copydoc EvButton::SetOnTouch()

void        EvKbd::SetOnTouch(void (*OnTouch)(EvKbd *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::drawEvent(void)
{
  uint16_t    i, c, color;
  char        str[2] = "?";
  const EvKbdMapping  *map;
  const EvKeyStyle    *style;
  const EvKbdLayout   *layout = &mKb->layout[mLayout];

  if (mKb->keyShadow > 0)
  {
    Disp->ColorA(100);

    for (i = 0, map = layout->kbdMap; i < layout->keyCount; i++, map++)
    {
      style = &layout->keyStyle[map->style];
      FillRectangle(map->left, map->top, style->width, style->height + mKb->keyShadow, RGB555(0, 0, 0), style->radius);
    }

    Disp->ColorA(255);
  }

  for (i = 0, map = layout->kbdMap; i < layout->keyCount; i++, map++)
  {
    Disp->TagValue(i + 1);
    c = layout->keyChar[i];
    style = &layout->keyStyle[map->style];
    color = (c == SHIFT_KEY && mShiftKey) ? style->colorDown : style->colorUp;
    FillRectangle(map->left, map->top, style->width, style->height, color, style->radius);
  }

  Disp->TagMask(0);

  for (i = 0, map = layout->kbdMap; i < layout->keyCount; i++, map++)
  {
    const char  *label = NULL;

    c = layout->keyChar[i];
    style = &layout->keyStyle[map->style];

    if (c > ' ' && c <= '~')
    {
      str[0] = (!mShiftKey) ? c : toupper(c);
      label = str;
    }
    else if (c < 16)
    {
      if (c == SHIFT_KEY && mShiftKey > 1)
        c = 0;

      label = layout->keyLabel[c];
    }

    if (label)
      DrawText(map->left, map->top, style->width, style->height, label, style->textColorUp, map->font, CENTER);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::refreshEvent(void)
{
  if (mKinMotion.IsEnabled())
    MoveTo(mKb->left, mTop + mKinMotion.Value());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);

  const EvKbdLayout *layout = &mKb->layout[mLayout];

  switch (Touch->event)
  {
    case TOUCH_START:
      if (Touch->tag > layout->keyCount)
        mKey = 0;
      else 
      {
        uint16_t            i;
        const EvKbdMapping  *map;
        const EvKeyStyle    *style;

        i = Touch->tag - 1;
        map = &layout->kbdMap[i];
        mKey = layout->keyChar[i];
        style = &layout->keyStyle[map->style];

        if (mKey > ' ' && mKey <= '~')
        {
          char str[2] = "?";

          str[0] = !mShiftKey ? mKey : toupper(mKey);
          mOverKey->MoveTo(map->left, map->top - mKb->overKeyExtend);
          mOverKey->ReSize(style->width, style->height + mKb->overKeyExtend + mKb->keyShadow);
          mOverKey->TextFont(mKb->overKeyFont);
          mOverKey->TextAlign(CENTER_TOP);
          mOverKey->TextLabel(str);
        }
        else
        {
          mOverKey->MoveTo(map->left, map->top);
          mOverKey->ReSize(style->width, style->height + mKb->keyShadow);
          mOverKey->TextFont(map->font);
          mOverKey->TextAlign(CENTER);
          mOverKey->TextLabel(mKey < 16 ? layout->keyLabel[mKey] : "");
        }

        mOverKey->TextColor(style->textColorDown);
        mOverKey->BgColor(style->colorDown);
        mOverKey->BdRadius(style->radius);
        mOverKey->ToFront();
        mOverKey->Show();
      }

      mDoubleTouch = false;
      Touch->event = 0;
      break;

    case TOUCH_DOUBLE:
      if (mPrevKey == mKey)
        mDoubleTouch = true;

      Touch->event = 0;
      break;

    case TOUCH_REPEAT:
    case TOUCH_HOLD:
      if (mKey == ' ' || mKey == '\b')
        writeKey(mKey);

      Touch->event = 0;
      break;

    case TOUCH_END:
      switch (mPrevKey = mKey)
      {
        case 0: break;
        case CLOSE_KEY: LostFocus(); break;
        case NUM_KEY: SetLayout(LAYOUT_NUM1); break;
        case SYM2_KEY: SetLayout(LAYOUT_NUM1); break;
        case SYM1_KEY: SetLayout(LAYOUT_NUM2); break;
        case ALPHA_KEY: SetLayout(LAYOUT_ALPHA); break;
        case SHIFT_KEY: SetLayout(mDoubleTouch ? LAYOUT_CAPS : (!mShiftKey ? LAYOUT_SHIFT : LAYOUT_ALPHA)); break;
        default: writeKey(!mShiftKey ? mKey : toupper(mKey)); mKey = 0; break;
      }

      Touch->event = 0;
      mOverKey->Hide();
      break;

    case TOUCH_CANCEL:
      mKey = mPrevKey = 0;
      Touch->event = 0;
      mOverKey->Hide();
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::writeKey(uint8_t Key)
{
  int16_t   newLayout = GetFocus()->WriteKey(Key, mLayout, mShiftKey, mDoubleTouch);

  if (newLayout != -1)
    SetLayout(newLayout);
}

