
#ifndef     _EV_PIXELBOX_H_
#define     _EV_PIXELBOX_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPixelBox : public EvObj
{
  protected:
    EvPixelBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvPixelBox(void);

    void          ScrollUp(void);
    void          ScrollDown(void);
    void          SetPixel(uint16_t X, uint16_t Y, uint8_t Color);
    void          SetPixel(uint16_t X, uint16_t Y, uint8_t *Color, uint16_t Count);
    void          SetOnTouch(void (*OnTouch)(EvPixelBox *Sender, const EvTouchEvent *Touch));

  protected:
    uint16_t      mBufInd;
    EvBmp         mBmp;
    const EvMem   *mDest;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvPixelBox *Sender, const EvTouchEvent *Touch);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvPixelBox  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PIXELBOX_H_ */
