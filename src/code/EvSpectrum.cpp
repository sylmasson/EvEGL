
#include    <EvGUI.h>

#define     LINE_COLOR      RGB555(255, 255,   0)
#define     FILL_COLOR      RGB555(128, 128,   0)

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
  return !Dest ? nullptr : (EvSpectrum *)EvObj::TryCreate(new EvSpectrum(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvSpectrum" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSpectrum::EvSpectrum(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  if ((mData = (uint8_t *)calloc(mDataSize = Width / 2, sizeof(*mData))) == nullptr)
    abortCreate();
  else
  {
    SetColor(LINE_COLOR, FILL_COLOR);
    SetOnTouch(nullptr);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSpectrum::~EvSpectrum(void)
{
  free(mData);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvSpectrum::RawData(uint16_t X, uint8_t Data)
{
  if (X < mDataSize)
  {
    mData[X] = Data;
    Modified();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvSpectrum::RawData(uint16_t X, uint8_t *Data, uint16_t Count)
{
  if (X < mDataSize)
  {
    if (X + Count > mDataSize)
      Count = mDataSize - X;

    memcpy(mData + X, Data, Count);
    Modified();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Spectrum.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  LineColor  The Line Color of the Spectrum.
 * @param[in]  FillColor  The Fill Color of the Spectrum.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void          EvSpectrum::SetColor(uint16_t LineColor, uint16_t FillColor)
{
  if (mLineColor != LineColor || mFillColor != FillColor)
  {
    mLineColor = LineColor;
    mFillColor = FillColor;
    Modified();
  }
}

/// @copydoc EvButton::SetOnTouch()

void        EvSpectrum::SetOnTouch(void (*OnTouch)(EvSpectrum *Sender, EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::drawEvent(void)
{
  uint16_t  i, len, vCnt, vCall, size = mDataSize;
  int16_t   x, y, prevX, prevY, xBuf[size], yBuf[size];

//  Disp->CmdGradient(0, 0, 0xc0c000, 0, mHeight, 0x000000);

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
    if (mFillColor == mBgColor)
    {
      Disp->ColorRGB(mLineColor);
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

      Disp->ColorRGB(mFillColor);
      Disp->Begin(EDGE_STRIP_B);
      Disp->LineWidth(16);
      Disp->Call(vCall);
      Disp->ColorRGB(mLineColor);
      Disp->Begin(LINE_STRIP);
      Disp->LineWidth(16);
      Disp->Call(vCall);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSpectrum::dataScaling(uint16_t Ind, int16_t &X, int16_t &Y)
{
  X = (Ind * (mWidth * 16)) / mDataSize;
  Y = (mData[Ind] * mHeight) >> 4;
}

