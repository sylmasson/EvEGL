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

const char * const EvSmeter::TypeName = "EvSmeter";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter    *EvSmeter::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvSmeter *)EvObj::TryCreate(new EvSmeter(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSmeter::EvSmeter(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvPanel(Left, Top, Width == 0 ? Smeter->Width : Width, Height == 0 ? Smeter->Height : Height, Disp, !Tag ? TypeName : Tag, State),
  mLock(false),
  mValue(0),
  mAngle(0),
  mOnTouch(nullptr)
{
  mBG = (EvImage *)EvImage::Create(0, 0, mWidth, mHeight, this, "SmeterBG");
  mNeedle = (EvNeedle *)TryCreate(new EvNeedle(0, 0, 0, 0, Disp, "SmeterNeedle"), this);

  if (!mBG || !mNeedle || !mBG->Load(Smeter, OPT_MEDIAFIFO) || !mNeedle->Load(SmeterNeedle))
    abortCreate();
  else
  {
    mBG->SetMode(SCALE_TO_FIT, BILINEAR);
    mNeedle->SetMode(SCALE_TO_FIT, BILINEAR);
    mNeedle->RotateAround(SmeterNeedle->Width / 2, SmeterNeedle->Height - 6);
    BgColor(CL_BLACK);
    resizeEvent();
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
    mNeedle->Scale(mScale = mWidth / (float)Smeter->Width);
    mNeedle->MoveTo((1 + Smeter->Width - SmeterNeedle->Width) * mScale * 0.5, 60 * mScale);
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

EvNeedle::EvNeedle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag) :
  EvImage(Left, Top, Width, Height, Disp, Tag)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNeedle::drawEvent(void)
{
  Disp->ScissorSize(mView.w, mView.h - (17 * mScale));
  drawSetup();
  Disp->ColorRGB(CL_WHITE);
  Disp->ColorA(64);
  Disp->Vertex2ii(0, 20 * mScale);
  Disp->ColorA(255);
  Disp->Vertex2f(0, 0);
}
