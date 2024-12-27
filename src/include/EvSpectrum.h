
#ifndef     _EV_SPECTRUM_H_
#define     _EV_SPECTRUM_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSpectrum : public EvObj
{
  protected:
    EvSpectrum(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvSpectrum(void);

    bool          SetData(uint16_t X, uint8_t Data);
    bool          SetData(uint16_t X, uint8_t *Data, uint16_t Count);
    bool          SetDataSize(uint16_t DataSize, uint8_t Value = 255);
    void          SetColor(uint16_t LineColor, uint16_t FillColor, uint8_t mFillColorA = 128);
    void          SetOnTouch(void (*OnTouch)(EvSpectrum *Sender, const EvTouchEvent *Touch));

  protected:
    uint8_t       *mData;
    uint16_t      mDataSize;
    color16       mLineColor;
    color16       mFillColor;
    uint8_t       mFillColorA;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);
    virtual void  dataScaling(uint16_t Ind, int16_t &X, int16_t &Y);

  private:
    void          (*mOnTouch)(EvSpectrum *Sender, const EvTouchEvent *Touch);

  public:
    const uint16_t  &DataSize = mDataSize;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvSpectrum  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SPECTRUM_H_ */
