
#ifndef     _EV_LABEL_H_
#define     _EV_LABEL_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvLabel class is used to create a standard Label.
 * 
 * By default the text is aligned to RIGHT_CENTER with a font number of 25.
 * 
 * @par Example
 * ~~~
 * EvLabel *myLabel = EvLabel::Create(50, 50, 150, 50, myPanel, "MyLabel");
 * myLabel->TextLabel("Hello World!");
 * myLabel->TextAlign(CENTER);
 * myLabel->TextFont(29);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvLabel : public EvObj
{
  protected:
    EvLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    void          SetOnTouch(void (*OnTouch)(EvLabel *Sender, EvTouchEvent *Touch));

  protected:
    bool          mTouchFlag;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvLabel *Sender, EvTouchEvent *Touch);

  public:
    static EvLabel  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_LABEL_H_ */
