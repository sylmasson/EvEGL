
#include    <EvEGL.h>

// #define     SIDEBAR_DEBUG   SIDEBAR_RIGHT

const char * const EvSideBar::TypeName = "EvSideBar";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard SideBar.
 * 
 * A new standard SideBar is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the SideBar.
 * @param[in]  Top     The top position of the SideBar.
 * @param[in]  Width   The width of the SideBar.
 * @param[in]  Height  The height of the SideBar.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the SideBar. If nullptr, the default tag name is "EvSideBar".
 * @param[in]  State   The initial state of the SideBar. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     EvSideBar address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSideBar   *EvSideBar::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvSideBar *)EvObj::TryCreate(new EvSideBar(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSideBar::EvSideBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvPanel(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State)
{
  Setup(SIDEBAR_TOP);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Open(void)
{
  if (!mOpen)
  {
    int16_t pixel = 0;

    switch (mType)
    {
      case SIDEBAR_LEFT: pixel = mPadX - mLeft; mShift = -pixel; break;
      case SIDEBAR_TOP: pixel = mPadY - mTop; mShift = -pixel; break;
      case SIDEBAR_RIGHT: pixel = GetOwner()->Width() - mPadX - mWidth - mLeft; mShift = 0; break;
      case SIDEBAR_BOTTOM: pixel = GetOwner()->Height() - mPadY - mHeight - mTop; mShift = 0; break;
    }

    #ifdef SIDEBAR_DEBUG
      if (mType == SIDEBAR_DEBUG)
        EvOut->printf("SideBar Open: pixel = %d\n", pixel);
    #endif

    mKinMotion.Start(pixel);
    mOpen = true;
    Show();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Close(void)
{
  if (mOpen)
  {
    int16_t pixel = 0;

    switch (mType)
    {
      case SIDEBAR_LEFT: pixel = -mLeft - mWidth - 1; mShift = 0; break;
      case SIDEBAR_TOP: pixel = -mTop - mHeight - 1; mShift = 0; break;
      case SIDEBAR_RIGHT: pixel = GetOwner()->Width() - mLeft + 1; mShift = -pixel; break;
      case SIDEBAR_BOTTOM: pixel = GetOwner()->Height() - mTop + 1; mShift = -pixel; break;
    }

    #ifdef SIDEBAR_DEBUG
      if (mType == SIDEBAR_DEBUG)
        EvOut->printf("SideBar Close: pixel = %d\n", pixel);
    #endif

    mKinMotion.Start(pixel);
    mOpen = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSideBar::IsOpen(void)
{
  return mOpen;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::SetPosition(void)
{
  int16_t   left, top;

  if (mKinMotion.IsEnabled())
  {
    left = mLeft;
    top = mTop;

    switch (mType)
    {
      case SIDEBAR_LEFT: left = mShift; break;
      case SIDEBAR_TOP: top = mShift; break;
      case SIDEBAR_RIGHT: left = GetOwner()->Width() + mShift; break;
      case SIDEBAR_BOTTOM: top = GetOwner()->Height() + mShift; break;
    }
  }
  else
  {
    left = mPadX;
    top = mPadY;

    if (mOpen)
      switch (mType)
      {
        case SIDEBAR_LEFT: break;
        case SIDEBAR_TOP:  break;
        case SIDEBAR_RIGHT: left = GetOwner()->Width() - mPadX - mWidth; break;
        case SIDEBAR_BOTTOM: top = GetOwner()->Height() - mPadY - mHeight; break;
      }
    else
      switch (mType)
      {
        case SIDEBAR_LEFT: left = -mWidth - 1; break;
        case SIDEBAR_TOP:  top = -mHeight - 1; break;
        case SIDEBAR_RIGHT: left = GetOwner()->Width() + 1; break;
        case SIDEBAR_BOTTOM: top = GetOwner()->Height() + 1; break;
      }
  }

  #ifdef SIDEBAR_DEBUG
    if (mType == SIDEBAR_DEBUG)
      EvOut->printf("SideBar SetPosition: left = %d top = %d\n", left, top);
  #endif

  MoveTo(left, top);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::SetPadding(int16_t PadX, int16_t PadY)
{
  mPadX = PadX;
  mPadY = PadY;
  SetPosition();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Setup(uint8_t Type, bool OpenState, uint16_t Period)
{
  Setup(0, 0, Type, OpenState, Period);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Setup(int16_t PadX, int16_t PadY, uint8_t Type, bool OpenState, uint16_t Period)
{
  mShift = 0;
  mPadX = PadX;
  mPadY = PadY;
  mOpen = OpenState;
  mType = (Type > SIDEBAR_BOTTOM) ? SIDEBAR_TOP : Type;
  mKinMotion.Setup(Period);
  SetPosition();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::resizeEvent(void)
{
  SetPosition();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::refreshEvent(void)
{
  if (mKinMotion.IsEnabled())
  {
    mShift += mKinMotion.Value();
    SetPosition();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
