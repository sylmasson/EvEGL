/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include    <EvEGL.h>

EvColor     SystemColor;

uint8_t     EvColor::sTheme = 0;
bool        EvColor::sModified = true;
uint16_t    EvColor::sColorTable[2][256] = {0};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const uint16_t     sLightTheme[256] =
{
  0,                      // CL_NOCOLOR
  CL_WHITE,               // CL_PANEL_BG
  CL_LIGHT_GRAY,          // CL_DISPLAY_BG
  CL_WHITE,               // CL_DEFAULT_BG
  CL_DIM_GRAY,            // CL_SCROLL_BAR
  RGB555(  0,  50, 160),  // CL_TEXTCURSOR

  CL_BLACK,               // CL_DARK_TEXT
  CL_WHITE,               // CL_LIGHT_TEXT
  CL_DARK_TEXT,           // CL_DEFAULT_TEXT
  CL_LIGHT_TEXT,          // CL_REVERSE_TEXT
  CL_DEFAULT_TEXT,        // CL_LABEL_TEXT

  CL_LIGHT_GRAY,          // CL_BUTTON_FACE
  CL_DARK_GRAY,           // CL_BUTTON_FACE_DOWN
  CL_DEFAULT_TEXT,        // CL_BUTTON_TEXT
  CL_DEFAULT_TEXT,        // CL_BUTTON_TEXT_DOWN

  RGB555( 50, 200, 100),  // CL_TOGGLE_ON
  CL_BUTTON_FACE,         // CL_TOGGLE_OFF
  CL_WHITE,               // CL_TOGGLE_KNOB

  RGB555(  0,   0, 160),  // CL_SLIDER_LOWER
  CL_BUTTON_FACE,         // CL_SLIDER_UPPER
  CL_WHITE,               // CL_SLIDER_KNOB

  RGB555( 50, 200, 100),  // CL_PROGRESS_LOWER
  CL_LIGHT_GRAY,          // CL_PROGRESS_UPPER
  CL_LIGHT_TEXT,          // CL_PROGRESS_TEXT

  CL_LIGHT_GRAY,          // CL_SELECTOR_BG
  RGB555(  0,   0, 160),  // CL_SELECTOR_TRUE
  RGB555(160, 160, 210),  // CL_SELECTOR_NEW
  CL_DEFAULT_TEXT,        // CL_SELECTOR_TEXT
  CL_LIGHT_TEXT,          // CL_SELECTOR_TEXT2

  CL_DEFAULT_BG,          // CL_TEXTBOX_BG
  CL_LIGHT_GRAY,          // CL_TEXTBOX_BD
  RGB555(  0,  50, 160),  // CL_TEXTBOX_FOCUS
  CL_TEXTCURSOR,          // CL_TEXTBOX_CURSOR
  RGB555(180, 210, 255),  // CL_TEXTBOX_SELECT
  CL_DEFAULT_TEXT,        // CL_TEXTBOX_TEXT

  CL_DEFAULT_BG,          // CL_NUMINT_BG
  RGB555(  0,   0, 160),  // CL_NUMINT_HOLD
  RGB555( 50, 200, 100),  // CL_NUMINT_INC
  RGB555(230,   0,   0),  // CL_NUMINT_DEC
  CL_DEFAULT_TEXT,        // CL_NUMINT_TEXT

  RGB555(  0,   0, 160),  // CL_CHECKBOX_TRUE
  CL_DEFAULT_BG,          // CL_CHECKBOX_FALSE
  RGB555(160, 160, 160),  // CL_CHECKBOX_BD
  CL_DEFAULT_TEXT,        // CL_CHECKBOX_TEXT
  CL_LIGHT_TEXT,          // CL_CHECKBOX_TEXT2

  CL_BUTTON_FACE,         // CL_KBD_BG
  CL_SILVER,              // CL_KBD_KEY_BD
  CL_WHITE,               // CL_KBD_STD_KEY_UP
  CL_WHITE,               // CL_KBD_STD_KEY_DOWN
  CL_SILVER,              // CL_KBD_CTRL_KEY_UP
  CL_WHITE,               // CL_KBD_CTRL_KEY_DOWN
  CL_WHITE,               // CL_KBD_SPACE_KEY_UP
  CL_SILVER,              // CL_KBD_SPACE_KEY_DOWN
  CL_DEFAULT_TEXT,        // CL_KBD_TEXT_UP
  CL_DEFAULT_TEXT,        // CL_KBD_TEXT_DOWN

  CL_PANEL_BG,            // CL_TERMINAL_BG
  CL_DEFAULT_TEXT,        // CL_TERMINAL_TEXT

  RGB555( 30,  30,  30),  // CL_PLAYER_BG
  RGB555(230,   0,   0),  // CL_PALYER_TIMELINE_LOWER
  CL_PLAYER_TEXT,         // CL_PLAYER_TIMELINE_UPPER
  CL_PLAYER_TEXT,         // CL_PLAYER_TIMELINE_KNOB
  CL_PLAYER_TEXT,         // CL_PLAYER_ICON
  CL_WHITE,               // CL_PLAYER_TEXT
};

