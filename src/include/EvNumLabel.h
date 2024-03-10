
#ifndef     _EV_NUMLABEL_H_
#define     _EV_NUMLABEL_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvNumLabel class is used to create a numeric Label.
 * 
 * A EvNumLabel is a child of EvLabel providing text formatting for numeric values.
 * 
 * By default the text is aligned to RIGHT_CENTER with a font number of 25.
 * 
 * @par Example
 * ~~~
 * EvNumLabel *myNum = EvNumLabel::Create(50, 50, 100, 50, myPanel, "MyNumLabel");
 * myNum->SetFormat("%.2f%%");
 * myNum->SetValueFloat(66.66);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvNumLabel : public EvLabel
{
  protected:
    EvNumLabel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int32_t       Value(void);
    float         ValueFloat(void);
    bool          SetValue(int32_t Value);
    bool          SetValueFloat(float Value);
    void          SetFormat(const char *Format);

  protected:
    union
    {
      int32_t     i;
      float       f;
    } mValue;
    const char    *mFormat;

  public:
    static EvNumLabel *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_NUMLABEL_H_ */
