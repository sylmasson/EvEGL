
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
  public:
    int32_t       Value(void);
    bool          SetValue(int32_t Value);
    void          SetFormat(const char *Format);
    void          SetRange(int32_t Min, int32_t Max);
    void          SetOnTouch(void (*OnTouch)(EvNumInt *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvNumInt *Sender, int32_t Value));

  protected:
    EvNumInt(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    int32_t       mValue;
    int32_t       mMin;
    int32_t       mMax;
    int8_t        mInc;
    bool          mSkipUp;
    const char    *mFormat;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvNumInt *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvNumInt *Sender, int32_t Value);

  public:
    static EvNumInt *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_NUMINT_H_ */