static const uint16_t     sDarkTheme[256] =
{
  0,                      // CL_NOCOLOR
  RGB555( 30,  30,  30),  // CL_PANEL_BG
  CL_BLACK,               // CL_DISPLAY_BG
  RGB555( 60,  60,  60),  // CL_DEFAULT_BG
  CL_DIM_GRAY,            // CL_SCROLL_BAR
  RGB555(180, 200, 255),  // CL_TEXTCURSOR

  CL_BLACK,               // CL_DARK_TEXT
  CL_WHITE_SMOKE,         // CL_LIGHT_TEXT
  CL_LIGHT_TEXT,          // CL_DEFAULT_TEXT
  CL_DARK_TEXT,           // CL_REVERSE_TEXT
  CL_DEFAULT_TEXT,        // CL_LABEL_TEXT

  CL_DEFAULT_BG,          // CL_BUTTON_FACE
  CL_DIM_GRAY,            // CL_BUTTON_FACE_DOWN
  CL_DEFAULT_TEXT,        // CL_BUTTON_TEXT
  CL_DEFAULT_TEXT,        // CL_BUTTON_TEXT_DOWN

  RGB555( 50, 200, 100),  // CL_TOGGLE_ON
  CL_DEFAULT_BG,          // CL_TOGGLE_OFF
  CL_WHITE_SMOKE,         // CL_TOGGLE_KNOB

  RGB555(  0,   0, 160),  // CL_SLIDER_LOWER
  CL_DEFAULT_BG,          // CL_SLIDER_UPPER
  CL_WHITE_SMOKE,         // CL_SLIDER_KNOB

  RGB555( 50, 200, 100),  // CL_PROGRESS_LOWER
  CL_DEFAULT_BG,          // CL_PROGRESS_UPPER
  CL_LIGHT_TEXT,          // CL_PROGRESS_TEXT

  CL_DEFAULT_BG,          // CL_SELECTOR_BG
  RGB555(  0,   0, 160),  // CL_SELECTOR_TRUE
  RGB555(160, 160, 210),  // CL_SELECTOR_NEW
  CL_DEFAULT_TEXT,        // CL_SELECTOR_TEXT
  CL_LIGHT_TEXT,          // CL_SELECTOR_TEXT2

  CL_DEFAULT_BG,          // CL_TEXTBOX_BG
  CL_DEFAULT_BG,          // CL_TEXTBOX_BD
  CL_GRAY,                // CL_TEXTBOX_FOCUS
  CL_TEXTCURSOR,          // CL_TEXTBOX_CURSOR
  RGB555( 90, 100, 128),  // CL_TEXTBOX_SELECT
  CL_DEFAULT_TEXT,        // CL_TEXTBOX_TEXT

  CL_DEFAULT_BG,          // CL_NUMINT_BG
  RGB555(  0,   0, 160),  // CL_NUMINT_HOLD
  RGB555( 50, 200, 100),  // CL_NUMINT_INC
  RGB555(230,   0,   0),  // CL_NUMINT_DEC
  CL_DEFAULT_TEXT,        // CL_NUMINT_TEXT

  RGB555(  0,   0, 160),  // CL_CHECKBOX_TRUE
  CL_DEFAULT_BG,          // CL_CHECKBOX_FALSE
  CL_NOCOLOR,             // CL_CHECKBOX_BD
  CL_DEFAULT_TEXT,        // CL_CHECKBOX_TEXT
  CL_LIGHT_TEXT,          // CL_CHECKBOX_TEXT2

  CL_PANEL_BG,            // CL_KBD_BG
  CL_GRAY,                // CL_KBD_KEY_BD
  CL_BUTTON_FACE,         // CL_KBD_STD_KEY_UP
  CL_BUTTON_FACE,         // CL_KBD_STD_KEY_DOWN
  RGB555( 15,  15,  15),  // CL_KBD_CTRL_KEY_UP
  CL_BUTTON_FACE,         // CL_KBD_CTRL_KEY_DOWN
  CL_BUTTON_FACE,         // CL_KBD_SPACE_KEY_UP
  CL_BUTTON_FACE,         // CL_KBD_SPACE_KEY_DOWN
  CL_DEFAULT_TEXT,        // CL_KBD_TEXT_UP
  CL_DEFAULT_TEXT,        // CL_KBD_TEXT_DOWN

  CL_BUTTON_FACE,         // CL_TERMINAL_BG
  RGB555( 50, 230, 100),  // CL_TERMINAL_TEXT

  RGB555( 30,  30,  30),  // CL_PLAYER_BG
  RGB555(230,   0,   0),  // CL_PALYER_TIMELINE_LOWER
  CL_PLAYER_TEXT,         // CL_PLAYER_TIMELINE_UPPER
  CL_PLAYER_TEXT,         // CL_PLAYER_TIMELINE_KNOB
  CL_PLAYER_TEXT,         // CL_PLAYER_ICON
  CL_WHITE_SMOKE,         // CL_PLAYER_TEXT
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvColor::EvColor(void)
{
  LoadDefaultTheme();
  SelectTheme(0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvColor::GetTheme(void)
{
  return sTheme;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvColor::SwitchTheme(void)
{
  SelectTheme(!sTheme);
  return sTheme;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvColor::SelectTheme(uint16_t Theme)
{
  if (Theme > 1)
    return false;

  if (sTheme != Theme)
  {
    sTheme = Theme;
    sModified = true;
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvColor::LoadDefaultTheme(void)
{
  LoadTheme(0, sLightTheme);
  LoadTheme(1, sDarkTheme);
  sModified = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvColor::LoadTheme(uint16_t Theme, const uint16_t *ColorTable)
{
  if (Theme > 1)
    return false;

  memcpy(sColorTable[Theme], ColorTable, sizeof(sColorTable[0]));

  if (sTheme == Theme)
    sModified = true;

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvColor::EditTheme(uint16_t Theme, uint8_t Index, uint16_t Color)
{
  if (Theme > 1)
    return false;

  sColorTable[Theme][Index] = Color;

  if (sTheme == Theme)
    sModified = true;

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvColor::IsModified(bool ClearAfter)
{
  bool      result = sModified;

  if (ClearAfter)
    sModified = false;

  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

color16::color16(void) : mColor(0)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

color16::color16(uint16_t Color)
{
  Set(Color);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        color16::Set(color16 &Color)
{
  return Set(Color.mColor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        color16::Set(uint16_t Color)
{
  if (!(Color & 0x8000))
    Color &= 0x00FF;

  if (mColor == Color)
    return false;

  mColor = Color;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    color16::Get(void)
{
  uint16_t  color = mColor;
  uint16_t  *table = EvColor::sColorTable[EvColor::sTheme];

  for (uint16_t cnt = 5; color != 0 && !(color & 0x8000); )
    color = (!--cnt) ? 0 : table[color & 0xFF];

  return color;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    color16::Raw(void)
{
  return mColor;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
