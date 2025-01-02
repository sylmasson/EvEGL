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
