
#include    <EvGUI.h>
#include    <include/system/EvKeyboard.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char   sKeyAlpha[] = "qwertyuiopasdfghjklzxcvbnm, .\n\b\4\3\1";
static const char   sKeyNum[]   = "1234567890#$%():!\'\"*-+/=;?, .\n\b\4\2\5";
static const char   sKeyExtra[] = "1234567890~|@[]{}\'\"^_&\\<>?, .\n\b\4\2\6";

static const char   *sKeyLabel[16] =
{
  "CAPS", "shift", "ABC", "123", "close", "@\\^", "#+=", nullptr, "del", nullptr, "return", nullptr, nullptr, nullptr, nullptr, nullptr
};

static const EvKeyStyle     sKeyStyle1066x276[4] =
{
  { 98, 58, 8 << 4, CL_KBD_STD_KEY_UP, CL_KBD_STD_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},    // Standard Key
  { 98, 58, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Small Control Key
  {150, 58, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Large Control Key
  {514, 58, 8 << 4, CL_KBD_SPACE_KEY_UP, CL_KBD_SPACE_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping1066x276[34] =
{ // keys 0 @ 9
  { 16, 10, 27, 0}, {120, 10, 27, 0}, {224, 10, 27, 0}, {328, 10, 27, 0}, {432, 10, 27, 0},
  {536, 10, 27, 0}, {640, 10, 27, 0}, {744, 10, 27, 0}, {848, 10, 27, 0}, {952, 10, 27, 0},
  // keys 10 @ 18
  { 68, 76, 27, 0}, {172, 76, 27, 0}, {276, 76, 27, 0}, {380, 76, 27, 0}, {484, 76, 27, 0},
  {588, 76, 27, 0}, {692, 76, 27, 0}, {796, 76, 27, 0}, {900, 76, 27, 0},
  // keys 19 @ 26
  {120,142, 27, 0}, {224,142, 27, 0}, {328,142, 27, 0}, {432,142, 27, 0},
  {536,142, 27, 0}, {640,142, 27, 0}, {744,142, 27, 0}, {848,142, 27, 0},
  // keys 27 @ 30
  {276,208, 27, 3}, {796,208, 27, 0}, {900,208, 26, 2}, {952,142, 26, 1},
  // keys 31 @ 33
  {172,208, 26, 1}, { 16,208, 26, 2}, { 16,142, 26, 1}
};

static const EvKbdLayout    sKbdLayout1066x276[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle1066x276, sKbdMapping1066x276},
  {34,   sKeyNum, sKeyLabel, sKeyStyle1066x276, sKbdMapping1066x276},
  {34, sKeyExtra, sKeyLabel, sKeyStyle1066x276, sKbdMapping1066x276}
};

EvKeyboard                  Keyboard1066x276 =
{
  0, 0, 1066, 276, CL_KBD_BG, 0, 2, 28, 85, CL_KBD_KEY_BD, 3, sKbdLayout1066x276
};
