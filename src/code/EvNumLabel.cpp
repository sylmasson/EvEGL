
#include    <EvGUI.h>

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard NumLabel.
 * 
 * A new NumLabel is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the NumLabel.
 * @param[in]  Top     The top position of the NumLabel.
 * @param[in]  Width   The width of the NumLabel.
 * @param[in]  Height  The height of the NumLabel.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the NumLabel. If nullptr, the default tag name is "EvNumLabel".
 * @param[in]  State   The initial state of the NumLabel. Default is set to VISIBLE_OBJ.
 *
 * @return     EvNumLabel address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumLabel  *EvNumLabel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvNumLabel *)EvObj::TryCreate(new EvNumLabel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvNumLabel" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvNumLabel::EvNumLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvLabel(Left, Top, Width, Height, Disp, Tag, State)
{
  mValue.i = -1;
  SetFormat("%ld");
  SetValue(0L);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the value of the NumLabel as integer type.
 * 
 * @return     The value of the NumLabel as integer type.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t     EvNumLabel::Value(void)
{
  return mValue.i;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the value of the NumLabel as float type.
 * 
 * @return     The value of the NumLabel as float type.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float       EvNumLabel::ValueFloat(void)
{
  return mValue.f;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the value of the NumLabel as integer type.
 * 
 * @param[in]  Value   The value as integer type.
 * 
 * @return     true if the value is changed, otherwise returns false.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvNumLabel::SetValue(int32_t Value)
{
  char      str[80];

  if (mValue.i == Value || mFormat == nullptr)
    return false;

  snprintf(str, sizeof(str) - 1, mFormat, mValue.i = Value);
  TextLabel(str);
  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the value of the NumLabel as float type.
 * 
 * @param[in]  Value   The value as float type.
 * 
 * @return     true if the value is changed, otherwise returns false.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvNumLabel::SetValueFloat(float Value)
{
  char      str[80];

  if (mValue.f == Value || mFormat == nullptr)
    return false;

  snprintf(str, sizeof(str) - 1, mFormat, mValue.f = Value);
  TextLabel(str);
  return true;
}

/// @copydoc EvNumInt::SetFormat

void        EvNumLabel::SetFormat(const char *Format)
{
  mFormat = Format;
}
