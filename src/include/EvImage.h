
#ifndef     _EV_IMAGE_H_
#define     _EV_IMAGE_H_

#define     RESIZE_NONE             0
#define     RESIZE_PROPORTIONAL     1
#define     RESIZE_STRETCH          2

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvImage class is used to create a standard Image.
 * 
 * @par Example
 * ~~~
 * EvImage *myImage = EvImage::Create(50, 50, 150, 50, myPanel, "MyImage");
 * myImage->Load(...)
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvImage : public EvObj
{
  public:
    ~EvImage(void);

    bool          Unload(void);
    const EvBmp   *Load(const EvBmp *Bmp);
    const EvBmp   *Load(const char *Filename, SDClass &Dev = SD);
    void          ModifiedCoeff(void);
    void          Scale(float ScaleXY);
    void          ScaleX(float Scale);
    void          ScaleY(float Scale);
    void          Rotate(float A);
    void          RotateAdd(float A);
    void          RotateAround(int16_t X, int16_t Y);
    void          RotateAround(int16_t X, int16_t Y, float A, float ScaleXY);
    void          SetMode(uint8_t ResizeMode, uint8_t FilterMode = NEAREST);
    void          SetOnTouch(void (*OnTouch)(EvImage *Sender, EvTouchEvent *Touch));

  protected:
    EvImage(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_DIS_OBJ);

    void          resize(void);
    void          drawSetup(void);
    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

    bool          mRefreshCoeff;
    bool          mResizeLock;
    uint8_t       mResizeMode;
    uint8_t       mFilterMode;
    int16_t       mOffsetX;
    int16_t       mOffsetY;
    int16_t       mPivotX;
    int16_t       mPivotY;
    float         mSinA;
    float         mCosA;
    float         mAngle;
    float         mScaleX;
    float         mScaleY;
    int32_t       mCoeff[6];    // 0-5 -> A-F

    const EvMem   *mLoad;
    const EvBmp   *mBmp;

  private:
    void          (*mOnTouch)(EvImage *Sender, EvTouchEvent *Touch);

  public:
    static EvImage  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        IsValidJPEG(const uint8_t *Data, uint32_t DataSize, EvBmp *Bmp = NULL, const char *Tag = NULL);
bool        IsValidPNG(const uint8_t *Data, uint32_t DataSize, EvBmp *Bmp = NULL, const char *Tag = NULL);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_IMAGE_H_ */
