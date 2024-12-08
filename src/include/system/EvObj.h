
#ifndef     _EV_OBJ_H_
#define     _EV_OBJ_H_

// Object Status Flags constant

#define     VISIBLE_OBJ     (1 << 0)
#define     MODIFIED_OBJ    (1 << 1)
#define     DISABLED_OBJ    (1 << 2)
#define     MOVED_OBJ       (1 << 3)
#define     FIXED_OBJ       (1 << 4)
#define     FLOATING_OBJ    (1 << 5)
#define     CONTROL_OBJ     (1 << 6)
#define     MODIF_TEXT_OBJ  (1 << 7)
#define     FUNCT_USED_OBJ  (1 << 8)
#define     FULLSCREEN_OBJ  (1 << 9)
#define     FILTER_DIS_OBJ  (1 << 13)
#define     ABORT_OBJ       (1 << 14)
#define     SYSTEM_OBJ      (1 << 15) // reserved for GUI System

#define     VISIBLE_DIS_OBJ (VISIBLE_OBJ | DISABLED_OBJ)

// Shape constant

#define     ROUND_CORNERS   0
#define     RATIO_CORNERS   1
#define     FIXED_CORNERS   2
#define     SQUARE_CORNERS  3
#define     USER_CORNERS    4
#define     SHADOW          (1 << 3)  // Additionnal shadow if supported

// Alignment constant

#define     LEFT_TOP        0
#define     CENTER_TOP      1
#define     RIGHT_TOP       2
#define     JUSTIFY_TOP     3
#define     LEFT_CENTER     4
#define     CENTER_CENTER   5
#define     RIGHT_CENTER    6
#define     JUSTIFY_CENTER  7
#define     LEFT_BOTTOM     8
#define     CENTER_BOTTOM   9
#define     RIGHT_BOTTOM    10
#define     JUSTIFY_BOTTOM  11
#define     CENTER          5

#define     ALIGNMENT_LOCK  (1 << 7)

// Trace flags constant

