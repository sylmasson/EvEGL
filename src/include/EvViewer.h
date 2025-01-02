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

#ifndef     _EV_VIEWER_H_
#define     _EV_VIEWER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvViewer : public EvScrollBox
{
  protected:
    EvViewer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    bool          Unload(void);
    const EvBmp   *Load(const EvBmp *Bmp, uint32_t Options = 0);
    const EvBmp   *Load(const char *Filename, SDClass &Dev = SD, uint32_t Options = 0);
    float         SetScale(float Scale);
    float         SetAngle(float Angle);
    void          SetOnScaling(void (*OnScaling)(EvViewer *Sender, float Scale));
    void          SetOnRotate(void (*OnRotate)(EvViewer *Sender, float Angle));

    virtual void  resizeEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

    EvImage       *Image;

  private:
    bool          mRotate;
    uint8_t       mMoveCnt;
    int16_t       mHypoZero;
    int16_t       mMoveX, mMoveY;
    int16_t       mDeltaX, mDeltaY;
    int16_t       mFocusX, mFocusY;
    int16_t       mPosX[2], mPosY[2];
    float         mScale, mScaleStart;
    float         mAngle, mAngleStart;
    float         mAnglePrev, mAngleZero, mAngleOffset;

    void          (*mOnScaling)(EvViewer *Sender, float Scale);
    void          (*mOnRotate)(EvViewer *Sender, float Angle);

    void          update(float Scale, float Angle);
    int32_t       getHypo(int32_t OppSide, int32_t AdjSide);
    float         getRotation(int32_t OppSide, int32_t AdjSide, bool Start = false);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvViewer   *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_VIEWER_H_ */
