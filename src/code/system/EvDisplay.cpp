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

//#define     DEBUG     1

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char * const EvDisplay::TypeName = "EvDisplay";

uint16_t    EvDisplay::sDispCount = 0;
uint16_t    EvDisplay::sTraceFlags = 0;
uint32_t    EvDisplay::sFrameRate = 60000;
uint32_t    EvDisplay::sFrameTime = 16667;
uint32_t    EvDisplay::sFrameNumber = 0;
uint32_t    EvDisplay::sSecondTimer = 0;
uint32_t    EvDisplay::sUpdateTimer = 0;
EvDisplay   *EvDisplay::sDispList[DISP_MAX];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay   *EvDisplay::Create(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST, uint32_t Baudrate, SPIClass *Spi)
{
  return new EvDisplay(Width, Height, Tag, Config, CS, RST, Baudrate, Spi);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay::EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST, uint32_t Baudrate, SPIClass *Spi) :
  EvPanel(0, 0, Width, Height, this, !Tag ? TypeName : Tag, VISIBLE_OBJ | SYSTEM_OBJ),
  EvEVE(Config, CS, RST, Baudrate, Spi),
  EvSysFont(this),
  mFrameCount(0),
  mTimeUsed(0),
  mSizeDL(0),
  mMaxDL(0),
  mOnUpdate(nullptr),
  mOnUpdateFPS(nullptr)
{
  if (sDispCount >= DISP_MAX)
  {
    EvErr->println("Error: Too many instances of the EvDisplay class");
    exit(0);
  }

  sDispList[sDispCount++] = this;

#ifdef DEBUG
  pinMode(28, OUTPUT);   // debug Disp1 update
  pinMode(37, OUTPUT);   // debug Disp2 update
  digitalWrite(28, LOW);
  digitalWrite(37, LOW);
#endif

  Kbd = EvKbd::Create(0, 0, 0, 0, this, "KbdSystem", SYSTEM_OBJ);
  BgColor(CL_DISPLAY_BG);
  Rotate(rd8(REG_ROTATE));
  Brightness(64);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::SetFrameRate(float FrameRate)
{
  uint32_t  frameRate = FrameRate * 1000;

  if (frameRate > 60000 || frameRate < 23976)
    return false;

  sFrameTime = 1000000000L / frameRate;
  sFrameRate = frameRate;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::Rotate(uint8_t Orientation)
{
  uint16_t  w, h;

  mOrientation = Orientation & 3;
  CmdSetRotate(mOrientation);
  w = rd16(REG_HSIZE);
  h = rd16(REG_VSIZE);

  if (mOrientation & 2)
    ReSize(h, w);
  else
    ReSize(w, h);

  Kbd->SetKeyboard((mOrientation & 2) ? 1 : 0, Kbd->IsOpen());
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnUpdate(void (*OnUpdate)(EvDisplay *Sender))
{
  mOnUpdate = OnUpdate;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnUpdateFPS(void (*OnUpdateFPS)(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL))
{
  mOnUpdateFPS = OnUpdateFPS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnTouch(void (*OnTouch)(EvObj *Sender, const EvTouchEvent *Touch))
{
  Touch.mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::Update(void)
{
  uint16_t  i;
  uint32_t  usec = micros();

  if (usec - sSecondTimer >= 1000000L)
  {
    for (i = 0; i < sDispCount; i++)
    {
      EvDisplay *disp = sDispList[i];

      if ((sTraceFlags & TRACE_FPS) && disp->mFrameCount > 0)
      {
        char    str[80];
        float   timeUsed = disp->mTimeUsed / 1000.0;

        snprintf(str, sizeof(str) - 1, "\n[%s] %u FPS", disp->Tag, disp->mFrameCount);
        EvOut->print(str);
        snprintf(str, sizeof(str) - 1, " avg %.2f msec/frame (%4.1f%%)", timeUsed / disp->mFrameCount, timeUsed / 10);
        EvOut->print(str);
        snprintf(str, sizeof(str) - 1, " %4u bytes of DL (%4.1f%%)", disp->mMaxDL, (disp->mMaxDL * 100.0) / 8192.0);
        EvOut->print(str);
      }

      if (disp->mOnUpdateFPS != nullptr)
        disp->mOnUpdateFPS(disp, disp->mTimeUsed, disp->mFrameCount, disp->mMaxDL);

      disp->mFrameCount = 0;
      disp->mTimeUsed = 0;
      disp->mMaxDL = 0;
    }

    sSecondTimer += 1000000L;
  }

  if (!sDispCount || usec - sUpdateTimer < sFrameTime)
    return false;

  for (i = 0; i < sDispCount; i++)
    if (!sDispList[i]->wrCmdBufEmpty())
    {
//      EvErr->println("Delayed by 1ms");
      sUpdateTimer += 1000;
      return false;
    }

  sUpdateTimer = usec;
  sFrameNumber++;

  if (SystemColor.IsModified(true))
    for (i = 0; i < sDispCount; i++)
      sDispList[i]->ModifiedAll();

#ifdef DEBUG
  digitalWrite(28, HIGH);
  sDispList[0]->update();
  digitalWrite(28, LOW);
  digitalWrite(37, HIGH);
  sDispList[1]->update();
  digitalWrite(37, LOW);
#else
  for (i = 0; i < sDispCount; i++)
    sDispList[i]->update();
#endif

  if (TaskDMA.InQueues())
  {
    uint32_t  timeFrame, maxByteTranfer;

    timeFrame = sFrameTime - (micros() - sUpdateTimer) - 500;
    maxByteTranfer = (((EvSPI::baudrate / 1000) * timeFrame) / 8000) & ~1023;
    // EvDbg->printf("maxByteTranfer = %d, timeFrame = %d usec\n", maxByteTranfer, timeFrame);

    if (maxByteTranfer > 0)
      TaskDMA.Update();
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::update(void)
{
  uint32_t  usec = micros();

  mFrameCount++;
  Touch.update(this);
  Refresh();

  Kbd->ToFront();
  EvEditor::AlwaysToFront();

  Opacity(OPACITY_MAX);
  ColorA(255);
  ClearColorRGB(CL_NOCOLOR);
  ClearStencil(0);
  ClearTag(255);
  Clear();
  VertexFormat(4);
  Draw();

  if (mOnUpdate)
    mOnUpdate(this);

  if ((mSizeDL = ReadDL()) > mMaxDL)
    mMaxDL = mSizeDL;

  SwapDL();
  mTimeUsed += micros() - usec;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
