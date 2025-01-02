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

static const EvKeyStyle     sKeyStyle480x230[4] =
{
  { 42, 46, 8 << 4, CL_KBD_STD_KEY_UP, CL_KBD_STD_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},    // Standard Key
  { 42, 46, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Small Control Key
  { 66, 46, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Large Control Key
  {230, 46, 8 << 4, CL_KBD_SPACE_KEY_UP, CL_KBD_SPACE_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping480x230[34] =
{ // keys 0 @ 9
  {  7, 10, 27, 0}, { 54, 10, 27, 0}, {101, 10, 27, 0}, {148, 10, 27, 0}, {195, 10, 27, 0},
  {242, 10, 27, 0}, {289, 10, 27, 0}, {336, 10, 27, 0}, {383, 10, 27, 0}, {430, 10, 27, 0},
  // keys 10 @ 18
  { 31, 64, 27, 0}, { 78, 64, 27, 0}, {125, 64, 27, 0}, {172, 64, 27, 0}, {219, 64, 27, 0},
  {266, 64, 27, 0}, {313, 64, 27, 0}, {360, 64, 27, 0}, {407, 64, 27, 0},
  // keys 19 @ 26
  { 54,118, 27, 0}, {101,118, 27, 0}, {148,118, 27, 0}, {195,118, 27, 0},
  {242,118, 27, 0}, {289,118, 27, 0}, {336,118, 27, 0}, {383,118, 27, 0},
  // keys 27 @ 30
  {125,172, 27, 3}, {360,172, 27, 0}, {407,172, 24, 2}, {430,118, 24, 1},
  // keyx 31 @ 33
  { 78,172, 24, 1}, {  7,172, 24, 2}, {  7,118, 24, 1}
};

static const EvKbdLayout    sKbdLayout480x230[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle480x230, sKbdMapping480x230},
  {34,   sKeyNum, sKeyLabel, sKeyStyle480x230, sKbdMapping480x230},
  {34, sKeyExtra, sKeyLabel, sKeyStyle480x230, sKbdMapping480x230}
};

EvKeyboard                  Keyboard480x230 =
{
  0, 0, 480, 230, CL_KBD_BG, 0, 2, 28, 75, CL_KBD_KEY_BD, 3, sKbdLayout480x230
};
