
#ifndef     _EV_SMETER_H_
#define     _EV_SMETER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvNeedle : public EvImage
{
  public:
    EvNeedle(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag);

    virtual void  drawEvent(void);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSmeter : public EvPanel
{
  protected:
    EvSmeter(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    bool          SetValue(int32_t Value);
    void          SetOnTouch(void (*OnTouch)(EvSmeter *Sender, const EvTouchEvent *Touch));

  protected:
    bool          mLock;
    int16_t       mValue;
    float         mAngle;
    float         mScale;

    EvImage       *mBG;
    EvNeedle      *mNeedle;

    virtual void  resizeEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSmeter *Sender, const EvTouchEvent *Touch);

  public:
    const int16_t &Value = mValue;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvSmeter *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const EvBmp   *Smeter;
extern const EvBmp   *SmeterNeedle;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SMETER_H_ */
