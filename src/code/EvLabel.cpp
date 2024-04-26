
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Width   The width of **EvLabel**.
 * @param[in]  Height  The height of **EvLabel**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvLabel" : Tag, State), Dest)) != nullptr)
  {
    obj->TextLabel(obj->Tag);
    obj->TextAlign(RIGHT_CENTER);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Label   The object **EvLabel**.
 * @param[in]  Font    The font size of **EvLabel**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, const char *Label, uint8_t Font, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, 0, 0, Dest->Disp, !Tag ? (!Label ? "EvLabel" : Label) : Tag, State), Dest)) != nullptr)
  {
    obj->TextFont(Font);
    obj->TextLabel(Label);
    obj->mHeight = 10 * ((obj->TextHeight(Font) + 5) / 10);
    obj->mWidth = obj->TextWidth(Label);
    obj->SetView();
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvLabel**.
 * 
 * A new standard **EvLabel** is created at relative position of its owner Dest.
 * 
 * @param[in]  Left    The left position of **EvLabel**.
 * @param[in]  Top     The top position of **EvLabel**.
 * @param[in]  Label   The object **EvLabel**.
 * @param[in]  *Src    The address pointer of the **EvLabel**. source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvLabel**. If nullptr, the default tag name is **"EvLabel"**.
 * @param[in]  State   The initial state of the **EvLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel     *EvLabel::Create(int16_t Left, int16_t Top, const char *Label, const EvLabel *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvLabel   *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvLabel *)EvObj::TryCreate(new EvLabel(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, !Tag ? (!Label ? "EvLabel" : Label) : Tag, State), Dest)) != nullptr)
  {
    obj->TextLabel(Label);
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mBdWidth = Src->mBdWidth;
    obj->mBdColor = Src->mBdColor;
    obj->mWidth = obj->TextWidth(Label);
    obj->SetView();
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvLabel::EvLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mTouchFlag(false),
  mOnTouch(nullptr)
{
}

/// @copydoc EvButton::SetOnTouch()

void        EvLabel::SetOnTouch(void (*OnTouch)(EvLabel *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::drawEvent(void)
{
  DrawText(0, 0, mWidth, mHeight, c_str(Label));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvLabel::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

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
