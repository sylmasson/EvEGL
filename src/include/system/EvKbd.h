/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef     _EV_KBD_H_
#define     _EV_KBD_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     LAYOUT_ALPHA    0
#define     LAYOUT_NUM1     1
#define     LAYOUT_NUM2     2
#define     LAYOUT_KEYPAD   3
#define     LAYOUT_SHIFT    (1 << 6)
#define     LAYOUT_CAPS     (1 << 7)

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
  uint16_t    overKeyBdColor;
  uint8_t     layoutCount;
  const EvKbdLayout *layout;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvKbd : public EvSideBar
{
  protected:
    EvKbd(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    void          Open(void);
    void          Close(void);
    void          SetLayout(uint8_t Layout);
    void          SetKeyboard(int Keyboard, bool OpenState = false);
    void          SetKeyboard(EvKeyboard *Keyboard, bool OpenState = false);
    void          SetKeyboard(EvKeyboard *Landscape, EvKeyboard *Portrait);
    void          SetOnTouch(void (*OnTouch)(EvKbd *Sender, const EvTouchEvent *Touch));

  protected:
    uint8_t       mKey;
    uint8_t       mKeyId;
    uint8_t       mLayout;
    uint8_t       mPrevKey;
    uint8_t       mShiftKey;
    bool          mDoubleTouch;
    EvLabel       *mOverKey;
    EvKeyboard    *mKb;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvKbd *Sender, const EvTouchEvent *Touch);

    void          writeKey(uint8_t Key);

  public:
    EvObj         *FocusObj;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvKbd  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_KBD_H_ */

