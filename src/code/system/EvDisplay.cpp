
#include    <EvGUI.h>

#define     DEBUG     1

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvDisplay::sDispCount = 0;
uint16_t    EvDisplay::sTraceFlags = 0;
uint32_t    EvDisplay::sFrameNumber = 0;
uint32_t    EvDisplay::sSecondTimer = 0;
uint32_t    EvDisplay::sUpdateTimer = 0;
EvDisplay   *EvDisplay::sDispList[DISP_MAX];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay   *EvDisplay::Create(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST, SPIClass *Spi, uint32_t Baudrate)
{
  return new EvDisplay(Width, Height, Tag, Config, CS, RST, Spi, Baudrate);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay::EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, uint8_t RST, SPIClass *Spi, uint32_t Baudrate) :
EvEVE(Config, CS, RST, Spi, Baudrate), EvPanel(0, 0, Width, Height, this, Tag, VISIBLE_OBJ | SYSTEM_OBJ), EvSysFont(this)
{
  if (sDispCount >= 3)
  {
    EvOut->println("Error: Too many instances of the EvDisplay class");
    exit(0);
  }

  #if defined(ESP32)
    if ((mMutex = xSemaphoreCreateRecursiveMutex()) == nullptr)
    {
      EvOut->println("Error: Cannot create mutex");
      exit(0);
    }
  #endif

  sDispList[sDispCount++] = this;
  mKbd = nullptr;
  mTimeUsed = 0;
  mMaxDL = 0;

#ifdef DEBUG
  pinMode(28, OUTPUT);   // debug Disp1 update
  pinMode(37, OUTPUT);   // debug Disp2 update
  digitalWrite(28, LOW);
  digitalWrite(37, LOW);
#endif

  BgColor(RGB555(0, 0, 0));
  Rotate(rd8(REG_ROTATE));
  SetOnUpdate(nullptr);
  SetOnTouch(nullptr);
  InitSystemFont();
  Brightness(64);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay::~EvDisplay(void)
{
  delete this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::Lock(void)
{
  #if defined(ESP32)
    return xSemaphoreTakeRecursive(mMutex, portMAX_DELAY);
  #else
    return true;
  #endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::Unlock(void)
{
  #if defined(ESP32)
    return xSemaphoreGiveRecursive(mMutex);
  #else
    return true;
  #endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::TryLock(void)
{
  #if defined(ESP32)
    return xSemaphoreTakeRecursive(mMutex, 1);
  #else
    return true;
  #endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::KbdDelete(void)
{
  if (mKbd != nullptr)
  {
    if (mKbd->FocusObj != nullptr)
      mKbd->FocusObj->LostKbdFocus();

    mKbd->Delete();
    mKbd = nullptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvDisplay::FrameNumber(void)
{
  return sFrameNumber;
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

  if (mKbd != nullptr)
    mKbd->SetKeyboard((mOrientation & 2) ? 1 : 0, mKbd->IsOpen());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnUpdate(void (*OnUpdate)(EvDisplay *Disp))
{
  mOnUpdate = OnUpdate;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnTouch(void (*OnTouch)(EvObj *Obj, const EvTouchEvent *Touch))
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
        snprintf(str, sizeof(str) - 1, " using %.2f msec/frame (%4.1f%%)", timeUsed / disp->mFrameCount, timeUsed / 10);
        EvOut->print(str);
        snprintf(str, sizeof(str) - 1, " with %4u bytes of DL (%4.1f%%)", disp->mMaxDL, (disp->mMaxDL * 100.0) / 8192.0);
        EvOut->print(str);
      }

      disp->mFrameCount = 0;
      disp->mTimeUsed = 0;
      disp->mMaxDL = 0;
    }

    sSecondTimer += 1000000L;
  }

  if (!sDispCount || usec - sUpdateTimer < PERIOD_REFRESH)
    return false;

  for (i = 0; i < sDispCount; i++)
    if (!sDispList[i]->wrCmdBufEmpty())
    {
//      EvOut->println("Delayed by 1ms");
      sUpdateTimer += 1000;
      return false;
    }

  sUpdateTimer = usec;
  sFrameNumber++;

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

  TaskDMA.Update();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::update(void)
{
  uint32_t  usec = micros();

  mFrameCount++;
  Touch.update(this);
  Refresh();

  if (mKbd)
    mKbd->ToFront();

  EvEditor::AlwaysToFront();

  Opacity(OPACITY_MAX);
  ColorA(255);
  ClearColorRGB(TRANSPARENT);
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
