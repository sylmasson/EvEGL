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

#ifndef     _EV_NUMINT_H_
#define     _EV_NUMINT_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvNumInt class is used to create an editable visual object of an integer.
 * 
 * You can touch the object to edit the value. The valid range is set by SetRange function.
 * 
 * By default the text is aligned to RIGHT_CENTER, the font number is 25 and the format is "%ld".
 * 
 * @par Example
 * ~~~
 * EvNumInt *myNum = EvNumInt::Create(50, 50, 100, 50, myPanel, "MyNumInt");
 * myNum->SetFormat("%ld%%");
 * myNum->SetRange(0, 100);
 * myNum->SetValue(50);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvNumInt : public EvObj
{
  protected:
    EvNumInt(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int32_t       Value(void);
    bool          IncValue(int32_t Inc);
    bool          SetValue(int32_t Value);
    void          SetTurnOver(bool TurnOver);
    void          SetFormat(const char *Format);
    void          SetRange(int32_t Min, int32_t Max);
    void          SetColor(uint16_t ColorHold, uint16_t ColorInc = CL_NUMINT_INC, uint16_t ColorDec = CL_NUMINT_DEC);
    void          SetOnTouch(void (*OnTouch)(EvNumInt *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvNumInt *Sender, int32_t Value));

  protected:
    int32_t       mValue;
    int32_t       mMin;
    int32_t       mMax;
    int8_t        mInc;
    bool          mBusy;
    bool          mSkipUp;
    bool          mTurnOver;
    uint16_t      mColorHold;
    uint16_t      mColorInc;
    uint16_t      mColorDec;
    const char    *mFormat;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvNumInt *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvNumInt *Sender, int32_t Value);

    void          printValue(int32_t Value);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvNumInt *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
    static EvNumInt *Create(int16_t Left, int16_t Top, int32_t Value, const EvNumInt *Src, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_NUMINT_H_ */
