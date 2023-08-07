
#ifndef     _EV_EVE_H_
#define     _EV_EVE_H_

#include    <SPI.h>
#include    <include/system/EvSPI.h>
#include    <include/system/EvMalloc.h>

// Color constant

#define     OPACITY_MAX     256
#define     TRANSPARENT     ((uint16_t)0)
#define     C8(c5)          (((((c5) & 0x1F) * 527) + 23) >> 6)
#define     C5(c8)          (((((c8) & 0xFF) * 249) + 1014) >> 11)
#define     RGB555(r,g,b)   ((uint16_t)((C5(r) << 10) | (C5(g) << 5) | C5(b) | 0x8000))
#define     ARGB(a,r,g,b)   ((uint32_t)((((a) & 0xFFL) << 24) | (((r) & 0xFFL) << 16) | (((g) & 0xFFL) << 8) | ((b) & 0xFFL)))

// Fixed point convertion

#define     s7f8(x)         ((uint16_t)((int16_t)((float)(x) * 256.0) & 0xFFFF))
#define     u8f8(x)         ((uint16_t)((float)(x) * 256.0) & 0xFFFF)
#define     s8f8(x)         ((uint32_t)((int32_t)((float)(x) * 256.0) & 0x1FFFF))
#define     s1f15(x)        ((uint32_t)((int32_t)((float)(x) * 32768.0) & 0x1FFFF))
#define     s15f8(x)        ((uint32_t)((int32_t)((float)(x) * 256.0) & 0xFFFFFF))
#define     u16f8(x)        ((uint32_t)((float)(x) * 256.0) & 0xFFFFFF)
#define     s15f16(x)       ((uint32_t)((int32_t)((float)(x) * 65536.0)))
#define     u16f16(x)       ((uint32_t)((float)(x) * 65536.0))

// Primitive operation constant

#define     BITMAPS         1
#define     POINTS          2
#define     LINES           3
#define     LINE_STRIP      4
#define     EDGE_STRIP_R    5
#define     EDGE_STRIP_L    6
#define     EDGE_STRIP_A    7
#define     EDGE_STRIP_B    8
#define     RECTS           9

// Bitmap Layout/ext Format constant

#define     ARGB1555        0
#define     L1              1
#define     L4              2
#define     L8              3
#define     RGB332          4
#define     ARGB2           5
#define     ARGB4           6
#define     RGB565          7
#define     TEXT8X8         9
#define     TEXTVGA         10
#define     BARGRAPH        11
#define     PALETTED565     14
#define     PALETTED4444    15
#define     PALETTED8       16
#define     L2              17
#define     ASTC_4x4        37808
#define     ASTC_5x4        37809
#define     ASTC_5x5        37810
#define     ASTC_6x5        37811
#define     ASTC_6x6        37812
#define     ASTC_8x5        37813
#define     ASTC_8x6        37814
#define     ASTC_8x8        37815
#define     ASTC_10x5       37816
#define     ASTC_10x6       37817
#define     ASTC_10x8       37818
#define     ASTC_10x10      37819
#define     ASTC_12x10      37820
#define     ASTC_12x12      37821

// Bitmap Rendering mode constant

#define     NEAREST         0
#define     BILINEAR        1

// Bitmap Wrap mode constant

#define     BORDER          0
#define     REPEAT          1

// AlphaFunc and StencilFunc constant

#define     NEVER           0
#define     LESS            1
#define     LEQUAL          2
#define     GREATER         3
#define     GEQUAL          4
#define     EQUAL           5
#define     NOTEQUAL        6
#define     ALWAYS          7

// StencilOp constant

#define     KEEP            1
#define     REPLACE         2
#define     INCR            3
#define     DECR            4
#define     INVERT          5

// BlendFunc constant

#define     ZERO                    0
#define     ONE                     1
#define     SRC_ALPHA               2
#define     DST_ALPHA               3
#define     ONE_MINUS_SRC_ALPHA     4
#define     ONE_MINUS_DST_ALPHA     5

// BitmapSwizzle constant

#define     RED                     2
#define     GREEN                   3
#define     BLUE                    4
#define     ALPHA                   5

// Memory Register Mapping Address

#define     DLSWAP_FRAME            2

#define     FONT_ROOT               0x2FFFFC
#define     RAM_DL                  0x300000
#define     RAM_CMD                 0x308000
#define     RAM_JTBOOT              0x30B000
#define     EXTERNAL_FLASH          0x800000

