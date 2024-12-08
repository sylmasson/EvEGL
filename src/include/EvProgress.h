
#ifndef     _EV_PROGRESS_H_
#define     _EV_PROGRESS_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvProgress class is used to create a standard Progress Bar.
 * 
 * The valid range for SetValue is fixed from 0 to 1000. However, you can scale the
 * output display value with M and B factors using the SetFormat function.
 * 
 * @par Example
 * ~~~
 * EvProgress *myProgress = EvProgress::Create(50, 50, 200, 30, myPanel, "MyProgress");
 * myProgress->SetFormat("%.1f%%", 0.1);
 * myProgress->SetValue(0);
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvProgress : public EvObj
{
  protected:
    EvProgress(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          SetColor(uint16_t ColorLower, uint16_t ColorUpper = CL_PROGRESS_UPPER);
    void          SetFormat(const char *Format, const float M = 0.1, const float B = 0.0);
    void          SetOnTouch(void (*OnTouch)(EvProgress *Sender, const EvTouchEvent *Touch));

  protected:
    int16_t       mValue;
    color16       mColorLower;
    color16       mColorUpper;
    const char    *mFormat;    // must be valid float printf format
    float         mM, mB;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvProgress *Sender, const EvTouchEvent *Touch);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvProgress *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PROGRESS_H_ */
