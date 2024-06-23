
#include    <EvGUI.h>

// #define     VERBOSE

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

EvImage::EvImage(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mRefreshCoeff(true),
  mResizeLock(false),
  mResizeMode(RESIZE_PROPORTIONAL | RESIZE_ON_LOAD),
  mFilterMode(NEAREST),
  mRed(255),
  mGreen(255),
  mBlue(255),
  mShiftX(0),
  mShiftY(0),
  mPivotX(0),
  mPivotY(0),
  mAngle(-1),
  mScale(1.0),
  mLoadDMA(nullptr),
  mLoad(nullptr),
  mBmp(nullptr),
  mOnTouch(nullptr)
{
  TextAlign(LEFT_CENTER | ALIGNMENT_LOCK);
  Rotate(0);
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
    mScaleMax = 2045.0 / sqrt((Bmp->Width * Bmp->Width) + (Bmp->Height * Bmp->Height));

    if (Bmp->Layout >= 31)
      Options = 0;

    if (Options == 0)
      return (load(Bmp, Options));

    if (Options == OPT_MEDIAFIFO && mLoadDMA == nullptr)
      if ((mLoadDMA = TaskDMA.Add(Disp, (uint8_t *)Bmp->Data, Bmp->DataSize, (void *)Bmp, (void *)this, sOnLoading)) != nullptr)
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
      EvOut->printf("\nDevice or file %s not found\n", Filename);
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
        EvOut->printf("\nReading file %s in %lu msec\n", Filename, millis() - msec);
      #endif

      if (IsValidJPEG(data, fileSize, bmp, Filename) || IsValidPNG(data, fileSize, bmp, Filename) || IsValidASTC(data, fileSize, bmp, Filename))
      {
        #ifdef  VERBOSE
          EvOut->printf("Width = %u Height = %u\n", bmp->Width, bmp->Height);
          EvOut->printf("Layout = %u PalSize = %u\n", bmp->Layout, bmp->PalSize);
          EvOut->printf("DataSize = %u BmpSize = %u\n", bmp->DataSize, bmp->BmpSize);
          EvOut->printf("%s is valid image format\n", bmp->Tag);
        #endif

        bmp->Format |= BMP_MALLOC;

        if (Load(bmp, Options))
          return bmp;
      }
      else
      {
        #ifdef  VERBOSE
          EvOut->printf("%s is not valid image format\n", Filename);
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

float       EvImage::Scale(float Scale)
{
  if (Scale < 0.1)
    Scale = 0.1;
  else if (Scale > mScaleMax)
    Scale = mScaleMax;

  if (mScale != Scale)
  {
    mScale = Scale;
    ModifiedCoeff();
    refreshEvent();
  }

  return mScale;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float       EvImage::ScaleToFit(uint16_t Width, uint16_t Height)
{
  if (mBmp != nullptr && mBmp->Width != 0 && mBmp->Height != 0)
  {
    float   scaleW = (float)(Width - 1) / mBmp->Width;
    float   scaleH = (float)(Height - 1) / mBmp->Height;

    Scale((scaleH > scaleW) ? scaleW : scaleH);
  }

  return mScale;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::Rotate(float A)
{
  if (mAngle != A)
  {
    mAngle = A;
    A *= -M_PI / 180.0;
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

void        EvImage::RotateAround(int16_t X, int16_t Y, float A, float Scale)
{
  RotateAround(X, Y);
  Rotate(A);
  EvImage::Scale(Scale);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::SetMode(uint8_t ResizeMode, uint8_t FilterMode)
{
  mResizeMode = ResizeMode;
  mFilterMode = FilterMode;
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::SetColor(uint8_t Red, uint8_t Green, uint8_t Blue)
{
  mRed = Red;
  mGreen = Green;
  mBlue = Blue;
  Modified();
}

/// @copydoc EvButton::SetOnTouch()

void        EvImage::SetOnTouch(void (*OnTouch)(EvImage *Sender, const EvTouchEvent *Touch))
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
    float   coeffA = mCosA * mScale;
    float   coeffB = -mSinA * mScale;
    float   coeffD = mSinA * mScale;
    float   coeffE = coeffA;
    float   coeffC = (mPivotX * mScale) - ((mPivotX * coeffA) + (mPivotY * coeffB));
    float   coeffF = (mPivotY * mScale) - ((mPivotX * coeffD) + (mPivotY * coeffE));
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

    x = mShiftX;
    y = mShiftY;
    mShiftX = left;
    mShiftY = top;

    ReSize(right - left + 1, bottom - top + 1);
    MoveRel(mShiftX - x, mShiftY - y);
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
    Disp->ColorRGB(mRed, mGreen, mBlue);
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

      case RESIZE_PROPORTIONAL:
        float   scaleW = (float)mWidth / mBmp->Width;
        float   scaleH = (float)mHeight / mBmp->Height;

        Scale((scaleW > scaleH) ? scaleH : scaleW);
        mWidth = mBmp->Width * mScale;
        mHeight = mBmp->Height * mScale;
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
    pivotX = mPivotX - (mShiftX / mScale);
    pivotY = mPivotY - (mShiftY / mScale);
    mCoeff[1] = s15f16(mSinA / mScale);
    mCoeff[2] = s15f16(mPivotX - ((pivotX * mCosA) + (pivotY * mSinA)));
    mCoeff[3] = s15f16(-mSinA / mScale);
    mCoeff[4] = mCoeff[0] = s15f16(mCosA / mScale);
    mCoeff[5] = s15f16(mPivotY - ((pivotX * -mSinA) + (pivotY * mCosA)));

    mRefreshCoeff = false;
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
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
      SetMode(RESIZE_PROPORTIONAL | RESIZE_ON_LOAD, BILINEAR);
    }

    Modified();
    mBmp = Bmp;
    return mBmp;
  }

  return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvImage::sOnLoading(EvDMA *Data)
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
      EvOut->print("JPEG file signature found");
    #endif

    for (i = total = 2; Data[i++] == 0xFF && i < DataSize; i += length)
    {
      total += 2;
      marker = Data[i++];

      #ifdef  VERBOSE
        EvOut->printf("\nMarker FF %02X", marker);
      #endif

      while (marker == 0xDA && i < DataSize)  // can have several SOS
      {
        for (length = i; i < DataSize; )  // skip byte stuffing and restart marker
          if (Data[i++] == 0xFF && (marker = Data[i++]) != 0x00 && (marker & ~7) != 0xD0)
            break;

        length = i - length;
        total += length;

        #ifdef  VERBOSE
          EvOut->printf(" %u bytes\nMarker FF %02X", length, marker);
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
          EvOut->printf("\nTotal length = %u bytes\n", total);
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
        EvOut->printf(" %u bytes", length + 2);
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
      EvOut->print("PNG file signature found\n");
    #endif

    for (i = total = sizeof(signature); i + 12 <= DataSize; i += length + 4)
    {
      length = (((((Data[i+0] << 8) + Data[i+1]) << 8) + Data[i+2]) << 8) + Data[i+3];
      memcpy(chunk, &Data[i+4], 4);
      total += length + 12;
      i += 8;

      #ifdef  VERBOSE
        EvOut->printf("Chunk %s %u bytes\n", chunk, length + 12);
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
          EvOut->printf("Total length = %u bytes\n", total);
        #endif

        return true;
      }
    }
  }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        IsValidASTC(uint8_t *Data, uint32_t DataSize, EvBmp *Bmp, const char *Tag)
{
  uint16_t    i, texelX, texelY;
  uint32_t    width, height, length;
  uint8_t     layout, block_x, block_y;
  static const char magicNumber[4] = {0x13, 0xAB, 0xA1, 0x5C};

  struct ASTC_HEADER
  {
    uint8_t   magic[4];
    uint8_t   block_x;
    uint8_t   block_y;
    uint8_t   block_z;
    uint8_t   dim_x[3];
    uint8_t   dim_y[3];
    uint8_t   dim_z[3];
  } *astc = (ASTC_HEADER *)Data;

  struct TEXEL
  {
    uint64_t  lsb;
    uint64_t  msb;
  } *texel, *buf;

  if (DataSize >= sizeof(ASTC_HEADER) && memcmp(astc->magic, magicNumber, sizeof(magicNumber)) == 0) // Check magicNumber
  {
    #ifdef  VERBOSE
      EvOut->print("ASTC file signature found\n");
    #endif

    block_x = astc->block_x;
    block_y = astc->block_y;
    width = astc->dim_x[0] + (astc->dim_x[1] << 8) + (astc->dim_x[2] << 16);
    height = astc->dim_y[0] + (astc->dim_y[1] << 8) + (astc->dim_y[2] << 16);

    switch (block_x)
    {
      case 4:  layout = (block_y == 4) ? 31 : 0; break;
      case 5:  layout = (block_y == 4) ? 32 : ((block_y == 5) ? 33 : 0); break;
      case 6:  layout = (block_y == 5) ? 34 : ((block_y == 6) ? 35 : 0); break;
      case 8:  layout = (block_y == 5) ? 36 : ((block_y == 6) ? 37 : ((block_y == 8) ? 38 : 0)); break;
      case 10: layout = (block_y == 5) ? 39 : ((block_y == 6) ? 40 : ((block_y == 8) ? 41 : ((block_y == 10) ? 42 : 0))); break;
      case 12: layout = (block_y == 10) ? 43 : ((block_y == 12) ? 44 : 0); break;
      default: layout = 0; break;
    }

    if (layout == 0 || astc->block_z != 1 || astc->dim_z[0] != 1 || astc->dim_z[1] != 0 || astc->dim_z[2] != 0)
      return false;

    Data += sizeof(ASTC_HEADER);
    DataSize -= sizeof(ASTC_HEADER);
    texelX = (width + block_x - 1) / block_x;
    texelY = (height + block_y - 1) / block_y;
    length = texelX * texelY * 16;

    if (length == DataSize && (buf = (TEXEL *)malloc(texelX * 2 * sizeof(TEXEL))) != nullptr)
    {
      // bitmap texel remapping according to EVE decoder (2x2 tiles)

      for (texel = (TEXEL *)Data; texelY >= 2; texelY -= 2)
      {
        memcpy(buf, texel, texelX * 2 * sizeof(TEXEL));

        for (i = 0; i < (texelX & ~1); texel += 4)
        {
          texel[0] = buf[i];
          texel[1] = buf[texelX + i++];
          texel[2] = buf[texelX + i];
          texel[3] = buf[i++];
        }

        if (i < texelX)
        {
          texel[0] = buf[i];
          texel[1] = buf[texelX + i];
          texel += 2;
        }
      }

      Bmp->Format = RAW_DATA;
      Bmp->Layout = layout;
      Bmp->Width = width;
      Bmp->Height = height;
      Bmp->PalSize = 0;
      Bmp->BmpSize = DataSize;
      Bmp->DataSize = DataSize;
      Bmp->Data = Data;
      Bmp->Tag = Tag;
      free(buf);
      return true;
    }
  }

  return false;
}

