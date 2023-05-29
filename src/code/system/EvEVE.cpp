
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

EvEVE::EvEVE(const uint32_t *Config, uint8_t CS, int16_t RST, SPIClass *Spi, uint32_t Baudrate)
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

  ChipID = 0x800 + rd8(0xC0001);
  CapacitiveTouchEngine = !(rd16(REG_TOUCH_CONFIG) & 0x8000);
  snprintf(str, sizeof(str), "ChipID = %cT%03X", ChipID < 0x815 ? 'F' : 'B', ChipID);
  Serial.println(str);

  while ((reg = *Config++) != 0)
    wr32(reg, *Config++);

  while (rd16(REG_CMDB_SPACE) != 0xFFC)
    delay(10);

  wrCmdBufClear();
  mColorCalibration = 0;
  mStackContextCount = 0;
  mActiveContext.format = 4;
  ClearPrimitive();
  ClearContext();
  ClearColorRGB(0, 0, 0);
  Clear();
  SwapDL();
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

const EvMem *EvEVE::LoadBmp(const EvBmp *Bmp)
{
  EvMem     *ptr;

  if (Bmp->Layout > L2)
    return NULL;

  if ((ptr = (EvMem *)RAM_G.FindByOwner(Bmp)) == NULL)
  {
    if ((ptr = (EvMem *)RAM_G.Malloc(Bmp->PalSize + Bmp->BmpSize, Bmp, EV_BMP)) == NULL)
      return NULL;

    if (Bmp->DataSize)
    {
      switch (Bmp->Format)
      {
        case RAW_DATA:  CmdMemwrite(ptr->addr, Bmp->DataSize); break;
        case JPEG_DATA: CmdLoadImage(ptr->addr, 0); break;
        case ZIP_DATA:  CmdInflate(ptr->addr); break;
      }

      wrCmdBufData(Bmp->Data, Bmp->DataSize);
      wrCmdBufAlign();
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
  if (ptr == NULL)
    return false;

  if (--(((EvMem *)ptr)->count) == 0)
    RAM_G.Free(ptr);

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint16_t    EvEVE::ReadDL(void)
{
  wrCmdBufFlush();
  return rd16(REG_CMD_DL);
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
  wrCmdBuf32(EV_ALPHA_FUNC(Func, Ref));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Begin(uint8_t Prim)
{
  if ((Prim &= 0x0F) == mActivePrim && ((Prim == POINTS) || (Prim == BITMAPS) || ((Prim == LINES || Prim == RECTS) && !(mVertexCount & 1))))
    return;

  wrCmdBuf32(EV_BEGIN(Prim));
  mActivePrim = Prim;
  mVertexCount = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapHandle(uint8_t Handle)
{
  if ((Handle &= 0x1F) != mActiveContext.handle)
  {
    wrCmdBuf32(EV_BITMAP_HANDLE(Handle));
    mActiveContext.handle = Handle;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapLayout(uint8_t Format, uint16_t Linestride, uint16_t Height)
{
  wrCmdBuf32(EV_BITMAP_LAYOUT_H(Linestride, Height));
  wrCmdBuf32(EV_BITMAP_LAYOUT(Format, Linestride, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapSize(uint8_t Filter, uint8_t WrapX, uint8_t WrapY, uint16_t Width, uint16_t Height)
{
  wrCmdBuf32(EV_BITMAP_SIZE_H(Width, Height));
  wrCmdBuf32(EV_BITMAP_SIZE(Filter, WrapX, WrapY, Width, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapSource(uint32_t Addr)
{
  wrCmdBuf32(EV_BITMAP_SOURCE(Addr));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformA(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_A(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformB(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_B(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformC(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_C(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformD(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_D(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformE(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_E(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BitmapTransformF(int32_t Coeff)
{
  wrCmdBuf32(EV_BITMAP_TRANSFORM_F(Coeff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::BlendFunc(uint8_t Src, uint8_t Dst)
{
  wrCmdBuf32(EV_BLEND_FUNC(Src, Dst));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Call(uint16_t Dest)
{
  ClearPrimitive();
  wrCmdBuf32(EV_CALL(Dest));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Cell(uint8_t Cell)
{
  wrCmdBuf32(EV_CELL(Cell));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Clear(void)
{
  wrCmdBuf32(EV_CLEAR(1, 1, 1));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Clear(bool C, bool S, bool T)
{
  wrCmdBuf32(EV_CLEAR(C, S, T));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvEVE::ClearColorA(uint8_t Alpha)
{
  if (mActiveContext.opacity < OPACITY_MAX)
    Alpha = (mActiveContext.opacity * Alpha) >> 8;

  if (Alpha != mActiveContext.clearColorA)
  {
    wrCmdBuf32(EV_CLEAR_COLOR_A(Alpha));
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
    wrCmdBuf32(EV_CLEAR_COLOR_RGB24(rgb));
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
  wrCmdBuf32(EV_CLEAR_STENCIL(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ClearTag(uint8_t Value)
{
  wrCmdBuf32(EV_CLEAR_TAG(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvEVE::ColorA(uint8_t Alpha)
{
  if (mActiveContext.opacity < OPACITY_MAX)
    Alpha = (mActiveContext.opacity * Alpha) >> 8;

  if (Alpha != mActiveContext.colorA)
  {
    wrCmdBuf32(EV_COLOR_A(Alpha));
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
    wrCmdBuf32(EV_COLOR_RGB24(rgb));
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
  wrCmdBuf32(EV_COLOR_MASK(R, G, B, A));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ColorMask(uint8_t Mask)
{
  wrCmdBuf32(EV_COLOR_MASK4(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Display(void)
{
  wrCmdBuf32(EV_DISPLAY());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::End(void)
{
  wrCmdBuf32(EV_END());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Jump(uint16_t Dest)
{
  wrCmdBuf32(EV_JUMP(Dest));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::LineWidth(uint16_t Radius)
{
  if ((Radius &= 0xFFF) != mActiveContext.lineWidth)
  {
    wrCmdBuf32(EV_LINE_WIDTH(Radius));
    mActiveContext.lineWidth = Radius;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Macro(uint8_t M)
{
  wrCmdBuf32(EV_MACRO(M));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Nop(void)
{
  wrCmdBuf32(EV_NOP());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::PaletteSource(uint32_t Addr)
{
  wrCmdBuf32(EV_PALETTE_SOURCE(Addr));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::PointSize(uint16_t Radius)
{
  if ((Radius &= 0x1FFF) != mActiveContext.pointSize)
  {
    wrCmdBuf32(EV_POINT_SIZE(Radius));
    mActiveContext.pointSize = Radius;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Return(void)
{
  wrCmdBuf32(EV_RETURN());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::RestoreContext(void)
{
  if (--mStackContextCount < 0)
    Serial.println("EvEVE RestoreContext Error: 4-Level Context Stack Underflow");

  if ((uint16_t)mStackContextCount <= 3)
    mActiveContext = mStackContext[mStackContextCount];

  wrCmdBuf32(EV_RESTORE_CONTEXT());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::SaveContext(void)
{
  if ((uint16_t)mStackContextCount <= 3)
    mStackContext[mStackContextCount] = mActiveContext;

  if (++mStackContextCount > 3)
    Serial.println("EvEVE SaveContext Error: 4-Level Context Stack Overflow");

  wrCmdBuf32(EV_SAVE_CONTEXT());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ScissorSize(uint16_t Width, uint16_t Height)
{
  wrCmdBuf32(EV_SCISSOR_SIZE(Width, Height));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::ScissorXY(uint16_t X, uint16_t Y)
{
  wrCmdBuf32(EV_SCISSOR_XY(X, Y));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask)
{
  wrCmdBuf32(EV_STENCIL_FUNC(Func, Ref, Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilMask(uint8_t Mask)
{
  wrCmdBuf32(EV_STENCIL_MASK(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::StencilOp(uint8_t Fail, uint8_t Pass)
{
  wrCmdBuf32(EV_STENCIL_OP(Fail, Pass));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::TagValue(uint8_t Value)
{
  wrCmdBuf32(EV_TAG(Value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::TagMask(uint8_t Mask)
{
  wrCmdBuf32(EV_TAG_MASK(Mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::Vertex2f(int16_t X, int16_t Y)
{
  int16_t   shift, fmt = mActiveContext.format;

  if (fmt < 4)
  {
    shift = 4 - fmt;
    X >>= shift;
    Y >>= shift;
  }

  if (abs(X) >> 14 || abs(Y) >> 14)
  {
    do
    {
      if (fmt == 0)
        break;

      X >>= 1;
      Y >>= 1;
      fmt--;
    }
    while (abs(X) >> 14 || abs(Y) >> 14);

    VertexFormat(fmt);
  }

  wrCmdBuf32(EV_VERTEX2F(X, Y));
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
  wrCmdBuf32(EV_VERTEX2II(X, Y, Handle, Cell));
  mVertexCount++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexFormat(uint8_t Fmt)
{
  if ((Fmt &= 0x07) == 4 || Fmt != mActiveContext.format)
  {
    wrCmdBuf32(EV_VERTEX_FORMAT(Fmt));
    mActiveContext.format = Fmt;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexTranslateX(uint32_t X)
{
  wrCmdBuf32(EV_VERTEX_TRANSLATE_X(X));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::VertexTranslateY(uint32_t Y)
{
  wrCmdBuf32(EV_VERTEX_TRANSLATE_Y(Y));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdAppend(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_APPEND);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdCalibrate(void)
{
  wrCmdBuf32(CMD_CALIBRATE);
  wrCmdBuf32(0xFFFFFFFF);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdDlStart(void)
{
  wrCmdBuf32(CMD_DLSTART);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdInflate(uint32_t Addr)
{
  wrCmdBuf32(CMD_INFLATE);
  wrCmdBuf32(Addr);
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

void        EvEVE::CmdMemcpy(uint32_t Dest, uint32_t Src, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMCPY);
  wrCmdBuf32(Dest);
  wrCmdBuf32(Src);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemset(uint32_t Addr, uint8_t Value, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMSET);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Value);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemwrite(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMWRITE);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdMemzero(uint32_t Addr, uint32_t Num)
{
  wrCmdBuf32(CMD_MEMZERO);
  wrCmdBuf32(Addr);
  wrCmdBuf32(Num);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdRotate(int32_t Angle)
{
  wrCmdBuf32(CMD_ROTATE);
  wrCmdBuf32(Angle);
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetBitmap(uint32_t Addr, uint16_t Layout, uint16_t Width, uint16_t Height)
{
  wrCmdBuf32(CMD_SETBITMAP);
  wrCmdBuf32(Addr);
  wrCmdBuf16(Layout);
  wrCmdBuf16(Width);
  wrCmdBuf32(Height);
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSetMatrix(void)
{
  wrCmdBuf32(CMD_SETMATRIX);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdSwap(void)
{
  wrCmdBuf32(CMD_SWAP);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEVE::CmdTranslate(int32_t X, int32_t Y)
{
  wrCmdBuf32(CMD_TRANSLATE);
  wrCmdBuf32(X);
  wrCmdBuf32(Y);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32_t    EvEVE::colorCorrection(uint32_t Color)
{
  argb32    color, calib;

  calib.rgb = mColorCalibration;
  color.rgb = Color;
  color.a = 0;

  if (calib.r) color.r = (color.r * calib.r) >> 8;
  if (calib.g) color.g = (color.g * calib.g) >> 8;
  if (calib.b) color.b = (color.b * calib.b) >> 8;

  return color.rgb;
}
