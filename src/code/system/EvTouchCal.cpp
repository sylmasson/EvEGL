
#include    <EvGUI.h>

#define     DOT_COLOR     RGB555(   0,   0, 220)
#define     BG_COLOR      RGB555(   0,   0,   0)

#define     CANCEL_BTN    1
#define     RESTART_BTN   2
#define     SAVE_BTN      3
#define     TARGET_POINT  4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char   *DefaultText[5] = {"Touch Screen Calibration", "Calibration Completed", "Cancel", "Restart", "Save"};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTouchCal  *EvTouchCal::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTouchCal *)EvObj::TryCreate(new EvTouchCal(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTouchCal" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTouchCal::EvTouchCal(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mInd(0),
  mTouchTag(0),
  mTimer(0),
  mText(DefaultText),
  mOnExit(nullptr)
{
  mTargetX[0] = mTargetX[3] = mWidth / 10;
  mTargetX[1] = mTargetX[2] = mWidth - mTargetX[0];
  mTargetY[0] = mTargetY[2] = mHeight / 10;
  mTargetY[1] = mTargetY[3] = mHeight - mTargetY[0];
  mTargetSize = mPointSize = min(mWidth, mHeight) / 10;

  if (Disp->CapacitiveTouchEngine)
    Disp->wr8(REG_CTOUCH_EXTENDED, 1);

  Disp->Kbd->Close();
  BdColor(DOT_COLOR);
  BgColor(BG_COLOR);
  TextFont(25);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTouchCal::~EvTouchCal(void)
{
  if (Disp->CapacitiveTouchEngine)
    Disp->wr8(REG_CTOUCH_EXTENDED, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouchCal::SetOnExit(void (*OnExit)(EvTouchCal *Sender, bool Save))
{
  mOnExit = OnExit;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouchCal::SetText(const char *Text[])
{
  mText = Text;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouchCal::drawEvent(void)
{
  uint16_t  x1, x2, x3, y, w, h;

  w = max(mWidth, mHeight) / 8;
  h = w / 2;
  y = (mHeight / 2) + (h / 3);
  x2 = (mWidth / 2) - (w / 2);
  x1 = x2 - w - (w / 8);
  x3 = x2 + w + (w / 8);

  Disp->DrawText(0, (mHeight / 2) - h, mWidth, h, mText[mInd > 3], DOT_COLOR, mStyle.font + 2, CENTER);
  Disp->TagValue(CANCEL_BTN);
  Disp->FillRectangle(x1, y, w, h, mTouchTag == CANCEL_BTN ? RGB555(100, 0, 0) : RGB555(200, 0, 0), mHeight);
  Disp->DrawText(x1, y, w, h, mText[2], EV_BLACK, mStyle.font, CENTER);
  Disp->TagValue(RESTART_BTN);
  Disp->FillRectangle(x2, y, w, h, mTouchTag == RESTART_BTN ? RGB555(128, 85, 0) : RGB555(255, 165, 0), mHeight);
  Disp->DrawText(x2, y, w, h, mText[3], EV_BLACK, mStyle.font, CENTER);
  Disp->TagValue(SAVE_BTN);
  Disp->FillRectangle(x3, y, w, h, mInd > 3 ? (mTouchTag == SAVE_BTN ? RGB555(30, 100, 30) : RGB555(60, 200, 60)) : RGB555(100, 100, 100), mHeight);
  Disp->DrawText(x3, y, w, h, mText[4], EV_BLACK, mStyle.font, CENTER);

  if (mInd <= 3)
  {
    Disp->TagValue(255);
    Disp->LineWidth(8);
    Disp->Begin(LINES);
    Disp->ColorA(255);
    Disp->ColorRGB(DOT_COLOR);
    Disp->Vertex2i(0, mTargetY[mInd]);
    Disp->Vertex2i(mWidth, mTargetY[mInd]);
    Disp->Vertex2i(mTargetX[mInd], 0);
    Disp->Vertex2i(mTargetX[mInd], mHeight);
    Disp->Begin(POINTS);
    Disp->TagValue(TARGET_POINT);
    Disp->ColorMask(0);
    Disp->PointSize(mTargetSize << 4);
    Disp->Vertex2i(mTargetX[mInd], mTargetY[mInd]);
    Disp->ColorMask(0x0F);
    Disp->PointSize(mPointSize << 3);
    Disp->Vertex2i(mTargetX[mInd], mTargetY[mInd]);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouchCal::refreshEvent(void)
{
  uint16_t  msec, timer;

  if (mInd <= 3)
  {
    msec = millis();
    timer = msec - mTimer;

    if (timer > 700)
    {
      mTimer = msec;
      timer = 0;
    }

    mPointSize = mTargetSize - ((mTargetSize * timer) / 700);
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTouchCal::touchEvent(const EvTouchEvent *Touch)
{
  int32_t   mX, mY, oX, oY;

  switch (Touch->event)
  {
    case TOUCH_START:
    case TOUCH_MOVE:
      mtouchRawPos.xy = Disp->rd32(REG_TOUCH_RAW_XY);
      mTouchTag = (mtouchRawPos.xy & 0x80008000) ? 0 : Touch->tag;
      Modified();
      break;

    case TOUCH_END:
      switch (mTouchTag)
      {
        case TARGET_POINT:
          if (mInd <= 3)
          {
            if (Disp->Orientation & 2)
            {
              mTouchX[mInd] = mtouchRawPos.y;
              mTouchY[mInd] = mtouchRawPos.x;
            }
            else
            {
              mTouchX[mInd] = mtouchRawPos.x;
              mTouchY[mInd] = mtouchRawPos.y;
            }

            mInd++;
          }

          mTouchTag = 0;
          Modified();
          break;

        case SAVE_BTN:
          if (mInd <= 3)
          {
            mTouchTag = 0;
            break;
          }

          // Add Left/Top to Target point
          mTargetX[0] += mView.x1;
          mTargetX[1] += mView.x1;
          mTargetY[0] += mView.y1;
          mTargetY[1] += mView.y1;

          // Average Touch point
          mTouchX[0] = (mTouchX[0] + mTouchX[3]) / 2;
          mTouchX[1] = (mTouchX[1] + mTouchX[2]) / 2;
          mTouchY[0] = (mTouchY[0] + mTouchY[2]) / 2;
          mTouchY[1] = (mTouchY[1] + mTouchY[3]) / 2;

          oX = ((((mTargetX[0] * mTouchX[1]) - (mTouchX[0] * mTargetX[1])) * 256L) / (mTouchX[1] - mTouchX[0])) * 256L;
          oY = ((((mTargetY[0] * mTouchY[1]) - (mTouchY[0] * mTargetY[1])) * 256L) / (mTouchY[1] - mTouchY[0])) * 256L;
          mX = ((mTargetX[1] * 65536L) - oX) / mTouchX[1];
          mY = ((mTargetY[1] * 65536L) - oY) / mTouchY[1];

          if (Disp->Orientation & 2)
          {
            Matrix[0] = 0;
            Matrix[1] = mX;
            Matrix[2] = oX;
            Matrix[3] = mY;
            Matrix[4] = 0;
            Matrix[5] = oY;
          }
          else
          {
            Matrix[0] = mX;
            Matrix[1] = 0;
            Matrix[2] = oX;
            Matrix[3] = 0;
            Matrix[4] = mY;
            Matrix[5] = oY;
          }

          for (int32_t i = 0, r = REG_TOUCH_TRANSFORM_A; i < 6; i++, r += 4)
            Disp->wr32(r, Matrix[i]);

        case CANCEL_BTN:
          if (mOnExit != nullptr)
            mOnExit(this, mTouchTag == SAVE_BTN);

          Delete();
          break;

        case RESTART_BTN:
          mTouchTag = 0;
          mInd = 0;
          break;
      }
  }
}
