
#include    <EvGUI.h>
#include    <include/system/EvShell.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static EvShell  sShell;

static EvCmd    sCmdList[] =
{
  {CD,       1,    "", "cd",      "cd [1-4|Name]         Change display"},
  {LIST,     1,   "l", "list",    "l, list [DL|#Id|Tag]  List display memory"},
  {DUMP,     1,   "d", "dump",    "d, dump [0x|#Id|Tag]  Dump display memory"},
  {RADIX,    1,   "r", "radix",   "r, radix [b|w|d]      Change radix"},
  {EDITOR,   1,   "e", "editor",  "e, editor [close]     Open/close editor"},
  {TRACE,    1,   "t", "trace",   "t, trace opt          [modif|touch|fps|off]"},
  {ROTATE,   1, "rot", "rotate",  "rot, rotate 0-3       Set display orientation"},
  {CALIB,    0,    "", "calib",   "calib                 Touchscreen calibration"},
  {INFO,     0,    "", "info",    "info                  Show display information"},
  {FONT,     0,    "", "font",    "font                  List font metrix block"},
  {ROMFONT,  0,    "", "romfont", "romfont               List display romfont"},
  {HELP,     0,   "h", "help",    "h, help               Show shell commands"}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void sOnSaveTouchCalibration(EvTouchCal *Sender, bool Save)
{
  if (!Save)
    Serial.println("\nCanceling touchscreen calibration");
  else    
  {
    char    str[32];

    Serial.println("\nTouchscreen calibration factors");

    for (int i = 0; i < 6; i++)
    {
      snprintf(str, sizeof(str)-1, "%c =%9.3f", 'A' + i, Sender->Matrix[i] / 65536.0);
      Serial.println(str);
    }
  }

  sShell.ShowPrompt();
} 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        ShellInput(const char C)
{
  sShell.Input(C);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvShell::EvShell(void)
{
  mAddr = 0;
  mSize = EV_MALLOC_SIZE;
  mRadix = 0;
  mActDisp = 0;
  mBufCount = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::ShowPrompt(void)
{
  char      str[64];

  snprintf(str, sizeof(str)-1, "[%s]-> ", EvDisplay::sDispList[mActDisp]->Tag);
  Serial.print(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::Input(const char C)
{
  char        c;
  uint32_t    size;
  EvCmd       *cmd;
  EvDisplay   *Disp;
  EvTouchCal  *tCal;
  int         i, argc;
  char        str[64];
  char        arg[4][sizeof(mBuffer)];
  const EvMem *ptr = NULL;
  const char  *msg = NULL;
  const char  *InvalidArg = "Invalid argument";
  const char  *radixName[] = {"byte", "word", "dword"};

  if (C != 0x0A)
  {
    if (C >= ' ' && C <= '~' && mBufCount < sizeof(mBuffer) - 1)
      mBuffer[mBufCount++] = C;
    return;
  }

  cmd = NULL;
  mBuffer[mBufCount] = 0;
  Serial.println(mBuffer);
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
          msg = (argc == 1 && !ChangeDisplay(arg[1])) ? "Invalid display selection" : NULL;
          break;

        case LIST:
          if (argc == 0)
            DisplayMallocBlock(Disp);
          else if (stricmp(arg[1], "DL") == 0)
          {
            size = Disp->SizeDL;
            DisplayObjectRamG(Disp, RAM_DL, RAM_DL, size);
            snprintf(str, sizeof(str) - 1, "DL Size:%-4lu (%.1f%%)  ", size, (float)(size * 100) / 8192.0);
            Serial.println(str);
          }
          else if (((ptr = Disp->RAM_G.FindByTag(arg[1], EV_OBJ)) == NULL && (sscanf(arg[1], "#%d%c", &i, &c) != 1 || (ptr = Disp->RAM_G.FindById(i)) == NULL)) || ptr->size == 0 || ptr->typeId != EV_OBJ)
          {
            msg = InvalidArg;
            break;
          }
          else
          {
            DisplayObjectRamG(Disp, ptr->addr, ptr->addr - ptr->startDL, ptr->size);
            snprintf(str, sizeof(str) - 1, "Size:%-4lu (%.1f%%)  ", ptr->size, (float)(ptr->size * 100) / 8192.0);
            Serial.print(str);
            ((EvObj *)(ptr->owner))->DisplayTagList();
            Serial.println();
          }
          msg = NULL;
          break;

        case DUMP:
          if (argc)
          {
            if ((ptr = Disp->RAM_G.FindByTag(arg[1])) != NULL)
            {
              mAddr = ptr->addr;
              mSize = ptr->size;
            }
            else
            {
              if (sscanf(arg[1], "#%i%c", &i, &c) == 1)
              {
                if ((ptr = Disp->RAM_G.FindById(i)) != NULL && ptr->size != 0)
                {
                  mAddr = ptr->addr;
                  mSize = ptr->size;
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

          if ((size = (mSize > 256 && ptr == NULL) ? 256 : mSize) == 0)
            msg = "Select new address";
          else
          {
            DumpEveMemory(Disp, mAddr, size, mRadix);
            mSize -= size;
            mAddr += size;
            msg = NULL;
          }
          break;

        case RADIX:
          msg = (argc == 1 && !SetRadix(arg[1])) ? InvalidArg : radixName[mRadix];
          break;

        case EDITOR:
          if (argc == 0)
            msg = OpenEditor(Disp);
          else if (stricmp(arg[1], "close") == 0)
            msg = CloseEditor();
          break;

        case TRACE:
          if (argc)
            msg = !SetTrace(arg[1], &Disp->sTraceFlags) ? InvalidArg : NULL;
          break;

        case ROTATE:
          if (argc == 1 && sscanf(arg[1], "%d%c", &i, &c) == 1 && (uint)i <= 3)
          {
            Disp->Rotate(i);
            msg = NULL;
          }
          break;

        case CALIB:
          if ((tCal = EvTouchCal::Create(0, 0, Disp->Width(), Disp->Height(), Disp)) == NULL)
            msg = "Open fail";
          else
          {
            msg = "Touchscreen calibration is started";
            tCal->SetOnExit(sOnSaveTouchCalibration);
            tCal->BdWidth(2 << 4);
            SetEditObj(NULL);
          }
          break;

        case INFO:
          break;

        case FONT:
          DisplayFontMetrixBlock(Disp);
          msg = NULL;
          break;

        case ROMFONT:
          DisplayRomFont(Disp);
          msg = NULL;
          break;

        case HELP:
          for (uint i = 0; i < sizeof(sCmdList) / sizeof(sCmdList[0]); i++)
          {
            Serial.print("  ");
            Serial.println(sCmdList[i].help);
          }
          msg = NULL;
          break;
      }
    }
  }

  if (msg)
    Serial.println(msg);

  mBufCount = 0;
  ShowPrompt();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DumpEveMemory(EvDisplay *Disp, uint32_t Addr, int32_t Count, uint16_t Radix)
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
    Serial.print(str);

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
            Serial.print(' ');
            j++;
          }

          snprintf(str, sizeof(str) - 1, " %02X", b[i++]);
          j += 3;
          break;

        case 1:
          if (i == 0)
            Serial.print(' ');

          snprintf(str, sizeof(str) - 1, " %04X", w[i >> 1]);
          i += 2;
          break;

        case 2:
          snprintf(str, sizeof(str) - 1, "  %08lX", d[i >> 2]);
          i += 4;
          break;
      }

      Serial.print(str);
    }

    if (Radix == 0)
    {
      for (; j < 52; j++)
        Serial.print(' ');

      for (i = 0; i < count; i++)
      {
        if (i == 8)
          Serial.print(' ');

        Serial.print((b[i] >= ' ' && b[i] <= '~') ? (char)b[i] : '.');
      }
    }

    Serial.println();
    Count -= count;
    Addr += count;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DisplayObjectRamG(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL, int32_t Count)
{
  for (mSp = 0, mVt.fmt = 4; Count > 0; Addr += 4, Count -= 4)
    DisplayListCommand(Disp, Addr, RamDL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DisplayListCommand(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL)
{
  char        cmd, fmt, addr[32], tab[20], param[80], comment[80];
  uint16_t    commentLen, mask = 0xFF;
  float       x = 0, y = 0;
  const char  *name;
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
  d = Disp->rd32(Addr);
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
    case 21: snprintf(param, sizeof(param) - 1, "(%+.5f);", (float)((int32_t)(d & 0xFFFFFF)) / (1L << 8)); break;
    case 22: snprintf(param, sizeof(param) - 1, "(%+.5f);", (b[2] & 2) ? ((float)(d & 0x01FFFF) / (1L << 16)) : ((float)((int32_t)(d & 0x01FFFF)) / (1L << 8))); break;
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

    if (ptr != NULL && ptr->typeId == EV_BMP && (bmp = (const EvBmp *)(ptr->owner)) != NULL)
    {
      if (d == (ptr->addr + bmp->PalSize) && bmp->Tag != NULL)
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

  sprintf(addr, "0x%05lX:  %08lX  %s", Addr, d, tab);
  Serial.print(addr);
  Serial.print(name);
  Serial.print(param);
  Serial.println(comment);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DisplayMallocBlock(EvDisplay *Disp)
{
  EvMem       *ptr;
  const char  *tag;
  const void  *owner;
  char        str[80];
  uint16_t    index, cnt;
  uint32_t    sumFree = 0;
  static char tagNull[] = "nullptr";

  if ((ptr = (EvMem *)Disp->RAM_G.FindFirst()) == NULL)
    Serial.println("No list avaible");
  else
  {
    for (index = cnt = 0; ptr != NULL; ptr = ptr->next, index++)
    {
      snprintf(str, sizeof(str) - 1, "0x%05lX:%7lu bytes", ptr->addr, ptr->length);
      Serial.print(str);
      snprintf(str, sizeof(str) - 1, ptr->size == 0 ? "   free" : " %6lu used %5u ", ptr->size, ptr->id);
      Serial.print(str);

      if ((owner = ptr->owner) != NULL)
      {
        switch (ptr->typeId)
        {
          case EV_UNDEFINED:
            tag = (char *)owner;
            snprintf(str, sizeof(str) - 1, " N/A \"%s\"", !tag ? tagNull : tag);
            Serial.print(str);
            break;

          case EV_FONT:
            tag = ((const EvFont *)owner)->Tag;
            snprintf(str, sizeof(str) - 1, " FNT \"%s\" used by %u System Font", !tag ? tagNull : tag, ptr->count);
            Serial.print(str);
            if (ptr->count > 1)
              Serial.print('s');
            break;

          case EV_BMP:
            tag = ((const EvBmp *)owner)->Tag;
            snprintf(str, sizeof(str) - 1, " BMP \"%s\" used by %u Object", !tag ? tagNull : tag, ptr->count);
            Serial.print(str);
            if (ptr->count > 1)
              Serial.print('s');
            break;

          case EV_OBJ:
            Serial.print(" OBJ ");
            if (((EvObj *)owner)->Tag != NULL)
              ((EvObj *)owner)->DisplayTagList();
            break;
        }
      }

      Serial.println();

      if (ptr->size == 0)
      {
        sumFree += ptr->length;
        cnt++;
      }
    }

    snprintf(str, sizeof(str) - 1, "Total:  %7lu bytes   free (%.1f%%) in %u blocks", sumFree, sumFree * (100.0 / EV_MALLOC_SIZE), cnt);
    Serial.println(str);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DisplayFontMetrixBlock(EvDisplay *Disp)
{
  int           font;
  const EvFont  *fnt;

  Serial.print("FNT  ");

  for (int i = ' '; i < 128; i++)
  {
    Serial.print((char)i);
    Serial.print("  ");
  }

  Serial.println("LOUT LS   W   H  CW BEG CNT    ADDR        TAG");

  for (font = 1; font <= LAST_FONT; font++)
    if ((fnt = Disp->SystemFont[font]) != NULL)
    {
      char      str[32];
      uint32_t  addr = (fnt->RomFont == 0) ? Disp->RAM_G.FindByOwner(fnt)->addr + FMB_SIZE : fnt->Addr;

      snprintf(str, sizeof(str) - 1, "%2u:", font);
      Serial.print(str);

      for (int i = ' '; i < 128; i++)
      {
        snprintf(str, sizeof(str) - 1, "%3u", fnt->Width[i]);
        Serial.print(str);
      }

      snprintf(str, sizeof(str) - 1, " %3lu %3lu %3lu %3lu", fnt->Layout, fnt->Stride, fnt->WidthMax, fnt->Height);
      Serial.print(str);
      snprintf(str, sizeof(str) - 1, " %3u %3u %3u  0x%06lx", fnt->CurWidth, fnt->CharBeg, fnt->CharCnt, addr);
      Serial.print(str);
      snprintf(str, sizeof(str) - 1, "  \"%s\"", (!fnt->Tag) ? "" : fnt->Tag);
      Serial.println(str);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvShell::DisplayRomFont(EvDisplay *Disp)
{
  char      str[64];
  uint32_t  addr = Disp->rd32(FONT_ROOT);
  int       i, j, romFont, begChar, curWidth = 0;

  Serial.println("\n#include    <EvGUI.h>");

  for (romFont = FIRST_ROM_FONT; romFont <= LAST_ROM_FONT; romFont++, addr += 148)
  {
    Serial.print("\nstatic const EvFont    sEvRomFont");
    Serial.print(romFont);
    Serial.println(" =\n{");

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

        snprintf(str, sizeof(str) - 1, (i + j) == 0 ? "  {%2u," : " %3u%c", byte, (i + j) == 127 ? '}' : ',');
        Serial.print(str);
        data >>= 8;
      }

      if (((i += 4) & 0x0F) == 0)
        Serial.print(i == 128 ? ",\n " : "\n ");
    }

    for (i = 128; i < 144; i += 4)
    {
      snprintf(str, sizeof(str) - 1, "%4lu,", Disp->rd32(addr + i));
      Serial.print(str);
    }

    snprintf(str, sizeof(str) - 1, " %p,%4u,%4u,%4u,%4u", (void *)Disp->rd32(addr + 144), romFont, curWidth, begChar, 128 - begChar);
    Serial.print(str);
    snprintf(str, sizeof(str) - 1, ",   0,   0,   0, \"RomFont%u\"\n};", romFont);
    Serial.println(str);
  }

  Serial.println("\nconst EvFont   *sEvRomFont[19] =\n{");

  for (romFont = FIRST_ROM_FONT; romFont <= LAST_ROM_FONT; romFont++)
  {
    snprintf(str, sizeof(str) - 1, "  &sEvRomFont%u,", romFont);
    Serial.println(str);
  }

  Serial.println("};\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvShell::ChangeDisplay(char *Str)
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

bool        EvShell::SetTrace(char *Str, uint16_t *Flags)
{
  if (stricmp(Str, "modif") == 0)
    *Flags = TRACE_MODIFIED;
  else if (stricmp(Str, "touch") == 0)
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

bool        EvShell::SetRadix(char *Str)
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

