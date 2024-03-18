
#include    <EvGUI.h>

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

EvTouch::EvTouch(void)
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
    mTouchEvent[1].id = 1;
    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH1_XY);
    mTouchEvent[1].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG1));
    update(Disp, &mTouchEvent[1], touchPos, msec);

    mTouchEvent[2].id = 2;
    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH2_XY);
    mTouchEvent[2].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG2));
    update(Disp, &mTouchEvent[2], touchPos, msec);

    mTouchEvent[3].id = 3;
    touchPos.xy = Disp->rd32(REG_CTOUCH_TOUCH3_XY);
    mTouchEvent[3].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG3));
    update(Disp, &mTouchEvent[3], touchPos, msec);

    mTouchEvent[4].id = 4;
    touchPos.x = Disp->rd16(REG_CTOUCH_TOUCH4_X);
    touchPos.y = Disp->rd16(REG_CTOUCH_TOUCH4_Y);
    mTouchEvent[4].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_CTOUCH_TAG4));
    update(Disp, &mTouchEvent[4], touchPos, msec);
  }

  mTouchEvent[0].id = 0;
  touchPos.xy = Disp->rd32(REG_TOUCH_SCREEN_XY);
  mTouchEvent[0].tag = ((touchPos.xy & 0x80008000) ? 0 : Disp->rd8(REG_TOUCH_TAG));
  update(Disp, &mTouchEvent[0], touchPos, msec);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouch::update(EvDisplay *Disp, EvTouchEvent *Touch, EvTouchPos TouchPos, uint32_t msec)
{
  if (Touch->status & TOUCH_RELEASE)
  {
    if (!Touch->tag)
      Touch->status = 0;
    return;
  }

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
            mOnTouch(Touch->owner, Touch);

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

      if ((owner = Disp->Touching(Touch)) != nullptr)
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
      mOnTouch(Touch->owner, Touch);

    if (Touch->event)
      Touch->owner->TouchUpdate(Touch);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
