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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static uint8_t  kineticTable[64] =
{
    0,  16,  30,  44,  57,  69,  80,  91, 101, 111, 120, 128, 136, 143, 150, 157,
  163, 169, 174, 180, 184, 189, 193, 197, 201, 205, 208, 211, 214, 217, 219, 222,
  224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 242, 243, 244, 245, 246,
  247, 248, 248, 249, 250, 251, 251, 252, 252, 253, 253, 254, 254, 254, 255, 255
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKinScroll::EvKinScroll(uint8_t Speed, uint16_t Period)
{
  Setup(Speed, Period);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinScroll::Setup(uint8_t Speed, uint16_t Period)
{
  if ((mPeriod = ((uint32_t)Period * EvDisplay::FrameRate()) / 1000000L) >= sizeof(kineticTable))
    mPeriod = sizeof(kineticTable) - 1;

  mSpeed = (mPeriod == 0) ? 0 : ((uint)Speed * 256) / kineticTable[mPeriod]; // 4 bits fixed point
  mValue = mIndex = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinScroll::Start(int16_t Velocity)
{
  mVelocity = (Velocity * (int)mSpeed) >> 4;
  mIndex = mPeriod;
  mValue = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvKinScroll::IsEnabled(void)
{
  return mIndex != 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinScroll::Stop(void)
{
  mValue = mIndex = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvKinScroll::Value(void)
{
  int16_t   value = 0;

  if (mIndex)
  {
    mValue += (mVelocity * (uint32_t)kineticTable[mIndex--]) >> 8;
    value = mValue >> 4;
    mValue &= 0x0F;

    if (value == 0)
      mIndex = 0;
  }

  return value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvKinMove::EvKinMove(uint16_t Period)
{
  Setup(Period);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinMove::Setup(uint16_t Period)
{
  int       i;

  if ((mPeriod = ((uint32_t)Period * EvDisplay::FrameRate()) / 1000000L) >= sizeof(kineticTable))
    mPeriod = sizeof(kineticTable) - 1;

  for (mTotal = 0, i = 0; i <= mPeriod; i++)
    mTotal += kineticTable[i];

  mValue = mIndex = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinMove::Start(int16_t Pixels)
{
  if (Pixels == 0)
  {
    Stop();
    return;
  }
  
  mNegative = (Pixels < 0);
  mPixels = abs(Pixels);
  mScale = ((uint32_t)mPixels << 16) / mTotal;
  mIndex = mPeriod;
  mValue = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvKinMove::IsEnabled(void)
{
  return mIndex != 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvKinMove::Stop(void)
{
  mValue = mIndex = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvKinMove::Value(void)
{
  int16_t   value = 0;

  if (mIndex)
  {
    mValue += (mScale * kineticTable[mIndex]) >> 12;
    value = mValue >> 4;
    mValue &= 0x0F;

    if (--mIndex == 0 || value > mPixels)
    {
      value = mPixels;
      mIndex = 0;
    }

    mPixels -= value;

    if (mNegative)
      value = -value;
  }

  return value;
}
