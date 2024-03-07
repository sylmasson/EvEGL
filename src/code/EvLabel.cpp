
#include    <EvGUI.h>

#define     TEXT_COLOR      RGB555(255, 255, 255)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard Label.
 * 
 * A new standard Label is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of Label.
 * @param[in]  Top     The top position of Label.
 * @param[in]  Width   The width of Label.
 * @param[in]  Height  The height of Label.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Label. If nullptr, the default tag name is "EvLabel".
 * @param[in]  State   The initial state of the Label. Default is set to VISIBLE_OBJ.
 *
 * @return     EvLabel address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvLabel" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel::EvLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  TextFont(25);
  TextLabel("Label");
  TextAlign(RIGHT_CENTER);
  TextColor(TEXT_COLOR);
  SetOnTouch(nullptr);
  mTouchFlag = false;
}

/// @copydoc EvButton::SetOnTouch()

void        EvLabel::SetOnTouch(void (*OnTouch)(EvLabel *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::drawEvent(void)
{
  DrawText(0, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    (*mOnTouch)(this, Touch);

  switch (Touch->event)
  {
    case TOUCH_START:
      mTouchFlag = true;
      Touch->event = 0;
      break;

    case TOUCH_CANCEL:
    case TOUCH_END:
      mTouchFlag = false;
      Touch->event = 0;
      break;
  }
}
