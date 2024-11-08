
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

static const EvKeyStyle     sKeyStyle800x220[4] =
{
  { 72, 44, 8 << 4, CL_KBD_STD_KEY_UP, CL_KBD_STD_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},    // Standard Key
  { 72, 44, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Small Control Key
  {111, 44, 8 << 4, CL_KBD_CTRL_KEY_UP, CL_KBD_CTRL_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN},  // Large Control Key
  {384, 44, 8 << 4, CL_KBD_SPACE_KEY_UP, CL_KBD_SPACE_KEY_DOWN, CL_KBD_TEXT_UP, CL_KBD_TEXT_DOWN} // Spacebar Key
};

static const EvKbdMapping   sKbdMapping800x220[34] =
{ // keys 0 @ 9
  { 13, 10, 27, 0}, { 91, 10, 27, 0}, {169, 10, 27, 0}, {247, 10, 27, 0}, {325, 10, 27, 0},
  {403, 10, 27, 0}, {481, 10, 27, 0}, {559, 10, 27, 0}, {637, 10, 27, 0}, {715, 10, 27, 0},
  // keys 10 @ 18
  { 52, 62, 27, 0}, {130, 62, 27, 0}, {208, 62, 27, 0}, {286, 62, 27, 0}, {364, 62, 27, 0},
  {442, 62, 27, 0}, {520, 62, 27, 0}, {598, 62, 27, 0}, {676, 62, 27, 0},
  // keys 19 @ 26
  { 91,114, 27, 0}, {169,114, 27, 0}, {247,114, 27, 0}, {325,114, 27, 0},
  {403,114, 27, 0}, {481,114, 27, 0}, {559,114, 27, 0}, {637,114, 27, 0},
  // keys 27 @ 30
  {208,166, 27, 3}, {598,166, 27, 0}, {676,166, 26, 2}, {715,114, 26, 1},
  // keyx 31 @ 33
  {130,166, 26, 1}, { 13,166, 26, 2}, { 13,114, 26, 1}
};

static const EvKbdLayout    sKbdLayout800x220[3] =
{
  {34, sKeyAlpha, sKeyLabel, sKeyStyle800x220, sKbdMapping800x220},
  {34,   sKeyNum, sKeyLabel, sKeyStyle800x220, sKbdMapping800x220},
  {34, sKeyExtra, sKeyLabel, sKeyStyle800x220, sKbdMapping800x220}
};

EvKeyboard                  Keyboard800x220 =
{
  0, 0, 800, 220, CL_KBD_BG, 0, 2, 28, 85, CL_KBD_KEY_BD, 3, sKbdLayout800x220
};
