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

#ifndef     _EV_TEXTBLOCK_H_
#define     _EV_TEXTBLOCK_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTextBlock : public EvScrollBox
{
  struct SubString
  {
    uint16_t      from;
    uint16_t      to;
  };

  protected:
    EvTextBlock(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    bool          WrapText(void);
    bool          WrapText(bool Wrap);
    int16_t       LineSpacing(void);
    int16_t       LineSpacing(int Pixel);
    int16_t       LineSpacing(float Ratio);
    String        GetLine(uint16_t Line);
    void          ClearBuffer(void);
    bool          SetBufferSize(uint16_t Size);
    void          SetOnTouch(void (*OnTouch)(EvTextBlock *Sender, const EvTouchEvent *Touch));

    EvTextBlock   &operator+=(const char C);
    EvTextBlock   &operator+=(const char *Str);
    EvTextBlock   &operator+=(const String &Str);

    virtual size_t  write(uint8_t C);
    virtual size_t  write(const uint8_t *Buffer, size_t Count);

  protected:
    const int8_t  mMinLineHeight = 5;

    bool          mWrapText;
    int8_t        mLineSpacing;
    uint16_t      mLinesCount;
    uint16_t      mBufferSize;
    int32_t       mMaxWidth;
    SubString     *mLines;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

    bool          parseTextAsLines(void);
    String        getLine(uint16_t Line);

  private:
    void          (*mOnTouch)(EvTextBlock *Sender, const EvTouchEvent *Touch);

  public:
    const String    &Text = mLabel;
    const uint16_t  &LinesCount = mLinesCount;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvTextBlock  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TEXTBLOCK_H_ */
