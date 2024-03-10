
#include    <EvGUI.h>

#define     COLOR_BAR       RGB555( 92,  92,  92)
#define     COLOR_TOUCH     RGB555( 30,  30,  30)

#define     KNOB_MIN        (24 << 4)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard ScrollBar.
 * 
 * A new ScrollBar is created at the specified size and relative position
 * of its owner Dest.
 *
 * @note       If the height is greater than the width, the ScrollBar works vertically.
 *             
 * @param[in]  Left    The left position of the ScrollBar.
 * @param[in]  Top     The top position of the ScrollBar.
 * @param[in]  Width   The width of the ScrollBar.
 * @param[in]  Height  The height of the ScrollBar.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the ScrollBar. If nullptr, the default tag name is "EvScrollBar".
 * @param[in]  State   The initial state of the ScrollBar. Default is set to VISIBLE_OBJ.
 *
 * @return     EvScrollBar address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvScrollBar *EvScrollBar::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvScrollBar *)EvObj::TryCreate(new EvScrollBar(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvScrollBar" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvScrollBar::EvScrollBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mValue = 0;
  mTimer = 0;
  mPageSize = 0;
  mViewSize = 0;
  mScrolling = 0;
  mTouchBar = false;
  mScrollBarSync = nullptr;
  mKinScroll.Setup(48);
  BdShape(ROUND_CORNERS);
  SetBarStyle(SCROLL_BAR_AUTO);
  SetBarColor(COLOR_BAR, COLOR_TOUCH);
  SetOnChange(nullptr);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the position value of the ScrollBar.
 * 
 * @return     The position value of the ScrollBar.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvScrollBar::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Wake up the ScrollBar.
 * 
 * This function wakes up the ScrollBar. This means that the ScrollBar becomes visible
 * if it is not. This function has no effect if the operating mode is not SCROLL_BAR_AUTO.
 * 
 * @see        SetBarStyle()
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::WakeUp(void)
{
  wakeup();

  if (mScrollBarSync != nullptr)
    mScrollBarSync->wakeup();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if the knob of the ScrollBar is touched.
 * 
 * @return     true if touched.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvScrollBar::TouchBar(void)
{
  return mTouchBar;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the status of ScrollBar.
 * 
 * @return     Status of ScrollBar.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


int16_t     EvScrollBar::Scrolling(void)
{
  return mScrolling;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Smooth scrolling to the beginning.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::ToBeginning(bool Wakeup)
{
  if (mValue > 0)
  {
    uint16_t  size = mPageSize - mViewSize;

    mKinScroll.Start(((-mValue * size)) / (size * 4));
    mTouchBar = false;
    Modified();

    if (Wakeup)
      WakeUp();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Smooth scrolling to the end.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::ToTheEnd(bool Wakeup)
{
  if (mValue != mPageSize - mViewSize)
  {
    uint16_t  size = mPageSize - mViewSize;

    mKinScroll.Start(((size - mValue) * size) / (size * 4));
    mTouchBar = false;
    Modified();

    if (Wakeup)
      WakeUp();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the position value of the ScrollBar.
 * 
 * The value of the range is from 0 to (PageSize - ViewSize). See SetPageSize().
 * 
 * @param[in]  Value   The position value of the ScrollBar.
 * @param[in]  Wakeup  On change, WakeUp the ScrollBar if true.
 * 
 * @return     true if the position value is changed, otherwise returns false.
 * 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvScrollBar::SetValue(int16_t Value, bool Wakeup)
{
  if (Value < 0)
    Value = 0;
  else if (Value > mPageSize - mViewSize)
    Value = mPageSize - mViewSize;

  if (mValue == Value)
    return false;

  mScrolling |= SCROLL_MOVING;
  mValue = Value;
  Modified();

  if (Wakeup)
    WakeUp();

  if (mOnChange != nullptr)
    mOnChange(this, mValue);

  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets another ScrollBar to synchronize with this one on WakeUp() call.
 * 
 * @param[in]  ScrollBarSync  The address of the ScrollBar to synchronize.
 * 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::SetScrollBarSync(EvScrollBar *ScrollBarSync)
{
  mScrollBarSync = ScrollBarSync;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the page size and view size of the ScrollBar.
 * 
 * This function allows you to set the range that the value of the ScrollBar can take.
 * The ScrollBar works only when the PageSize is larger than the ViewSize. In this
 * condition, the range of the value is from 0 to (PageSize - ViewSize). Otherwise, the
 * value is always 0.
 * 
 * @param[in]  PageSize   The size of the page that the ScrollBar should control.
 * @param[in]  ViewSize   The view size of the page that the ScrollBar allows to see.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::SetPageSize(uint16_t PageSize, uint16_t ViewSize)
{
  if (ViewSize >= PageSize || PageSize == 0)
    ViewSize = PageSize = 0;

  mPageSize = PageSize;
  mViewSize = ViewSize;
  resize();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the ScrollBar.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorKnob    The color of the ScrollBar knob when is visible and released.
 * @param[in]  ColorTouch   The color of the ScrollBar knob when is visible and touched.
 * @param[in]  Opacity      The opacity of the ScrollBar knob.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::SetBarColor(uint16_t ColorKnob, uint16_t ColorTouch, uint8_t Opacity)
{
  mColorKnob = ColorKnob;
  mColorTouch = ColorTouch;
  mAlpha = mOpacityKnob = Opacity;
  Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the style of the ScrollBar.
 * 
 * @param[in]  Mode     The operating mode of the ScrollBar. See below.
 * @param[in]  Padding  The padding in pixels all around the knob of the ScrollBar.
 * @param[in]  Delay    The timeout delay before the knob of the ScrollBar disappears.\n
 *                      Default delay is 1.5 sec. Valid only with the SCROLL_BAR_AUTO mode.
 * 
 * @tableofcontents
 * Operating Mode  |                         Description
 * --------------- | ----------------------------------------------------------------
 * SCROLL_BAR_OFF  | Sets the ScrollBar always off.
 * SCROLL_BAR_ON   | Sets the ScrollBar always on.
 * SCROLL_BAR_AUTO | Sets the ScrollBar in automatic off mode after releasing it and timeout expires.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::SetBarStyle(uint8_t Mode, uint8_t Padding, uint16_t Delay)
{
  mMode = Mode;
  mPadding = Padding;
  mDelay = Delay;
  Modified();

  switch (mMode)
  {
    default: if (mTimer > 0) break;
    case SCROLL_BAR_OFF: Hide(); break;
    case SCROLL_BAR_ON: Show(); break;
  }
}

/// @copydoc EvButton::SetOnChange()

void        EvScrollBar::SetOnChange(void (*OnChange)(EvScrollBar *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::drawEvent(void)
{
  int16_t   l, t, w, h, padL, padR, padT, padB;

  if (mMode != SCROLL_BAR_OFF && mPageSize > 0)
  {
    padL = padR = padT = padB = mPadding;

    if (mHeight > mWidth)
    {
      if (mTouchBar)
        padL = 0;

      t = (padT << 4) + ((mValue * mKnobZone) / mPageSize);
      w = (mWidth - padL - padR) << 4;
      h = mKnobSize;
      l = padL << 4;
    }
    else
    {
      if (mTouchBar)
        padT = 0;

      l = (padL << 4) + ((mValue * mKnobZone) / mPageSize);
      h = (mHeight - padT - padB) << 4;
      w = mKnobSize;
      t = padT << 4;
    }

    if (w > 0 && h > 0)
    {
      Disp->ColorA(mAlpha);
      FillRectangle2f(l, t, w, h, mTouchBar ? mColorTouch : mColorKnob, mBdRadius);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::resizeEvent(void)
{
  resize();
  SetValue(mValue);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::refreshEvent(void)
{
  if (!mTouchBar)
  {
    int16_t   v;
    uint16_t  msec;

    if ((v = mKinScroll.Value()) != 0)
      SetValue(mValue + v);
    else
      mScrolling &= ~SCROLL_MOVING;

    if (mTimer > 0)
    {
      msec = millis();
      msec -= mTimer;

      if (mMode < SCROLL_BAR_AUTO)
      {
        if (msec >= mDelay)
        {
          mScrolling &= ~SCROLL_TIMER;
          mTimer = 0;
        }
      }
      else
      {
        if (msec >= mDelay)
        {
          mScrolling &= ~SCROLL_TIMER;
          mTimer = 0;
          Hide();
        }
        else if (msec > mDelay - mOpacityKnob)
        {
          mAlpha = mDelay - msec;
          Modified();
        }
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::touchEvent(EvTouchEvent *Touch)
{
  int16_t   len;

  len = mKnobZone >> 4;

  switch (Touch->event)
  {
    case TOUCH_START:
      if (mPageSize > 0)
      {
        int16_t   pos, begin, end;

        pos = (mHeight > mWidth) ? Touch->y : Touch->x;
        begin = mPadding + ((mValue * len) / mPageSize);
        end = begin + (mKnobSize >> 4) + 1;
        mTouchBar = (pos >= begin && pos <= end);
        mSpeed = 0;
        Touch->event = 0;
        Modified();
      }
      break;

    case TOUCH_MOVE:
      if (mTouchBar && len > 0)
      {
        int16_t   move = (mHeight > mWidth) ? Touch->move.y : Touch->move.x;

        SetValue(mValue + ((move * mPageSize) / len));
        mSpeed = (mSpeed >> 2) + (move * 3 << 2);
        Touch->event = 0;
      }
      break;

    case TOUCH_END:
      mKinScroll.Start(mSpeed);
      mTouchBar = false;
      Modified();
      WakeUp();

    case TOUCH_CANCEL:
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::wakeup(void)
{
  switch (mMode)
  {
    case SCROLL_BAR_AUTO:
      if (mAlpha != mOpacityKnob)
      {
        mAlpha = mOpacityKnob;
        Modified();
      }

      Show();

    case SCROLL_BAR_ON:
      mTimer = millis_nz();
      mScrolling |= SCROLL_TIMER;

    case SCROLL_BAR_OFF:
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBar::resize(void)
{
  if (mPageSize > 0)
  {
    mKnobZone = (((mHeight > mWidth) ? mHeight : mWidth) - (mPadding << 1)) << 4;

    if ((mKnobSize = (mKnobZone * mViewSize) / mPageSize) < KNOB_MIN)
    {
      mKnobZone -= KNOB_MIN - mKnobSize;
      mKnobSize = KNOB_MIN;
    }

    Modified();
  }
}
