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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnSaveTouchCalibration(EvTouchCal *Sender, bool Save);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static EvCmd    sCmdList[] =
{
  {CD,       1,    "", "cd",       "cd [1-4|Name]         Change display"},
  {DUMP,     1,   "d", "dump",     "d, dump [0x|Id|Tag]   Dump display memory"},
  {LIST,     1,   "l", "list",     "l, list [DL|Id|Tag]   Show display memory"},
  {INFO,     1,   "i", "info",     "i, info [more]        Show object information"},
  {RADIX,    1,   "r", "radix",    "r, radix [b|w|d]      Change radix"},
  {EDITOR,   1,   "e", "editor",   "e, editor [close]     Open/close editor"},
  {TRACE,    1,   "t", "trace",    "t, trace Option       Option: touch|fps|off"},
  {THEME,    1,  "th", "theme",    "th, theme [0-1]       Change theme of display"},
  {ROTATE,   1, "rot", "rotate",   "rot, rotate 0-3       Set display orientation"},
  {CALIB,    0,    "", "calib",    "calib                 Touchscreen calibration"},
  {FONT,     0,    "", "font",     "font                  Show font metrix block"},
  {ROMFONT,  0,    "", "romfont",  "romfont               Show romfont of display"},
  {LISTSD,   1,  "ls", "dir",      "ls, dir               Show SD card directory"},
  {CLRCACHE, 0,  "cc", "clrcache", "cc, clrcache          Clear display list cache"},
  {HELP,     0,   "h", "help",     "h, help               Show shell commands"}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvShell::EvShell(Stream &SerialMonitor, SDClass *SDCard) :
  mAddr(0),
  mSize(EV_MALLOC_SIZE),
  mRadix(0),
  mActDisp(0),
  mBufCount(0),
  mSDCard(SDCard)
{
  EvIn = &SerialMonitor;
  EvOut = &SerialMonitor;
  EvErr = &SerialMonitor;
  EvDbg = &SerialMonitor;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::Input(const String &Str)
{
  Input(c_str(Str));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::Input(const char *Str)
{
  while (*Str)
    Input(*Str++);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::Input(const char C)
{
  uint32_t    size;
  EvCmd       *cmd;
  EvDisplay   *Disp;
  EvTouchCal  *tCal;
  int         i, argc;
  char        str[64], c = C;
  char        arg[4][sizeof(mBuffer)];
  const EvMem *ptr = nullptr;
  const char  *msg = nullptr;
  const char  *InvalidArg = "Invalid argument";
  const char  *radixName[] = {"byte", "word", "dword"};

  if (c != '\n')
  {
    if (c >= ' ' && c <= '~' && mBufCount < sizeof(mBuffer) - 1)
      mBuffer[mBufCount++] = c;
    return;
  }

  cmd = nullptr;
  mBuffer[mBufCount] = 0;
  EvOut->println(mBuffer);
  argc = sscanf(mBuffer, " %s %s %s %s", arg[0], arg[1], arg[2], arg[3]);
  Disp = EvDisplay::sDispList[mActDisp];

  if (--argc >= 0)
  {
    msg = "Bad command";
 
    for (uint i = 0; i < sizeof(sCmdList) / sizeof(sCmdList[0]); i++)
      if (strcmp(arg[0], sCmdList[i].shortName) == 0 || strcmp(arg[0], sCmdList[i].fullName) == 0)
      {
        if (argc <= sCmdList[i].argMax)
          cmd = &sCmdList[i];
        break;
      }

    if (cmd)
    {
      switch (cmd->id)
      {
        case CD:
          msg = (argc == 1 && !displayChange(arg[1])) ? "Invalid display selection" : nullptr;
          break;

        case DUMP:
          if (argc)
          {
            if ((ptr = Disp->RAM_G.FindByTag(arg[1])) != nullptr)
            {
              mAddr = ptr->addr;
              mSize = ptr->used;
            }
            else
            {
              if (sscanf(arg[1], "%d%c", &i, &c) == 1)
              {
                if ((ptr = Disp->RAM_G.FindById(i)) != nullptr && ptr->used != 0)
                {
                  mAddr = ptr->addr;
                  mSize = ptr->used;
                }
                else
                {
                  msg = InvalidArg;
                  break;
                }
              }
              else
              {
                if (sscanf(arg[1], "%x%c", &i, &c) != 1)
                {
                  msg = InvalidArg;
                  break;
                }

                mAddr = i;
                mSize = EV_MALLOC_SIZE - i;
              }
            }
          }

          if ((size = (mSize > 256 && ptr == nullptr) ? 256 : mSize) == 0)
            msg = "Select new address";
          else
          {
            dumpEveMemory(Disp, mAddr, size, mRadix);
            mSize -= size;
            mAddr += size;
            msg = nullptr;
          }
          break;

        case LIST:
          if (argc == 0)
            displayMallocBlock(Disp);
          else if (stricmp(arg[1], "DL") == 0)
          {
            size = Disp->mSizeDL;
            displayObjectRamG(Disp, RAM_DL, RAM_DL, size);
            snprintf(str, sizeof(str) - 1, "DL Size:%-4lu (%.1f%%)  ", size, (float)(size * 100) / 8192.0);
            EvOut->println(str);
          }
          else if (((ptr = Disp->RAM_G.FindByTag(arg[1], EV_OBJ)) == nullptr && (sscanf(arg[1], "%d%c", &i, &c) != 1 || (ptr = Disp->RAM_G.FindById(i)) == nullptr)) || ptr->used == 0 || ptr->typeId != EV_OBJ)
          {
            msg = InvalidArg;
            break;
          }
          else
          {
            ((EvObj *)(ptr->owner))->DisplayTagList(EvOut);
            EvOut->println();
            displayObjectRamG(Disp, ptr->addr, ptr->addr - ptr->startDL, ptr->used);
            snprintf(str, sizeof(str) - 1, "Size:%-4lu (%.1f%%)\n", ptr->used, (float)(ptr->used * 100) / 8192.0);
            EvOut->print(str);
          }
          msg = nullptr;
          break;

        case INFO:
          if (argc == 0)
            displayObjectInformation(Disp, Disp, 0, false);
          else if (stricmp(arg[1], "more") == 0)
            displayObjectInformation(Disp, Disp, 0, true);
          else
          {
            msg = InvalidArg;
            break;
          }

          msg = nullptr;
          break;

        case RADIX:
          msg = (argc == 1 && !setRadix(arg[1])) ? InvalidArg : radixName[mRadix];
          break;

        case EDITOR:
          if (argc == 0)
          {
            if (EvEditor::IsAlreadyOpen())
              msg = "Editor is already open";
            else
              msg = EvEditor::Open(Disp) ? "Editor is open" : "Unable to open editor";
          }
          else if (stricmp(arg[1], "close") == 0)
            msg = EvEditor::Close() ? "Editor is closed" : "Editor is already closed";
          break;

        case TRACE:
          if (argc)
            msg = !setTrace(arg[1], &Disp->sTraceFlags) ? InvalidArg : nullptr;
          break;

        case THEME:
          if (argc == 1 && sscanf(arg[1], "%d%c", &i, &c) == 1 && (uint)i < 2)
          {
            SystemColor.SelectTheme(i);
            argc = 0;
          }
          if (argc == 0)
          {
            sprintf(str, "Active theme is %u", SystemColor.GetTheme());
            msg = str;
          }
          break;

        case ROTATE:
          if (argc == 1 && sscanf(arg[1], "%d%c", &i, &c) == 1 && (uint)i <= 3)
          {
            Disp->Rotate(i);
            msg = nullptr;
          }
          break;

        case CALIB:
          if ((tCal = EvTouchCal::Create(0, 0, Disp->Width(), Disp->Height(), Disp)) == nullptr)
            msg = "Open fail";
          else
          {
            msg = "Touchscreen calibration is started";
            tCal->SetOnExit(sOnSaveTouchCalibration);
            tCal->BdWidth(2 << 4);
            EvEditor::SelectObj(nullptr);
          }
          break;

        case FONT:
          displayFontMetrixBlock(Disp);
          msg = nullptr;
          break;

        case ROMFONT:
          displayRomFont(Disp);
          msg = nullptr;
          break;

        case LISTSD:
          if (!mSDCard)
            msg = "No SD card reader specified";
          else if (!mSDCard->mediaPresent())
            msg = "SD card reading failed";
          else
          {
            File root = mSDCard->open("/");
            listDirectory(root);
            msg = nullptr;
          }
          break;

        case CLRCACHE:
          Disp->ClearCache();
          displayMallocBlock(Disp);
          msg = nullptr;
          break;

        case HELP:
          for (uint i = 0; i < sizeof(sCmdList) / sizeof(sCmdList[0]); i++)
          {
            EvOut->print("  ");
            EvOut->println(sCmdList[i].help);
          }
          msg = nullptr;
          break;
      }
    }
  }

  if (msg)
    EvOut->println(msg);

  mBufCount = 0;
  ShowPrompt();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::ShowPrompt(void)
{
  char      str[64];

  snprintf(str, sizeof(str)-1, "[%s]-> ", EvDisplay::sDispList[mActDisp]->Tag);
  EvOut->print(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::dumpEveMemory(EvDisplay *Disp, uint32_t Addr, int32_t Count, uint16_t Radix)
{
  char      str[16];
  int16_t   i, j, count;
  union
  {
    uint32_t  d[4];
    uint16_t  w[8];
    uint8_t   b[16];
  };

  while (Radix < 3 && Count > 0)
  {
    snprintf(str, sizeof(str) - 1, "0x%05lX:", Addr);
    EvOut->print(str);

    count = (Count > 16) ? 16 : Count;

    for (i = 0; i < count; i += 4)
      d[i >> 2] = Disp->rd32(Addr + i);

    for (i = j = 0; i < count; )
    {
      switch (Radix)
      {
        case 0:
          if ((i & 7) == 0)
          {
            EvOut->print(' ');
            j++;
          }

          snprintf(str, sizeof(str) - 1, " %02X", b[i++]);
          j += 3;
          break;

        case 1:
          if (i == 0)
            EvOut->print(' ');

          snprintf(str, sizeof(str) - 1, " %04X", w[i >> 1]);
          i += 2;
          break;

        case 2:
          snprintf(str, sizeof(str) - 1, "  %08lX", d[i >> 2]);
          i += 4;
          break;
      }

      EvOut->print(str);
    }

    if (Radix == 0)
    {
      for (; j < 52; j++)
        EvOut->print(' ');

      for (i = 0; i < count; i++)
      {
        if (i == 8)
          EvOut->print(' ');

        EvOut->print((b[i] >= ' ' && b[i] <= '~') ? (char)b[i] : '.');
      }
    }

    EvOut->println();
    Count -= count;
    Addr += count;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayObjectRamG(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL, int32_t Count)
{
  for (mSp = 0, mVt.fmt = 4; Count > 0; Addr += 4, Count -= 4)
    displayListCommand(Disp, Addr, RamDL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayListCommand(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL)
{
  char        cmd, fmt, addr[32], tab[20], param[80], comment[80];
  uint16_t    commentLen, mask = 0xFF;
  float       x = 0, y = 0;
  const char  *name;
  uint32_t    data;
  union
  {
    uint32_t  d;
    uint16_t  w[2];
    uint8_t   b[4];
  };

  static const char invalid[]  = "-- INVALID --";
  static const char *wrap[2]   = {"BORDER", "REPEAT"};
  static const char *filter[2] = {"NEAREST", "BILINEAR"};
  static const char *pixel[5]  = {"1", "1/2", "1/4", "1/8", "1/16"};

  static const char *op[8] =
  {
    "ZERO", "KEEP", "REPLACE", "INCR", "DECR", "INVERT", invalid, invalid
  };

  static const char *funcA[8] =
  {
    "NEVER", "LESS", "LEQUAL", "GREATER", "GEQUAL", "EQUAL", "NOTEQUAL", "ALWAYS"
  };

  static const char *funcB[8] =
  {
    "ZERO", "ONE", "SRC_ALPHA", "DST_ALPHA", "ONE_MINUS_SRC_ALPHA", "ONE_MINUS_DST_ALPHA", invalid, invalid
  };

  static const char *prim[16] =
  {
    invalid, "BITMAPS", "POINTS", "LINES", "LINE_STRIP", "EDGE_STRIP_R", "EDGE_STRIP_L",
    "EDGE_STRIP_A", "EDGE_STRIP_B", "RECTS", invalid, invalid, invalid, invalid, invalid, invalid
  };

  static const char *layout[32] =
  {
    "ARGB1555", "L1", "L4", "L8", "RGB332", "ARGB2", "ARGB4", "RGB565", "PALETTED", "TEXT8X8", "TEXTVGA",
    "BARGRAPH", invalid, invalid, "PALETTED565", "PALETTED4444", "PALETTED8", "L2", invalid, invalid, invalid,
    invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, invalid, "GLFORMAT"
  };

  static const char *extFmt[14] =
  {
    "ASTC_4x4", "ASTC_5x4", "ASTC_5x5", "ASTC_6x5", "ASTC_6x6", "ASTC_8x5", "ASTC_8x6",
    "ASTC_8x8", "ASTC_10x5", "ASTC_10x6", "ASTC_10x8", "ASTC_10x10", "ASTC_12x10", "ASTC_12x12"
  };

  static const char *chanSrc[8] =
  {
    "ZERO", "ONE", "RED", "GREEN", "BLUE", "ALPHA", invalid, invalid
  };

  tab[0] = 0;
  param[0] = 0;
  comment[0] = 0;
  commentLen = 0;
  data = d = Disp->rd32(Addr);
  cmd = d >> 24;

  if ((cmd & 0xC0) != 0)
    cmd &= 0xC0;

  switch (cmd)
  {
    case 0x09: fmt =  7; name = "AlphaFunc"; break;
    case 0x1F: fmt =  6; name = "Begin"; break;
    case 0x2E: fmt = 24; name = "BitmapExtFormat"; break;
    case 0x05: fmt =  1; name = "BitmapHandle"; break;
    case 0x07: fmt = 11; name = "BitmapLayout"; break;
    case 0x28: fmt =  2; name = "BitmapLayoutH"; break;
    case 0x08: fmt = 12; name = "BitmapSize"; break;
    case 0x29: fmt =  2; name = "BitmapSizeH"; break;
    case 0x01: fmt = 16; name = "BitmapSource"; break;
    case 0x2F: fmt = 25; name = "BitmapSwizzle"; break;
    case 0x15: fmt = 22; name = "BitmapTransformA"; break;
    case 0x16: fmt = 22; name = "BitmapTransformB"; break;
    case 0x17: fmt = 21; name = "BitmapTransformC"; break;
    case 0x18: fmt = 22; name = "BitmapTransformD"; break;
    case 0x19: fmt = 22; name = "BitmapTransformE"; break;
    case 0x1A: fmt = 21; name = "BitmapTransformF"; break;
    case 0x0B: fmt = 10; name = "BlendFunc"; break;
    case 0x1D: fmt = 15; name = "Call"; break;
    case 0x06: fmt = 23; name = "Cell"; mask = 0x7F; break;
    case 0x26: fmt =  3; name = "Clear"; if ((b[0] & 7) == 7) fmt = 0; break;
    case 0x0F: fmt =  1; name = "ClearColorA"; break;
    case 0x02: fmt =  4; name = "ClearColorRGB"; break;
    case 0x11: fmt =  1; name = "ClearStencil"; break;
    case 0x12: fmt =  1; name = "ClearTag"; break;
    case 0x10: fmt =  1; name = "ColorA"; break;
    case 0x20: fmt =  5; name = "ColorMask"; break;
    case 0x04: fmt =  4; name = "ColorRGB"; break;
    case 0x00: fmt =  0; name = "Display"; break;
    case 0x21: fmt =  0; name = "End"; break;
    case 0x1E: fmt = 15; name = "Jump"; break;
    case 0x0E: fmt = 17; name = "LineWidth"; break;
    case 0x25: fmt =  1; name = "Macro"; mask = 1; break;
    case 0x2D: fmt =  0; name = "Nop"; break;
    case 0x2A: fmt = 16; name = "PaletteSource"; break;
    case 0x0D: fmt = 18; name = "PointSize"; break;
    case 0x23: fmt =  0; name = "RestoreContext"; break;
    case 0x24: fmt =  0; name = "Return"; break;
    case 0x22: fmt =  0; name = "SaveContext"; break;
    case 0x1C: fmt = 13; name = "ScissorSize"; mask = 0xFFF; break;
    case 0x1B: fmt = 13; name = "ScissorXY"; mask = 0x7FF; break;
    case 0x0A: fmt =  8; name = "StencilFunc"; break;
    case 0x13: fmt =  1; name = "StencilMask"; break;
    case 0x0C: fmt =  9; name = "StencilOp"; break;
    case 0x03: fmt =  1; name = "TagValue"; break;
    case 0x14: fmt =  1; name = "TagMask"; mask = 1; break;
    case 0x40: fmt = 20; name = "Vertex2f"; break;
    case 0x80: fmt = 14; name = "Vertex2ii"; break;
    case 0x27: fmt =  1; name = "VertexFormat"; mVt.fmt = (b[0] & (mask = 7)); break;
    case 0x2B: fmt = 19; name = "VertexTranslateX"; mVt.x = (float)((int32_t)(d << 15)) / (1L << (15 + 4)); break;
    case 0x2C: fmt = 19; name = "VertexTranslateY"; mVt.y = (float)((int32_t)(d << 15)) / (1L << (15 + 4)); break;
    default:   fmt = -1; name = invalid; break;
  }

  switch (fmt)
  {
    case  0: snprintf(param, sizeof(param) - 1, "();"); break;
    case  1: snprintf(param, sizeof(param) - 1, "(%u);", b[0] & mask); break;
    case  2: snprintf(param, sizeof(param) - 1, "(%u, %u);", (b[0] >> 2) & 3, b[0] & 3); break;
    case  3: snprintf(param, sizeof(param) - 1, "(%u, %u, %u);", (b[0] >> 2) & 1, (b[0] >> 1) & 1, b[0] & 1); break;
    case  4: snprintf(param, sizeof(param) - 1, "(%u, %u, %u);", b[2], b[1], b[0]); break;
    case  5: snprintf(param, sizeof(param) - 1, "(%u, %u, %u, %u);", (b[0] >> 3) & 1, (b[0] >> 2) & 1, (b[0] >> 1) & 1, b[0] & 1); break;
    case  6: snprintf(param, sizeof(param) - 1, "(%s);", (prim[b[0] & 0x0F])); break;
    case  7: snprintf(param, sizeof(param) - 1, "(%s, %u);", funcA[b[1] & 7], b[0]); break;
    case  8: snprintf(param, sizeof(param) - 1, "(%s, %u, %u);", funcA[b[2] & 7], b[1], b[0]); break;
    case  9: snprintf(param, sizeof(param) - 1, "(%s, %s);", op[(b[0] >> 3) & 7], op[b[0] & 7]); break;
    case 10: snprintf(param, sizeof(param) - 1, "(%s, %s);", funcB[(b[0] >> 3) & 7], funcB[b[0] & 7]); break;
    case 11: snprintf(param, sizeof(param) - 1, "(%s, %u, %u);", layout[(d >> 19) & 0x1F], (uint16_t)(d >> 9) & 0x3FF, w[0] & 0x1FF); break;
    case 12: snprintf(param, sizeof(param) - 1, "(%s, %s, %s, %u, %u);", filter[(w[1] >> 4) & 1], wrap[(w[1] >> 3) & 1], wrap[(w[1] >> 2) & 1], (uint16_t)(d >> 9) & 0x1FF, w[0] & 0x1FF); break;
    case 13: snprintf(param, sizeof(param) - 1, "(%u, %u);", ((uint16_t)(d / (mask + 1))) & mask, w[0] & mask); break;
    case 14: snprintf(param, sizeof(param) - 1, "(%3.0f, %3.0f, %2u, 0x%02X);", x = ((d >> 21) & 0x1FF), y = ((d >> 12) & 0x1FF), (w[0] >> 7) & 0x1F, b[0] & 0x7F); break;
    case 15: snprintf(param, sizeof(param) - 1, "(0x%06lX);", RamDL + ((w[0] & 0x1FFF) << 2)); break;
    case 16: snprintf(param, sizeof(param) - 1, "(0x%06lX);", d & 0xFFFFFF); break;
    case 17: snprintf(param, sizeof(param) - 1, "(%.2f);", (float)(w[0] & 0x0FFF) / 16.0); break;
    case 18: snprintf(param, sizeof(param) - 1, "(%.2f);", (float)(w[0] & 0x1FFF) / 16.0); break;
    case 19: snprintf(param, sizeof(param) - 1, "(%7.2f);", (cmd == 0x2B) ? mVt.x : mVt.y); break;
    case 20: snprintf(param, sizeof(param) - 1, "(%7.2f,%7.2f);", x = ((float)((int16_t)((d >> 14) & 0xFFFE)) / (1 << (mVt.fmt + 1))), y = ((float)((int16_t)(d << 1)) / (1 << (mVt.fmt + 1)))); break;
    case 21: snprintf(param, sizeof(param) - 1, "(%+.5f);", (float)((int32_t)(d << 8)) / (1L << (8 + 8))); break;
    case 22: snprintf(param, sizeof(param) - 1, "(%+.5f);", (b[2] & 2) ? ((float)((int32_t)(d << 15)) / (1L << (15 + 15))) : ((float)((int32_t)(d << 15)) / (1L << (15 + 8)))); break;
    case 23: snprintf(param, sizeof(param) - 1, "(0x%02X);", b[0] & mask); break;
    case 24: snprintf(param, sizeof(param) - 1, "(%s);", w[0] < 18 ? layout[w[0]] : ((w[0] < 37808 || w[0] > 37821) ? invalid : extFmt[w[0] - 37808])); break;
    case 25: snprintf(param, sizeof(param) - 1, "(%s, %s, %s, %s);", chanSrc[(w[0]>>9) & 3], chanSrc[(w[0]>>6) & 3], chanSrc[(w[0]>>3) & 3], chanSrc[w[0] & 3]); break;
  }

  if (cmd == 0x23 && --mSp < DL_STACK_SIZE)  // RestoreContext
  {
    if (mSp < 0)
      mSp = 0;
    else
      mVt = mStack[mSp];
  }

  for (int8_t i = 0; i < (mSp * 2) && i < (DL_STACK_SIZE * 2); )
  {
    tab[i++] = ' ';
    tab[i++] = ' ';
    tab[i] = 0;
  }

  if (cmd == 0x23 || cmd == 0x27)   // RestoreContext or VertexFormat
  {
    commentLen = sprintf(comment, "  // \'%s\'", (mVt.fmt > 4) ? "Invalid" : pixel[mVt.fmt]);

    if (cmd == 0x23)
      sprintf(&comment[commentLen], "     (%7.2f,%7.2f)", mVt.x, mVt.y);
  }

  if (cmd == 0x06 || cmd == 0x80)   // Cell or Vertex2ii
  {
    uint8_t cell = b[0] & 0x7F;

    if (cell >= ' ' && cell <= '~') // Ascii representation of Cell
      commentLen = sprintf(comment, "  // \'%c\'%s", cell, (cmd == 0x80) ? " @ " : "");
  }

  if (cmd == 0x40 || cmd == 0x80|| cmd == 0x2C)   // Vertex2f or Vertex2ii or VertexTranslateY
  {
    if (commentLen == 0)
      commentLen = sprintf(comment, "  // ");

    sprintf(&comment[commentLen], "(%7.2f,%7.2f)", mVt.x + x, mVt.y + y);
  }

  if (cmd == 0x01)                  // BitmapSource
  {
    const EvBmp     *bmp;
    const EvMem     *ptr = Disp->RAM_G.FindByAddr(d &= 0x3FFFFF);

    if (ptr != nullptr && ptr->typeId == EV_BMP && (bmp = (const EvBmp *)(ptr->owner)) != nullptr)
    {
      if (d == (ptr->addr + bmp->PalSize) && bmp->Tag != nullptr)
        sprintf(comment, "  // BMP \"%s\"", bmp->Tag);
      else
        sprintf(comment, "  // BMP Unknown");
    }
  }

  if (cmd == 0x22)                  // SaveContext
  {
    if (mSp > 3)
      sprintf(comment, "  // 4-Level Stack Limit Error");

    if (++mSp <= DL_STACK_SIZE)
      mStack[mSp - 1] = mVt;
  }

  sprintf(addr, "0x%05lX:  %08lX  ", Addr, data);
  EvOut->print(addr);
  EvOut->print(tab);
  EvOut->print(name);
  EvOut->print(param);
  EvOut->println(comment);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayMallocBlock(EvDisplay *Disp)
{
  EvMem       *ptr;
  const char  *tag;
  const void  *owner;
  char        str[80];
  uint16_t    cnt, cntFree;
  uint32_t    sumFree = 0;
  uint32_t    sumUsed = 0;
  uint32_t    sumMalloc = 0;
  static char tagNull[] = "nullptr";

  if ((ptr = (EvMem *)Disp->RAM_G.FindFirst()) == nullptr)
    EvOut->println("No list avaible");
  else
  {
    for (cnt = cntFree = 0; ptr != nullptr; ptr = ptr->next)
    {
      snprintf(str, sizeof(str) - 1, "0x%05lX:%7lu bytes", ptr->addr, ptr->size);
      EvOut->print(str);
      snprintf(str, sizeof(str) - 1, ptr->used == 0 ? "   free" : " %6lu used %5u ", ptr->used, ptr->id);
      EvOut->print(str);

      if ((owner = ptr->owner) != nullptr)
      {
        switch (ptr->typeId)
        {
          case EV_UNDEFINED:
            tag = (char *)owner;
            snprintf(str, sizeof(str) - 1, " UND \"%s\"", !tag ? tagNull : tag);
            EvOut->print(str);
            break;

          case EV_FONT:
            tag = ((const EvFont *)owner)->Tag;
            snprintf(str, sizeof(str) - 1, " FNT \"%s\" used by %u System Font", !tag ? tagNull : tag, ptr->count);
            EvOut->print(str);
            if (ptr->count > 1)
              EvOut->print('s');
            break;

          case EV_BMP:
            tag = ((const EvBmp *)owner)->Tag;
            snprintf(str, sizeof(str) - 1, " BMP \"%s\" used by %u Object", !tag ? tagNull : tag, ptr->count);
            EvOut->print(str);
            if (ptr->count > 1)
              EvOut->print('s');
            break;

          case EV_OBJ:
            EvOut->print(" OBJ ");
            if (((EvObj *)owner)->Tag != nullptr)
              ((EvObj *)owner)->DisplayTagList(EvOut);
            break;
        }
      }

      EvOut->println();

      if (ptr->used == 0)
      {
        sumFree += ptr->size;
        cntFree++;
      }
      else
      {
        sumMalloc += ptr->size;
        sumUsed += ptr->used;
        cnt++;
      }
    }

    snprintf(str, sizeof(str) - 1, "Malloc: %7lu bytes %6lu used in %u blocks", sumMalloc, sumUsed, cnt);
    EvOut->println(str);
    snprintf(str, sizeof(str) - 1, "Free:   %7lu bytes (%.1f%%) in %u blocks", sumFree, sumFree * (100.0 / EV_MALLOC_SIZE), cntFree);
    EvOut->println(str);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayObjectInformation(EvDisplay *Disp, EvObj *Obj, int16_t SpacesCnt, bool Detailed)
{
  static const char  unknown[] = "Unknown";
  static const char  *shapeName[8] = {"ROUND", "RATIO", "FIXED", "SQUARE", "USER", unknown, unknown, unknown};
  static const char  *alignName[16] =
  {
    "LEFT_TOP", "CENTER_TOP", "RIGHT_TOP", "JUSTIFY_TOP",
    "LEFT_CENTER", "CENTER_CENTER", "RIGHT_CENTER", "JUSTIFY_CENTER",
    "LEFT_BOTTOM", "CENTER_BOTTOM", "RIGHT_BOTTOM", "JUSTIFY_BOTTOM",
    unknown, unknown, unknown, unknown
  };

  if (Obj != nullptr)
  {
    if (Obj == Disp)
      EvOut->printf("%s    TypeObj    Status Id [%s]\n", Detailed == true ? " Left  Top  Width  Height Corners Radius Font PadX PadY    Alignment   " : "", Obj->Tag);
    else if (!(Obj->mStatus & SYSTEM_OBJ))
    {
      if (Detailed == true)
      {
        EvOut->printf("%5d %5d %5d %5d ", Obj->mLeft, Obj->mTop, Obj->mWidth, Obj->mHeight);
        EvOut->printf("%9s %5u ", shapeName[Obj->mBdShape & 0x07], Obj->mBdRadius);
        EvOut->printf("%4u %4d %4d ", Obj->mStyle.font, Obj->mStyle.padX, Obj->mStyle.padY);
        EvOut->printf("%14s  ", alignName[Obj->mStyle.align & 0x0F]);
      }

      EvOut->printf("%-16s", Obj->TypeObj());
      EvOut->printf(" %c%c%c", Obj->IsViewable() ? 'V' : (Obj->IsVisible() ? 'v' : '.'), Obj->mStatus & FIXED_OBJ ? 'F' : '.', Obj->IsEnabled() ? 'E' : 'D');

      const EvMem *ptr = Disp->RAM_G.FindByOwner(Obj);

      if (ptr != nullptr)
        EvOut->printf(" %3u ", ptr->id);
      else
        EvOut->printf("   - ");

      if (SpacesCnt > 0)
        for (int i = 0; i < SpacesCnt; i += 4)
          EvOut->print(i < SpacesCnt - 4 ? " |  " : " |--");

      EvOut->printf("[%s]\n", Obj->Tag == nullptr ? unknown : Obj->Tag);
    }

    if (Obj->IsPanelObj())
      for (EvPanel::Node *node = ((EvPanel *)Obj)->mFirst; node != nullptr; node = node->next)
        displayObjectInformation(Disp, node->obj, SpacesCnt + 4, Detailed);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayFontMetrixBlock(EvDisplay *Disp)
{
  int           font;
  const EvFont  *fnt;

  EvOut->print("FNT  ");

  for (int i = ' '; i < 128; i++)
  {
    EvOut->print((char)i);
    EvOut->print("  ");
  }

  EvOut->println("LOUT LS   W   H  CW BEG CNT    ADDR        TAG");

  for (font = 1; font <= LAST_FONT; font++)
    if ((fnt = Disp->SystemFont[font]) != nullptr)
    {
      char      str[32];
      uint32_t  addr = (fnt->RomFont == 0) ? Disp->RAM_G.FindByOwner(fnt)->addr + FMB_SIZE : fnt->Addr;

      snprintf(str, sizeof(str) - 1, "%2u:", font);
      EvOut->print(str);

      for (int i = ' '; i < 128; i++)
      {
        snprintf(str, sizeof(str) - 1, "%3u", fnt->Width[i]);
        EvOut->print(str);
      }

      snprintf(str, sizeof(str) - 1, " %3lu %3lu %3lu %3lu", fnt->Layout, fnt->Stride, fnt->WidthMax, fnt->Height);
      EvOut->print(str);
      snprintf(str, sizeof(str) - 1, " %3u %3u %3u  0x%06lx", fnt->CurWidth, fnt->CharBeg, fnt->CharCnt, addr);
      EvOut->print(str);
      snprintf(str, sizeof(str) - 1, "  \"%s\"", (!fnt->Tag) ? "" : fnt->Tag);
      EvOut->println(str);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::displayRomFont(EvDisplay *Disp)
{
  char      str[64];
  uint32_t  addr = Disp->rd32(FONT_ROOT);
  int       i, j, romFont, begChar, curWidth = 0;

  EvOut->println("\n#include    <EvEGL.h>");

  for (romFont = FIRST_ROM_FONT; romFont <= LAST_ROM_FONT; romFont++, addr += 148)
  {
    snprintf(str, sizeof(str), "\nstatic const EvFont    sEvRomFont%u =\n{", romFont);
    EvOut->println(str);

    for (i = 0, begChar = -1; i < 128; )
    {
      uint8_t   byte;
      uint32_t  data = Disp->rd32(addr + i);

      if (i == ' ')
        curWidth = (data & 0xFF) / 3;

      for (j = 0; j < 4; j++)
      {
        if ((byte = data & 0xFF) != 0 && begChar == -1)
          begChar = i + j;

        snprintf(str, sizeof(str) - 1, (i + j) == 0 ? "  {%2u," : "%4u%c", byte, (i + j) == 127 ? '}' : ',');
        EvOut->print(str);
        data >>= 8;
      }

      if (((i += 4) & 0x0F) == 0)
        EvOut->print(i == 128 ? ",\n" : "\n ");
    }

    for (i = 128; i < 144; i += 4)
    {
      snprintf(str, sizeof(str) - 1, (i == 128) ? "%5lu," : "%4lu,", Disp->rd32(addr + i));
      EvOut->print(str);
    }

    snprintf(str, sizeof(str) - 1, "%9p,%4u,%4u,%4u,%4u", (void *)Disp->rd32(addr + 144), romFont, curWidth, begChar, 128 - begChar);
    EvOut->print(str);
    snprintf(str, sizeof(str) - 1, ",   0,   0,   0, \"RomFont%u\"\n};", romFont);
    EvOut->println(str);
  }

  EvOut->println("\nconst EvFont   *sEvRomFont[19] =\n{");

  for (romFont = FIRST_ROM_FONT; romFont <= LAST_ROM_FONT; romFont++)
  {
    snprintf(str, sizeof(str) - 1, "  &sEvRomFont%u,", romFont);
    EvOut->println(str);
  }

  EvOut->println("};\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvShell::displayChange(char *Str)
{
  uint      i;
  char      c;

  if (sscanf(Str, "%u%c", &i, &c) == 1 && --i < EvDisplay::sDispCount)
  {
    mActDisp = i;
    return true;
  }

  for (i = 0; i < EvDisplay::sDispCount; i++)
    if (strcmp(Str, EvDisplay::sDispList[i]->Tag) == 0)
    {
      mActDisp = i;
      return true;
    }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvShell::setTrace(char *Str, uint16_t *Flags)
{
  if (stricmp(Str, "touch") == 0)
    *Flags = TRACE_TOUCH;
  else if (stricmp(Str, "fps") == 0)
    *Flags = TRACE_FPS;
  else if (stricmp(Str, "off") == 0)
    *Flags = 0;
  else
    return false;

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvShell::setRadix(char *Str)
{
  if (stricmp(Str, "b") == 0)
    mRadix = 0;
  else if (stricmp(Str, "w") == 0)
    mRadix = 1;
  else if (stricmp(Str, "d") == 0)
    mRadix = 2;
  else
    return false;

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::listDirectory(File Dir, int16_t SpacesCnt)
{
  const char months[13][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec","???"};

  while (1)
  {
    File    entry = Dir.openNextFile();

    if (!entry)
      return;

    if (entry.name()[0] != '.')   // skip hidden file
    {
      char   info[80];
      DateTimeFields tm;

      if (entry.getModifyTime(tm))
        snprintf(info, sizeof(info), "%s %2u %4u %02u:%02u", months[tm.mon < 12 ? tm.mon : 12], tm.mday, tm.year + 1900, tm.hour, tm.min);

      EvOut->print(info);
      snprintf(info, sizeof(info), "%12llu  ", entry.size());
      EvOut->print(info);

      for (int i = 0; i < SpacesCnt; i++)
        EvOut->print(' ');

      EvOut->print(entry.name());

      if (!entry.isDirectory())
        EvOut->println();
      else
      {
        EvOut->println("/");
        listDirectory(entry, SpacesCnt + 2);
      }
    }

    entry.close();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnSaveTouchCalibration(EvTouchCal *Sender, bool Save)
{
  if (!Save)
    EvOut->println("\nCanceling touchscreen calibration");
  else    
  {
    char    str[32];

    EvOut->println("\nTouchscreen calibration factors");

    for (int i = 0; i < 6; i++)
    {
      snprintf(str, sizeof(str)-1, "%c =%9.3f", 'A' + i, Sender->Matrix[i] / 65536.0);
      EvOut->println(str);
    }
  }

  EvOut->println();
} 

