
#include    <EvGUI.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter    *EvSmeter::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvSmeter *)EvObj::TryCreate(new EvSmeter(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvSmeter" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter::EvSmeter(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width == 0 ? sSmeter.Width : Width, Height == 0 ? sSmeter.Height : Height, Disp, Tag, State)
{
  mBG = (EvImage *)EvImage::Create(0, 0, mWidth, mHeight, this, "SmeterBG");
  mNeedle = (EvNeedle *)TryCreate(new EvNeedle(0, 0, 0, 0, Disp, "SmeterNeedle"), this);

  if (!mBG || !mNeedle || !mBG->Load(&sSmeter, OPT_MEDIAFIFO) || !mNeedle->Load(&sSmeterNeedle))
    abortCreate();
  else
  {
    mValue = 0;
    mAngle = 0;
    mLock = false;
    mBG->SetMode(RESIZE_PROPORTIONAL, BILINEAR);
    mNeedle->SetMode(RESIZE_PROPORTIONAL, BILINEAR);
    mNeedle->RotateAround(sSmeterNeedle.Width / 2, sSmeterNeedle.Height - 6);
    BgColor(RGB555(0, 0, 0));
    resizeEvent();
    SetOnTouch(nullptr);
    SetValue(-1000);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSmeter::SetValue(int32_t Value)
{
  mValue = (Value > 1000) ? 1000 : ((Value < -1000) ? -1000 : Value);
  mAngle = mValue * (45.0 / 1000.0);
  mNeedle->Rotate(mAngle);
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::SetOnTouch(void (*OnTouch)(EvSmeter *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::resizeEvent(void)
{
  if (!mLock)
  {
    mLock = true;
    mBG->ReSize(mWidth, mHeight);
    ReSize(mBG->Width(), mBG->Height());
    mNeedle->Rotate(0);
    mNeedle->Refresh();
    mNeedle->Scale(mScale = mWidth / (float)sSmeter.Width);
    mNeedle->MoveTo((1 + sSmeter.Width - sSmeterNeedle.Width) * mScale * 0.5, 60 * mScale);
    mNeedle->Rotate(mAngle);
    mLock = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNeedle::EvNeedle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag) : EvImage(Left, Top, Width, Height, Disp, Tag)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNeedle::drawEvent(void)
{
  Disp->ScissorSize(mView.w, mView.h - (17 * mScaleY));
  drawSetup();
  Disp->ColorRGB(RGB555(255, 255, 255));
  Disp->ColorA(64);
  Disp->Vertex2ii(0, 20 * mScaleY);
  Disp->ColorA(255);
  Disp->Vertex2f(0, 0);
}
