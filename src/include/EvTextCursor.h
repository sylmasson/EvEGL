
#ifndef     _EV_CURSOR_H_
#define     _EV_CURSOR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTextCursor : public EvObj
{
  public:
    uint8_t       Style(void);
    uint8_t       Style(uint8_t CursorStyle);

  protected:
    EvTextCursor(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    uint8_t       mCursorState;
    uint16_t      mCursorTimer;

    virtual void  refreshEvent(void);

  public:
    static EvTextCursor *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_CURSOR_H_ */
