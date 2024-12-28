
#include    <EvEGL.h>

#define     LINE_COLOR      CL_WHITE_SMOKE
#define     FILL_COLOR      CL_WHITE_SMOKE
#define     BD_COLOR        CL_WHITE_SMOKE

const char * const EvSpectrum::TypeName = "EvSpectrum";

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of the Spectrum.
 * 
 * A new standard Spectrum is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of Spectrum.
 * @param[in]  Top     The top position of Spectrum.
 * @param[in]  Width   The width of Spectrum.
 * @param[in]  Height  The height of Spectrum.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Spectrum. If nullptr, the default tag name is "EvSpectrum".
 * @param[in]  State   The initial state of the Spectrum. Default is set to VISIBLE_OBJ.
 *
 * @return     EvSpectrum address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSpectrum     *EvSpectrum::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvSpectrum *)EvObj::TryCreate(new EvSpectrum(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSpectrum::EvSpectrum(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mData(nullptr),
  mDataSize(0),
  mOnTouch(nullptr)
{
  SetColor(LINE_COLOR, FILL_COLOR);
  BdShape(FIXED_CORNERS);
  BdColor(BD_COLOR);
  BdWidth(32);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSpectrum::~EvSpectrum(void)
{
  free(mData);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool          EvSpectrum::SetData(uint16_t X, uint8_t Data)
{
  if (X >= mDataSize || mData == nullptr)
    return false;

  mData[X] = Data;
  Modified();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool          EvSpectrum::SetData(uint16_t X, uint8_t *Data, uint16_t Count)
{
  if (X >= mDataSize || mData == nullptr)
    return false;

  if (X + Count > mDataSize)
    Count = mDataSize - X;

  memcpy(mData + X, Data, Count);
  Modified();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool          EvSpectrum::SetDataSize(uint16_t DataSize, uint8_t Value)
{
  free(mData);
  mDataSize = 0;

  if (DataSize < 2 || (mData = (uint8_t *)malloc(DataSize)) == nullptr)
    return false;

  memset(mData, Value, mDataSize = DataSize);
  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Spectrum.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  LineColor  The Line Color of the Spectrum.
 * @param[in]  FillColor  The Fill Color of the Spectrum.
 * @param[in]  FillColorA The Fill Color Opacity of the Spectrum.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvSpectrum::SetColor(uint16_t LineColor, uint16_t FillColor, uint8_t FillColorA)
{
  if ((mLineColor.Set(LineColor) | mFillColor.Set(FillColor)) || mFillColorA != FillColorA)
  {
    mFillColorA = FillColorA;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvSpectrum::SetOnTouch(void (*OnTouch)(EvSpectrum *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::drawEvent(void)
{
  uint16_t  i, len, vCnt, vCall, size = mDataSize;
  int16_t   x, y, prevX, prevY, xBuf[size], yBuf[size];

  for (i = x = y = len = vCnt = 0; i < size; i++)
  {
    prevX = x;
    prevY = y;
    dataScaling(i, x, y);

    if (i > 0 && y == prevY)
      len++;
    else
    {
      if (len)
      {
        xBuf[vCnt] = prevX;
        yBuf[vCnt++] = prevY;
      }

      xBuf[vCnt] = x;
      yBuf[vCnt++] = y;
      len = 0;
    }
  }

  if (len)
  {
    xBuf[vCnt] = x;
    yBuf[vCnt++] = y;
  }

  if (vCnt)
  {
    if (mFillColor.Get() == 0 || mFillColorA == 0)
    {
      Disp->ColorRGB(mLineColor.Get());
      Disp->Begin(LINE_STRIP);
      Disp->LineWidth(16);

      for (i = 0; i < vCnt; i++)
        Disp->Vertex2f(xBuf[i], yBuf[i]);
    }
    else
    {
      BeginFunction(vCall, vCnt);
        for (i = 0; i < vCnt; i++)
          Disp->Vertex2f(xBuf[i], yBuf[i]);
      EndFunction(vCall, vCnt);

      Disp->ColorRGB(mFillColor.Get());
      Disp->ColorA(mFillColorA);
      Disp->Begin(EDGE_STRIP_B);
      Disp->LineWidth(16);
      Disp->Call(vCall);
      Disp->ColorRGB(mLineColor.Get());
      Disp->ColorA(255);
      Disp->Begin(LINE_STRIP);
      Disp->LineWidth(16);
      Disp->Call(vCall);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::dataScaling(uint16_t Ind, int16_t &X, int16_t &Y)
{
  X = ((Ind * (uint32_t)((mWidth * 16) - (mBdWidth * 2) - 32)) / (mDataSize - 1)) + mBdWidth + 16;
  Y = ((mData[Ind] * (uint32_t)((mHeight * 16) - (mBdWidth * 2) - 32)) / 256) + mBdWidth + 16;
}

