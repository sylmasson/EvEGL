
#ifndef     _EV_SPECTRUM_H_
#define     _EV_SPECTRUM_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSpectrum : public EvObj
{
  protected:
    EvSpectrum(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvSpectrum(void);

    void          RawData(uint16_t X, uint8_t Data);
    void          RawData(uint16_t X, uint8_t *Data, uint16_t Count);
    void          SetColor(uint16_t LineColor, uint16_t FillColor);
    void          SetOnTouch(void (*OnTouch)(EvSpectrum *Sender, EvTouchEvent *Touch));

  protected:
    uint8_t       *mData;
    uint16_t      mDataSize;
    uint16_t      mLineColor;
    uint16_t      mFillColor;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);
    virtual void  dataScaling(uint16_t Ind, int16_t &X, int16_t &Y);

  private:
    void          (*mOnTouch)(EvSpectrum *Sender, EvTouchEvent *Touch);

  public:
    const uint16_t  &DataSize = mDataSize;

    static EvSpectrum  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SPECTRUM_H_ */
