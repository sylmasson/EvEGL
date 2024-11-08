
#ifndef     _EV_TEXTBOX_H_
#define     _EV_TEXTBOX_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTextBox : public EvPanel
{
  protected:
    EvTextBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    void          Unselect(void);
    void          SelectAll(void);
    void          TextClear(void);
    void          TextLabel(const char *Label);
    void          TextLabel(const String &Label);
    void          SetMaxLength(uint16_t MaxLength);
    void          SetColor(uint16_t ColorBd, uint16_t ColorBdFocus = CL_TEXTBOX_FOCUS, uint16_t ColorTextSelect = CL_TEXTBOX_SELECT);
    void          SetOnTouch(void (*OnTouch)(EvTextBox *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvTextBox *Sender, const String &Str));
    void          SetOnReturn(void (*OnChange)(EvTextBox *Sender, const String &Str));
    void          SetOnFilter(int (*OnFilter)(EvTextBox *Sender, const uint8_t C));
    void          SetOnKbdFocus(void (*OnSetKbdFocus)(EvTextBox *Sender), void (*OnLostKbdFocus)(EvTextBox *Sender));

    EvTextBox     &operator+=(const char C);
    EvTextBox     &operator+=(const char *Str);
    EvTextBox     &operator+=(const String &Str);

    virtual size_t  WriteKey(uint8_t Key, uint8_t Layout, uint8_t ShiftKey, bool DoubleTouch);
    virtual size_t  write(uint8_t C);
    virtual size_t  write(const uint8_t *Buffer, size_t Count);

  protected:
    void          selectWord(const EvTouchEvent *Touch);
    void          moveToWord(const EvTouchEvent *Touch);
    void          moveToChar(const EvTouchEvent *Touch);
    void          moveCursor(void);
    int16_t       textLeft(void);
    int16_t       textTop(void);

    virtual int   filter(uint8_t C);
    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  setKbdFocusEvent(void);
    virtual void  lostKbdFocusEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

    uint8_t       mFlags;
    uint8_t       mAlign;
    uint16_t      mMaxLength;
    color16       mColorBd;
    color16       mColorBdFocus;
    color16       mColorTextSelect;
    uint16_t      mCursorIndex;
    int16_t       mSelectBegin;
    int16_t       mSelectCount;

  private:
    void          (*mOnTouch)(EvTextBox *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvTextBox *Sender, const String &Str);
    void          (*mOnReturn)(EvTextBox *Sender, const String &Str);
    int           (*mOnFilter)(EvTextBox *Sender, const uint8_t C);
    void          (*mOnSetKbdFocus)(EvTextBox *Sender);
    void          (*mOnLostKbdFocus)(EvTextBox *Sender);

  public:
    EvTextCursor  *Cursor;
    const String  &Text = mLabel;
    bool          SelectAllOnSetKbdFocus;

    static EvTextBox  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TEXTBOX_H_ */
