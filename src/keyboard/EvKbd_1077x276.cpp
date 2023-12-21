
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

static const EvKeyStyle     sKeyStyle1077x276[4] =
{
  {100, 58, 8 << 4, STD_KEY_COLOR_UP, STD_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},    // Standard Key
  {100, 58, 8 << 4, CTRL_KEY_COLOR_UP, CTRL_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},  // Small Control Key
  {153, 58, 8 << 4, CTRL_KEY_COLOR_UP, CTRL_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},  // Large Control Key
  {524, 58, 8 << 4, SPACE_KEY_COLOR_UP, SPACE_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping1077x276[34] =
{ // keys 0 @ 9
  { 11, 10, 27, 0}, {117, 10, 27, 0}, {223, 10, 27, 0}, {329, 10, 27, 0}, {435, 10, 27, 0},
  {541, 10, 27, 0}, {647, 10, 27, 0}, {753, 10, 27, 0}, {859, 10, 27, 0}, {965, 10, 27, 0},
  // keys 10 @ 18
  { 64, 76, 27, 0}, {170, 76, 27, 0}, {276, 76, 27, 0}, {382, 76, 27, 0}, {488, 76, 27, 0},
  {594, 76, 27, 0}, {700, 76, 27, 0}, {806, 76, 27, 0}, {912, 76, 27, 0},
  // keys 19 @ 26
  {117,142, 27, 0}, {223,142, 27, 0}, {329,142, 27, 0}, {435,142, 27, 0},
  {541,142, 27, 0}, {647,142, 27, 0}, {753,142, 27, 0}, {859,142, 27, 0},
  // keys 27 @ 30
  {276,208, 27, 3}, {806,208, 27, 0}, {912,208, 26, 2}, {965,142, 26, 1},
  // keys 31 @ 33
  {170,208, 26, 1}, { 11,208, 26, 2}, { 11,142, 26, 1}
};

static const EvKbdLayout    sKbdLayout1077x276[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle1077x276, sKbdMapping1077x276},
  {34,   sKeyNum, sKeyLabel, sKeyStyle1077x276, sKbdMapping1077x276},
  {34, sKeyExtra, sKeyLabel, sKeyStyle1077x276, sKbdMapping1077x276}
};

EvKeyboard                  Keyboard1077x276 =
{
  0, 0, 1077, 276, BG_COLOR, 0, 2, 28, 85, 3, sKbdLayout1077x276
};
