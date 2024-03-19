
#ifndef     _EV_CHECKBOX_H_
#define     _EV_CHECKBOX_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvCheckBox class is used to create a standard Button.
 * 
 * By default the CheckBox is checked.
 * 
 * @par Example
 * ~~~
 * EvCheckBox *myCheckBox = EvCheckBox::Create(50, 50, 100, 35, myPanel, "MyCheckBox");
 * myCheckBox->BdShape(ROUND_CORNERS);
 * myCheckBox->SetValue(false);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvCheckBox : public EvObj
{
  protected:
    EvCheckBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          SetColor(uint16_t ColorCheck, uint16_t ColorUncheck, uint16_t ColorBorder);
    void          SetOnTouch(void (*OnTouch)(EvCheckBox *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvCheckBox *Sender, int32_t Value));

  protected:
    int16_t       mValue;
    uint16_t      mColorCheck;
    uint16_t      mColorUncheck;
    uint16_t      mColorBorder;
    bool          mTouchFlag;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvCheckBox *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvCheckBox *Sender, int32_t Value);

  public:
    static EvCheckBox *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_CHECKBOX_H_ */
