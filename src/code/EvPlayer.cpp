
#include    <EvGUI.h>

#define     HIDE_DELAY      3000

#define     TIMELINE_KNOB   RGB555(230, 230, 230)
#define     TIMELINE_LOWER  RGB555(230,   0,   0)
#define     TIMELINE_UPPER  TIMELINE_KNOB

#define     PLAYBUTTON_BG   RGB555(  0,   0,   0)
#define     PLAYBUTTON_ICON TIMELINE_KNOB

#define     SIDEBAR_HEIGHT  50

#define     PLAYBUTTON      0
#define     FULLBUTTON      1
#define     SPEEDBUTTON     2
#define     TIMELAPSE       3

#define     BG_COLOR        RGB555(20, 20, 20)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnLoadFrame(EvVideo *Sender, uint32_t FrameNbr);
static void sOnChangeTimeLine(EvSlider *Sender, int32_t Value);
static void sOnTouchTimeLine(EvSlider *Sender, const EvTouchEvent *Touch);
static void sOnTouchTimeLapse(EvLabel *Sender, const EvTouchEvent *Touch);
static void sOnTouchPlayButton(EvLabel *Sender, const EvTouchEvent *Touch);
static void sOnTouchFullButton(EvLabel *Sender, const EvTouchEvent *Touch);
static void sOnTouchSpeedButton(EvLabel *Sender, const EvTouchEvent *Touch);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPlayer    *EvPlayer::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvPlayer *)EvObj::TryCreate(new EvPlayer(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvPlayer" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPlayer::EvPlayer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  if (!(Video = EvVideo::Create(0, 0, mWidth, mHeight, this, "VideoFrame", VISIBLE_DIS_OBJ)) ||
      !(TopBar = EvSideBar::Create(0, 0, mWidth, SIDEBAR_HEIGHT, this, "TopBar", DISABLED_OBJ)) ||
      !(BottomBar = EvSideBar::Create(0, 0, mWidth, SIDEBAR_HEIGHT, this, "BottomBar", DISABLED_OBJ)) ||
      !(TimeLine = EvSlider::Create(0, 0, mWidth - 20, SIDEBAR_HEIGHT - 16, BottomBar, "TimeLine", VISIBLE_OBJ)) ||
      !(TimeLapse = (Button *)TryCreate(new Button(mWidth, SIDEBAR_HEIGHT, Disp, "TimeLapse"), TopBar)) ||
      !(PlayButton = (Button *)TryCreate(new Button(110, 110, Disp, "PlayButton"), this)) ||
      !(FullButton = (Button *)TryCreate(new Button(60, SIDEBAR_HEIGHT, Disp, "FullButton"), BottomBar)) ||
      !(SpeedButton = (Button *)TryCreate(new Button(60, SIDEBAR_HEIGHT, Disp, "SpeedButton"), BottomBar)))
  {
    abortCreate();
    return;
  }

  mRun = false;
  mLock = false;
  mSpeedIndex = 0;
  mTimerHide = 0;
  mTouchCount = 0;
  mSmallOwner = nullptr;
  Video->SetOnLoadFrame(sOnLoadFrame);

  TopBar->BgColor(RGB555(0, 0, 0), 128);
  TopBar->OwnerAlign(CENTER_TOP);
  TopBar->Setup(SIDEBAR_TOP);

  BottomBar->BgColor(RGB555(0, 0, 0), 128);
  BottomBar->OwnerAlign(CENTER_BOTTOM);
  BottomBar->Setup(SIDEBAR_BOTTOM);

  TimeLine->SetColor(TIMELINE_LOWER, TIMELINE_UPPER, TIMELINE_KNOB);
  TimeLine->BdShape(RATIO_CORNERS);
  TimeLine->SetOnChange(sOnChangeTimeLine);
  TimeLine->SetOnTouch(sOnTouchTimeLine);
  TimeLine->SetRange(0, 1);
  TimeLine->SetDelay(0);

  TimeLapse->TextFont(26);
  TimeLapse->TextLabel("");
  TimeLapse->TextAlign(CENTER);
  TimeLapse->TextColor(TIMELINE_KNOB);
  TimeLapse->SetOnTouch(sOnTouchTimeLapse);
  TimeLapse->TagId = TIMELAPSE;

  PlayButton->Hide();
  PlayButton->BdShape(ROUND_CORNERS);
  PlayButton->SetOnTouch(sOnTouchPlayButton);
  PlayButton->TagId = PLAYBUTTON;

  FullButton->SetOnTouch(sOnTouchFullButton);
  FullButton->TagId = FULLBUTTON;

  SpeedButton->TextFont(25);
  SpeedButton->TextLabel("1 X");
  SpeedButton->TextAlign(CENTER);
  SpeedButton->TextColor(TIMELINE_KNOB);
  SpeedButton->SetOnTouch(sOnTouchSpeedButton);
  SpeedButton->TagId = SPEEDBUTTON;

  BgColor(BG_COLOR);
  resize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Open(const char *FileName, SDClass &Dev)
{
  if (!Video->Open(FileName, Dev))
    return false;

  TimeLine->SetRange(0, Video->AviInfo.FrameCount);
  TimeLine->SetValue(0);
  resize();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::Close(void)
{
  Video->Close();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Play(void)
{
  PlayButton->Modified();
  return Video->Play();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Stop(void)
{
  PlayButton->Modified();
  return Video->Stop();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Pause(void)
{
  PlayButton->Modified();
  return Video->Pause();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::Faster(void)
{
  char      str[10];
  static uint8_t  speedTable[4] = {1, 2, 5, 10};

  if (++mSpeedIndex >= sizeof(speedTable))
    mSpeedIndex = 0;

  if (!Video->Forward(speedTable[mSpeedIndex]))
    return false;

  snprintf(str, sizeof(str) - 1, "%u X", speedTable[mSpeedIndex]);
  SpeedButton->TextLabel(str);
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::ShowInfo(void)
{
  TopBar->Open();
  BottomBar->Open();
  PlayButton->Show();
  mTimerHide = millis_nz();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::HideInfo(void)
{
  TopBar->Close();
  BottomBar->Close();
  PlayButton->Hide();
  mTouchCount = 0;
  mTimerHide = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::ScreenSize(void)
{
  if (!IsFullScreen())
    FullScreen();
  else
    SmallScreen();

  Video->SetOpacity(OPACITY_MAX);
  BgColor(BG_COLOR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::FullScreen(void)
{
  if (!IsFullScreen())
  {
    Disp->KbdDelete();
    mSmallLeft = mLeft;
    mSmallTop = mTop;
    mSmallWidth = mWidth;
    mSmallHeight = mHeight;

    if ((mSmallOwner = mOwner) != Disp)
      MoveTo(Disp);

    ToFront();
    MoveTo(0, 0);
    ReSize(Disp->Width(), Disp->Height());
    FullButton->Modified();
    mStatus |= FULLSCREEN_OBJ;
    mMovable = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::SmallScreen(void)
{
  if (IsFullScreen())
  {
    if (mSmallOwner != nullptr && mSmallOwner != Disp)
      MoveTo(mSmallOwner);

    MoveTo(mSmallLeft, mSmallTop);
    ReSize(mSmallWidth, mSmallHeight);
    FullButton->Modified();
    mStatus &= ~FULLSCREEN_OBJ;
    mMovable = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::IsRunning(void)
{
  return Video->IsRunning();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPlayer::IsMovable(void)
{
  return mMovable;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::SetMovable(bool Value)
{
  mMovable = Value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::TouchInfo(const EvTouchEvent *Touch)
{
  switch (Touch->event)
  {
    case TOUCH_START:
      Touch->repeatTimer = 100;
      mTouchCount++;
      break;

    case TOUCH_END:
      if (!mTouchCount)
        break;

      mTouchCount--;

    case TOUCH_HOLD:
      ShowInfo();
      break;

    case TOUCH_CANCEL:
      HideInfo();
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::resizeEvent(void)
{
  if (!mLock)
  {
    mLock = true;
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

  if (mRun != IsRunning())
  {
    PlayButton->Modified();
    mRun = !mRun;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::touchEvent(const EvTouchEvent *Touch)
{
  if (Touch->event == TOUCH_END && PlayButton->IsVisible() && mTouchCount <= 1)
    HideInfo();
  else
    TouchInfo(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::Button::drawEvent(void)
{
  uint16_t  x, y;

  switch (TagId)
  {
    case TIMELAPSE:
    case SPEEDBUTTON:
      EvLabel::drawEvent();
      break;

    case FULLBUTTON:
      x = (mWidth - 24) / 2;
      y = (mHeight - 24) / 2;

      Disp->ColorRGB(TIMELINE_KNOB);
      Disp->LineWidth(24);
      Disp->Begin(LINES);

      if (!((EvPlayer *)GetOwner(2))->IsFullScreen())
      { // Full screen button
        Disp->Vertex2i(x +  0, y +  7);
        Disp->Vertex2i(x +  0, y +  0);
        Disp->Vertex2i(x +  0, y +  0);
        Disp->Vertex2i(x +  7, y +  0);

        Disp->Vertex2i(x + 17, y +  0);
        Disp->Vertex2i(x + 24, y +  0);
        Disp->Vertex2i(x + 24, y +  0);
        Disp->Vertex2i(x + 24, y +  7);

        Disp->Vertex2i(x + 24, y + 17);
        Disp->Vertex2i(x + 24, y + 24);
        Disp->Vertex2i(x + 24, y + 24);
        Disp->Vertex2i(x + 17, y + 24);

        Disp->Vertex2i(x +  7, y + 24);
        Disp->Vertex2i(x +  0, y + 24);
        Disp->Vertex2i(x +  0, y + 24);
        Disp->Vertex2i(x +  0, y + 17);
      }
      else
      { // Small screen button
        Disp->Vertex2i(x +  0, y +  7);
        Disp->Vertex2i(x +  7, y +  7);
        Disp->Vertex2i(x +  7, y +  7);
        Disp->Vertex2i(x +  7, y +  0);

        Disp->Vertex2i(x + 17, y +  0);
        Disp->Vertex2i(x + 17, y +  7);
        Disp->Vertex2i(x + 17, y +  7);
        Disp->Vertex2i(x + 24, y +  7);

        Disp->Vertex2i(x + 24, y + 17);
        Disp->Vertex2i(x + 17, y + 17);
        Disp->Vertex2i(x + 17, y + 17);
        Disp->Vertex2i(x + 17, y + 24);

        Disp->Vertex2i(x +  7, y + 24);
        Disp->Vertex2i(x +  7, y + 17);
        Disp->Vertex2i(x +  7, y + 17);
        Disp->Vertex2i(x +  0, y + 17);
      }
      break;

    case PLAYBUTTON:
      x = (mWidth - 40) / 2;
      y = (mHeight - 70) / 2;
      BgColor(PLAYBUTTON_BG, mTouchFlag ? 200 : 128);

      if (((EvPlayer *)GetOwner())->IsRunning())
      { // Pause button
        Disp->ColorRGB(TIMELINE_KNOB);
        Disp->LineWidth(16);
        Disp->Begin(RECTS);
        Disp->Vertex2i(x +  0, y +  0);
        Disp->Vertex2i(x + 10, y + 70);
        Disp->Vertex2i(x + 30, y +  0);
        Disp->Vertex2i(x + 40, y + 70);
      }
      else
      { // Play button
        Disp->ColorRGB(TIMELINE_KNOB);
        Disp->StencilOp(INCR, INCR);
        Disp->ColorMask(0, 0, 0, 0);
        Disp->Begin(EDGE_STRIP_L);
        Disp->Vertex2i(x +  0, y +  0);
        Disp->Vertex2i(x + 50, y + 35);
        Disp->Vertex2i(x +  0, y + 70);
        Disp->Vertex2i(x +  0, y +  0);

        Disp->ColorMask(1, 1, 1, 1);
        Disp->StencilFunc(EQUAL, 1, 255);
        Disp->Begin(EDGE_STRIP_L);
        Disp->Vertex2i(x + 50, y +  0);
        Disp->Vertex2i(x + 50, y + 70);

        Disp->StencilFunc(ALWAYS, 0, 255);
        Disp->LineWidth(16);
        Disp->Begin(LINE_STRIP);
        Disp->Vertex2i(x +  0, y +  0);
        Disp->Vertex2i(x + 50, y + 35);
        Disp->Vertex2i(x +  0, y + 70);
        Disp->Vertex2i(x +  0, y +  0);
      }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPlayer::resize(void)
{
  Video->ReSize(mWidth, mHeight);
  Video->OwnerAlign(CENTER);

  TopBar->ReSize(mWidth, SIDEBAR_HEIGHT);
  TopBar->SetPosition();
  TimeLapse->ReSize(mWidth, SIDEBAR_HEIGHT);
  TimeLapse->OwnerAlign(CENTER);

  BottomBar->ReSize(mWidth, SIDEBAR_HEIGHT);
  BottomBar->SetPosition();
  TimeLine->ReSize(mWidth - SpeedButton->Width() - FullButton->Width(), SIDEBAR_HEIGHT - 16);
  TimeLine->OwnerAlign(CENTER, 0, 5);

  PlayButton->OwnerAlign(CENTER);
  FullButton->OwnerAlign(RIGHT_CENTER);
  SpeedButton->OwnerAlign(LEFT_CENTER);

  FullButton->Modified();
  mStatus &= ~FULLSCREEN_OBJ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnLoadFrame(EvVideo *Sender, uint32_t FrameNbr)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner();
  float     FrameRate = player->Video->AviInfo.FrameRate;
  uint32_t  FrameCount = player->Video->AviInfo.FrameCount;

  player->TimeLine->SetValue(FrameNbr);

  if (FrameRate > 0)
  {
    char      str[40];
    uint16_t  time, length;

    time = (float)FrameNbr / FrameRate;
    length = (float)FrameCount / FrameRate;
    snprintf(str, sizeof(str) - 1, "%0u:%02u / %0u:%02u", time / 60, time % 60, length / 60, length % 60);
    player->TimeLapse->TextLabel(str);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnChangeTimeLine(EvSlider *Sender, int32_t Value)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  player->Video->Seek(Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnTouchTimeLine(EvSlider *Sender, const EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  player->TouchInfo(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnTouchTimeLapse(EvLabel *Sender, const EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  if (player->IsFullScreen() == false)
    switch (Touch->event)
    {
      case TOUCH_START:
        player->Video->SetOpacity(128);
        player->BgColor(BG_COLOR, 64);
        player->SetMovable(true);
        player->ToFront();
        break;

      case TOUCH_END:
        player->Video->SetOpacity(OPACITY_MAX);
        player->BgColor(BG_COLOR);
        break;

      case TOUCH_MOVE:
        if (player->IsMovable())
          player->MoveRel(Touch->move.x, Touch->move.y);
        break;
    }

  player->TouchInfo(Touch);
  Touch->event = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnTouchPlayButton(EvLabel *Sender, const EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner();

  switch (Touch->event)
  {
    case TOUCH_START:
      Sender->Modified();
      break;

    case TOUCH_END:
      if (player->IsRunning())
        player->Pause();
      else
        player->Play();

      break;
  }

  player->TouchInfo(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnTouchFullButton(EvLabel *Sender, const EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  if (Touch->event == TOUCH_END)
    player->ScreenSize();

  player->TouchInfo(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnTouchSpeedButton(EvLabel *Sender, const EvTouchEvent *Touch)
{
  EvPlayer  *player = (EvPlayer *)Sender->GetOwner(2);

  if (Touch->event == TOUCH_END)
    player->Faster();

  player->TouchInfo(Touch);
}
