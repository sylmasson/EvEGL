/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef     _EV_IMAGE_H_
#define     _EV_IMAGE_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     SCALING_NONE      0
#define     SCALE_TO_FIT      1
#define     RESIZE_ON_LOAD    (1 << 3)

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
    float         ScaleToFit(void);
    float         ScaleToFit(uint16_t Width, uint16_t Height);
    float         ScaleToFit(uint16_t Width, uint16_t Height, uint16_t BmpWidth, uint16_t BmpHeight);
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
