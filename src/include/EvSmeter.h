
#ifndef     _EV_SMETER_H_
#define     _EV_SMETER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSmeterNeedle : public EvObj
{
  public:
    EvSmeterNeedle(int16_t Left, int16_t Top, float Scale, EvDisplay *Disp, const char *Tag);

    bool          SetDegree(int32_t Degree);

  protected:
    int32_t       mDegree;
    int32_t       mScale;

    virtual void  drawEvent(void);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSmeter : public EvPanel
{
  public:
    ~EvSmeter(void);

    void          Show(void);
    void          Hide(void);
    bool          SetValue(int16_t Value);
    void          SetFilterCoeff(uint16_t Coeff);
    void          SetOnTouch(void (*OnTouch)(EvSmeter *Sender, EvTouchEvent *Touch));

  protected:
    EvSmeter(int16_t Left, int16_t Top, float Scale, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    int32_t       mDegree;
    int32_t       mScale;
    uint16_t      mCoeff;

    EvSmeterNeedle  *mNeedle;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSmeter *Sender, EvTouchEvent *Touch);

  public:
    const int16_t &Value = mValue;

    static EvSmeter *Create(int16_t Left, int16_t Top, float Scale, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const EvBmp   sSmeter;
extern const EvBmp   sSmeterNeedle;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SMETER_H_ */
