
#include    <EvGUI.h>

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
  return !Dest ? nullptr : (EvSideBar *)EvObj::TryCreate(new EvSideBar(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvSideBar" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSideBar::EvSideBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvPanel(Left, Top, Width, Height, Disp, Tag, State)
{
  Setup(SIDEBAR_NONE);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Open(void)
{
  if (!IsViewable())
  {
    int16_t delta = 0;

    switch (mType)
    {
      case SIDEBAR_LEFT:
      case SIDEBAR_RIGHT: delta = mOpenLeft - mLeft; break;
      case SIDEBAR_TOP:
      case SIDEBAR_BOTTOM: delta = mOpenTop - mTop; break;
    }

    mKinMotion.Start(delta);
    Show();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Close(void)
{
  if (IsViewable())
  {
    int16_t delta = 0;

    switch (mType)
    {
      case SIDEBAR_LEFT: delta = -mLeft - mWidth - 1; break;
      case SIDEBAR_RIGHT: delta = GetOwner()->Width() - mLeft + 1; break;
      case SIDEBAR_TOP: delta = -mTop - mHeight - 1; break;
      case SIDEBAR_BOTTOM: delta = GetOwner()->Height() - mTop + 1; break;
    }

    mKinMotion.Start(delta);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Setup(uint8_t Type, uint16_t Period)
{
  Setup(mLeft, mTop, Type, Period);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::Setup(int16_t Left, int16_t Top, uint8_t Type, uint16_t Period)
{
  mKinMotion.Setup(Period);
  mOpenLeft = Left;
  mOpenTop = Top;

  if (!IsViewable())
    switch (mType = Type)
    {
      case SIDEBAR_LEFT: Left = -mWidth; break;
      case SIDEBAR_RIGHT: Left = GetOwner()->Width(); break;
      case SIDEBAR_TOP: Top = -mHeight; break;
      case SIDEBAR_BOTTOM: Top = GetOwner()->Height(); break;
    }

  MoveTo(Left, Top);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSideBar::refreshEvent(void)
{
  if (mKinMotion.IsEnabled())
  {
    int16_t left = mOpenLeft;
    int16_t top = mOpenTop;

    switch (mType)
    {
      case SIDEBAR_LEFT:
      case SIDEBAR_RIGHT: left = mLeft + mKinMotion.Value(); break;
      case SIDEBAR_TOP:
      case SIDEBAR_BOTTOM: top = mTop + mKinMotion.Value(); break;
    }

    MoveTo(left, top);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
