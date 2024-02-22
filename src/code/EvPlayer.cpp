
#include    <EvGUI.h>

#define     HIDE_DELAY      3000

#define     TIMELINE_LOWER  RGB555(230,   0,   0)
#define     TIMELINE_UPPER  RGB555(230, 230, 230)
#define     TIMELINE_KNOB   RGB555(230, 230, 230)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnLoadFrame(EvVideo *Sender, uint32_t FrameNbr);
static void OnChangeTimeline(EvSlider *Sender, int32_t Value);
static void OnTouchTimeline(EvSlider *Sender, EvTouchEvent *Touch);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPlayer    *EvPlayer::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvPlayer *)EvObj::TryCreate(new EvPlayer(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvPlayer" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPlayer::EvPlayer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  if (!(Video = EvVideo::Create(0, 0, mWidth, mHeight, this, "VideoFrame", VISIBLE_DIS_OBJ)) ||
      !(TopBar = EvSideBar::Create(0, 0, mWidth, 40, this, "TopBar", DISABLED_OBJ)) ||
      !(BottomBar = EvSideBar::Create(0, 0, mWidth, 40, this, "BottomBar", DISABLED_OBJ)) ||
      !(Timelapse = EvLabel::Create(0, 0, 170, 40, TopBar, "Timelapse", VISIBLE_DIS_OBJ)) ||
      !(Timeline = EvSlider::Create(0, 0, mWidth - 20, 30, BottomBar, "Timeline", VISIBLE_OBJ)))
  {
    Abort();
    return;
  }

  mLock = false;
  mTimerHide = 0;
  mTouchCount = 0;
  Video->SetOnLoadFrame(OnLoadFrame);

  TopBar->BgColor(RGB555(0, 0, 0), 128);
  BottomBar->BgColor(RGB555(0, 0, 0), 128);

  Timelapse->TextAlign(CENTER);
  Timelapse->TextColor(TIMELINE_UPPER);
  Timelapse->TextLabel("");
  Timelapse->TextFont(26);

  Timeline->SetColor(TIMELINE_LOWER, TIMELINE_UPPER, TIMELINE_KNOB);
  Timeline->BdShape(RATIO_CORNERS);
  Timeline->SetOnChange(OnChangeTimeline);
  Timeline->SetOnTouch(OnTouchTimeline);
  Timeline->SetRange(0, 1);
  Timeline->SetDelay(0);

  BgColor(RGB555(20, 20, 20));
  SetOnTouch(nullptr);
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Open(const char *FileName, SDClass &Dev)
{
  if (Video == nullptr || !Video->Open(FileName, Dev))
    return false;

  Timeline->SetRange(0, Video->AviInfo.FrameCount);
  Timeline->SetValue(0);
  resizeEvent();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::Close(void)
{
  if (Video != nullptr)
    Video->Close();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::ShowInfo(void)
{
  TopBar->Open();
  BottomBar->Open();
  mTimerHide = millis_nz();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::HideInfo(void)
{
  TopBar->Close();
  BottomBar->Close();
  mTimerHide = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::TouchInfo(EvTouchEvent *Touch)
{
  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchCount++;
      break;

    case TOUCH_END:
      mTimerHide = millis_nz();
      mTouchCount--;

    case TOUCH_HOLD:
      ShowInfo();
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::SetOnTouch(void (*OnTouch)(EvPlayer *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::resize(void)
{
  Video->ReSize(mWidth, mHeight);
  Video->OwnerAlign(CENTER);

  TopBar->ReSize(mWidth, 40);
  TopBar->OwnerAlign(CENTER_TOP);
  TopBar->Setup(SIDEBAR_TOP);
  Timelapse->OwnerAlign(CENTER);

  BottomBar->ReSize(mWidth, 40);
  BottomBar->OwnerAlign(CENTER_BOTTOM);
  BottomBar->Setup(SIDEBAR_BOTTOM);
  Timeline->ReSize(mWidth - 20, 30);
  Timeline->OwnerAlign(CENTER, 0, 5);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::resizeEvent(void)
{
  if (!mLock && Video != nullptr)
  {
    mLock = true;
//    Serial.println("Resize");
//    Serial.printf("mWidth  = %d  mHeight  = %d\n", mWidth, mHeight);
//    Serial.printf("Width() = %d  Height() = %d\n", Video->Width(), Video->Height());
    resize();
    mLock = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::refreshEvent(void)
{
  if (!mTouchCount && mTimerHide)
  {
    uint16_t  msec = millis() - mTimerHide;

    if (msec > HIDE_DELAY)
      HideInfo();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    (*mOnTouch)(this, Touch);

  TouchInfo(Touch);
  Touch->event = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnLoadFrame(EvVideo *Sender, uint32_t FrameNbr)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner();
  float     FrameRate = player->Video->AviInfo.FrameRate;
  uint32_t  FrameCount = player->Video->AviInfo.FrameCount;

  player->Timeline->SetValue(FrameNbr);

  if (FrameRate > 0)
  {
    char      str[40];
    uint16_t  time, length;

    time = (float)FrameNbr / FrameRate;
    length = (float)FrameCount / FrameRate;
    snprintf(str, sizeof(str) - 1, "%0u:%02u / %0u:%02u", time / 60, time % 60, length / 60, length % 60);
    player->Timelapse->TextLabel(str);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeTimeline(EvSlider *Sender, int32_t Value)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  player->Video->Seek(Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnTouchTimeline(EvSlider *Sender, EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  player->TouchInfo(Touch);
}