#define     REG_CLOCK               0x302008
#define     REG_CMD_DL              0x302100
#define     REG_CMD_READ            0x3020f8
#define     REG_CMD_WRITE           0x3020fc
#define     REG_CMDB_SPACE          0x302574
#define     REG_CMDB_WRITE          0x302578
#define     REG_CPURESET            0x302020
#define     REG_CSPREAD             0x302068
#define     REG_DITHER              0x302060
#define     REG_DLSWAP              0x302054
#define     REG_FRAMES              0x302004
#define     REG_FREQUENCY           0x30200c
#define     REG_GPIO                0x302094
#define     REG_GPIO_DIR            0x302090
#define     REG_HCYCLE              0x30202c
#define     REG_HOFFSET             0x302030
#define     REG_HSIZE               0x302034
#define     REG_HSYNC0              0x302038
#define     REG_HSYNC1              0x30203c
#define     REG_ID                  0x302000
#define     REG_INT_EN              0x3020ac
#define     REG_INT_FLAGS           0x3020a8
#define     REG_INT_MASK            0x3020b0
#define     REG_MACRO_0             0x3020d8
#define     REG_MACRO_1             0x3020dc
#define     REG_MEDIAFIFO_READ      0x309014
#define     REG_MEDIAFIFO_WRITE     0x309018
#define     REG_OUTBITS             0x30205c
#define     REG_PCLK                0x302070
#define     REG_PCLK_POL            0x30206c
#define     REG_PLAY                0x30208c
#define     REG_PLAYBACK_FORMAT     0x3020c4
#define     REG_PLAYBACK_FREQ       0x3020c0
#define     REG_PLAYBACK_LENGTH     0x3020b8
#define     REG_PLAYBACK_LOOP       0x3020c8
#define     REG_PLAYBACK_PLAY       0x3020cc
#define     REG_PLAYBACK_READPTR    0x3020bc
#define     REG_PLAYBACK_START      0x3020b4
#define     REG_PWM_DUTY            0x3020d4
#define     REG_PWM_HZ              0x3020d0
#define     REG_ROTATE              0x302058
#define     REG_SOUND               0x302088
#define     REG_SWIZZLE             0x302064
#define     REG_TAG                 0x30207c
#define     REG_TAG_X               0x302074
#define     REG_TAG_Y               0x302078
#define     REG_TOUCH_ADC_MODE      0x302108
#define     REG_TOUCH_CHARGE        0x30210c
#define     REG_TOUCH_DIRECT_XY     0x30218c
#define     REG_TOUCH_DIRECT_Z1Z2   0x302190
#define     REG_TOUCH_MODE          0x302104
#define     REG_TOUCH_OVERSAMPLE    0x302114
#define     REG_TOUCH_RAW_XY        0x30211c
#define     REG_TOUCH_RZ            0x302120
#define     REG_TOUCH_RZTHRESH      0x302118
#define     REG_TOUCH_SCREEN_XY     0x302124
#define     REG_TOUCH_SETTLE        0x302110
#define     REG_TOUCH_TAG_XY        0x302128
#define     REG_TOUCH_TAG           0x30212c
#define     REG_TOUCH_TRANSFORM_A   0x302150
#define     REG_TOUCH_TRANSFORM_B   0x302154
#define     REG_TOUCH_TRANSFORM_C   0x302158
#define     REG_TOUCH_TRANSFORM_D   0x30215c
#define     REG_TOUCH_TRANSFORM_E   0x302160
#define     REG_TOUCH_TRANSFORM_F   0x302164
#define     REG_TOUCH_CONFIG        0x302168
#define     REG_TRACKER             0x309000
#define     REG_TRIM                0x302180
#define     REG_VCYCLE              0x302040
#define     REG_VOFFSET             0x302044
#define     REG_VOL_PB              0x302080
#define     REG_VOL_SOUND           0x302084
#define     REG_VSIZE               0x302048
#define     REG_VSYNC0              0x30204c
#define     REG_VSYNC1              0x302050

// Touch Engine Register

