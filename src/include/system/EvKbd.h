
#ifndef     _EV_KBD_H_
#define     _EV_KBD_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvKeyStyle
{
  uint16_t    width;
  uint16_t    height;
  uint16_t    radius;
  uint16_t    colorUp;
  uint16_t    colorDown;
  uint16_t    textColorUp;
  uint16_t    textColorDown;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvKbdMapping
{
  int16_t     left;
  int16_t     top;
  uint8_t     font;
  uint8_t     style;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvKbdLayout
{
  uint16_t    keyCount;
  const char  *keyChar;
  const char  **keyLabel;
  const EvKeyStyle  *keyStyle;
  const EvKbdMapping  *kbdMap;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvKeyboard
{
  int16_t     left;
  int16_t     top;
  uint16_t    width;
  uint16_t    height;
  uint16_t    bgColor;
  uint8_t     bdRadius;
  uint8_t     keyShadow;
  uint8_t     overKeyFont;
  uint8_t     overKeyExtend;
  uint8_t     layoutCount;
  const EvKbdLayout *layout;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvKbd : public EvSideBar
{
  public:
    void          Open(void);
    void          Close(void);
    void          SetLayout(uint8_t Layout);
    void          SetKeyboard(int Keyboard, bool OpenState = false);
    void          SetKeyboard(EvKeyboard *Keyboard, bool OpenState = false);
    void          SetOnTouch(void (*OnTouch)(EvKbd *Sender, EvTouchEvent *Touch));

  protected:
    EvKbd(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    uint8_t       mKey;
    uint8_t       mKeyId;
    uint8_t       mLayout;
    uint8_t       mPrevKey;
    uint8_t       mShiftKey;
    bool          mDoubleTouch;
    EvLabel       *mOverKey;
    EvKeyboard    *mKb;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvKbd *Sender, EvTouchEvent *Touch);

    void          writeKey(uint8_t Key);

  public:
    EvObj         *FocusObj;

    static EvKbd  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_KBD_H_ */

