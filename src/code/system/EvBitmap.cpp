
#include    <EvGUI.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char   bitPixel[18] = {16, 1, 4, 8, 8, 8, 16, 16, 0, 0, 0, 0, 0, 0, 8, 8, 8, 2};

int16_t     rsin(int16_t r, uint16_t th);
int16_t     rcos(int16_t r, uint16_t th);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvBitmap::EvBitmap(const EvBmp *Bmp)
{
  mBmp = Bmp;
  Disp = NULL;
  mDest = NULL;
  mOffset.x = 0;
  mOffset.y = 0;
  mCenter.x = 0;
  mCenter.y = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvBitmap::Unload(void)
{
  if (Disp != NULL)
    Disp->UnloadBmp(mDest);

  mDest = NULL;
  Disp = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvBitmap::Load(EvDisplay *Disp)
{
  if (Disp == NULL || EvBitmap::Disp != NULL || mBmp->Format > ZIP_DATA || (mDest = Disp->LoadBmp(mBmp)) == NULL)
    return false;

  EvBitmap::Disp = Disp;
  Align(LEFT_TOP);
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvBitmap::Setup(int16_t &X, int16_t &Y, int16_t Angle, int32_t ScaleX, int32_t ScaleY)
{
  int16_t   sin, cos;
  XY        pos, size, center;

  if (Disp == NULL)
    return false;

  pos.x = X;
  pos.y = Y;
  size.x = mBmp->Width;
  size.y = mBmp->Height;
  center.x = mCenter.x;
  center.y = mCenter.y;

  Disp->Begin(BITMAPS);
  Disp->BitmapHandle(0);
  Disp->BitmapSource(mDest->addr + mBmp->PalSize);
  Disp->BitmapLayout(mBmp->Layout, mBmp->Width /* (((uint32_t)mBmp->Width * bitPixel[mBmp->Layout]) + 7) / 8 */, mBmp->Height);

  if (ScaleY == 0)
    ScaleY = ScaleX;

  if (Angle == 0 && ScaleX == 65536 && ScaleY == 65536)
  {
    Disp->BitmapSize(NEAREST, BORDER, BORDER, size.x, size.y);
    pos.x -= center.x;
    pos.y -= center.y;
    X = pos.x;
    Y = pos.y;

    size.x <<= 4;
    size.y <<= 4;
  }

  else
  {
    size.x <<= 4;
    size.y <<= 4;
    sin = rsin(32767, Angle);
    cos = rcos(32767, Angle);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    XY  c, lt, rb, corners[4] = {{0, 0}, {size.x, 0}, {size.x, size.y}, {0, size.y}};
    #pragma GCC diagnostic pop

    for (int i = 0; i < 4; i++)
    {
      c = corners[i];
      c.x = ((c.x - center.x) * ScaleX) >> 16;
      c.y = ((c.y - center.y) * ScaleY) >> 16;

      if (Angle != 0)
      {
        int32_t tmp = ((c.x * sin) + (c.y * cos)) >> 15;
        c.x = ((c.x * cos) - (c.y * sin)) >> 15;
        c.y = tmp;
      }

      c.x += pos.x;
      c.y += pos.y;

      if (i == 0)
      {
        lt.x = rb.x = c.x;
        lt.y = rb.y = c.y;
      }
      else
      {
        if (c.x < lt.x) lt.x = c.x;
        if (c.y < lt.y) lt.y = c.y;
        if (c.x > rb.x) rb.x = c.x;
        if (c.y > rb.y) rb.y = c.y;
      }
    }

    pos.x -= lt.x;
    pos.y -= lt.y;
    size.x = rb.x - lt.x + 1;
    size.y = rb.y - lt.y + 1;

    Disp->BitmapSize(BILINEAR, BORDER, BORDER, (size.x + 15) >> 4, (size.y + 15) >> 4);
    Disp->CmdLoadIdentity();
    Disp->CmdTranslate(pos.x << 12, pos.y << 12);
    Disp->CmdRotate(Angle);
    Disp->CmdScale(ScaleX, ScaleY);
    Disp->CmdTranslate(-center.x << 12, -center.y << 12);
    Disp->CmdSetMatrix();
    X = lt.x;
    Y = lt.y;
  }

  if (mBmp->PalSize > 0 && mBmp->Layout != PALETTED8)
    Disp->PaletteSource(mDest->addr);

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvBitmap::Draw(int16_t X, int16_t Y, int16_t Angle, int32_t Scale)
{
  if (Disp == NULL)
    return false;

  Disp->SaveContext();
  Setup(X, Y, Angle, Scale);
  Vertex2f(X, Y);
  Disp->RestoreContext();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvBitmap::Vertex2f(int16_t X, int16_t Y)
{
  if (Disp != NULL)
  {
    if (mBmp->Layout != PALETTED8)
      Disp->Vertex2f(X, Y);
    else
    {
      Disp->BlendFunc(ONE, ZERO);

      // Draw Alpha channel
      Disp->ColorMask(0, 0, 0, 1);
      Disp->PaletteSource(mDest->addr + 3);
      Disp->Vertex2f(X, Y);

      // Draw Red channel
      Disp->ColorMask(1, 0, 0, 0);
      Disp->PaletteSource(mDest->addr + 2);
      Disp->Vertex2f(X, Y);

      // Draw Green channel
      Disp->ColorMask(0, 1, 0, 0);
      Disp->PaletteSource(mDest->addr + 1);
      Disp->Vertex2f(X, Y);

      // Draw Red channel
      Disp->ColorMask(0, 0, 1, 0);
      Disp->PaletteSource(mDest->addr + 0);
      Disp->Vertex2f(X, Y);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvBitmap::Center(int16_t X, int16_t Y)
{
  mCenter.x = X << 4;
  mCenter.y = Y << 4;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvBitmap::Align(uint8_t Align)
{
  int16_t   x, y;

  switch (Align & 3)
  {
    case 0: x = 0; break;
    case 1: x = (mBmp->Width << 4) / 2; break;
    case 2:
    case 3: x = (mBmp->Width - 1) << 4; break;
  }

  switch ((Align >> 2) & 3)
  {
    case 0: y = 0; break;
    case 1: y = (mBmp->Height << 4) / 2; break;
    case 2:
    case 3: y = (mBmp->Height - 1) << 4; break;
  }

  mCenter.x = x;
  mCenter.y = y;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const uint16_t sintab[257] =
  {
        0,   402,   804,  1206,  1608,  2010,  2412,  2813,  3215,  3617,  4018,  4419,  4821,  5221,  5622,  6023,
     6423,  6823,  7223,  7622,  8022,  8421,  8819,  9218,  9615, 10013, 10410, 10807, 11203, 11599, 11995, 12390,
    12785, 13179, 13573, 13966, 14358, 14750, 15142, 15533, 15923, 16313, 16702, 17091, 17479, 17866, 18252, 18638,
    19023, 19408, 19791, 20174, 20557, 20938, 21319, 21699, 22078, 22456, 22833, 23210, 23585, 23960, 24334, 24707,
    25079, 25450, 25820, 26189, 26557, 26924, 27290, 27655, 28019, 28382, 28744, 29105, 29465, 29823, 30181, 30537,
    30892, 31247, 31599, 31951, 32302, 32651, 32999, 33346, 33691, 34035, 34378, 34720, 35061, 35400, 35737, 36074,
    36409, 36742, 37075, 37406, 37735, 38063, 38390, 38715, 39039, 39361, 39682, 40001, 40319, 40635, 40950, 41263,
    41574, 41885, 42193, 42500, 42805, 43109, 43411, 43711, 44010, 44307, 44603, 44896, 45189, 45479, 45768, 46055,
    46340, 46623, 46905, 47185, 47463, 47739, 48014, 48287, 48558, 48827, 49094, 49360, 49623, 49885, 50145, 50403,
    50659, 50913, 51165, 51415, 51664, 51910, 52155, 52397, 52638, 52876, 53113, 53347, 53580, 53810, 54039, 54265,
    54490, 54712, 54933, 55151, 55367, 55581, 55793, 56003, 56211, 56416, 56620, 56821, 57021, 57218, 57413, 57606,
    57796, 57985, 58171, 58355, 58537, 58717, 58894, 59069, 59242, 59413, 59582, 59748, 59912, 60074, 60234, 60391,
    60546, 60699, 60849, 60997, 61143, 61287, 61428, 61567, 61704, 61838, 61970, 62100, 62227, 62352, 62474, 62595,
    62713, 62828, 62941, 63052, 63161, 63267, 63370, 63472, 63570, 63667, 63761, 63853, 63942, 64029, 64114, 64196,
    64275, 64353, 64427, 64500, 64570, 64637, 64702, 64765, 64825, 64883, 64938, 64991, 65042, 65090, 65135, 65178,
    65219, 65257, 65293, 65326, 65357, 65385, 65411, 65435, 65456, 65474, 65490, 65504, 65515, 65523, 65530, 65533,
    65535
  };

int16_t     rsin(int16_t r, uint16_t th)
{
  th >>= 6; // angle 0-1023
  // return int(r * sin((2 * M_PI) * th / 1024.));
  int th4 = th & 511;

  if (th4 & 256)
    th4 = 512 - th4; // 256->256 257->255, etc

  uint16_t s = sintab[th4];
  int16_t p = ((uint32_t)s * r) >> 16;

  if (th & 512)
    p = -p;

  return p;
}

int16_t     rcos(int16_t r, uint16_t th)
{
  return rsin(r, th + 0x4000);
}
