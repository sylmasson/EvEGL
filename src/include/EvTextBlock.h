
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

  public:
    bool          WrapText(void);
    bool          WrapText(bool Wrap);
    int16_t       LineSpacing(void);
    int16_t       LineSpacing(int Pixel);
    int16_t       LineSpacing(float Ratio);
    String        GetLine(uint16_t Line);
    void          SetOnTouch(void (*OnTouch)(EvTextBlock *Sender, EvTouchEvent *Touch));

    EvTextBlock   &operator+=(const char C);
    EvTextBlock   &operator+=(const char *Str);
    EvTextBlock   &operator+=(const String &Str);

    virtual size_t  write(uint8_t C);
    virtual size_t  write(const uint8_t *Buffer, size_t Count);
//    virtual int     availableForWrite(void) { return 1; };

  protected:
    EvTextBlock(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    const int8_t  mMinLineHeight = 5;

    bool          mWrapText;
    int8_t        mLineSpacing;
    uint16_t      mLinesCount;
    int32_t       mMaxWidth;
    SubString     *mLines;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

    bool          parseTextAsLines(void);
    String        getLine(uint16_t Line);

  private:
    void          (*mOnTouch)(EvTextBlock *Sender, EvTouchEvent *Touch);

  public:
    const String    &Text = mLabel;
    const uint16_t  &LinesCount = mLinesCount;

    static EvTextBlock  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TEXTBLOCK_H_ */
