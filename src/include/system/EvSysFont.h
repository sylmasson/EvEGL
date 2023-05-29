
#ifndef     _EV_SYSFONT_H_
#define     _EV_SYSFONT_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvFont
{
// Begin of Font Metrics Blocks
  uint8_t         Width[128]; ///< Width of each character in pixels.
  uint32_t        Layout;     ///< Font bitmap layout: L1, L2, L4 or L8.
  uint32_t        Stride;     ///< Font line stride in bytes.
  uint32_t        WidthMax;   ///< Font width in pixels.
  uint32_t        Height;     ///< Font height in pixels.
  uint32_t        Addr;       ///< Address of font graphics data in EVE controller.
// End of Font Metrics Blocks
  uint8_t         RomFont;    ///< ROM Font number. Otherwise 0.
  uint8_t         CurWidth;   ///< Normal cursor width in pixels.
  uint8_t         CharBeg;    ///< First character in font graphics data.
  uint8_t         CharCnt;    ///< Character count in font graphics data.
  uint32_t        BmpSize;    ///< Size of all bitmap font graphics data.
  uint32_t        DataSize;   ///< Size of compressed font graphics data.
  const uint8_t   *Data;      ///< Compressed font graphics data pointer.
  const char      *Tag;       ///< C string Tag pointer.
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSysFont
{
  public:
    EvSysFont(EvDisplay *Disp);

    uint8_t       FindFont(const EvFont *Fnt);
    uint8_t       LoadFont(const EvFont *Fnt);
    uint8_t       LoadFont(const EvFont *Fnt, uint8_t Font);
    uint8_t       LoadRomFont(uint8_t Font, uint8_t RomFont);
    void          UnloadFont(const EvFont *Fnt);
    void          UnloadFont(uint8_t Font);
    void          InitSystemFont(void);

  public:
    EvDisplay     *Disp;
    const EvFont  *SystemFont[32];  // SysFont[0] is always NULL. Handle 0 is reserved for Bitmap.
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SYSFONT_H_ */
