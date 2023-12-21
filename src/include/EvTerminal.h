
#ifndef     _EV_TERMINAL_H_
#define     _EV_TERMINAL_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTerminal : public EvTextBlock
{
  protected:
    EvTerminal(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    uint16_t      mScrolling;

    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  public:
    EvTextCursor  *Cursor;
    uint16_t      MaxBufferSize;

    static EvTerminal *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TERMINAL_H_ */
