
#ifndef     _EV_TEXTBOX_H_
#define     _EV_TEXTBOX_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTextBox : public EvPanel
{
  public:
    void          Unselect(void);
    void          SelectAll(void);
    void          TextClear(void);
    void          TextLabel(const char *Label);
    void          TextLabel(const String &Label);
    void          SetMaxLength(uint16_t MaxLength);
    void          SetOnTouch(void (*OnTouch)(EvTextBox *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvTextBox *Sender, const String &Str));
    void          SetOnFilter(int (*OnFilter)(EvTextBox *Sender, const uint8_t C));
    void          SetOnFocus(void (*OnSetFocus)(EvTextBox *Sender), void (*OnLostFocus)(EvTextBox *Sender));

    EvTextBox     &operator+=(const char C);
    EvTextBox     &operator+=(const char *Str);
    EvTextBox     &operator+=(const String &Str);

    virtual size_t  WriteKey(uint8_t Key, uint8_t Layout, uint8_t ShiftKey, bool DoubleTouch);
    virtual size_t  write(uint8_t C);
    virtual size_t  write(const uint8_t *Buffer, size_t Count);
//    virtual int     availableForWrite(void) { return 1; };

    virtual void  setFocusEvent(void);
    virtual void  lostFocusEvent(void);

  protected:
    EvTextBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    void          selectWord(EvTouchEvent *Touch);
    void          moveToWord(EvTouchEvent *Touch);
    void          moveToChar(EvTouchEvent *Touch);
    void          moveCursor(void);
    int16_t       textLeft(void);
    int16_t       textTop(void);

    virtual int   filter(uint8_t C);
    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

    uint8_t       mFlags;
    uint16_t      mMaxLength;
    uint16_t      mCursorIndex;
    int16_t       mSelectBegin;
    int16_t       mSelectCount;

  private:
    void          (*mOnTouch)(EvTextBox *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvTextBox *Sender, const String &Str);
    int           (*mOnFilter)(EvTextBox *Sender, const uint8_t C);
    void          (*mOnSetFocus)(EvTextBox *Sender);
    void          (*mOnLostFocus)(EvTextBox *Sender);

  public:
    EvTextCursor  *Cursor;
    const String  &Text = mLabel;
    bool          SelectAllOnFocus;

    static EvTextBox  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TEXTBOX_H_ */