#define     TRACE_TOUCH     (1 << 0)
#define     TRACE_FPS       (1 << 1)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvView is the informations struct of the partial View of the object.
 * 
 * EvView is redefined by the EvObj::SetView() system function each time the object is moved or resized.
 * @li  \b ox and \b oy are used for the VertexTranslate command.
 * @li  \b x1 and \b y1 are used for the ScissorXY command.
 * @li  \b w and \b h are used for the ScissorSize command.
 * 
 * @image html ViewSchema.png
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvView
{
  int16_t     ox;   ///< Horizontal offset of the Left position of the object from to the upper left corner of the display.
  int16_t     oy;   ///< Vertial offset of the Top position of the object from to the upper left corner of the display.
  int16_t     x1;   ///< Horizontal position of the upper left corner of the partial View of the object.
  int16_t     y1;   ///< Vertial position of the upper left corner of the partial View of the object.
  int16_t     x2;   ///< Horizontal position of the lower right corner of the partial View of the object.
  int16_t     y2;   ///< Vertial position of the lower right corner of the partial View of the object.
  int16_t     w;    ///< Width of the partial View of the object.
  int16_t     h;    ///< Height of the partial View of the object.
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvTextStyle is the struct that contains information for style of the text.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvTextStyle
{
  uint8_t     font;         ///< Text Font number.
  uint8_t     align;        ///< Text alignment. see EvObj::TextAlign()
  int8_t      padX;         ///< Horizontal text padding.
  int8_t      padY;         ///< Vertical text padding.
  color16     color;        ///< Normal text color.
  color16     color2;       ///< Second text color option. See object details.
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvObj class is the parent class for all visual object classes.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvObj : public Stream
{
  friend class EvPanel;
  friend class EvShell;

  protected:
    EvObj(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvObj(void);

    int16_t       Left(void);
    int16_t       Top(void);
    uint16_t      Width(void);
    uint16_t      Height(void);
    uint8_t       Shape(void);
    void          Enable(void);
    void          Disable(void);
    void          Delete(void);
    void          Modified(void);
    void          ModifiedText(void);
    bool          IsVisible(void);
    bool          IsViewable(void);
    bool          IsEnabled(void);
    bool          IsModified(void);
    bool          IsModifiedText(void);
    bool          IsControlObj(void);
    bool          IsFullScreen(void);
    bool          IsMultiTouchObj(void);
    void          BdShape(uint8_t Shape);
    void          BdRadius(uint16_t Radius);  // 1/16 pixel
    void          BdWidth(uint16_t Width);    // 1/16 pixel
    void          BdColor(uint16_t Color);
    void          BgColor(uint16_t Color);
    void          BgColor(uint16_t Color, uint8_t Alpha);
    void          MoveRel(int16_t X, int16_t Y);
    void          MoveTo(int16_t Left, int16_t Top);
    void          MoveTo(EvPanel *Dest);
    void          ReSize(uint16_t Width, uint16_t Height);
    void          OwnerAlign(uint8_t Align, int16_t PadX = 0, int16_t PadY = 0);
    EvPanel       *GetOwner(uint16_t Level = 1);
    void          SetOwner(EvPanel *Owner);
    void          ToFront(bool AllOwner = true);
    void          SetKbdFocus(void);
    void          LostKbdFocus(void);
    EvObj         *GetKbdFocus(void);
    bool          IsOnKbdFocus(void);
    uint16_t      TouchCnt(void);
    uint16_t      TouchMax(void);
    uint16_t      TouchMax(uint16_t Max);

    virtual void    Show(void);
    virtual void    Hide(void);
    virtual void    ClearCache(void);
    virtual void    SetOpacity(uint16_t Opacity);
    virtual void    ModifiedAll(void) { Modified(); };
    virtual bool    IsPanelObj(void) { return false; };
    virtual size_t  WriteKey(uint8_t Key, uint8_t Layout, uint8_t ShiftKey, bool DoubleTouch) { write(Key); return -1; };

    // Pure virtual Stream functions
    virtual int     available(void) { return 0; };
    virtual int     peek(void) { return -1; };
    virtual int     read(void) { return -1; };
    virtual size_t  write(uint8_t C) { return 1; };
    virtual int     availableForWrite(void) { return 1; };

    // Text functions
    void          TextClear(void);
    void          TextFont(uint8_t Font);
    void          TextAlign(uint8_t Align);
    void          TextPadding(int8_t X, int8_t Y);
    void          TextColor(uint16_t Color);
    void          TextColor(uint16_t Color, uint16_t Color2);
    void          TextLabel(const char *Label);
    void          TextLabel(const String &Label);
    int16_t       TextHeight(uint8_t Font = 0);
    int16_t       TextWidth(const char C, uint8_t Font = 0);
    int16_t       TextWidth(String &Str, uint8_t Font = 0, int16_t Count = -1);
    int16_t       TextWidth(const char *Str, uint8_t Font = 0, int16_t Count = -1);
    int16_t       TextCursorWidth(uint8_t Font = 0);

    // Drawing functions
    void          DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str);
    void          DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str, uint16_t Color);
    void          DrawText(int16_t Left, int16_t Top, int16_t Width, int16_t Height, const char *Str, uint16_t Color, uint8_t Font, uint8_t Align = LEFT_TOP, int16_t PadX = 0, int16_t PadY = 0);
    void          DrawCircle(int16_t X, int16_t Y, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth = 16);
    void          DrawCircle2f(int16_t X, int16_t Y, uint16_t BdColor, uint16_t Radius, uint16_t BdWidth = 16);
    void          FillCircle(int16_t x, int16_t y, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth = 0, uint16_t BdColor = 0);
    void          FillCircle2f(int16_t x, int16_t y, uint16_t FillColor, uint16_t Radius, uint16_t BdWidth = 0, uint16_t BdColor = 0);
    void          DrawRectangle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t BdColor, uint16_t Radius = 16, uint16_t BdWidth = 16);
    void          DrawRectangle2f(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t BdColor, uint16_t Radius = 16, uint16_t BdWidth = 16);
    void          FillRectangle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t FillColor, uint16_t Radius = 16, uint16_t BdWidth = 0, uint16_t BdColor = 0);
    void          FillRectangle2f(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, uint16_t FillColor, uint16_t Radius = 16, uint16_t BdWidth = 0, uint16_t BdColor = 0);

    // Miscellaneous functions
    void          BeginFunction(uint16_t &Label, uint16_t CmdSize);
    void          EndFunction(uint16_t Label, uint16_t CmdSize);

    // Public functions reserved for the system
    void          DisplayTagList(Stream *Out);
    bool          TouchStart(void);
    void          TouchEnd(void);
    void          TouchUpdate(const EvTouchEvent *Touch);
    virtual EvObj *Touching(const EvTouchEvent *Touch);
    virtual void  SetDisplay(EvDisplay *Disp);
    virtual void  SetView(void);
    virtual void  Refresh(void);
    virtual void  Draw(void);

  protected:
    virtual void  drawEvent(void) {};
    virtual void  resizeEvent(void) {};
    virtual void  refreshEvent(void) {};
    virtual void  setKbdFocusEvent(void) {};
    virtual void  lostKbdFocusEvent(void) {};
    virtual void  touchEvent(const EvTouchEvent *Touch) {};

    uint16_t      mStatus;
    uint16_t      mOpacity;

    int16_t       mLeft;
    int16_t       mTop;
    uint16_t      mWidth;
    uint16_t      mHeight;

    uint8_t       mTouchCnt;
    uint8_t       mTouchMax;

    color16       mBgColor;
    uint8_t       mBgColorA;
    uint8_t       mBdShape;
    uint16_t      mBdRadius;  // 1/16 pixel
    uint16_t      mBdWidth;   // 1/16 pixel
    color16       mBdColor;

    String        mLabel;
    EvView        mView;
    EvTextStyle   mStyle;
    EvPanel       *mOwner;

  private:
    const EvMem   *mCache;

  public:
    EvDisplay         *Disp;            ///< The destination display pointer.
    const char        *Tag;             ///< The tag name of the object.
    int16_t           TagId;            ///< The tag Id of the object.
    const String      &Label = mLabel;  ///< The text label of the object.
    const EvTextStyle &Style = mStyle;  ///< The informations struct of style of the text of the object.
    const EvView      &View = mView;    ///< The informations struct of the partial View of the object.

  protected:
    void          abortCreate(void);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvObj  *TryCreate(EvObj *Obj, EvPanel *Dest);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_OBJ_H_ */
