
#include    <EvGUI.h>

// #define     VERBOSE

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
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Image. If nullptr, the default tag name is "EvImage".
 * @param[in]  State   The initial state of the Image. Default is set to VISIBLE_DIS_OBJ.
 *
 * @return     EvImage address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage     *EvImage::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvImage *)EvObj::TryCreate(new EvImage(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvImage" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage::EvImage(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  mBmp = nullptr;
  mLoad = nullptr;
  mLoadDMA = nullptr;
  mOffsetX = 0;
  mOffsetY = 0;
  mResizeLock = false;
  mStyle.align = LEFT_CENTER | ALIGNMENT_LOCK;
  mPivotX = mPivotY = 0;
  mScaleX = mScaleY = 1.0;
  mAngle = -1;
  Rotate(0);
  SetMode(RESIZE_PROPORTIONAL | RESIZE_ON_LOAD, NEAREST);
  SetOnTouch(nullptr);
  ModifiedCoeff();
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvImage::~EvImage(void)
{
  Unload();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvImage::Unload(void)
{
  bool      result = false;

  if (mBmp != nullptr && mLoad != nullptr)
  {
    if (mLoad->count == 1 && (mBmp->Format & BMP_MALLOC) != 0)
      free((void *)mBmp);

    result = Disp->UnloadBmp(mLoad);
    mLoad = nullptr;
    mBmp = nullptr;
    Modified();
  }

  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvBmp *EvImage::Load(const EvBmp *Bmp, uint32_t Options)
{
  if (Bmp != nullptr && Bmp->Layout != PALETTED8)
  {
    if (Options == 0)
      return (load(Bmp, Options));

    if (Options == OPT_MEDIAFIFO && mLoadDMA == nullptr)
      if ((mLoadDMA = TaskDMA.Add(Disp, (uint8_t *)Bmp->Data, Bmp->DataSize, (void *)Bmp, (void *)this, onLoading)) != nullptr)
        return Bmp;
  }

  return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvBmp *EvImage::Load(const char *Filename, SDClass &Dev, uint32_t Options)
{
  File      file;
  EvBmp     *bmp;
  uint8_t   *data;
  uint32_t  fileSize;

  #ifdef  VERBOSE
  uint32_t  msec = millis();
  #endif

  if (Options != OPT_MEDIAFIFO)
    Options = 0;

  if (!Dev.mediaPresent() || !(file = Dev.open(Filename)))
  {
    #ifdef  VERBOSE
      Serial.printf("\nDevice or file %s not found\n", Filename);
    #endif
    return nullptr;
  }

  if ((fileSize = file.size()) > 0 && (bmp = (EvBmp *)malloc(sizeof(EvBmp) + fileSize)) != nullptr)
  {
    data = (uint8_t *)bmp + sizeof(EvBmp);

    if (file.read(data, fileSize) == fileSize)
    {
      file.close();

      #ifdef  VERBOSE
        Serial.printf("\nReading file %s in %lu msec\n", Filename, millis() - msec);
      #endif

      if (IsValidJPEG(data, fileSize, bmp, Filename) || IsValidPNG(data, fileSize, bmp, Filename))
      {
        #ifdef  VERBOSE
          Serial.printf("Width = %u Height = %u\n", bmp->Width, bmp->Height);
          Serial.printf("Layout = %u PalSize = %u\n", bmp->Layout, bmp->PalSize);
          Serial.printf("DataSize = %u BmpSize = %u\n", bmp->DataSize, bmp->BmpSize);
          Serial.printf("%s is valid image format\n", bmp->Tag);
        #endif

        bmp->Format |= BMP_MALLOC;

        if (Load(bmp, Options))
          return bmp;
      }
      else
      {
        #ifdef  VERBOSE
          Serial.printf("%s is not valid image format\n", Filename);
        #endif
      }

      free(bmp);
      return nullptr;
    }
  }

  file.close();
  return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::ModifiedCoeff(void)
{
  mRefreshCoeff = true;
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
  if (mBmp != nullptr && !mResizeLock)
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
  if (mBmp != nullptr)
  {
    drawSetup();
    Disp->ColorRGB(RGB555(255, 255, 255));
    Disp->Vertex2f(0, 0);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::resizeEvent(void)
{
  if (!mResizeLock && mWidth > 0 && mHeight > 0 && mBmp != nullptr && mBmp->Width > 0 && mBmp->Height > 0)
  {
    switch (mResizeMode & ~RESIZE_ON_LOAD)
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
  if (mOnTouch != nullptr)
    (*mOnTouch)(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const EvBmp *EvImage::load(const EvBmp *Bmp, uint32_t Options)
{
  Unload();

  if ((mLoad = Disp->LoadBmp(Bmp, Options)) != nullptr)
  {
    if (mResizeMode & RESIZE_ON_LOAD)
    {
      ReSize(Bmp->Width, Bmp->Height);
      RotateAround(Bmp->Width >> 1, Bmp->Height >> 1, 0.0, 1.0);
      SetMode(RESIZE_PROPORTIONAL | RESIZE_ON_LOAD, NEAREST);
    }

    Modified();
    mBmp = Bmp;
    return Bmp;
  }

  return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::onLoading(EvDMA *Data)
{
  EvImage   *Sender = (EvImage *)Data->Sender;

  if (Data->Status == EvDMA::COMPLETED)
    Sender->mLoadDMA = nullptr;

  else if (Data->Status == EvDMA::BEGIN)
  {
    if (Sender->load((const EvBmp *)(Data->Tag), OPT_MEDIAFIFO) == nullptr)
    {
      Sender->Disp->ClearCache(); // Clear cache and try again before aborting

      if (Sender->load((const EvBmp *)(Data->Tag), OPT_MEDIAFIFO) == nullptr)
      {
        Data->Status = EvDMA::ABORT;
        Sender->mLoadDMA = nullptr;
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        IsValidJPEG(const uint8_t *Data, uint32_t DataSize, EvBmp *Bmp, const char *Tag)
{
  uint32_t  width = 0;
  uint32_t  height = 0;
  uint8_t   layout = 0;
  uint32_t  i, marker, length, total = 0;

  if (Data[0] == 0xFF && Data[1] == 0xD8) // Check SOI
  {
    #ifdef  VERBOSE
      Serial.print("JPEG file signature found");
    #endif

    for (i = total = 2; Data[i++] == 0xFF && i < DataSize; i += length)
    {
      total += 2;
      marker = Data[i++];

      #ifdef  VERBOSE
        Serial.printf("\nMarker FF %02X", marker);
      #endif

      while (marker == 0xDA && i < DataSize)  // can have several SOS
      {
        for (length = i; i < DataSize; )  // skip byte stuffing and restart marker
          if (Data[i++] == 0xFF && (marker = Data[i++]) != 0x00 && (marker & ~7) != 0xD0)
            break;

        length = i - length;
        total += length;

        #ifdef  VERBOSE
          Serial.printf(" %u bytes\nMarker FF %02X", length, marker);
        #endif
      }

      if (marker == 0xD9)  // EOI
      {
        if (i != DataSize || layout == 0)
          break;

        if (Bmp != nullptr)
        {
          Bmp->Format = JPEG_DATA;
          Bmp->Layout = layout;
          Bmp->Width = width;
          Bmp->Height = height;
          Bmp->PalSize = 0;
          Bmp->BmpSize = width * height * sizeof(uint16_t);
          Bmp->DataSize = DataSize;
          Bmp->Data = Data;
          Bmp->Tag = Tag;
        }

        #ifdef  VERBOSE
          Serial.printf("\nTotal length = %u bytes\n", total);
        #endif

        return true;
      }
      
      if (marker == 0xC0)   // SOF for baseline image
      {
        height = (Data[i+3] << 8) + Data[i+4];
        width = (Data[i+5] << 8) + Data[i+6];
        layout = RGB565;
      }

      length = (Data[i] << 8) + Data[i+1];  // add length of marker
      total += length;

      #ifdef  VERBOSE
        Serial.printf(" %u bytes", length + 2);
      #endif
    }
  }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        IsValidPNG(const uint8_t *Data, uint32_t DataSize, EvBmp *Bmp, const char *Tag)
{
  uint32_t  width = 0;
  uint32_t  height = 0;
  uint8_t   layout = 0;
  uint16_t  palSize = 0;
  char      chunk[] = "????";
  uint32_t  i, length, total = 0;
  static const char  signature[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};

  if (DataSize >= sizeof(signature) && memcmp(Data, signature, sizeof(signature)) == 0) // Check signature
  {
    #ifdef  VERBOSE
      Serial.print("PNG file signature found\n");
    #endif

    for (i = total = sizeof(signature); i + 12 <= DataSize; i += length + 4)
    {
      length = (((((Data[i+0] << 8) + Data[i+1]) << 8) + Data[i+2]) << 8) + Data[i+3];
      memcpy(chunk, &Data[i+4], 4);
      total += length + 12;
      i += 8;

      #ifdef  VERBOSE
        Serial.printf("Chunk %s %u bytes\n", chunk, length + 12);
      #endif

      if (strcmp(chunk, "IHDR") == 0)
      {
        if (i != 16 || length != 13 || Data[i+8] != 8) // IHDR must be after signature with 8 BitDepth
          break;

        if (Data[i+10] != 0 || Data[i+11] != 0 || Data[i+12] != 0) // Compression, Filter and Interlace should be zero
          break;

        switch (Data[i+9])  // ColorType
        {
          case 0:
          case 2: layout = RGB565; break;
          case 3: layout = PALETTED565; break;
          case 4: layout = ARGB4; break;
          default: return false;
        }

        width  = (((((Data[i+0] << 8) + Data[i+1]) << 8) + Data[i+2]) << 8) + Data[i+3];
        height = (((((Data[i+4] << 8) + Data[i+5]) << 8) + Data[i+6]) << 8) + Data[i+7];
      }
      else if (strcmp(chunk, "PLTE") == 0)
      {
        if (layout != PALETTED565 && layout != PALETTED4444)
          break;

        palSize = (length * 2) / 3;
      }
      else if (strcmp(chunk, "tRNS") == 0)
      {
        if (layout != PALETTED565)
          break;

        layout = PALETTED4444;
      }
      else if (strcmp(chunk, "IEND") == 0)
      {
        if (i + 4 != DataSize || layout == 0)
          break;

        if (palSize == 0 && (layout == PALETTED565 || layout == PALETTED4444))
          break;

        if (Bmp != nullptr)
        {
          Bmp->Format = PNG_DATA;
          Bmp->Layout = layout;
          Bmp->Width = width;
          Bmp->Height = height;
          Bmp->PalSize = palSize;
          Bmp->BmpSize = width * height * sizeof(uint16_t);
          Bmp->DataSize = DataSize;
          Bmp->Data = Data;
          Bmp->Tag = Tag;
        }

        #ifdef  VERBOSE
          Serial.printf("Total length = %u bytes\n", total);
        #endif

        return true;
      }
    }
  }

  return false;
}

