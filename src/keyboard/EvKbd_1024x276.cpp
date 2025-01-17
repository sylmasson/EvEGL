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
#include    <include/system/EvKeyboard.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char   sKeyAlpha[] = "qwertyuiopasdfghjklzxcvbnm, .\n\b\4\3\1";
static const char   sKeyNum[]   = "1234567890#$%():!\'\"*-+/=;?, .\n\b\4\2\5";
static const char   sKeyExtra[] = "1234567890~|@[]{}\'\"^_&\\<>?, .\n\b\4\2\6";

static const char   *sKeyLabel[16] =
{
  "CAPS", "shift", "ABC", "123", "close", "@\\^", "#+=", nullptr, "del", nullptr, "return", nullptr, nullptr, nullptr, nullptr, nullptr
};

static const EvKeyStyle     sKeyStyle1024x276[4] =
{
  { 94, 58, 8 << 4, CL_KBD_STD_KEY_UP, CL_KBD_STD_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},    // Standard Key
  { 94, 58, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Small Control Key
  {144, 58, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Large Control Key
  {494, 58, 8 << 4, CL_KBD_SPACE_KEY_UP, CL_KBD_SPACE_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping1024x276[34] =
{ // keys 0 @ 9
  { 15, 10, 27, 0}, {115, 10, 27, 0}, {215, 10, 27, 0}, {315, 10, 27, 0}, {415, 10, 27, 0},
  {515, 10, 27, 0}, {615, 10, 27, 0}, {715, 10, 27, 0}, {815, 10, 27, 0}, {915, 10, 27, 0},
  // keys 10 @ 18
  { 65, 76, 27, 0}, {165, 76, 27, 0}, {265, 76, 27, 0}, {365, 76, 27, 0}, {465, 76, 27, 0},
  {565, 76, 27, 0}, {665, 76, 27, 0}, {765, 76, 27, 0}, {865, 76, 27, 0},
  // keys 19 @ 26
  {115,142, 27, 0}, {215,142, 27, 0}, {315,142, 27, 0}, {415,142, 27, 0},
  {515,142, 27, 0}, {615,142, 27, 0}, {715,142, 27, 0}, {815,142, 27, 0},
  // keys 27 @ 30
  {265,208, 27, 3}, {765,208, 27, 0}, {865,208, 26, 2}, {915,142, 26, 1},
  // keys 31 @ 33
  {165,208, 26, 1}, { 15,208, 26, 2}, { 15,142, 26, 1}
};

static const EvKbdLayout    sKbdLayout1024x276[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle1024x276, sKbdMapping1024x276},
  {34,   sKeyNum, sKeyLabel, sKeyStyle1024x276, sKbdMapping1024x276},
  {34, sKeyExtra, sKeyLabel, sKeyStyle1024x276, sKbdMapping1024x276}
};

EvKeyboard                  Keyboard1024x276 =
{
  0, 0, 1024, 276, CL_KBD_BG, 0, 2, 28, 85, CL_KBD_KEY_BD, 3, sKbdLayout1024x276
};
