
#include    <EvGUI.h>

const char * const EvVuBar::TypeName = "EvVuBar";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVuBar     *EvVuBar::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvVuBar *)EvObj::TryCreate(new EvVuBar(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVuBar::EvVuBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mValue(0),
  mDotValue(0),
  mPeakValue(0),
  mPeakTimer(0),
  mOnTouch(nullptr)
{
  BgColor(CL_NOCOLOR);
  SetColor(CL_NOCOLOR);
  SetFormat(12, 3, 5);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVuBar::SetValue(int16_t Value)
{
  int16_t   dotValue;

  mValue = (Value > 1000) ? 1000 : ((Value < 0) ? 0 : Value);
  dotValue = (((uint32_t)mValue * mDotCount) + 500) / 1000;

  if (mDotValue == dotValue)
    return false;

  if (mPeakValue > -1 && dotValue >= mPeakValue)
  {
    mPeakValue = dotValue;
    mPeakTimer = 0;
  }

  mDotValue = dotValue;
  Modified();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::SetPeakDetect(bool Enabled)
{
  mPeakValue = (Enabled == true) ? 0 : -1;
  mPeakTimer = 0;
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::SetOnTouch(void (*OnTouch)(EvVuBar *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::SetColor(uint16_t OffColor, uint16_t Low, uint16_t Med, uint16_t High)
{
  if (mOffColor.Set(OffColor) | mDotColor[0].Set(Low) | mDotColor[1].Set(Med) | mDotColor[2].Set(High))
    Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVuBar::SetFormat(uint8_t Low, uint8_t Med, uint8_t High, uint8_t SpaceWidth)
{
  mDotCount  = mDotCnt[0] = Low;
  mDotCount += mDotCnt[1] = Med;
  mDotCount += mDotCnt[2] = High;
  mSpWidth = (uint16_t)SpaceWidth << 8;
  resize();
  return (mDotWidth != 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::drawEvent(void)
{
  int16_t   i, j;
  uint16_t  color, limit;
  int32_t   x, y, count;

  if (mDotWidth > 0)
  {
    Disp->Begin(RECTS);
    Disp->LineWidth(1 << 4);
    x = !mVertical ? 0 : ((int32_t)mWidth - 1) << 8;
    y = ((int32_t)mHeight - 1) << 8;

    if (mSpWidth > 0)
    { // Draw all separate blocks
      for (i = 1, limit = mDotCnt[j = 0]; i <= mDotCount; i++)
      {
        while (i > limit && j < 2)
          limit += mDotCnt[++j];

        if ((color = (i > mDotValue && i != mPeakValue) ? mOffColor.Get() : mDotColor[j].Get()) != 0)
        {
          Disp->ColorRGB(color);

          if (!mVertical)
          {
            Disp->Vertex2f(x >> 4, 0);
            x += mDotWidth;
            Disp->Vertex2f((x - 256 + 8) >> 4, y >> 4);
            x += mSpWidth;
          }
          else
          {
            Disp->Vertex2f(0, y >> 4);
            y -= mDotWidth;
            Disp->Vertex2f(x >> 4, (y + 256) >> 4);
            y -= mSpWidth;
          }
        }
        else
        {
          if (!mVertical)
            x += mDotWidth + mSpWidth;
          else
            y -= mDotWidth + mSpWidth;
        }
      }
    }

    else
    { // Optimize Vertex blocks when no space
      for (i = mDotValue, j = limit = 0; i > 0 && j <= 2; i -= count, j++)
        if ((count = mDotCnt[j]) > 0)
        {
          limit += count;

          if (i < count)
           count = i;

          Disp->ColorRGB(mDotColor[j].Get());

          if (!mVertical)
          {
            Disp->Vertex2f(x >> 4, 0);
            x += count * mDotWidth;
            Disp->Vertex2f((x - 256 + 8) >> 4, y >> 4);
          }
          else
          {
            Disp->Vertex2f(0, y >> 4);
            y -= count * mDotWidth;
            Disp->Vertex2f(x >> 4, (y + 256) >> 4);
          }
        }

      if (mPeakValue > 0 && mPeakValue != mDotValue)
      {
        for (limit = mDotCnt[j = 0]; mPeakValue > limit && j < 2; )
          limit += mDotCnt[++j];

        Disp->ColorRGB(mDotColor[j].Get());

        if (!mVertical)
        {
          x = 0 + (uint32_t)(mPeakValue - 1) * mDotWidth;
          Disp->Vertex2f(x >> 4, 0);
          Disp->Vertex2f((x + mDotWidth - 256 + 8) >> 4, y >> 4);
        }
        else
        {
          y = (((int32_t)mHeight - 1) << 8) - ((uint32_t)(mPeakValue - 1) * mDotWidth);
          Disp->Vertex2f(0, y >> 4);
          Disp->Vertex2f(x >> 4, (y - mDotWidth + 256) >> 4);
        }
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::resizeEvent(void)
{
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::refreshEvent(void)
{
  if (mPeakTimer == 0)
  {
    if (mDotValue < mPeakValue)
      mPeakTimer = millis_nz();
  }
  else
  {
    uint16_t  timer = millis_nz() - mPeakTimer;

    if (timer > 1000)
    {
      timer = (timer - 1000) / 200;

      if ((mPeakValue -= timer) <= mDotValue)
      {
        mPeakValue = mDotValue;
        mPeakTimer = 0;
      }

      Modified();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVuBar::resize(void)
{
  int32_t   w = mWidth;

  mVertical = false;
  mDotWidth = 0;

  if (mHeight > mWidth)
  {
    w = mHeight;
    mVertical = true;
  }

  if (mDotCount > 0)
  {
    w = ((w << 8) - ((mDotCount - 1) * (int32_t)mSpWidth)) / mDotCount;
    mDotWidth = (w < (1 << 8)) ? 0 : w;
  }

  Modified();
}
