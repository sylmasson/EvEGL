
#include    <EvGUI.h>

#define     SCROLLX         (mWidth < mPageWidth)
#define     SCROLLY         (mHeight < mPageHeight)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard ScrollBox.
 * 
 * A new ScrollBox is created at the specified size and relative position
 * of its owner Dest.
 *
 * @param[in]  Left    The left position of the ScrollBox.
 * @param[in]  Top     The top position of the ScrollBox.
 * @param[in]  Width   The width of the ScrollBox.
 * @param[in]  Height  The height of the ScrollBox.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the ScrollBox. If nullptr, the default tag name is "EvScrollBox".
 * @param[in]  State   The initial state of the ScrollBox. Default is set to VISIBLE_OBJ | CONTROL_OBJ.
 *
 * @return     EvScrollBox address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvScrollBox *EvScrollBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvScrollBox *)EvObj::TryCreate(new EvScrollBox(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvScrollBox" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvScrollBox::EvScrollBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  mTouchFlag = false;
  mPageWidth = Width;
  mPageHeight = Height;
  mScrollBarX = EvScrollBar::Create(0, 0, Width, 20, this, nullptr, VISIBLE_OBJ | FIXED_OBJ | SYSTEM_OBJ);
  mScrollBarY = EvScrollBar::Create(0, 0, 20, Height, this, nullptr, VISIBLE_OBJ | FIXED_OBJ | SYSTEM_OBJ);

  if (mScrollBarX == nullptr || mScrollBarY == nullptr)
    abortCreate();
  else
  {
    mScrollBarX->SetScrollBarSync(mScrollBarY);
    mScrollBarY->SetScrollBarSync(mScrollBarX);
    mScrollBarX->SetOnChange(sOnChangeScrollBarX);
    mScrollBarY->SetOnChange(sOnChangeScrollBarY);
    SetBarStyle(SCROLL_BAR_AUTO);
    SetScrollSpeed(16, 640);
    TouchMax(5);
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Check if it's in scrolling.
 * 
 * @return     true if it's in scrolling.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvScrollBox::Scrolling(void)
{
  if (!mTouchFlag && !mScrollBarY->Scrolling() && !mScrollBarX->Scrolling())
    return false;

  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the scrolling speed of the ScrollBox.
 * 
 * The scrolling speed is the speed at which scrolling will continue based
 * on the speed upon release.
 * 
 * @param[in]  Speed    The speed of the scrolling. ScrollBox used 16 by default.
 * @param[in]  Period   The period in msec of the scrolling. Default is 640 msec.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::SetScrollSpeed(uint8_t Speed, uint16_t Period)
{
  mKinScrollX.Setup(Speed, Period);
  mKinScrollY.Setup(Speed, Period);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the page size of the ScrollBox.
 * 
 * The two ScrollBars are adjusted according to the size of the page.
 * 
 * @param[in]  PageWidth  The width of the page of the ScrollBox.
 * @param[in]  PageHeight The height of the page of the ScrollBox.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::SetPageSize(uint16_t PageWidth, uint16_t PageHeight)
{
  mPageWidth = PageWidth;
  mPageHeight = PageHeight;
  resize();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the two ScrollBars.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorKnob    The color of the two ScrollBars knob when are visible and released.
 * @param[in]  ColorTouch   The color of the two ScrollBars knob when are visible and touched.
 * @param[in]  Opacity      The opacity of the two ScrollBars knob.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::SetBarColor(uint16_t ColorKnob, uint16_t ColorTouch, uint8_t Opacity)
{
  mScrollBarX->SetBarColor(ColorKnob, ColorTouch, Opacity);
  mScrollBarY->SetBarColor(ColorKnob, ColorTouch, Opacity);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the style of the two ScrollBars.
 * 
 * @param[in]  Mode       The operating mode of the two ScrollBars. See below.
 * @param[in]  Thickness  The thickness of the two ScrollBars.
 * @param[in]  Padding    The padding in pixels all around the knob of the two ScrollBars.
 * @param[in]  Delay      The timeout delay before the knob of the two ScrollBars disappears.\n
 *                        Default delay is 1.5 sec. Valid only with the SCROLL_BAR_AUTO mode.
 * 
 * @tableofcontents
 * Operating Mode  |                         Description
 * --------------- | ----------------------------------------------------------------
 * SCROLL_BAR_OFF  | Sets the two ScrollBars always off.
 * SCROLL_BAR_ON   | Sets the two ScrollBars always on.
 * SCROLL_BAR_AUTO | Sets the two ScrollBars in automatic off mode after releasing it and timeout expires.
 * 
 * @see        EvScrollBar
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::SetBarStyle(uint8_t Mode, uint8_t Thickness, uint8_t Padding, uint16_t Delay)
{
  mScrollBarX->SetBarStyle(Mode, Padding, Delay);
  mScrollBarY->SetBarStyle(Mode, Padding, Delay);
  mBarThickness = Thickness;
  mBarPadding = Padding;
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvScrollBox::Touching(const EvTouchEvent *Touch)
{
  EvObj         *obj = EvPanel::Touching(Touch);
  EvTouchEvent  *touch = (EvTouchEvent *)Touch;

  if (obj != nullptr && touch->owner == nullptr)
  {
    touch->obj = (obj->IsControlObj() || obj == this) ? nullptr : obj;
    touch->owner = obj = this;
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::Draw(void)
{
  if (mLast->obj != mScrollBarY)
  {
    SetToFront(mScrollBarX, false);
    SetToFront(mScrollBarY, false);
  }

  EvPanel::Draw();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::resizeEvent(void)
{
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::refreshEvent(void)
{
  if (mTouchFlag)
    mScrollBarX->WakeUp();

  scrollX(mKinScrollX.Value());
  scrollY(mKinScrollY.Value());
  EvPanel::refreshEvent();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::touchEvent(const EvTouchEvent *Touch)
{
  EvTouchEvent  *touch = (EvTouchEvent *)Touch;

  if (touch->obj != nullptr)
  {
    touch->x = touch->abs.x - touch->obj->View.ox;
    touch->y = touch->abs.y - touch->obj->View.oy;

    if (touch->obj == mScrollBarX || touch->obj == mScrollBarY)
      mScrollBarX->WakeUp();
  }

  if (touch->event != TOUCH_MOVE || (!SCROLLX && !SCROLLY))
  {
    if (touch->obj != nullptr)
      touch->obj->TouchUpdate(Touch);
    else if (touch->event == TOUCH_START)
    {
      mKinScrollX.Stop();
      mKinScrollY.Stop();
      mSpeedX = 0;
      mSpeedY = 0;
    }
    else if (touch->event == TOUCH_END)
    {
      mKinScrollX.Start(mSpeedX);
      mKinScrollY.Start(mSpeedY);
      mTouchFlag = false;
      touch->obj = nullptr;
    }
  }
  else
  {
    if (touch->obj == nullptr)
      mTouchFlag = true;
    else
    {
      touch->obj->TouchUpdate(Touch);

      if (touch->event == 0)
        return;

      touch->event = TOUCH_CANCEL;
      touch->obj->TouchUpdate(Touch);
      touch->obj = nullptr;
    }

    mSpeedX = (mSpeedX >> 2) + (touch->move.x * 3 << 2);
    mSpeedY = (mSpeedY >> 2) + (touch->move.y * 3 << 2);
    scrollX(touch->move.x);
    scrollY(touch->move.y);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::resize(void)
{
  uint16_t  size = mBarThickness + (mBarPadding << 1);

  mScrollBarX->MoveTo(0, mHeight - size);
  mScrollBarX->ReSize(mWidth - (!SCROLLY ? 0 : size), size);
  mScrollBarX->SetPageSize(mPageWidth, mWidth);

  mScrollBarY->MoveTo(mWidth - size, 0);
  mScrollBarY->ReSize(size, mHeight - (!SCROLLX ? 0 : size));
  mScrollBarY->SetPageSize(mPageHeight, mHeight);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::scrollX(int16_t X)
{
  if (X != 0 && SCROLLX)
  {
    int16_t left = X + mOffsetX;

    if (X > 0)
    {
      if (left > 0)
      {
        mKinScrollX.Stop();
        left = 0;
      }
    }
    else if ((left + mPageWidth) < mWidth)
    {
      mKinScrollX.Stop();
      left = mWidth - mPageWidth;
    }

    if (left != mOffsetX)
    {
      mScrollBarX->SetValue(-left);
      mScrollBarY->WakeUp();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::scrollY(int16_t Y)
{
  if (Y != 0 && SCROLLY)
  {
    int16_t top = Y + mOffsetY;

    if (Y > 0)
    {
      if (top > 0)
      {
        mKinScrollY.Stop();
        top = 0;
      }
    }
    else if ((top + mPageHeight) < mHeight)
    {
      mKinScrollY.Stop();
      top = mHeight - mPageHeight;
    }

    if (top != mOffsetY)
    {
      mScrollBarY->SetValue(-top);
      mScrollBarY->WakeUp();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::sOnChangeScrollBarX(EvScrollBar *Sender, int32_t Value)
{
  EvScrollBox *Owner = (EvScrollBox *)Sender->GetOwner();

  Owner->mOffsetX = -Value;
  Owner->SetView();
  Owner->Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvScrollBox::sOnChangeScrollBarY(EvScrollBar *Sender, int32_t Value)
{
  EvScrollBox *Owner = (EvScrollBox *)Sender->GetOwner();

  Owner->mOffsetY = -Value;
  Owner->SetView();
  Owner->Modified();
}
