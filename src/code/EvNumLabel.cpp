
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvNumLabel**.
 * 
 * A new **EvNumLabel** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvNumLabel**.
 * @param[in]  Top     The top position of the **EvNumLabel**.
 * @param[in]  Width   The width of the **EvNumLabel**.
 * @param[in]  Height  The height of the **EvNumLabel**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvNumLabel**. If nullptr, the default tag name is **"EvNumLabel"**.
 * @param[in]  State   The initial state of the **EvNumLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvNumLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumLabel  *EvNumLabel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvNumLabel  *obj = nullptr;

  if (Dest != nullptr && (obj = (EvNumLabel *)EvObj::TryCreate(new EvNumLabel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvNumLabel" : Tag, State), Dest)) != nullptr)
  {
    obj->BdShape(FIXED_CORNERS);
    obj->TextAlign(RIGHT_CENTER);
    obj->TextPadding(5, 0);
    obj->SetValue(0);
  }

  return obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvNumLabel**.
 * 
 * A new **EvNumLabel** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvNumLabel**.
 * @param[in]  Top     The top position of the **EvNumLabel**.
 * @param[in]  Value   The value of the **EvNumLabel**.
 * @param[in]  *Src    The address pointer of the **EvNumLabel** source model.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvNumLabel**. If nullptr, the default tag name is **"EvNumLabel"**.
 * @param[in]  State   The initial state of the **EvNumLabel**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvNumLabel** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumLabel  *EvNumLabel::Create(int16_t Left, int16_t Top, float Value, const EvNumLabel *Src, EvPanel *Dest, const char *Tag, uint16_t State)
{
  EvNumLabel  *obj = nullptr;

  if (Dest != nullptr && Src != nullptr && (obj = (EvNumLabel *)EvObj::TryCreate(new EvNumLabel(Left, Top, Src->mWidth, Src->mHeight, Dest->Disp, !Tag ? "EvNumLabel" : Tag, State), Dest)) != nullptr)
  {
    obj->mStyle = Src->mStyle;
    obj->mOpacity = Src->mOpacity;
    obj->mBgColor = Src->mBgColor;
    obj->mBgColorA = Src->mBgColorA;
    obj->mBdShape = Src->mBdShape;
    obj->mBdRadius = Src->mBdRadius;
    obj->mBdWidth = Src->mBdWidth;
    obj->mBdColor = Src->mBdColor;
    obj->mFormat = Src->mFormat;
    obj->printValue(obj->mValue = Value);
  }

  return obj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumLabel::EvNumLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvLabel(Left, Top, Width, Height, Disp, Tag, State),
  mFormat("%.0f"),
  mValue(-1)
{
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the value of the NumLabel.
 * 
 * @return     The value of the NumLabel.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float       EvNumLabel::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the value of the NumLabel.
 * 
 * @param[in]  Value   The value as float type.
 * 
 * @return     true if the value is changed, otherwise returns false.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvNumLabel::SetValue(float Value)
{
  if (mValue == Value || mFormat == nullptr)
    return false;

  printValue(mValue = Value);
  return true;
}

/// @copydoc EvNumInt::SetFormat

void        EvNumLabel::SetFormat(const char *Format)
{
  mFormat = Format;
  printValue(mValue);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvNumLabel::printValue(float Value)
{
  char      str[80];

  snprintf(str, sizeof(str) - 1, mFormat, Value);
  TextLabel(str);
}
