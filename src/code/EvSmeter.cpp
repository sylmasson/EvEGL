
#include    <EvGUI.h>

EvBitmap    Smeter(&sSmeter);
EvBitmap    SmeterNeedle(&sSmeterNeedle);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter    *EvSmeter::Create(int16_t Left, int16_t Top, float Scale, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvSmeter *)EvObj::TryCreate(new EvSmeter(Left, Top, Scale, Dest->Disp, !Tag ? "EvSmeter" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter::EvSmeter(int16_t Left, int16_t Top, float Scale, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, sSmeter.Width * Scale, sSmeter.Height * Scale, Disp, Tag, State)
{
  mValue = 0;
  mDegree = 0;
  mScale = 65536.0 * Scale;
  mCoeff = (uint16_t)(65536.0 * 0.90);
  mNeedle = (EvSmeterNeedle *)TryCreate(new EvSmeterNeedle(0, 0, Scale, Disp, "Smeter Needle"), this);

  if (mNeedle == NULL || !Smeter.Load(Disp) || !SmeterNeedle.Load(Disp))
    Abort();
  else
  {
    mNeedle->Disable();
    SmeterNeedle.Align(CENTER_BOTTOM);
    SetValue(0);
    SetOnTouch(NULL);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter::~EvSmeter(void)
{
  Smeter.Unload();
  SmeterNeedle.Unload();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::Show(void)
{
  if (!IsVisible() && Smeter.Load(Disp))
  {
    if (!SmeterNeedle.Load(Disp))
      Smeter.Unload();
    else
    {
      SmeterNeedle.Align(CENTER_BOTTOM);
      EvPanel::Show();
      Modified();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::Hide(void)
{
  if (IsVisible())
  {
    EvPanel::Hide();
    Smeter.Unload();
    SmeterNeedle.Unload();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSmeter::SetValue(int16_t Value)
{
  int32_t   degree;

  mValue = (Value > 1000) ? 1000 : ((Value < 0) ? 0 : Value);
  degree = (((int32_t)mValue * 16384) / 1000) - 8192;
  mDegree = (mCoeff == 0) ? degree : ((mDegree * mCoeff) + (degree * (65536L - mCoeff))) >> 16;

  return (mNeedle != NULL) ? mNeedle->SetDegree(mDegree) : false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::SetFilterCoeff(uint16_t Coeff)
{
  mCoeff = Coeff;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::SetOnTouch(void (*OnTouch)(EvSmeter *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::drawEvent(void)
{
  Disp->ColorRGB(RGB555(255, 255, 255));
  Smeter.Draw(0, 0, 0, mScale);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeter::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeterNeedle::EvSmeterNeedle(int16_t Left, int16_t Top, float Scale, EvDisplay *Disp, const char *Tag) : EvObj(Left, Top, sSmeter.Width * Scale, (sSmeter.Height - 10) * Scale, Disp, Tag)
{
  mDegree = 0;
  mScale = 65536.0 * Scale;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSmeterNeedle::SetDegree(int32_t Degree)
{
  if (mDegree == Degree)
    return false;

  mDegree = Degree;
  Modified();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSmeterNeedle::drawEvent(void)
{
  int16_t   x, y;
  uint16_t  height = 257;

  x = (mWidth << 4) / 2;
  y = (height << 4) + ((((26 + 12) << 4) * mScale) >> 16);
  SmeterNeedle.Setup(x, y, mDegree, mScale);
  Disp->ColorRGB(RGB555(255, 255, 255));
//  Disp->ColorA(0x40);
//  SmeterNeedle.Vertex2f(x, y);
  y -= ((6 << 4) * mScale) >> 16;
  Disp->ColorA(0xFF);
  SmeterNeedle.Vertex2f(x, y);
}
