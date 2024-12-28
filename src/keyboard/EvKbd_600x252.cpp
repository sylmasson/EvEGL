
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

static const EvKeyStyle     sKeyStyle600x252[4] =
{
  { 52, 52, 8 << 4, CL_KBD_STD_KEY_UP, CL_KBD_STD_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},    // Standard Key
  { 52, 52, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Small Control Key
  { 81, 52, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Large Control Key
  {284, 52, 8 << 4, CL_KBD_SPACE_KEY_UP, CL_KBD_SPACE_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping600x252[34] =
{ // keys 0 @ 9
  { 13, 10, 27, 0}, { 71, 10, 27, 0}, {129, 10, 27, 0}, {187, 10, 27, 0}, {245, 10, 27, 0},
  {303, 10, 27, 0}, {361, 10, 27, 0}, {419, 10, 27, 0}, {477, 10, 27, 0}, {535, 10, 27, 0},
  // keys 10 @ 18
  { 42, 70, 27, 0}, {100, 70, 27, 0}, {158, 70, 27, 0}, {216, 70, 27, 0}, {274, 70, 27, 0},
  {332, 70, 27, 0}, {390, 70, 27, 0}, {448, 70, 27, 0}, {506, 70, 27, 0},
  // keys 19 @ 26
  { 71,130, 27, 0}, {129,130, 27, 0}, {187,130, 27, 0}, {245,130, 27, 0},
  {303,130, 27, 0}, {361,130, 27, 0}, {419,130, 27, 0}, {477,130, 27, 0},
  // keys 27 @ 30
  {158,190, 27, 3}, {448,190, 27, 0}, {506,190, 24, 2}, {535,130, 24, 1},
  // keyx 31 @ 33
  {100,190, 24, 1}, { 13,190, 24, 2}, { 13,130, 24, 1}
};

static const EvKbdLayout    sKbdLayout600x252[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle600x252, sKbdMapping600x252},
  {34,   sKeyNum, sKeyLabel, sKeyStyle600x252, sKbdMapping600x252},
  {34, sKeyExtra, sKeyLabel, sKeyStyle600x252, sKbdMapping600x252}
};

EvKeyboard                  Keyboard600x252 =
{
  0, 0, 600, 252, CL_KBD_BG, 0, 2, 28, 75, CL_KBD_KEY_BD, 3, sKbdLayout600x252
};
