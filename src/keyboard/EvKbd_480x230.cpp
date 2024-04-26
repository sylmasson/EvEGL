
#include    <EvGUI.h>
#include    <include/system/EvKeyboard.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     BG_COLOR              RGB555(213, 215, 220)
#define     OVER_KEY_BD_COLOR     RGB555(179, 183, 192)
#define     STD_KEY_COLOR_UP      RGB555(255, 255, 255)
#define     STD_KEY_COLOR_DOWN    RGB555(255, 255, 255)
#define     CTRL_KEY_COLOR_UP     RGB555(179, 183, 192)
#define     CTRL_KEY_COLOR_DOWN   RGB555(255, 255, 255)
#define     SPACE_KEY_COLOR_UP    RGB555(255, 255, 255)
#define     SPACE_KEY_COLOR_DOWN  RGB555(179, 183, 192)
#define     TEXT_COLOR_UP         RGB555(  0,   0,   0)
#define     TEXT_COLOR_DOWN       RGB555(  0,   0,   0)

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
  { 42, 46, 6 << 4, STD_KEY_COLOR_UP, STD_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},    // Standard Key
  { 42, 46, 6 << 4, CTRL_KEY_COLOR_UP, CTRL_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},  // Small Control Key
  { 66, 46, 6 << 4, CTRL_KEY_COLOR_UP, CTRL_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN},  // Large Control Key
  {230, 46, 6 << 4, SPACE_KEY_COLOR_UP, SPACE_KEY_COLOR_DOWN, TEXT_COLOR_UP, TEXT_COLOR_DOWN} // Spacebar Key
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
  0, 0, 480, 230, BG_COLOR, 0, 2, 28, 75, OVER_KEY_BD_COLOR, 3, sKbdLayout480x230
};
