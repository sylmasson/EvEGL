
#ifndef     _TEXTCURSOR_H_
#define     _TEXTCURSOR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     CURSOR_NONE     0
#define     CURSOR_SOLID    1
#define     CURSOR_BLINK    2
#define     CURSOR_SMOOTH   3

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTextCursor : public EvObj
{
  protected:
    EvTextCursor(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    uint8_t       Style(void);
    uint8_t       SetStyle(uint8_t CursorStyle);

  protected:
    uint8_t       mCursorState;
    uint16_t      mCursorTimer;

    virtual void  refreshEvent(void);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvTextCursor *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _TEXTCURSOR_H_ */
