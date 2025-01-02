#ifndef     _EV_COLOR_H_
#define     _EV_COLOR_H_

// Color constant

#define     OPACITY_MAX     256
#define     CL8(cl5)        (((((cl5) & 0x1F) * 527) + 23) >> 6)
#define     CL5(cl8)        (((((cl8) & 0xFF) * 249) + 1014) >> 11)
#define     RGB555(r,g,b)   ((uint16_t)((CL5(r) << 10) | (CL5(g) << 5) | CL5(b) | 0x8000))
#define     ARGB(a,r,g,b)   ((uint32_t)((((a) & 0xFFL) << 24) | (((r) & 0xFFL) << 16) | (((g) & 0xFFL) << 8) | ((b) & 0xFFL)))

// Basic color constant

#define     CL_BLACK        RGB555(  0,   0,   0)
#define     CL_WHITE        RGB555(255, 255, 255)
#define     CL_WHITE_SMOKE  RGB555(230, 230, 230)
#define     CL_RED          RGB555(255,   0,   0)
#define     CL_LIME         RGB555(  0, 255,   0)
#define     CL_BLUE         RGB555(  0,   0, 255)
#define     CL_YELLOW       RGB555(255, 255,   0)
#define     CL_CYAN         RGB555(  0, 255, 255)
#define     CL_MAGENTA      RGB555(255,   0, 255)
#define     CL_DIM_GRAY     RGB555(105, 105, 105)
#define     CL_GRAY         RGB555(128, 128, 128)
#define     CL_DARK_GRAY    RGB555(169, 169, 169)
#define     CL_SILVER       RGB555(192, 192, 192)
#define     CL_LIGHT_GRAY   RGB555(211, 211, 211)
#define     CL_MAROON       RGB555(128,   0,   0)
#define     CL_OLIVE        RGB555(128, 128,   0)
#define     CL_GREEN        RGB555(  0, 128,   0)
#define     CL_PURPLE       RGB555(128,   0, 128)
#define     CL_TEAL         RGB555(  0, 128, 128)
#define     CL_NAVY         RGB555(  0,   0, 128)

// Indexed system color

enum IndexedColor : uint16_t
{
  CL_NOCOLOR,
  CL_PANEL_BG,
  CL_DISPLAY_BG,
  CL_DEFAULT_BG,
  CL_SCROLL_BAR,
  CL_TEXTCURSOR,

  CL_DARK_TEXT,
  CL_LIGHT_TEXT,
  CL_DEFAULT_TEXT,
  CL_REVERSE_TEXT,
  CL_LABEL_TEXT,

  CL_BUTTON_FACE,
  CL_BUTTON_FACE_DOWN,
  CL_BUTTON_TEXT,
  CL_BUTTON_TEXT_DOWN,

  CL_TOGGLE_ON,
  CL_TOGGLE_OFF,
  CL_TOGGLE_KNOB,

  CL_SLIDER_LOWER,
  CL_SLIDER_UPPER,
  CL_SLIDER_KNOB,

  CL_PROGRESS_LOWER,
  CL_PROGRESS_UPPER,
  CL_PROGRESS_TEXT,

  CL_SELECTOR_BG,
  CL_SELECTOR_TRUE,
  CL_SELECTOR_NEW,
  CL_SELECTOR_TEXT,
  CL_SELECTOR_TEXT2,

  CL_TEXTBOX_BG,
  CL_TEXTBOX_BD,
  CL_TEXTBOX_FOCUS,
  CL_TEXTBOX_CURSOR,
  CL_TEXTBOX_SELECT,
  CL_TEXTBOX_TEXT,

  CL_NUMINT_BG,
  CL_NUMINT_HOLD,
  CL_NUMINT_INC,
  CL_NUMINT_DEC,
  CL_NUMINT_TEXT,

  CL_CHECKBOX_TRUE,
  CL_CHECKBOX_FALSE,
  CL_CHECKBOX_BD,
  CL_CHECKBOX_TEXT,
  CL_CHECKBOX_TEXT2,

  CL_KBD_BG,
  CL_KBD_KEY_BD,
  CL_KBD_STD_KEY_UP,
  CL_KBD_STD_KEY_DOWN,
  CL_KBD_CTRL_KEY_UP,
  CL_KBD_CTRL_KEY_DOWN,
  CL_KBD_SPACE_KEY_UP,
  CL_KBD_SPACE_KEY_DOWN,
  CL_KBD_TEXT_UP,
  CL_KBD_TEXT_DOWN,

  CL_TERMINAL_BG,
  CL_TERMINAL_TEXT,

  CL_PLAYER_BG,
  CL_PALYER_TIMELINE_LOWER,
  CL_PALYER_TIMELINE_UPPER,
  CL_PLAYER_TIMELINE_KNOB,
  CL_PLAYER_ICON,
  CL_PLAYER_TEXT,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvColor
{
  friend class      color16;

  public:
    EvColor(void);

    static uint16_t   GetTheme(void);
    static uint16_t   SwitchTheme(void);
    static bool       SelectTheme(uint16_t Theme);
    static void       LoadDefaultTheme(void);
    static bool       LoadTheme(uint16_t Theme, const uint16_t *Table);
    static bool       EditTheme(uint16_t Theme, uint8_t Index, uint16_t Color);
    static bool       IsModified(bool ClearAfter = false);

  protected:
    static uint8_t    sTheme;
    static bool       sModified;
    static uint16_t   sColorTable[2][256];
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class color16
{
  public:
    color16(void);
    color16(uint16_t Color);

    bool          Set(color16 &Color);
    bool          Set(uint16_t Color);
    uint16_t      Get(void);
    uint16_t      Raw(void);

  protected:
    uint16_t      mColor;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern EvColor    SystemColor;

#endif  /* _EV_COLOR_H_ */
