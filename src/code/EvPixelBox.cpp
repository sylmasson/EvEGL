
#include    <EvGUI.h>

const uint16_t  PowerConsciousHSV[] =
{
  0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11,12,
  12, 12, 13, 13, 13, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21,
  22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 30, 30, 30, 31, 31,
  63, 62, 94, 126, 157, 157, 188, 220, 252, 251, 283, 315, 346, 346, 378, 409, 441, 440, 472, 504, 535, 535,
  567, 598, 630, 630, 661, 693, 692, 724, 756, 787, 787, 819, 850, 882, 882, 913, 945, 976, 976, 1008, 1039,
  1071, 1071, 1102, 1134, 1165, 1165, 1197, 1228, 1260, 1260, 1291, 1323, 1355, 1354, 1386, 1417, 1417, 1449,
  1480, 1512, 1512, 1543, 1575, 1607, 1606, 1638, 1669, 1701, 1701, 1732, 1764, 1796, 1795, 1827, 1859, 1890,
  1890, 1921, 1953, 1985, 1984, 2016, 1984, 4032, 4000, 3968, 5984, 5984, 8000, 7968, 7936, 9984, 9952, 9920,
  11936, 11936, 11904, 13920, 13888, 15936, 15904, 15872, 17888, 17888, 17856, 19872, 19840, 19840, 21856,
  21824, 23872, 23840, 23808, 25824, 25824, 25792, 27808, 27776, 27776, 29792, 29760, 31776, 31776, 31744,
  33760, 33728, 33728, 35744, 35712, 37728, 37728, 37696, 39712, 39680, 39680, 41696, 41664, 41632, 43680,
  43648, 45664, 45664, 45632, 47648, 47616, 47616, 49632, 49600, 49568, 51616, 51584, 53600, 53568, 53568,
  55584, 55552, 55520, 57568, 57536, 57504, 59520, 59520, 61536, 61504, 61472, 63520, 63488
};