#define     REG_CTOUCH_MODE         0x302104  // same as REG_TOUCH_MODE
#define     REG_CTOUCH_EXTENDED     0x302108
#define     REG_CTOUCH_TOUCH1_XY    0x30211C  // or REG_TOUCH_RAW_XY in compatibility mode
#define     REG_CTOUCH_TOUCH4_Y     0x302120
#define     REG_CTOUCH_TOUCH_XY     0x302124  // same as REG_TOUCH_SCREEN_XY
#define     REG_CTOUCH_TAG_XY       0x302128  // same as REG_TOUCH_TAG_XY
#define     REG_CTOUCH_TAG          0x30212C  // same as REG_TOUCH_TAG
#define     REG_CTOUCH_TAG1_XY      0x302130
#define     REG_CTOUCH_TAG1         0x302134
#define     REG_CTOUCH_TAG2_XY      0x302138
#define     REG_CTOUCH_TAG2         0x30213C
#define     REG_CTOUCH_TAG3_XY      0x302140
#define     REG_CTOUCH_TAG3         0x302144
#define     REG_CTOUCH_TAG4_XY      0x302148
#define     REG_CTOUCH_TAG4         0x30214C
#define     REG_CTOUCH_TOUCH4_X     0x30216C
#define     REG_CTOUCH_TOUCH2_XY    0x30218C
#define     REG_CTOUCH_TOUCH3_XY    0x302190

// BT815

#define     REG_GPIOX_DIR           0x302098
#define     REG_GPIOX               0x30209c
#define     REG_FLASH_SIZE          0x309024
#define     REG_FLASH_STATUS        0x3025f0
#define     REG_ADAPTIVE_FRAMERATE  0x30257c

// BT817/8

#define     REG_PCLK_FREQ           0x302614
#define     REG_PCLK_2X             0x302618

// Coprocessor Supported Commands

#define     CMD_APPEND              0xFFFFFF1E
#define     CMD_CALIBRATE           0xFFFFFF15
#define     CMD_DLSTART             0xFFFFFF00
#define     CMD_GRADIENT            0xFFFFFF0B
#define     CMD_GRADIENTA           0xFFFFFF57
#define     CMD_INFLATE             0xFFFFFF22
#define     CMD_LOADIDENTITY        0xFFFFFF26
#define     CMD_LOADIMAGE           0xFFFFFF24
#define     CMD_MEMCPY              0xFFFFFF1D
#define     CMD_MEMSET              0xFFFFFF1B
#define     CMD_MEMWRITE            0xFFFFFF1A
#define     CMD_MEMZERO             0xFFFFFF1C
#define     CMD_ROTATE              0xFFFFFF29
#define     CMD_ROTATEAROUND        0xFFFFFF51
#define     CMD_ROMFONT             0xFFFFFF3F
#define     CMD_SCALE               0xFFFFFF28
#define     CMD_SETBITMAP           0xFFFFFF43
#define     CMD_SETFONT             0xFFFFFF2B
#define     CMD_SETFONT2            0xFFFFFF3B
#define     CMD_SETSCRATCH          0xFFFFFF3C
#define     CMD_SETROTATE           0xFFFFFF36
#define     CMD_SETMATRIX           0xFFFFFF2A
#define     CMD_SWAP                0xFFFFFF01
#define     CMD_TRANSLATE           0xFFFFFF27

#define     CMD_HSF                 0xFFFFFF62  // BT817/8
#define     CMD_APILEVEL            0xFFFFFF63

// Display List Commands encoding

