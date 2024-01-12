
#include    <EvGUI.h>

// Host Command List

#define     ACTIVE        0x00
#define     STANDBY       0x41
#define     SLEEP         0x42
#define     PWRDOWN       0x43
#define     PD_ROMS       0x49

#define     CLKEXT        0x44
#define     CLKINT        0x48
#define     CLKSEL        0x61
#define     RST_PULSE     0x68

#define     PINDRIVE      0x70
#define     PIN_PD_STATE  0x71

#define     CS_DISABLE    0xFF  // Invalid command force csDisable()

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char   bitPixel[18] = {16, 1, 4, 8, 8, 8, 16, 16, 0, 0, 0, 0, 0, 0, 8, 8, 8, 2};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEVE::EvEVE(const uint32_t *Config, uint8_t CS, uint8_t RST, SPIClass *Spi, uint32_t Baudrate)
{
  uint32_t  reg;
  char      str[32];

  hostSetup(CS, RST, Spi, Baudrate);
  hostCommand(RST_PULSE);
  hostCommand(SLEEP);
  delay(20);
  hostCommand(CLKINT);
//  hostCommand(CLKEXT);
  delay(100);
  hostCommand(CLKSEL);
  hostCommand(ACTIVE);
  delay(300);

  while (rd8(REG_ID) != 0x7C)
    delay(10);

  while (rd8(REG_CPURESET) != 0)
    delay(10);

  mDL = 0;
  ChipID = 0x800 + rd8(0xC0001);
  CapacitiveTouchEngine = !(rd16(REG_TOUCH_CONFIG) & 0x8000);
  snprintf(str, sizeof(str), "ChipID = %cT%03X", ChipID < 0x815 ? 'F' : 'B', ChipID);
  Serial.println(str);

  while ((reg = *Config++) != 0)
    wr32(reg, *Config++);

  while (rd16(REG_CMDB_SPACE) != RAM_CMD_EMPTY)
    delay(10);

  wrCmdBufClear();
  mVideoBuf = nullptr;
  mConvertToGray = 0;
  mColorCalibration = 0;
  mStackContextCount = 0;
  mActiveContext.format = 4;
  ClearPrimitive();
  ClearContext();
  ClearColorRGB(0, 0, 0);
  Clear();
  SwapDL();
  CmdMediaFifo(RAM_G.BufferDMA->addr, RAM_G.BufferDMA->size);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearContext(void)
{
  mActiveContext.handle = 0xFF;
  mActiveContext.lineWidth = 0xFFFF;
  mActiveContext.pointSize = 0xFFFF;
  mActiveContext.opacity = OPACITY_MAX;
  mActiveContext.colorA = 0xFFFF;
  mActiveContext.clearColorA = 0xFFFF;
  mActiveContext.colorRGB = 0xFFFFFFFF;
  mActiveContext.clearColorRGB = 0xFFFFFFFF;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearPrimitive(void)
{
  mActivePrim = 0xFF;
  mVertexCount = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearConvertToGray(void)
{
  mConvertToGray = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ConvertToGray(uint8_t Min, uint8_t Max)
{
  argb32    corr;
  uint16_t  range = Max - Min + 1;

  corr.a = Min;
  corr.r = (77 * range) >> 8;
  corr.g = (151 * range) >> 8;
  corr.b = (28 * range) >> 8;
  mConvertToGray = corr.argb;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ColorCalibration(int8_t R, int8_t G, int8_t B)
{
  argb32    corr;
  int8_t    mx = max(max(R, G), B);

  corr.a = 0;
  corr.r = 256 + R - mx;
  corr.g = 256 + G - mx;
  corr.b = 256 + B - mx;
  mColorCalibration = corr.argb;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Brightness(uint8_t Level)
{
  if (Level > 128)
    Level = 128;

  wr16(REG_PWM_DUTY, Level);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvEVE::Opacity(uint16_t Opacity)
{
  uint16_t  old = mActiveContext.opacity;

  mActiveContext.opacity = (Opacity > OPACITY_MAX) ? OPACITY_MAX : Opacity;
  return old;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvMem *EvEVE::LoadBmp(const EvBmp *Bmp, uint32_t Options)
{
  EvMem     *ptr;

  if (Bmp->Layout > L2 || ((Options & OPT_FLASH) != 0 && ChipID < 0x815))
  {
    Serial.println("LoadBmp: Options not supported");
    return nullptr;
  }

  if ((ptr = (EvMem *)RAM_G.FindByOwner(Bmp)) == nullptr)
  {
    if ((ptr = (EvMem *)RAM_G.Malloc(Bmp->PalSize + Bmp->BmpSize, Bmp)) == nullptr)
      return nullptr;

    if (Bmp->DataSize)
    {
      switch (Bmp->Format & ~BMP_MALLOC)
      {
        case RAW_DATA: CmdMemWrite(ptr->addr, Bmp->DataSize); Options = 0; break;
        case JPEG_DATA:
        case PNG_DATA: CmdLoadImage(ptr->addr, Options | OPT_NODL); break;
        case ZIP_DATA: (Options == 0) ? CmdInflate(ptr->addr) : CmdInflate2(ptr->addr, Options); break;
      }

      if (Options == 0)
      {
        wrCmdBufData(Bmp->Data, Bmp->DataSize);
        wrCmdBufAlign();
      }

      wrCmdBufUpdate();
    }
  }

  ptr->count++;
  return ptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvEVE::UnloadBmp(const EvBmp *Bmp)
{
  return UnloadBmp((EvMem *)RAM_G.FindByOwner(Bmp));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvEVE::UnloadBmp(const EvMem *ptr)
{
  if (ptr == nullptr || ptr->typeId != EV_BMP)
    return false;

  if (--(((EvMem *)ptr)->count) == 0)
    RAM_G.Free(ptr);

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::MediaFifoFree(uint32_t &Dst)
{
  uint32_t  free;

  Dst = mMediaFifo.base + mMediaFifo.wrPtr;

  if ((mMediaFifo.rdPtr = rd32(REG_MEDIAFIFO_READ)) > mMediaFifo.wrPtr)
    free = mMediaFifo.rdPtr - mMediaFifo.wrPtr - 1;
  else if ((free = mMediaFifo.size - mMediaFifo.wrPtr) && mMediaFifo.rdPtr == 0)
    free--;

  return (free);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::MediaFifoUpdate(uint32_t Cnt)
{
  if ((mMediaFifo.wrPtr += Cnt) >= mMediaFifo.size)
    mMediaFifo.wrPtr -= mMediaFifo.size;

  wr32(REG_MEDIAFIFO_WRITE, mMediaFifo.wrPtr);
  mMediaFifo.rdPtr = rd32(REG_MEDIAFIFO_READ);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::SetPlayVideoBuffer(uint32_t Size)
{
  if (Size == 0 || mVideoBuf != nullptr)
  {
    RAM_G.Free(mVideoBuf);
    mVideoBuf = nullptr;
  }

  if (Size > 0 && (mVideoBuf = RAM_G.Malloc(Size, "Video Buffer", EV_VIDEO)) != nullptr)
    return mVideoBuf->size;

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvEVE::ReadDL(void)
{
  if (mDL < 0)
  {
    wrCmdBufFlush();
    mDL = rd16(REG_CMD_DL);
  }

  return mDL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::SwapDL(void)
{
  Display();
  CmdSwap();
  CmdDlStart();
  wrCmdBufUpdate();
  ClearPrimitive();
  ClearContext();
  mStackContextCount = 0;
  hostCommand(CS_DISABLE);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::AlphaFunc(uint8_t Func, uint8_t Ref)
{
  wrCmdBufDL(EV_ALPHA_FUNC(Func, Ref));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Begin(uint8_t Prim)
{
  if ((Prim &= 0x0F) == mActivePrim && ((Prim == POINTS) || (Prim == BITMAPS) || ((Prim == LINES || Prim == RECTS) && !(mVertexCount & 1))))
    return;

  wrCmdBufDL(EV_BEGIN(Prim));
  mActivePrim = Prim;
  mVertexCount = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapExtFormat(uint16_t Format)
{
  wrCmdBufDL(EV_BITMAP_EXT_FORMAT(Format));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapHandle(uint8_t Handle)
{
  if ((Handle &= 0x1F) != mActiveContext.handle)
  {
    wrCmdBufDL(EV_BITMAP_HANDLE(Handle));
    mActiveContext.handle = Handle;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapLayout(uint8_t Format, uint16_t Width, uint16_t Height)
{
  uint16_t  Linestride = (((uint32_t)Width * bitPixel[Format]) + 7) / 8;

  wrCmdBufDL(EV_BITMAP_LAYOUT_H(Linestride, Height));
  wrCmdBufDL(EV_BITMAP_LAYOUT(Format, Linestride, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapSize(uint8_t Filter, uint8_t WrapX, uint8_t WrapY, uint16_t Width, uint16_t Height)
{
  wrCmdBufDL(EV_BITMAP_SIZE_H(Width, Height));
  wrCmdBufDL(EV_BITMAP_SIZE(Filter, WrapX, WrapY, Width, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapSource(uint32_t Addr)
{
  wrCmdBufDL(EV_BITMAP_SOURCE(Addr));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapSwizzle(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
  wrCmdBufDL(EV_BITMAP_SWIZZLE(R, G, B, A));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformA(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_A(transformCoeff(Coeff)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformB(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_B(transformCoeff(Coeff)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformC(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_C(Coeff >> 8));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformD(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_D(transformCoeff(Coeff)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformE(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_E(transformCoeff(Coeff)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformF(int32_t Coeff)
{
  wrCmdBufDL(EV_BITMAP_TRANSFORM_F(Coeff >> 8));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BlendFunc(uint8_t Src, uint8_t Dst)
{
  wrCmdBufDL(EV_BLEND_FUNC(Src, Dst));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Call(uint16_t Dest)
{
  ClearPrimitive();
  wrCmdBufDL(EV_CALL(Dest));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Cell(uint8_t Cell)
{
  wrCmdBufDL(EV_CELL(Cell));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Clear(void)
{
  wrCmdBufDL(EV_CLEAR(1, 1, 1));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Clear(bool C, bool S, bool T)
{
  wrCmdBufDL(EV_CLEAR(C, S, T));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvEVE::ClearColorA(uint8_t Alpha)
{
  if (mActiveContext.opacity < OPACITY_MAX)
    Alpha = (mActiveContext.opacity * Alpha) >> 8;

  if (Alpha != mActiveContext.clearColorA)
  {
    wrCmdBufDL(EV_CLEAR_COLOR_A(Alpha));
    mActiveContext.clearColorA = Alpha;
  }

  return Alpha;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ClearColorARGB(uint32_t Color)
{
  return (ClearColorRGB(Color) | (ClearColorA(Color >> 24) << 24));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ClearColorRGB(uint32_t Color)
{
  uint32_t  rgb = colorCorrection(Color);

  if (rgb != mActiveContext.clearColorRGB)
  {
    wrCmdBufDL(EV_CLEAR_COLOR_RGB24(rgb));
    mActiveContext.clearColorRGB = rgb;
  }

  return rgb;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ClearColorRGB(uint16_t Color555)
{
  argb32    color;

  color.r = C8(Color555 >> 10);
  color.g = C8(Color555 >> 5);
  color.b = C8(Color555);
  color.a = 0;

  return (ClearColorRGB(color.rgb));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ClearColorRGB(uint8_t R, uint8_t G, uint8_t B)
{
  argb32    color;

  color.r = R;
  color.g = G;
  color.b = B;
  color.a = 0;

  return (ClearColorRGB(color.rgb));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearStencil(uint8_t Value)
{
  wrCmdBufDL(EV_CLEAR_STENCIL(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearTag(uint8_t Value)
{
  wrCmdBufDL(EV_CLEAR_TAG(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvEVE::ColorA(uint8_t Alpha)
{
  if (mActiveContext.opacity < OPACITY_MAX)
    Alpha = (mActiveContext.opacity * Alpha) >> 8;

  if (Alpha != mActiveContext.colorA)
  {
    wrCmdBufDL(EV_COLOR_A(Alpha));
    mActiveContext.colorA = Alpha;
  }

  return Alpha;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ColorARGB(uint32_t Color)
{
  return (ColorRGB(Color) | (ColorA(Color >> 24) << 24));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ColorRGB(uint32_t Color)
{
  uint32_t  rgb = colorCorrection(Color);

  if (rgb != mActiveContext.colorRGB)
  {
    wrCmdBufDL(EV_COLOR_RGB24(rgb));
    mActiveContext.colorRGB = rgb;
  }

  return rgb;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ColorRGB(uint16_t Color555)
{
  argb32    color;

  color.r = C8(Color555 >> 10);
  color.g = C8(Color555 >> 5);
  color.b = C8(Color555);
  color.a = 0;

  return (ColorRGB(color.rgb));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::ColorRGB(uint8_t R, uint8_t G, uint8_t B)
{
  argb32    color;

  color.r = R;
  color.g = G;
  color.b = B;
  color.a = 0;

  return (ColorRGB(color.rgb));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ColorMask(bool R, bool G, bool B, bool A)
{
  wrCmdBufDL(EV_COLOR_MASK(R, G, B, A));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ColorMask(uint8_t Mask)
{
  wrCmdBufDL(EV_COLOR_MASK4(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Display(void)
{
  wrCmdBufDL(EV_DISPLAY());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::End(void)
{
  wrCmdBufDL(EV_END());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Jump(uint16_t Dest)
{
  wrCmdBufDL(EV_JUMP(Dest));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::LineWidth(uint16_t Radius)
{
  if ((Radius &= 0xFFF) != mActiveContext.lineWidth)
  {
    wrCmdBufDL(EV_LINE_WIDTH(Radius));
    mActiveContext.lineWidth = Radius;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Macro(uint8_t M)
{
  wrCmdBufDL(EV_MACRO(M));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Nop(void)
{
  wrCmdBufDL(EV_NOP());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::PaletteSource(uint32_t Addr)
{
  wrCmdBufDL(EV_PALETTE_SOURCE(Addr));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::PointSize(uint16_t Radius)
{
  if ((Radius &= 0x1FFF) != mActiveContext.pointSize)
  {
    wrCmdBufDL(EV_POINT_SIZE(Radius));
    mActiveContext.pointSize = Radius;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Return(void)
{
  wrCmdBufDL(EV_RETURN());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::RestoreContext(void)
{
  if (--mStackContextCount < 0)
    Serial.println("EvEVE RestoreContext Error: 4-Level Context Stack Underflow");

  if ((uint16_t)mStackContextCount <= 3)
    mActiveContext = mStackContext[mStackContextCount];

  wrCmdBufDL(EV_RESTORE_CONTEXT());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::SaveContext(void)
{
  if ((uint16_t)mStackContextCount <= 3)
    mStackContext[mStackContextCount] = mActiveContext;

  if (++mStackContextCount > 3)
    Serial.println("EvEVE SaveContext Error: 4-Level Context Stack Overflow");

  wrCmdBufDL(EV_SAVE_CONTEXT());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ScissorSize(uint16_t Width, uint16_t Height)
{
  wrCmdBufDL(EV_SCISSOR_SIZE(Width, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ScissorXY(uint16_t X, uint16_t Y)
{
  wrCmdBufDL(EV_SCISSOR_XY(X, Y));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask)
{
  wrCmdBufDL(EV_STENCIL_FUNC(Func, Ref, Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilMask(uint8_t Mask)
{
  wrCmdBufDL(EV_STENCIL_MASK(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilOp(uint8_t Fail, uint8_t Pass)
{
  wrCmdBufDL(EV_STENCIL_OP(Fail, Pass));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::TagValue(uint8_t Value)
{
  wrCmdBufDL(EV_TAG(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::TagMask(uint8_t Mask)
{
  wrCmdBufDL(EV_TAG_MASK(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Vertex2f(int16_t X, int16_t Y)
{
  int16_t   absXY, shift, fmt = mActiveContext.format;

  if (fmt < 4)
  {
    shift = 4 - fmt;
    X >>= shift;
    Y >>= shift;
  }

  absXY = (abs(X) | abs(Y)) >> 14;

  if (absXY)
  {
    do
    {
      if (fmt == 0)
        break;

      absXY >>= 1;
      X >>= 1;
      Y >>= 1;
      fmt--;
    }
    while (absXY);

    VertexFormat(fmt);
  }

  wrCmdBufDL(EV_VERTEX2F(X, Y));
  mVertexCount++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Vertex2i(int16_t X, int16_t Y)
{
  Vertex2f(X << 4, Y << 4);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Vertex2ii(uint16_t X, uint16_t Y, uint8_t Handle, uint8_t Cell)
{
  wrCmdBufDL(EV_VERTEX2II(X, Y, Handle, Cell));
  mVertexCount++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexFormat(uint8_t Fmt)
{
  if ((Fmt &= 0x07) == 4 || Fmt != mActiveContext.format)
  {
    wrCmdBufDL(EV_VERTEX_FORMAT(Fmt));
    mActiveContext.format = Fmt;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexTranslateX(uint32_t X)
{
  wrCmdBufDL(EV_VERTEX_TRANSLATE_X(X));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexTranslateY(uint32_t Y)
{
  wrCmdBufDL(EV_VERTEX_TRANSLATE_Y(Y));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdAppend(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_APPEND);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
  mDL += Num;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdCalibrate(void)
{
  wrCmdBuf32(CMD_CALIBRATE);
  wrCmdBuf32(0xFFFFFFFF);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdDlStart(void)
{
  wrCmdBuf32(CMD_DLSTART);
  mDL = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdGradient(int16_t X0, int16_t Y0, uint32_t Color0, int16_t X1, int16_t Y1, uint32_t Color1)
{
  wrCmdBuf32(CMD_GRADIENT);
  wrCmdBuf16(X0);
  wrCmdBuf16(Y0);
  wrCmdBuf32(colorCorrection(Color0));
  wrCmdBuf16(X1);
  wrCmdBuf16(Y1);
  wrCmdBuf32(colorCorrection(Color1));
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdGradientA(int16_t X0, int16_t Y0, uint32_t Color0, int16_t X1, int16_t Y1, uint32_t Color1)
{
  if (ChipID < 0x815)
    Serial.println("CMD_GRADIENTA not supported");
  else
  {
    wrCmdBuf32(CMD_GRADIENTA);
    wrCmdBuf16(X0);
    wrCmdBuf16(Y0);
    wrCmdBuf32(colorCorrection(Color0) | (Color0 & 0xFF000000));
    wrCmdBuf16(X1);
    wrCmdBuf16(Y1);
    wrCmdBuf32(colorCorrection(Color1) | (Color1 & 0xFF000000));
    mDL = -1;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdInflate(uint32_t Addr)
{
  wrCmdBuf32(CMD_INFLATE);
  wrCmdBuf32(Addr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdInflate2(uint32_t Addr, uint32_t Options)
{
  if (ChipID < 0x815)
    Serial.println("CMD_INFLATE2 not supported");
  else
  {
    wrCmdBuf32(CMD_INFLATE2);
    wrCmdBuf32(Addr);
    wrCmdBuf32(Options);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdLoadIdentity(void)
{
  wrCmdBuf32(CMD_LOADIDENTITY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdLoadImage(uint32_t Addr, uint32_t Options)
{
  wrCmdBuf32(CMD_LOADIMAGE);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Options);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMediaFifo(uint32_t Addr, uint32_t BufSize)
{
  wrCmdBuf32(CMD_MEDIAFIFO);
  wrCmdBuf32(Addr);
  wrCmdBuf32(BufSize);
  wrCmdBufFlush();

  mMediaFifo.rdPtr = rd32(REG_MEDIAFIFO_READ);
  mMediaFifo.wrPtr = rd32(REG_MEDIAFIFO_WRITE);
  mMediaFifo.base = rd32(REG_MEDIAFIFO_BASE);
  mMediaFifo.size = rd32(REG_MEDIAFIFO_SIZE);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemCpy(uint32_t Dest, uint32_t Src, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMCPY);
  wrCmdBuf32(Dest);
  wrCmdBuf32(Src);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemSet(uint32_t Addr, uint8_t Value, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMSET);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Value);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemWrite(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMWRITE);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemZero(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMZERO);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdPlayVideo(uint32_t Opts)
{
  wrCmdBuf32(CMD_PLAYVIDEO);
  wrCmdBuf32(Opts);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdRotate(int32_t Angle)
{
  wrCmdBuf32(CMD_ROTATE);
  wrCmdBuf32(Angle);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdRomFont(uint32_t Font, uint32_t Romslot)
{
  wrCmdBuf32(CMD_ROMFONT);
  wrCmdBuf32(Font);
  wrCmdBuf32(Romslot);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdScale(int32_t X, int32_t Y)
{
  wrCmdBuf32(CMD_SCALE);
  wrCmdBuf32(X);
  wrCmdBuf32(Y);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetBitmap(uint32_t Addr, uint16_t Layout, uint16_t Width, uint16_t Height)
{
  wrCmdBuf32(CMD_SETBITMAP);
  wrCmdBuf32(Addr);
  wrCmdBuf16(Layout);
  wrCmdBuf16(Width);
  wrCmdBuf32(Height);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetFont2(uint32_t Font, uint32_t Addr, uint32_t FirstChar)
{
  wrCmdBuf32(CMD_SETFONT2);
  wrCmdBuf32(Font);
  wrCmdBuf32(Addr);
  wrCmdBuf32(FirstChar);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetScratch(uint8_t Handle)
{
  wrCmdBuf32(CMD_SETSCRATCH);
  wrCmdBuf32(Handle & 0x1F);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetRotate(uint8_t R)
{
  wrCmdBuf32(CMD_SETROTATE);
  wrCmdBuf32(R & 3);
  wrCmdBufFlush();
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetMatrix(void)
{
  wrCmdBuf32(CMD_SETMATRIX);
  mDL = -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSwap(void)
{
  wrCmdBuf32(CMD_SWAP);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSync(void)
{
  if (ChipID < 0x817)
    Serial.println("CMD_SYNC not supported");
  else
    wrCmdBuf32(CMD_SYNC);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdTranslate(int32_t X, int32_t Y)
{
  wrCmdBuf32(CMD_TRANSLATE);
  wrCmdBuf32(X);
  wrCmdBuf32(Y);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::wrCmdBufDL(uint32_t Data)
{
  wrCmdBuf32(Data);

  if (mDL >= 0)
    mDL+=4;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t     EvEVE::transformCoeff(int32_t Coeff)
{
  if (ChipID < 0x817 || abs(Coeff) > 0x1FFFF)
    return ((Coeff >> 8) & 0x1FFFF);
  else
    return ((Coeff >> 1) | 0x20000);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::colorCorrection(uint32_t Color)
{
  argb32    color, factor;

  if (!mConvertToGray)
  {
    factor.rgb = mColorCalibration;
    color.rgb = Color;
    color.a = 0;

    if (factor.r) color.r = (color.r * factor.r) >> 8;
    if (factor.g) color.g = (color.g * factor.g) >> 8;
    if (factor.b) color.b = (color.b * factor.b) >> 8;
  }
  else
  {
    uint16_t tmp;

    factor.argb = mConvertToGray;
    color.rgb = Color;

    tmp  = color.r * factor.r;
    tmp += color.g * factor.g;
    tmp += color.b * factor.b;
    tmp = (tmp >> 8) + factor.a;

    color.r = color.g = color.b = tmp;
    color.a = 0;
  }

  return color.rgb;
}
