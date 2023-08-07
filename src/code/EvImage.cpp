
#include    <EvGUI.h>

#define     SCALE_LIMIT(x)      ((x < 0.01) ? 0.01 : x)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard Image.
 * 
 * A new standard Image is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of Image.
 * @param[in]  Top     The top position of Image.
 * @param[in]  Width   The width of Image.
 * @param[in]  Height  The height of Image.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Image. If NULL, the default tag name is "EvImage".
 * @param[in]  State   The initial state of the Image. Default is set to VISIBLE_OBJ.
 *
 * @return     EvImage address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage     *EvImage::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvImage *)EvObj::TryCreate(new EvImage(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvImage" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage::EvImage(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mBmp = NULL;
  mLoad = NULL;
  mOffsetX = 0;
  mOffsetY = 0;
  mResizeLock = false;
  mStyle.align = LEFT_CENTER | ALIGNMENT_LOCK;
  SetMode(RESIZE_PROPORTIONAL, NEAREST);
  RotateAround(0, 0, 0.0, 1.0);
  SetOnTouch(NULL);
  ModifiedCoeff();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage::~EvImage(void)
{
  Unload();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::ModifiedCoeff(void)
{
  mRefreshCoeff = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvImage::Load(const EvBmp *Bmp)
{
  if (mBmp != NULL)
    Unload();

  if (Bmp != NULL && Bmp->Layout != PALETTED8 && (mLoad = Disp->LoadBmp(Bmp)) != NULL)
  {
    mBmp = Bmp;
    ReSize(mBmp->Width, mBmp->Height);
    RotateAround(mBmp->Width >> 1, mBmp->Height >> 1, 0.0, 1.0);
    SetMode(RESIZE_PROPORTIONAL, NEAREST);
    Modified();
    return true;
  }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvImage::Unload(void)
{
  bool      retValue;

  retValue = Disp->UnloadBmp(mLoad);
  mLoad = NULL;
  mBmp = NULL;
  Modified();

  return retValue;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::Scale(float ScaleXY)
{
  float     scale = SCALE_LIMIT(ScaleXY);

  if (mScaleX != scale || mScaleY != scale)
  {
    mScaleX = scale;
    mScaleY = scale;
    ModifiedCoeff();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::ScaleX(float Scale)
{
  float     scale = SCALE_LIMIT(Scale);

  if (mScaleX != scale)
  {
    mScaleX = scale;
    ModifiedCoeff();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::ScaleY(float Scale)
{
  float     scale = SCALE_LIMIT(Scale);

  if (mScaleY != scale)
  {
    mScaleY = scale;
    ModifiedCoeff();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::Rotate(float A)
{
  if (mAngle != A)
  {
    mAngle = A;
    A *= M_PI / 180.0;
    mSinA = sin(A);
    mCosA = cos(A);
    ModifiedCoeff();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::RotateAdd(float A)
{
  Rotate(mAngle + A);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::RotateAround(int16_t X, int16_t Y)
{
  if (mPivotX != X || mPivotY != Y)
  {
    mPivotX = X;
    mPivotY = Y;
    ModifiedCoeff();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::RotateAround(int16_t X, int16_t Y, float A, float ScaleXY)
{
  Rotate(A);
  Scale(ScaleXY);
  RotateAround(X, Y);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::SetMode(uint8_t ResizeMode, uint8_t FilterMode)
{
  mResizeMode = ResizeMode;
  mFilterMode = FilterMode;
  Modified();
}

/// @copydoc EvButton::SetOnTouch()

void        EvImage::SetOnTouch(void (*OnTouch)(EvImage *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::resize(void)
{
  if (mBmp != NULL && !mResizeLock)
  {
    int16_t left, top;
    int16_t right, bottom;
    int16_t x = mBmp->Width - 1;
    int16_t y = mBmp->Height - 1;
    float   coeffA = mCosA * mScaleX;
    float   coeffB = -mSinA * mScaleY;
    float   coeffD = mSinA * mScaleX;
    float   coeffE = mCosA * mScaleY;
    float   coeffC = (mPivotX * mScaleX) - ((mPivotX * coeffA) + (mPivotY * coeffB));
    float   coeffF = (mPivotY * mScaleY) - ((mPivotX * coeffD) + (mPivotY * coeffE));
    int16_t corner[4][2] = {{0, 0}, {x, 0}, {x, y}, {0, y}};

    mResizeLock = true;

    left = right = coeffC * 16;
    top = bottom = coeffF * 16;

    for (int i = 1; i < 4; i++)
    {
      x = ((corner[i][0] * coeffA) + (corner[i][1] * coeffB) + coeffC) * 16;
      y = ((corner[i][0] * coeffD) + (corner[i][1] * coeffE) + coeffF) * 16;

      if (x < left) left = x;
      if (x > right) right = x;
      if (y < top) top = y;
      if (y > bottom) bottom = y;
    }

    left = left >> 4;
    top = top >> 4;
    right = (right + 15) >> 4;
    bottom = (bottom + 15) >> 4;

    x = mOffsetX;
    y = mOffsetY;
    mOffsetX = left;
    mOffsetY = top;

    ReSize(right - left + 1, bottom - top + 1);
    MoveRel(mOffsetX - x, mOffsetY - y);
    mResizeLock = false;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::drawSetup(void)
{
  if (mBdRadius > 16)
  {
    Disp->ColorMask(0, 0, 0, 1);
    Disp->Clear();
    FillRectangle2f(0, 0, mWidth << 4, mHeight << 4, RGB555(255, 255, 255), mBdRadius);
    Disp->ColorMask(1, 1, 1, 0);
    Disp->BlendFunc(DST_ALPHA, ONE_MINUS_DST_ALPHA);
  }

  if (mCoeff[0] != 65536) Disp->BitmapTransformA(mCoeff[0]);
  if (mCoeff[1] != 0)     Disp->BitmapTransformB(mCoeff[1]);
  if (mCoeff[2] != 0)     Disp->BitmapTransformC(mCoeff[2]);
  if (mCoeff[3] != 0)     Disp->BitmapTransformD(mCoeff[3]);
  if (mCoeff[4] != 65536) Disp->BitmapTransformE(mCoeff[4]);
  if (mCoeff[5] != 0)     Disp->BitmapTransformF(mCoeff[5]);

  Disp->Begin(BITMAPS);
  Disp->BitmapHandle(0);
  Disp->BitmapSource(mLoad->addr + mBmp->PalSize);
  Disp->BitmapLayout(mBmp->Layout, mBmp->Width, mBmp->Height);
  Disp->BitmapSize(mFilterMode, BORDER, BORDER, mWidth, mHeight);

  if (mBmp->PalSize > 0)
    Disp->PaletteSource(mLoad->addr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::drawEvent(void)
{
  if (mBmp != NULL)
  {
    drawSetup();
    Disp->ColorRGB(RGB555(255, 255, 255));
    Disp->Vertex2f(0, 0);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::resizeEvent(void)
{
  if (!mResizeLock && mWidth > 0 && mHeight > 0 && mBmp != NULL && mBmp->Width > 0 && mBmp->Height > 0)
  {
    switch (mResizeMode)
    {
      case RESIZE_NONE:
        mResizeLock = true;
        ReSize(mBmp->Width, mBmp->Height);
        mResizeLock = false;
        break;

      case RESIZE_STRETCH:
        ScaleX((float)mWidth / mBmp->Width);
        ScaleY((float)mHeight / mBmp->Height);
        break;

      case RESIZE_PROPORTIONAL:
        float   widthRatio = (float)mWidth / mBmp->Width;
        float   heightRatio = (float)mHeight / mBmp->Height;

        if (widthRatio > heightRatio)
        {
          mWidth = mBmp->Width * heightRatio;
          Scale(heightRatio);
        }
        else
        {
          mHeight = mBmp->Height * widthRatio;
          Scale(widthRatio);
        }
        break;
    }

    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::refreshEvent(void)
{
  if (mRefreshCoeff)
  {
    float   pivotX, pivotY;

    resize();
    pivotX = mPivotX - (mOffsetX / mScaleX);
    pivotY = mPivotY - (mOffsetY / mScaleY);
    mCoeff[0] = s15f16(mCosA / mScaleX);
    mCoeff[1] = s15f16(mSinA / mScaleX);
    mCoeff[2] = s15f16(mPivotX - ((pivotX * mCosA) + (pivotY * mSinA)));
    mCoeff[3] = s15f16(-mSinA / mScaleY);
    mCoeff[4] = s15f16(mCosA  / mScaleY);
    mCoeff[5] = s15f16(mPivotY - ((pivotX * -mSinA) + (pivotY * mCosA)));

//  --> mCoeff[2] and mCoeff[5] valid only when mScaleX equal mScaleY

//  Serial.printf("coeffC = %d - ((%d + %.3f) * %.3f) - ((%d + %.3f) * %.3f) = %.3f\n", mPivotX, mPivotX, offsetX, mCosA, mPivotY, offsetY, mSinA, (float)(mCoeff[2]) / 65536.0);
//  Serial.printf("coeffF = %d - ((%d + %.3f) * %.3f) - ((%d + %.3f) * %.3f) = %.3f\n", mPivotY, mPivotX, offsetX, -mSinA, mPivotY, offsetY, mCosA, (float)(mCoeff[5]) / 65536.0);

    mRefreshCoeff = false;
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != NULL)
    (*mOnTouch)(this, Touch);
}