#define     EV_ALPHA_FUNC(func,ref)        ((0x09UL << 24) | (((func) & 7) << 8) | (uint8_t)(ref))
#define     EV_BEGIN(prim)                 ((0x1FUL << 24) | ((prim) & 0x0F))
#define     EV_BITMAP_EXT_FORMAT(fmt)      ((0x2EUL << 24) | (fmt))
#define     EV_BITMAP_HANDLE(handle)       ((0x05UL << 24) | ((handle) & 0x1F))
#define     EV_BITMAP_LAYOUT(fmt,ls,h)     ((0x07UL << 24) | (((fmt) & 0x1F) << 19) | (((ls) & 0x3FF) << 9) | ((h) & 0x1FF))
#define     EV_BITMAP_LAYOUT_H(ls,h)       ((0x28UL << 24) | (((ls) >> 10) << 2) | (((h) >> 9) & 3))
#define     EV_BITMAP_SIZE(ft,wx,wy,w,h)   ((0x08UL << 24) | (((ft) & 1) << 20) | (((wx) & 1) << 19) | (((wy) & 1) << 18) | (((w) & 0x1FF) << 9) | ((h) & 0x1FF))
#define     EV_BITMAP_SIZE_H(w,h)          ((0x29UL << 24) | (((w) >> 9) << 2) | (((h) >> 9) & 3))
#define     EV_BITMAP_SOURCE(addr)         ((0x01UL << 24) | ((addr) & 0xFFFFFF))
#define     EV_BITMAP_SWIZZLE(r,g,b,a)     ((0x2FUL << 24) | ((((r) & 7) << 9) | (((g) & 7) << 6) | (((b) & 7) << 3) | ((a) & 7)))
#define     EV_BITMAP_TRANSFORM_A(coeff)   ((0x15UL << 24) | ((coeff) & 0x3FFFF))
#define     EV_BITMAP_TRANSFORM_B(coeff)   ((0x16UL << 24) | ((coeff) & 0x3FFFF))
#define     EV_BITMAP_TRANSFORM_C(coeff)   ((0x17UL << 24) | ((coeff) & 0xFFFFFF))
#define     EV_BITMAP_TRANSFORM_D(coeff)   ((0x18UL << 24) | ((coeff) & 0x3FFFF))
#define     EV_BITMAP_TRANSFORM_E(coeff)   ((0x19UL << 24) | ((coeff) & 0x3FFFF))
#define     EV_BITMAP_TRANSFORM_F(coeff)   ((0x1AUL << 24) | ((coeff) & 0xFFFFFF))
#define     EV_BLEND_FUNC(src,dst)         ((0x0BUL << 24) | (((src) & 7) << 3) | ((dst) & 7))
#define     EV_CALL(dest)                  ((0x1DUL << 24) | ((dest) & 0xFFFF))
#define     EV_CELL(cell)                  ((0x06UL << 24) | ((cell) & 0x7F))
#define     EV_CLEAR(c,s,t)                ((0x26UL << 24) | ((((c) & 1) << 2) | (((s) & 1) << 1) | ((t) & 1)))
#define     EV_CLEAR_COLOR_A(alpha)        ((0x0FUL << 24) | (uint8_t)(alpha))
#define     EV_CLEAR_COLOR_RGB(r,g,b)      ((0x02UL << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
#define     EV_CLEAR_COLOR_RGB24(rgb)      ((0x02UL << 24) | ((rgb) & 0xFFFFFF))
#define     EV_CLEAR_STENCIL(s)            ((0x11UL << 24) | (uint8_t)(s))
#define     EV_CLEAR_TAG(s)                ((0x12UL << 24) | (uint8_t)(s))
#define     EV_COLOR_A(alpha)              ((0x10UL << 24) | (uint8_t)(alpha))
#define     EV_COLOR_RGB(r,g,b)            ((0x04UL << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
#define     EV_COLOR_RGB24(rgb)            ((0x04UL << 24) | ((rgb) & 0xFFFFFF))
#define     EV_COLOR_MASK(r,g,b,a)         ((0x20UL << 24) | (((r) & 1) << 3) | (((g) & 1) << 2) | (((b) & 1) << 1) | ((a) & 1))
#define     EV_COLOR_MASK4(rgba)           ((0x20UL << 24) | ((rgba) & 0x0F))
#define     EV_DISPLAY()                   ((0x00UL << 24) | 0)
#define     EV_END()                       ((0x21UL << 24) | 0)
#define     EV_JUMP(dest)                  ((0x1EUL << 24) | ((dest) & 0xFFFF))
#define     EV_LINE_WIDTH(radius)          ((0x0EUL << 24) | ((radius) & 0xFFF))
#define     EV_MACRO(m)                    ((0x25UL << 24) | ((m) & 1))
#define     EV_NOP()                       ((0x2DUL << 24) | 0)
#define     EV_PALETTE_SOURCE(addr)        ((0x2AUL << 24) | ((addr) & 0x3FFFFF))
#define     EV_POINT_SIZE(radius)          ((0x0DUL << 24) | ((radius) & 0x1FFF))
#define     EV_RETURN()                    ((0x24UL << 24) | 0)
#define     EV_RESTORE_CONTEXT()           ((0x23UL << 24) | 0)
#define     EV_SAVE_CONTEXT()              ((0x22UL << 24) | 0)
#define     EV_SCISSOR_SIZE(w,h)           ((0x1CUL << 24) | (((w) & 0xFFF) << 12) | ((h) & 0xFFF))
#define     EV_SCISSOR_XY(x,y)             ((0x1BUL << 24) | (((x) & 0x7FF) << 11) | ((y) & 0x7FF))
#define     EV_STENCIL_FUNC(func,ref,mask) ((0x0AUL << 24) | (((func) & 0x0F) << 16) | (((uint8_t)(ref) << 8) | (uint8_t)(mask)))
#define     EV_STENCIL_MASK(mask)          ((0x13UL << 24) | (uint8_t)(mask))
#define     EV_STENCIL_OP(fail,pass)       ((0x0CUL << 24) | ((fail) & 7) << 3 | ((pass) & 7))
#define     EV_TAG(s)                      ((0x03UL << 24) | (uint8_t)(s))
#define     EV_TAG_MASK(mask)              ((0x14UL << 24) | ((mask) & 1))
#define     EV_VERTEX2F(x,y)               ((0x01UL << 30) | (((x) & 0x7FFF) << 15) | ((y) & 0x7FFF))
#define     EV_VERTEX2II(x,y,handle,cell)  ((0x02UL << 30) | (((x) & 0x1FF) << 21) | (((y) & 0x1FF) << 12) | (((handle) & 0x1F) << 7) | ((cell) & 0x7F))
#define     EV_VERTEX_FORMAT(fmt)          ((0x27UL << 24) | ((fmt) & 7))
#define     EV_VERTEX_TRANSLATE_X(x)       ((0x2BUL << 24) | ((x) & 0x1FFFF))
#define     EV_VERTEX_TRANSLATE_Y(y)       ((0x2CUL << 24) | ((y) & 0x1FFFF))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEVE : public EvSPI
{
  union argb32
  {
    uint32_t      rgb;
    uint32_t      argb;
    struct
    {
      uint8_t     b;
      uint8_t     g;
      uint8_t     r;
      uint8_t     a;
    };
  };

  struct Context
  {
    uint8_t       handle;
    uint8_t       format;
    uint16_t      lineWidth;
    uint16_t      pointSize;
    uint16_t      opacity;
    uint16_t      colorA;
    uint16_t      clearColorA;
    uint32_t      colorRGB;
    uint32_t      clearColorRGB;
  };

  public:
    EvEVE(const uint32_t *Config, uint8_t CS, int16_t RST = -1, SPIClass *Spi = NULL, uint32_t Baudrate = 16000000);

    void          ClearContext(void);
    void          ClearPrimitive(void);
    void          ClearConvertToGray(void);
    void          ConvertToGray(uint8_t Min = 0, uint8_t Max = 255);
    void          ColorCalibration(int8_t R, int8_t G, int8_t B);
    void          Brightness(uint8_t Level);
    uint16_t      Opacity(uint16_t Opacity);
    const EvMem   *LoadBmp(const EvBmp *Bmp);
    bool          UnloadBmp(const EvBmp *Bmp);
    bool          UnloadBmp(const EvMem *ptr);

    uint16_t      ReadDL(void);
    void          SwapDL(void);

    // Display list command supported
    void          AlphaFunc(uint8_t Func, uint8_t Ref);
    void          Begin(uint8_t Prim);
    void          BitmapExtFormat(uint16_t Format);
    void          BitmapHandle(uint8_t Handle);
    void          BitmapLayout(uint8_t Format, uint16_t Width, uint16_t Height);
    void          BitmapSize(uint8_t Filter, uint8_t WrapX, uint8_t WrapY, uint16_t Width, uint16_t Height);
    void          BitmapSource(uint32_t Addr);
    void          BitmapSwizzle(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
    void          BitmapTransformA(int32_t Coeff);  //
    void          BitmapTransformB(int32_t Coeff);  // All Coeff must be s15f16 format.
    void          BitmapTransformC(int32_t Coeff);  // Coeff is converted to s15f8,
    void          BitmapTransformD(int32_t Coeff);  // s1f15 or s8f8 optimized for
    void          BitmapTransformE(int32_t Coeff);  // the selected EVE chip.
    void          BitmapTransformF(int32_t Coeff);  //
    void          BlendFunc(uint8_t Src, uint8_t Dst);
    void          Call(uint16_t Dest);
    void          Cell(uint8_t Cell);
    void          Clear(void);
    void          Clear(bool C, bool S, bool T);
    uint8_t       ClearColorA(uint8_t Alpha);
    uint32_t      ClearColorARGB(uint32_t Color);
    uint32_t      ClearColorRGB(uint32_t Color);
    uint32_t      ClearColorRGB(uint16_t Color555);
    uint32_t      ClearColorRGB(uint8_t R, uint8_t G, uint8_t B);
    void          ClearStencil(uint8_t Value);
    void          ClearTag(uint8_t Value);
    uint8_t       ColorA(uint8_t Alpha);
    uint32_t      ColorARGB(uint32_t Color);
    uint32_t      ColorRGB(uint32_t Color);
    uint32_t      ColorRGB(uint16_t Color555);
    uint32_t      ColorRGB(uint8_t R, uint8_t G, uint8_t B);
    void          ColorMask(bool R, bool G, bool B, bool A);
    void          ColorMask(uint8_t Mask);
    void          Display(void);
    void          End(void);
    void          Jump(uint16_t Dest);
    void          LineWidth(uint16_t Radius);
    void          Macro(uint8_t M);
    void          Nop(void);
    void          PaletteSource(uint32_t Addr);
    void          Return(void);
    void          RestoreContext(void);
    void          SaveContext(void);
    void          PointSize(uint16_t Radius);
    void          ScissorSize(uint16_t Width, uint16_t Height);
    void          ScissorXY(uint16_t X, uint16_t Y);
    void          StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask);
    void          StencilMask(uint8_t Mask);
    void          StencilOp(uint8_t Fail, uint8_t Pass);
    void          TagValue(uint8_t Value);
    void          TagMask(uint8_t Mask);
    void          Vertex2f(int16_t X, int16_t Y);
    void          Vertex2i(int16_t X, int16_t Y);
    void          Vertex2ii(uint16_t X, uint16_t Y, uint8_t Handle = 0, uint8_t Cell = 0);
    void          VertexFormat(uint8_t Fmt);
    void          VertexTranslateX(uint32_t X);
    void          VertexTranslateY(uint32_t Y);

    // Co-Processor command supported
    void          CmdAppend(uint32_t Addr, uint32_t Num);
    void          CmdCalibrate(void);
    void          CmdDlStart(void);
    void          CmdGradient(int16_t X0, int16_t Y0, uint32_t Color0, int16_t X1, int16_t Y1, uint32_t Color1);
    void          CmdGradientA(int16_t X0, int16_t Y0, uint32_t Color0, int16_t X1, int16_t Y1, uint32_t Color1);
    void          CmdInflate(uint32_t Addr);
    void          CmdLoadIdentity(void);
    void          CmdLoadImage(uint32_t Addr, uint32_t Options);
    void          CmdMemcpy(uint32_t Dest, uint32_t Src, uint32_t Num);
    void          CmdMemset(uint32_t Addr, uint8_t Value, uint32_t Num);
    void          CmdMemwrite(uint32_t Addr, uint32_t Num);
    void          CmdMemzero(uint32_t Addr, uint32_t Num);
    void          CmdRotate(int32_t Angle);
    void          CmdRomFont(uint32_t Font, uint32_t Romslot);
    void          CmdScale(int32_t X, int32_t Y);
    void          CmdSetBitmap(uint32_t Addr, uint16_t Layout, uint16_t Width, uint16_t Height);
    void          CmdSetFont2(uint32_t Font, uint32_t Addr, uint32_t FirstChar);
    void          CmdSetScratch(uint8_t Handle);
    void          CmdSetRotate(uint8_t R);
    void          CmdSetMatrix(void);
    void          CmdSwap(void);
    void          CmdTranslate(int32_t X, int32_t Y);

    EvMalloc      RAM_G;
    uint16_t      ChipID;
    bool          CapacitiveTouchEngine;  

  private:
    int32_t       transformCoeff(int32_t Coeff);
    uint32_t      colorCorrection(uint32_t Color);

    uint8_t       mActivePrim;
    uint8_t       mVertexCount;
    Context       mActiveContext;
    Context       mStackContext[4];
    int16_t       mStackContextCount;
    uint32_t      mColorCalibration;
    uint32_t      mConvertToGray;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const uint32_t   EvConfig_800x480_43[];
extern const uint32_t   EvConfig_800x480_50[];
extern const uint32_t   EvConfig_1024x600_70[];
extern const uint32_t   EvConfig_1077x600_70[];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_EVE_H_ */

