
#include    <EvEGL.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     TOUCHING_FLAG   (1 << 0)
#define     REPEAT_FLAG     (1 << 1)
#define     DOUBLE_COUNT    (1 << 2)
#define     DOUBLE_FLAG     (1 << 3)
#define     MOVE_FLAG       (1 << 4)
#define     TOUCH_RELEASE   (1 << 5)

#define     HOLD_DELAY      800
#define     REPEAT_DELAY    150
#define     DOUBLE_DELAY    300

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTouch::EvTouch(void) :
  mOnTouch(nullptr)
{
  memset(&mTouchEvent, 0, sizeof(mTouchEvent));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouch::WaitForRelease(EvObj *Obj)
{
  for (int i = 0; i < 5; i++)
    if (mTouchEvent[i].status != 0 && (Obj == nullptr || mTouchEvent[i].owner == Obj || mTouchEvent[i].obj == Obj))
      mTouchEvent[i].status = TOUCH_RELEASE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouch::update(EvDisplay *Disp)
{
  EvTouchPos  touchPos;
  uint32_t    msec = millis();

  if (Disp->CapacitiveTouchEngine && !Disp->rd8(REG_CTOUCH_EXTENDED))
  {
    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH1_XY);
    mTouchEvent[1].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG1));
    update(Disp, 1, touchPos, msec);

    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH2_XY);
    mTouchEvent[2].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG2));
    update(Disp, 2, touchPos, msec);

    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH3_XY);
    mTouchEvent[3].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG3));
    update(Disp, 3, touchPos, msec);

    touchPos.x = Disp->rd16(REG_CTOUCH_TOUCH4_X);
    touchPos.y = Disp->rd16(REG_CTOUCH_TOUCH4_Y);
    mTouchEvent[4].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG4));
    update(Disp, 4, touchPos, msec);
  }

  touchPos.xy = Disp->rd32(REG_TOUCH_SCREEN_XY);
  mTouchEvent[0].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_TOUCH_TAG));
  update(Disp, 0, touchPos, msec);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouch::update(EvDisplay *Disp, uint8_t Id, EvTouchPos TouchPos, uint32_t msec)
{
  EvTouchEvent  *touch = &mTouchEvent[Id];

  if (touch->status & TOUCH_RELEASE)
  {
    if (!touch->tag)
      touch->status = 0;
    return;
  }

  touch->id = Id;
  touch->event = 0;
  touch->timer = msec - touch->startTimer;

  if (!touch->tag)
  {
    if ((touch->status & TOUCHING_FLAG) != 0 && touch->owner != nullptr)
    { // end & double event
      if (touch->timer > DOUBLE_DELAY)
        touch->status = 0;
      else
      {
        touch->status &= ~(TOUCHING_FLAG | REPEAT_FLAG);
        touch->status += DOUBLE_COUNT;

        if (touch->status & DOUBLE_FLAG)
        {
          touch->event = TOUCH_DOUBLE;

          if (mOnTouch != nullptr)
            mOnTouch(touch->owner, touch);

          if (touch->event)
            touch->owner->TouchUpdate(touch);

          touch->status = 0;
        }
      }

      touch->endTimer = msec;
      touch->event = TOUCH_END;
      touch->owner->TouchEnd();
    }
    else if (touch->status == 0 || touch->timer > DOUBLE_DELAY)
      touch->status = 0;
  }
  else
  {
    touch->abs.xy = TouchPos.xy;

    if ((touch->status & TOUCHING_FLAG) == 0)
    { // start event
      EvObj   *owner;

      touch->obj = nullptr;
      touch->owner = nullptr;

      if ((owner = Disp->Touching(touch)) != nullptr)
      {
        if (touch->timer > (DOUBLE_DELAY * 2))
          touch->status &= ~(DOUBLE_COUNT | DOUBLE_FLAG);

        touch->status |= TOUCHING_FLAG;

        if (!owner->TouchStart())
          touch->owner = nullptr;
        else
        {
          touch->owner = owner;
          touch->event = TOUCH_START;
          touch->x = touch->abs.x - touch->owner->View.ox;
          touch->y = touch->abs.y - touch->owner->View.oy;
          touch->prev.xy = touch->abs.xy;
          touch->move.xy = 0;
          touch->repeatDelay = REPEAT_DELAY;
          touch->repeatTimer = HOLD_DELAY;
          touch->startTimer = msec;
          touch->timer = 0;
        }
      }
    }
    else if (touch->owner != nullptr)
    { // hold, repeat and move event
      touch->x = touch->abs.x - touch->owner->View.ox;
      touch->y = touch->abs.y - touch->owner->View.oy;

      if (touch->timer >= touch->repeatTimer)
      {
        touch->event = (touch->status & REPEAT_FLAG) ? TOUCH_REPEAT : TOUCH_HOLD;
        touch->repeatTimer += touch->repeatDelay;
        touch->status |= REPEAT_FLAG;
      }
      else
      {
        touch->move.x = touch->abs.x - touch->prev.x;
        touch->move.y = touch->abs.y - touch->prev.y;

        if (touch->status & MOVE_FLAG)
        {
          touch->prev.xy = touch->abs.xy;

          if (touch->move.xy != 0)
            touch->event = TOUCH_MOVE;
        }
        else if (abs(touch->move.x) > 3 || abs(touch->move.y) > 3)
        {
          touch->prev.xy = touch->abs.xy;
          touch->status |= MOVE_FLAG;
          touch->event = TOUCH_MOVE;
        }
      }
    }
  }

  if (touch->event)
  {
    if (mOnTouch != nullptr)
      mOnTouch(touch->owner, touch);

    if (touch->event)
      touch->owner->TouchUpdate(touch);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
