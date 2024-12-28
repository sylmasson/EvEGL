
#include    <EvEGL.h>

const char * const EvViewer::TypeName = "EvViewer";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the standard **EvViewer**.
 * 
 * A new standard **EvViewer** is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the **EvViewer**.
 * @param[in]  Top     The top position of the **EvViewer**.
 * @param[in]  Width   The width of the **EvViewer**.
 * @param[in]  Height  The height of the **EvViewer**.
 * @param[out] *Dest   The address pointer of the **EvPanel** destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the **EvViewer**. If nullptr, the default tag name is **"EvViewer"**.
 * @param[in]  State   The initial state of the **EvViewer**. Default is set to VISIBLE_OBJ.
 *
 * @return     **EvViewer** address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvViewer  *EvViewer::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvViewer *)EvObj::TryCreate(new EvViewer(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvViewer::EvViewer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvScrollBox(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mScale(1.0),
  mAngle(0.0),
  mOnScaling(nullptr),
  mOnRotate(nullptr)
{
  if (!(Image = EvImage::Create(0, 0, Width, Height, this, nullptr, VISIBLE_DIS_OBJ)))
  {
    abortCreate();
    return;
  }

  TouchMax(2);
  BgColor(CL_DEFAULT_BG);
  BdShape(FIXED_CORNERS);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvViewer::Unload(void)
{
  return Image->Unload();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvBmp *EvViewer::Load(const EvBmp *Bmp, uint32_t Options)
{
  const EvBmp *bmp;

  if ((bmp = Image->Load(Bmp, Options)) != nullptr)
    update(Image->ScaleToFit(mWidth - 1, mHeight - 1), 0);

  return bmp;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvBmp *EvViewer::Load(const char *Filename, SDClass &Dev, uint32_t Options)
{
  const EvBmp *bmp;

  if ((bmp = Image->Load(Filename, Dev, Options)) != nullptr)
    update(Image->ScaleToFit(mWidth - 1, mHeight - 1), 0);

  return bmp;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvViewer::SetOnScaling(void (*OnScaling)(EvViewer *Sender, float Scale))
{
  mOnScaling = OnScaling;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float       EvViewer::SetAngle(float Angle)
{
  update(mScale, Angle);
  return mAngle;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvViewer::SetOnRotate(void (*OnRotate)(EvViewer *Sender, float Angle))
{
  mOnRotate = OnRotate;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvViewer::resizeEvent(void)
{
  update(mScale, mAngle);
  EvScrollBox::resizeEvent();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvViewer::touchEvent(const EvTouchEvent *Touch)
{
  if (Image == nullptr || Image->mBmp == nullptr)
    return;

  mPosX[Touch->id] = Touch->x;
  mPosY[Touch->id] = Touch->y;

  switch (Touch->event)
  {
    case TOUCH_START:
      if (TouchCnt() == 2)
      {
        mScaleStart = mScale;
        mAngleStart = mAngle;
        mDeltaX = mPosX[1] - mPosX[0];
        mDeltaY = mPosY[1] - mPosY[0];
        mFocusX = mPosX[0] + (mDeltaX / 2);
        mFocusY = mPosY[0] + (mDeltaY / 2);
        getRotation(mDeltaX, mDeltaY, true);
        mHypoZero = getHypo(mDeltaX, mDeltaY);
        mMoveX = mMoveY = 0;
        mRotate = false;
      }
      break;

    case TOUCH_MOVE:
      if (TouchCnt() >= 2 && mHypoZero > 5)
      {
        int32_t   deltaX, deltaY;
        float     rotation, scale, angle = mAngle;

        mMoveX = ((Touch->move.x * 8) + mMoveX) / 2;
        mMoveY = ((Touch->move.y * 8) + mMoveY) / 2;
        Touch->move.x = mMoveX / 16;
        Touch->move.y = mMoveY / 16;

        deltaX = mPosX[1] - mPosX[0];
        deltaY = mPosY[1] - mPosY[0];
        mDeltaX = (deltaX + (mDeltaX * 5)) / 6;
        mDeltaY = (deltaY + (mDeltaY * 5)) / 6;
        scale = (mScaleStart * getHypo(mDeltaX, mDeltaY)) / mHypoZero;
        rotation = getRotation(mDeltaX, mDeltaY);

        if (mRotate || fabs(rotation) > 15.0f)
        {
          if (!mRotate)
          {
            mRotate = true;
            mAngleZero += rotation;
            rotation = 0;
          }

          angle = floor(((rotation + mAngleStart) * 1.0f) + 0.5f) / 1.0f;

          while (angle > 180.0f)
            angle -= 360.0f;

          while (angle <= -180.0f)
            angle += 360.0f;
        }

        update(scale, angle);
      }
      break;

    case TOUCH_DOUBLE:
      if (TouchCnt() == 2)
      {
        mPanelOffsetX = 0;
        mPanelOffsetY = 0;
        Image->Rotate(0);
        update(Image->ScaleToFit(mWidth - 1, mHeight - 1), 0);
      }

      Touch->event = 0;
      break;

    case TOUCH_END:
      break;
  }

  if (Touch->event != 0)
    EvScrollBox::touchEvent(Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvViewer::update(float Scale, float Angle)
{
  if (Image != nullptr && Image->mBmp != nullptr)
  {
    float     focusX, focusY;

    if (mAngle != Angle)
    {
      Image->Rotate(mAngle = Angle);

      if (mOnRotate != nullptr)
        mOnRotate(this, mAngle);
    }

    focusX = (mFocusX + mScrollBarX->Value()) / mScale;
    focusY = (mFocusY + mScrollBarY->Value()) / mScale;

    Scale = Image->Scale(Scale);

    if (mOnScaling != nullptr && mScale != Scale)
      mOnScaling(this, mScale = Scale);

    SetPageSize(Image->Width(), Image->Height());
    SetPageOffset(Image->mShiftX, Image->mShiftY);

    focusX = (focusX * mScale) - mFocusX;
    focusY = (focusY * mScale) - mFocusY;
    mScrollBarX->SetValue(focusX, true, true);
    mScrollBarY->SetValue(focusY, true, true);

    if (Image->Width() < mWidth)
      mPanelOffsetX = (mWidth - (int16_t)(Image->mBmp->Width * mScale)) / 2;

    if (Image->Height() < mHeight)
      mPanelOffsetY = (mHeight - (int16_t)(Image->mBmp->Height * mScale)) / 2;

    SetView();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t     EvViewer::getHypo(int32_t AdjSide, int32_t OppSide)
{
  return sqrt((AdjSide * AdjSide) + (OppSide * OppSide));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float       EvViewer::getRotation(int32_t AdjSide, int32_t OppSide, bool Start)
{
  float     angle, rotation;

  if (AdjSide == 0)
    angle = 90.0f;
  else
    angle = (((AdjSide * OppSide) < 0) ? 0 : 180.0f) - atan((float)OppSide / (float)AdjSide) * (180.0f / PI);

  if (Start)
  {
    mAngleZero = angle;
    mAngleOffset = 0;
  }
  else
  {
    if (mAnglePrev > 135.0f && angle < 45.0f)
      mAngleOffset += 180.0f;
    else if (mAnglePrev < 45.0f && angle > 135.0f)
      mAngleOffset -= 180.0f;
  }

  rotation = angle + mAngleOffset - mAngleZero;
  mAnglePrev = angle;
  return rotation;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

