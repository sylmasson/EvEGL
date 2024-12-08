
#ifndef     _EV_IMAGE_H_
#define     _EV_IMAGE_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     RESIZE_NONE             0
#define     RESIZE_PROPORTIONAL     1
#define     RESIZE_ON_LOAD          2

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
  friend class EvViewer;

  protected:
    EvImage(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_DIS_OBJ);

  public:
    virtual       ~EvImage(void);

    bool          Unload(void);
    const EvBmp   *Load(const EvBmp *Bmp, uint32_t Options = 0);
    const EvBmp   *Load(const char *Filename, SDClass &Dev = SD, uint32_t Options = 0);
    void          ModifiedCoeff(void);
    float         Scale(float Scale);
    float         ScaleToFit(uint16_t Width, uint16_t Height);
    void          Rotate(float A);
    void          RotateAdd(float A);
    void          RotateAround(int16_t X, int16_t Y);
    void          RotateAround(int16_t X, int16_t Y, float A, float Scale);
    void          SetMode(uint8_t ResizeMode, uint8_t FilterMode = NEAREST);
    void          SetColor(uint8_t Red, uint8_t Green, uint8_t Blue);
    void          SetOnTouch(void (*OnTouch)(EvImage *Sender, const EvTouchEvent *Touch));

  protected:
    void          resize(void);
    void          drawSetup(void);
    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

    bool          mRefreshCoeff;
    bool          mResizeLock;
    uint8_t       mResizeMode;
    uint8_t       mFilterMode;
    uint8_t       mRed;
    uint8_t       mGreen;
    uint8_t       mBlue;
    int16_t       mShiftX;
    int16_t       mShiftY;
    int16_t       mPivotX;
    int16_t       mPivotY;
    float         mSinA;
    float         mCosA;
    float         mAngle;
    float         mScale;
    float         mScaleMax;
    int32_t       mCoeff[6];    // 0-5 -> A-F

    EvDMA         *mLoadDMA;
    const EvMem   *mLoad;
    const EvBmp   *mBmp;

  private:
    void          (*mOnTouch)(EvImage *Sender, const EvTouchEvent *Touch);

    static void   sOnLoading(EvDMA *Data);

    const EvBmp   *load(const EvBmp *Bmp, uint32_t Options);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvImage  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        IsValidJPEG(uint8_t *Data, uint32_t DataSize, EvBmp *Bmp = nullptr, const char *Tag = nullptr);
bool        IsValidPNG(uint8_t *Data, uint32_t DataSize, EvBmp *Bmp = nullptr, const char *Tag = nullptr);
bool        IsValidASTC(uint8_t *Data, uint32_t DataSize, EvBmp *Bmp = nullptr, const char *Tag = nullptr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_IMAGE_H_ */
