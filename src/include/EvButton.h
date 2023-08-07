
#ifndef     _EV_BUTTON_H_
#define     _EV_BUTTON_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvButton class is used to create a standard Button.
 * 
 * @par Example
 * ~~~
 * EvButton *myButton = EvButton::Create(50, 50, 100, 35, myPanel, "MyButton");
 * myButton->SetColor(RGB555(200, 200, 0), RGB555(150, 150, 0));
 * myButton->SetOnChange(MyButtonChangeEvent);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvButton : public EvObj
{
  public:
    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          SetColor(uint16_t ColorUp, uint16_t ColorDown);
    void          SetOnTouch(void (*OnTouch)(EvButton *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvButton *Sender, int32_t Value));

  protected:
    EvButton(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    uint16_t      mColorUp;
    uint16_t      mColorDown;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvButton *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvButton *Sender, int32_t Value);

  public:
    static EvButton *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_BUTTON_H_ */