const uint16_t  RegularHSV[] =
{
  0,  1,  1,  2,  2,  3,  4,  4,  5,  5,  6,  7,  7,  8,  9,  9,
  10, 10, 11, 12, 12, 13, 13, 14, 15, 15, 16, 16, 17, 18, 18, 19,
  19, 20, 21, 21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 27, 28, 29,
  29, 30, 30, 31, 63, 95, 159,  191,  223,  255,  319,  351,  383,  415,  479,  511,
  543,  575,  639,  671,  703,  735,  767,  831,  863,  895,  927,  991,  1023, 1055, 1087, 1151,
  1183, 1215, 1247, 1311, 1343, 1375, 1407, 1439, 1503, 1535, 1567, 1599, 1663, 1695, 1727, 1759,
  1823, 1855, 1887, 1919, 1983, 2015, 2047, 2046, 2046, 2045, 2045, 2044, 2043, 2043, 2042, 2042,
  2041, 2040, 2040, 2039, 2038, 2038, 2037, 2037, 2036, 2035, 2035, 2034, 2034, 2033, 2032, 2032,
  2031, 2031, 2030, 2029, 2029, 2028, 2028, 2027, 2026, 2026, 2025, 2025, 2024, 2023, 2023, 2022,
  2021, 2021, 2020, 2020, 2019, 2018, 2018, 2017, 2017, 2016, 4064, 4064, 6112, 6112, 8160, 10208,
  10208, 12256, 12256, 14304, 16352, 16352, 18400, 20448, 20448, 22496, 22496, 24544, 26592, 26592, 28640, 28640,
  30688, 32736, 32736, 34784, 34784, 36832, 38880, 38880, 40928, 40928, 42976, 45024, 45024, 47072, 47072, 49120,
  51168, 51168, 53216, 55264, 55264, 57312, 57312, 59360, 61408, 61408, 63456, 63456, 65504, 65472, 65440, 65376,
  65344, 65312, 65280, 65216, 65184, 65152, 65120, 65056, 65024, 64992, 64960, 64896, 64864, 64832, 64800, 64768,
  64704, 64672, 64640, 64608, 64544, 64512, 64480, 64448, 64384, 64352, 64320, 64288, 64224, 64192, 64160, 64128,
  64096, 64032, 64000, 63968, 63936, 63872, 63840, 63808, 63776, 63712, 63680, 63648, 63616, 63552, 63520, 63488
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the PixelBox.
 * 
 * A new standard PixelBox is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of PixelBox.
 * @param[in]  Top     The top position of PixelBox.
 * @param[in]  Width   The width of PixelBox.
 * @param[in]  Height  The height of PixelBox.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the PixelBox. If nullptr, the default tag name is "EvPixelBox".
 * @param[in]  State   The initial state of the PixelBox. Default is set to VISIBLE_OBJ.
 *
 * @return     EvPixelBox address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPixelBox     *EvPixelBox::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvPixelBox *)EvObj::TryCreate(new EvPixelBox(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvPixelBox" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPixelBox::EvPixelBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mBmp.Format = RAW_DATA;
  mBmp.Layout = PALETTED565;
  mBmp.Width = Width;
  mBmp.Height = Height;
  mBmp.PalSize = sizeof(RegularHSV);
  mBmp.BmpSize = Width * Height * 2;
  mBmp.DataSize = sizeof(RegularHSV);
  mBmp.Data = (const uint8_t *)RegularHSV;
  mBmp.Tag = Tag;
  mBufInd = 0;

  if ((mDest = Disp->LoadBmp(&mBmp)) == nullptr)
    abortCreate();
  else
  {
    Disp->CmdMemSet(mDest->addr + mBmp.PalSize, 0, mBmp.BmpSize);
    BdShape(SQUARE_CORNERS);
    mBmp.BmpSize /= 2;
    SetOnTouch(nullptr);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPixelBox::~EvPixelBox(void)
{
  if (mDest)
    Disp->UnloadBmp(mDest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvPixelBox::ScrollUp(void)
{
  uint32_t    src, dst;

  dst = mDest->addr + mBmp.PalSize;
  src = dst + mBmp.Width;
  Disp->CmdMemCpy(dst, src, mBmp.BmpSize - mBmp.Width);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvPixelBox::ScrollDown(void)
{
  uint32_t    src, dst;

  if (mBufInd == 0)
  {
    src = mDest->addr + mBmp.PalSize;
    dst = src + mBmp.BmpSize;
  }
  else
  {
    dst = mDest->addr + mBmp.PalSize;
    src = dst + mBmp.BmpSize;
  }

  Disp->CmdMemCpy(dst + mBmp.Width, src, mBmp.BmpSize - mBmp.Width);
  mBufInd = !mBufInd;
  Modified();

/*  uint32_t    src, dst;

  src = mDest->addr + mBmp.PalSize;
  dst = src + mBmp.BmpSize;
  Disp->CmdMemCpy(dst, src, mBmp.BmpSize);

  dst = src + mBmp.Width;
  src += mBmp.BmpSize;
  Disp->CmdMemCpy(dst, src, mBmp.BmpSize - mBmp.Width); */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvPixelBox::SetPixel(uint16_t X, uint16_t Y, uint8_t Color)
{
  if (X < mBmp.Width && Y < mBmp.Height)
    Disp->wr8(mDest->addr + mBmp.PalSize + (Y * mBmp.Width) + X + (mBufInd ? mBmp.BmpSize : 0), Color);
//    Disp->wr8(mDest->addr + mBmp.PalSize + (Y * mBmp.Width) + X, Color);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvPixelBox::SetPixel(uint16_t X, uint16_t Y, uint8_t *Color, uint16_t Count)
{
  if (X < mBmp.Width && Y < mBmp.Height)
  {
    // condition limit with count here
    Disp->CmdMemWrite(mDest->addr + mBmp.PalSize + (Y * mBmp.Width) + X + (mBufInd ? mBmp.BmpSize : 0), Count);
    Disp->wrCmdBufData(Color, Count);
    Disp->wrCmdBufAlign();

//    Disp->wr8(mDest->addr + mBmp.PalSize + (Y * mBmp.Width) + X + (mBufInd ? mBmp.BmpSize : 0), Color);
  }
//    Disp->wr8(mDest->addr + mBmp.PalSize + (Y * mBmp.Width) + X, Color);
}

/// @copydoc EvButton::SetOnTouch()

void        EvPixelBox::SetOnTouch(void (*OnTouch)(EvPixelBox *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPixelBox::drawEvent(void)
{
  Disp->ColorMask(1);
  Disp->Clear();
  Disp->Begin(RECTS);
  FillRectangle2f(0, 0, mWidth << 4, mHeight << 4, RGB555(255, 255, 255), mBdRadius);
  Disp->ColorMask(14);
  Disp->BlendFunc(DST_ALPHA, ONE_MINUS_DST_ALPHA);
  Disp->Begin(BITMAPS);
  Disp->BitmapHandle(0);
  Disp->BitmapSource(mDest->addr + mBmp.PalSize + (mBufInd ? mBmp.BmpSize : 0));
  Disp->BitmapLayout(mBmp.Layout, mBmp.Width, mBmp.Height);
  Disp->BitmapSize(NEAREST, BORDER, BORDER, mBmp.Width, mBmp.Height);

  if (mBmp.PalSize > 0)
    Disp->PaletteSource(mDest->addr);

  Disp->Vertex2ii(0, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPixelBox::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}
