
#include    <EvGUI.h>
#include    <include/system/EvKeyboard.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static EvKeyboard     *sKeyboard[2] = {&Keyboard800x220, &Keyboard480x230};

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
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Keyboard. If nullptr, the default tag name is "EvKbd".
 * @param[in]  State   The initial state of the Keyboard. Default is set to SYSTEM_OBJ.
 *
 * @return     EvKbd address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKbd       *EvKbd::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvKbd *)EvObj::TryCreate(new EvKbd(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvKbd" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKbd::EvKbd(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvSideBar(Left, Top, Width, Height, Disp, Tag, State),
  mKey(0),
  mKeyId(0),
  mLayout(0),
  mPrevKey(0),
  mShiftKey(0),
  FocusObj(nullptr)
{
  if ((mOverKey = EvLabel::Create(0, 0, (uint16_t)0, 0, this, "OverKey", DISABLED_OBJ | FLOAT_OBJ | SYSTEM_OBJ)) == nullptr)
  {
    abortCreate();
    return;
  }

  mOverKey->TextPadding(0, 7);
  mOverKey->BdWidth(24);

  TouchMax(2);
  SetOnTouch(nullptr);
  SetOwner(Disp); // Owner is not yet initialized
  SetKeyboard(Disp->Orientation & 2 ? 1 : 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::Open(void)
{
  EvSideBar::Open();
  ToFront();
  Enable();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::Close(void)
{
  EvSideBar::Close();
  mKey = mPrevKey = 0;
  mOverKey->Hide();
  Disable();
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

void        EvKbd::SetKeyboard(int Keyboard, bool OpenState)
{
  if (Keyboard < 2)
    SetKeyboard(sKeyboard[Keyboard], OpenState);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::SetKeyboard(EvKeyboard *Keyboard, bool OpenState)
{
  mKb = Keyboard;
  mKey = mPrevKey = 0;
  mOverKey->Hide();
  BgColor(mKb->bgColor);
  BdRadius(mKb->bdRadius);
  ReSize(mKb->width, mKb->height);
  Setup(SIDEBAR_BOTTOM, OpenState);
  mOverKey->BdColor(mKb->overKeyBdColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::SetKeyboard(EvKeyboard *Landscape, EvKeyboard *Portrait)
{
  if (Landscape != nullptr && Portrait != nullptr)
  {
    sKeyboard[0] = Landscape;
    sKeyboard[1] = Portrait;
    SetKeyboard(Disp->Orientation & 2 ? 1 : 0);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/// @copydoc EvButton::SetOnTouch()

void        EvKbd::SetOnTouch(void (*OnTouch)(EvKbd *Sender, const EvTouchEvent *Touch))
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
    const char  *label = nullptr;

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

void        EvKbd::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

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

        if (mKey > ' ' && mKey <= '~')
        {
          writeKey(!mShiftKey ? mKey : toupper(mKey));
          mOverKey->Hide();
        }

        mKeyId = Touch->id;
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

    case TOUCH_HOLD:
      Touch->repeatDelay = 100;

    case TOUCH_REPEAT:
      if (mKey == ' ' || mKey == '\b')
        writeKey(mKey);

      Touch->event = 0;
      break;

    case TOUCH_END:
      if (Touch->id != mKeyId)
        break;

      switch (mPrevKey = mKey)
      {
        case 0: break;
        case CLOSE_KEY: LostKbdFocus(); break;
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
      mKey = mKeyId = mPrevKey = 0;
      Touch->event = 0;
      mOverKey->Hide();
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKbd::writeKey(uint8_t Key)
{
  int16_t   newLayout = GetKbdFocus()->WriteKey(Key, mLayout, mShiftKey, mDoubleTouch);

  if (newLayout != -1)
    SetLayout(newLayout);
}

