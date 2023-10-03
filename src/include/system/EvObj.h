
#ifndef     _EV_OBJ_H_
#define     _EV_OBJ_H_

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
 * @brief     EvTouchEvent is the struct that contains the information of the Touch Event.
 * 
 * @subsection  TOUCH_EVENT List of Touch Event
 *
 * @tableofcontents
 *    Event     |                    Description
 * ------------ | ----------------------------------------------------
 * TOUCH_DONE   | No event occurs
 * TOUCH_START  | Triggered immediately upon touching the object
 * TOUCH_END    | Triggered after releasing the active object
 * TOUCH_MOVE   | Triggered when moving the touch position
 * TOUCH_HOLD   | Triggered when the hold touch time has expired
 * TOUCH_REPEAT | Triggered when the repeat touch time has expired
 * TOUCH_DOUBLE | Triggered when the double touch event is encountered
 * TOUCH_CANCEL | Triggered to force the release of the object
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

union EvTouchPos
{
  uint32_t    xy;
  struct
  {
    int16_t   y;
    int16_t   x;
  };
};

struct EvTouchEvent
{
  uint8_t     status;       ///< Status flags for the Touch Event process.
  uint8_t     event;        ///< Current Touch Event. See @ref TOUCH_EVENT.
  uint8_t     tag;          ///< Tag value provided by the display.
  uint8_t     id;
  int16_t     x;            ///< Horizontal relative touch position to the active object.
  int16_t     y;            ///< Vertical relative touch position to the active object.
  EvTouchPos  abs;          ///< Horizontal and vertical absolute touch position.
  EvTouchPos  prev;         ///< Horizontal and vertical previous absolute touch position.
  EvTouchPos  move;         ///< Horizontal and vertical moving touch position.
  EvObj       *obj;
  EvObj       *owner;
  uint32_t    timer;        ///< Timer value in msec since the object was initially touched.
  uint32_t    endTimer;     ///< Timestamp in msec when the finger is release.
  uint32_t    startTimer;   ///< Timestamp in msec when the object was initially touched.
  uint32_t    repeatTimer;  ///< Timestamp in msec used for the next TOUCH_HOLD or TOUCH_REPEAT events.
  uint16_t    repeatDelay;  ///< Time in msec defined between TOUCH_REPEAT events.
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
  uint16_t    color;        ///< Normal text color.
  uint16_t    color2;       ///< Second text color option. See object details.
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvObj class is the parent class for all visual object classes.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvObj : public Print
{
  public:
    EvObj(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
    virtual       ~EvObj(void);

    int16_t       Left(void);
    int16_t       Top(void);
    uint16_t      Width(void);
    uint16_t      Height(void);
    uint8_t       Shape(void);
    void          Show(void);
    void          Hide(void);
    void          Abort(void);
    void          Enable(void);
    void          Disable(void);
    void          Delete(void);
    void          Modified(void);
    void          ModifiedText(void);
    bool          IsVisible(void);
    bool          IsEnabled(void);
    bool          IsModified(void);
    bool          IsModifiedText(void);
    bool          IsControlObj(void);
    bool          IsMultiTouchObj(void);
    void          BdShape(uint8_t Shape);
    void          BdRadius(uint16_t Radius);  // 1/16 pixel
    void          BdWidth(uint16_t Width);    // 1/16 pixel
    void          BdColor(uint16_t Color);
    void          BgColor(uint16_t Color);
    void          MoveRel(int16_t X, int16_t Y);
    void          MoveTo(int16_t Left, int16_t Top);
    void          MoveTo(EvPanel *Dest);
    void          ReSize(uint16_t Width, uint16_t Height);
    EvPanel       *GetOwner(void);
    void          SetOwner(EvPanel *Owner);
    void          ToFront(bool AllOwner = true);
    void          SetKbdFocus(uint8_t Layout = LAYOUT_SHIFT);
    void          LostKbdFocus(void);
    EvObj         *GetKbdFocus(void);
    bool          IsOnKbdFocus(void);

    virtual void    ClearCache(void);
    virtual void    SetOpacity(uint16_t Opacity);
    virtual void    ModifiedAll(void) { Modified(); };
    virtual size_t  WriteKey(uint8_t Key, uint8_t Layout, uint8_t ShiftKey, bool DoubleTouch) { write(Key); return -1; };
    virtual size_t  write(uint8_t C) { return 1; };
    virtual size_t  write(const uint8_t *Buffer, size_t Count) { return Count; };
    virtual int     availableForWrite(void) { return 1; };

    // Text functions
    void          TextClear(void);
    void          TextFont(uint8_t Font);
    void          TextAlign(uint8_t Align);
    void          TextPadding(int8_t X, int8_t Y);
    void          TextColor(uint16_t Color, uint16_t Color2 = 0);
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

  public:         // Public functions reserved for the system
    void          DisplayTagList(void);
    bool          TouchStart(void);
    void          TouchEnd(void);
    void          TouchMax(uint8_t Max);
    void          TouchUpdate(EvTouchEvent *Touch);
    virtual EvObj *Touching(EvTouchEvent *Touch);
//    virtual bool  MustBeDraw(void);
    virtual void  SetDisplay(EvDisplay *Disp);
    virtual void  SetView(void);
    virtual void  Refresh(void);
    virtual void  Preload(void);
    virtual void  Draw(void);

  protected:
    virtual void  drawEvent(void) {};
    virtual void  resizeEvent(void) {};
    virtual void  refreshEvent(void) {};
    virtual void  preloadEvent(void) {};
    virtual void  setKbdFocusEvent(void) {};
    virtual void  lostKbdFocusEvent(void) {};
    virtual void  touchEvent(EvTouchEvent *Touch) {};

    uint16_t      mStatus;
    uint16_t      mOpacity;

    int16_t       mLeft;
    int16_t       mTop;
    uint16_t      mWidth;
    uint16_t      mHeight;

    uint8_t       mTouchCnt;
    uint8_t       mTouchMax;

    uint8_t       mBdShape;
    uint16_t      mBdRadius;  // 1/16 pixel
    uint16_t      mBdWidth;   // 1/16 pixel
    uint16_t      mBdColor;
    uint16_t      mBgColor;

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

    static EvObj      *TryCreate(EvObj *Obj, EvPanel *Dest);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_OBJ_H_ */
