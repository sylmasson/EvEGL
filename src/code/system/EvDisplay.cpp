
#include    <EvGUI.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvDisplay::sDispCount = 0;
uint16_t    EvDisplay::sFrameCount = 0;
uint16_t    EvDisplay::sTraceFlags = 0;
uint32_t    EvDisplay::sSecondTimer = 0;
uint32_t    EvDisplay::sUpdateTimer = 0;
EvDisplay   *EvDisplay::sDispList[DISP_MAX];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay::EvDisplay(uint16_t Width, uint16_t Height, const char *Tag, const uint32_t *Config, uint8_t CS, int16_t RST, SPIClass *Spi, uint32_t Baudrate) :
EvEVE(Config, CS, RST, Spi, Baudrate), EvPanel(0, 0, Width, Height, this, Tag, VISIBLE_OBJ | SYSTEM_OBJ), EvSysFont(this)
{
  if (sDispCount >= 3)
  {
    Serial.println("Error: Too many instances of the EvDisplay class");
    exit(0);
  }

  #if defined(ESP32)
    if ((mMutex = xSemaphoreCreateRecursiveMutex()) == nullptr)
    {
      Serial.println("Error: Cannot create mutex");
      exit(0);
    }
  #endif

  sDispList[sDispCount++] = this;
  Kbd = nullptr;
  memset(&mTouch, 0, sizeof(mTouch));
  mTimeUsed = 0;
  MaxDL = 0;

  BgColor(RGB555(0, 0, 0));
  Rotate(rd8(REG_ROTATE));
  SetOnUpdate(nullptr);
  SetOnTouch(nullptr);
  InitSystemFont();
  Brightness(64);
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
  if (Kbd != nullptr)
  {
    if (Kbd->FocusObj != nullptr)
      Kbd->FocusObj->LostKbdFocus();

    Kbd->Delete();
    Kbd = nullptr;
  }
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

  if (Kbd != nullptr)
    Kbd->SetKeyboard((mOrientation & 2) ? 1 : 0);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnUpdate(void (*OnUpdate)(EvDisplay *Disp))
{
  mOnUpdate = OnUpdate;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::SetOnTouch(void (*OnTouch)(EvObj *Obj, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvDisplay::UpdateAll(void)
{
  uint16_t  i;
  uint32_t  usec = micros();

  if (usec - sSecondTimer >= 1000000L)
  {
    if (sFrameCount)
      for (i = 0; i < sDispCount; i++)
      {
        EvDisplay *disp = sDispList[i];

        if (sTraceFlags & TRACE_FPS)
        {
          char    str[80];
          float   timeUsed = disp->mTimeUsed / 1000L;

          snprintf(str, sizeof(str) - 1, "\n[%s] %u FPS", disp->Tag, sFrameCount);
          Serial.print(str);
          snprintf(str, sizeof(str) - 1, " using %.2f msec/frame (%4.1f%%)", timeUsed / sFrameCount, timeUsed / 10);
          Serial.print(str);
          snprintf(str, sizeof(str) - 1, " with %4u bytes of DL (%4.1f%%)", disp->MaxDL, (disp->MaxDL * 100.0) / 8192.0);
          Serial.print(str);
        }

        disp->mTimeUsed = 0;
        disp->MaxDL = 0;
      }

    sSecondTimer += 1000000L;
    sFrameCount = 0;
  }

  if (!sDispCount || usec - sUpdateTimer < PERIOD_REFRESH)
    return false;

  for (i = 0; i < sDispCount; i++)
    if (!sDispList[i]->wrCmdBufEmpty())
    {
      sUpdateTimer += 1000;
      return false;
    }

  sUpdateTimer = usec;

  for (i = 0; i < sDispCount; i++)
  {
    digitalWrite(5, HIGH);
    sDispList[i]->Update();
    digitalWrite(5, LOW);
  }

  sFrameCount++;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::Update(void)
{
  uint32_t  usec = micros();

  touchUpdate();
  Refresh();

  if (Kbd)
    Kbd->ToFront();

  EditorToFront();
  displayUpdate();

  if ((SizeDL = ReadDL()) > MaxDL)
    MaxDL = SizeDL;

  if (mOnUpdate)
    (*mOnUpdate)(this);

  SwapDL();
  Preload();

  mTimeUsed += micros() - usec;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::displayUpdate(void)
{
  Opacity(OPACITY_MAX);
  ColorA(255);
  ClearColorRGB(TRANSPARENT);
  ClearStencil(0);
  ClearTag(255);
  Clear();
  VertexFormat(4);
  Draw();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::touchUpdate(void)
{
  EvTouchPos  touchPos;
  uint32_t    msec = millis();

  if (CapacitiveTouchEngine && !rd8(REG_CTOUCH_EXTENDED))
  {
    mTouch[1].id = 1;
    touchPos.xy = rd32(REG_CTOUCH_TOUCH1_XY);
    mTouch[1].tag = ((touchPos.xy & 0x80008000) ? 0 : rd8(REG_CTOUCH_TAG1));
    touchUpdatePtr(&mTouch[1], touchPos, msec);

    mTouch[2].id = 2;
    touchPos.xy = rd32(REG_CTOUCH_TOUCH2_XY);
    mTouch[2].tag = ((touchPos.xy & 0x80008000) ? 0 : rd8(REG_CTOUCH_TAG2));
    touchUpdatePtr(&mTouch[2], touchPos, msec);

    mTouch[3].id = 3;
    touchPos.xy = rd32(REG_CTOUCH_TOUCH3_XY);
    mTouch[3].tag = ((touchPos.xy & 0x80008000) ? 0 : rd8(REG_CTOUCH_TAG3));
    touchUpdatePtr(&mTouch[3], touchPos, msec);

    mTouch[4].id = 4;
    touchPos.x = rd16(REG_CTOUCH_TOUCH4_X);
    touchPos.y = rd16(REG_CTOUCH_TOUCH4_Y);
    mTouch[4].tag = ((touchPos.xy & 0x80008000) ? 0 : rd8(REG_CTOUCH_TAG4));
    touchUpdatePtr(&mTouch[4], touchPos, msec);
  }

  mTouch[0].id = 0;
  touchPos.xy = rd32(REG_TOUCH_SCREEN_XY);
  mTouch[0].tag = ((touchPos.xy & 0x80008000) ? 0 : rd8(REG_TOUCH_TAG));
  touchUpdatePtr(&mTouch[0], touchPos, msec);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDisplay::touchUpdatePtr(EvTouchEvent *Touch, EvTouchPos TouchPos, uint32_t msec)
{
  Touch->event = 0;
  Touch->timer = msec - Touch->startTimer;

  if (!Touch->tag)
  {
    if ((Touch->status & TOUCHING_FLAG) != 0 && Touch->owner != nullptr)
    { // end & double event
      if (Touch->timer > DOUBLE_DELAY)
        Touch->status = 0;
      else
      {
        Touch->status &= ~(TOUCHING_FLAG | REPEAT_FLAG);
        Touch->status += DOUBLE_COUNT;

        if (Touch->status & DOUBLE_FLAG)
        {
          Touch->event = TOUCH_DOUBLE;

          if (mOnTouch != nullptr)
            (*mOnTouch)(Touch->owner, Touch);

          if (Touch->event)
            Touch->owner->TouchUpdate(Touch);

          Touch->status = 0;
        }
      }

      Touch->endTimer = msec;
      Touch->event = TOUCH_END;
      Touch->owner->TouchEnd();
    }
    else if (Touch->status == 0 || Touch->timer > DOUBLE_DELAY)
      Touch->status = 0;
  }
  else
  {
    Touch->abs.xy = TouchPos.xy;

    if ((Touch->status & TOUCHING_FLAG) == 0)
    { // start event
      EvObj   *owner;

      Touch->obj = nullptr;
      Touch->owner = nullptr;

      if ((owner = Touching(Touch)) != nullptr)
      {
        if (Touch->timer > (DOUBLE_DELAY * 2))
          Touch->status &= ~(DOUBLE_COUNT | DOUBLE_FLAG);

        Touch->status |= TOUCHING_FLAG;

        if (!owner->TouchStart())
          Touch->owner = nullptr;
        else
        {
          Touch->owner = owner;
          Touch->event = TOUCH_START;
          Touch->x = Touch->abs.x - Touch->owner->View.ox;
          Touch->y = Touch->abs.y - Touch->owner->View.oy;
          Touch->prev.xy = Touch->abs.xy;
          Touch->move.xy = 0;
          Touch->repeatDelay = REPEAT_DELAY;
          Touch->repeatTimer = HOLD_DELAY;
          Touch->startTimer = msec;
          Touch->timer = 0;
        }
      }
    }
    else if (Touch->owner != nullptr)
    { // hold, repeat and move event
      Touch->x = Touch->abs.x - Touch->owner->View.ox;
      Touch->y = Touch->abs.y - Touch->owner->View.oy;

      if (Touch->timer >= Touch->repeatTimer)
      {
        Touch->event = (Touch->status & REPEAT_FLAG) ? TOUCH_REPEAT : TOUCH_HOLD;
        Touch->repeatTimer += Touch->repeatDelay;
        Touch->status |= REPEAT_FLAG;
      }
      else
      {
        Touch->move.x = Touch->abs.x - Touch->prev.x;
        Touch->move.y = Touch->abs.y - Touch->prev.y;

        if (Touch->status & MOVE_FLAG)
        {
          Touch->prev.xy = Touch->abs.xy;

          if (Touch->move.xy != 0)
            Touch->event = TOUCH_MOVE;
        }
        else if (abs(Touch->move.x) > 3 || abs(Touch->move.y) > 3)
        {
          Touch->prev.xy = Touch->abs.xy;
          Touch->status |= MOVE_FLAG;
          Touch->event = TOUCH_MOVE;
        }
      }
    }
  }

  if (Touch->event)
  {
    if (mOnTouch != nullptr)
      (*mOnTouch)(Touch->owner, Touch);

    if (Touch->event)
      Touch->owner->TouchUpdate(Touch);
  }
}
