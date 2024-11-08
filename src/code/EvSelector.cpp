
#include    <EvGUI.h>

#define     SEPARATOR_ON    (1 << 0)
#define     TAB_STYLE       (1 << 1)
#define     TAB_LEFT_TOP    (1 << 2)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Tab selector.
 * 
 * A new Tab selector is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @note       If the height is greater than the width, the Tab selector works vertically.
 *             
 * @param[in]  Left    The left position of the Tab selector.
 * @param[in]  Top     The top position of the Tab selector.
 * @param[in]  Width   The width of the Tab selector.
 * @param[in]  Height  The height of the Tab selector.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Tab selector. If nullptr, the default tag name is "EvTab".
 * @param[in]  State   The initial state of the Tab selector. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     EvTab address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTab       *EvTab::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvTab *)EvObj::TryCreate(new EvTab(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvTab" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTab::EvTab(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvSelector(Left, Top, Width, Height, Disp, Tag, State)
{
  mOption = TAB_STYLE | TAB_LEFT_TOP | SEPARATOR_ON;
  BdShape(RATIO_CORNERS);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Set the tab usage position.
 * 
 * @li  Top side if the Tab selector works horizontally.
 * @li  Left side if the Tab selector works vertically.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTab::SetTabLeftTop(void)
{
  mOption |= TAB_LEFT_TOP;
  Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Set the tab usage position.
 * 
 * @li  Right side if the Tab selector works vertically.
 * @li  Bottom side if the Tab selector works horizontally.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTab::SetTabRightBottom(void)
{
  mOption &= ~TAB_LEFT_TOP;
  Modified();
}

/// @copydoc EvButton::SetOnTouch()

void        EvTab::SetOnTouch(void (*OnTouch)(EvTab *Sender, const EvTouchEvent *Touch))
{
  EvSelector::SetOnTouch((void (*)(EvSelector *Sender, const EvTouchEvent *Touch))OnTouch);
}

/// @copydoc EvButton::SetOnChange()

void        EvTab::SetOnChange(void (*OnChange)(EvTab *Sender, int32_t Value))
{
  EvSelector::SetOnChange((void (*)(EvSelector *Sender, int32_t Value))OnChange);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Selector.
 * 
 * A new Selector is created at the specified size and relative position
 * of its owner Dest.
 * 
 * If you want, you can specify a Tag for this new Selector.
 *
 * @note       If the height is greater than the width, the Selector works vertically.
 *             
 * @param[in]  Left    The left position of the Selector.
 * @param[in]  Top     The top position of the Selector.
 * @param[in]  Width   The width of the Selector.
 * @param[in]  Height  The height of the Selector.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Selector. If nullptr, the default tag name is "EvSelector".
 * @param[in]  State   The initial state of the Selector. Default is set to VISIBLE_OBJ | FILTER_DIS_OBJ.
 *
 * @return     EvSelector address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSelector  *EvSelector::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvSelector *)EvObj::TryCreate(new EvSelector(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvSelector" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSelector::EvSelector(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mValue(0),
  mCount(0),
  mNewVal(-2),
  mOption(0),
  mColorBg(CL_SELECTOR_BG),
  mColorNew(CL_SELECTOR_NEW),
  mColorSelect(CL_SELECTOR_TRUE),
  mDest(nullptr),
  mBmp(nullptr),
  mOnTouch(nullptr),
  mOnChange(nullptr)
{
  TextAlign(CENTER);
  TextColor(CL_SELECTOR_TEXT, CL_SELECTOR_TEXT2);
  BdShape(FIXED_CORNERS);
  Items.SetOnChange(sOnStringListChange, this);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSelector::~EvSelector(void)
{
  if (mBmp)
    Disp->UnloadBmp(mDest);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the position value of the Selector.
 * 
 * @return     The position value of the Selector.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvSelector::Value(void)
{
  return mValue;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the position value of the Selector.
 * 
 * If the Value is changed, the OnChange callback function is called.
 * 
 * Define OnChange callback function with SetOnChange function.
 *
 * @param[in]  Value   The position value of the Selector.
 * 
 * @return     true if the position value is changed, otherwise returns false.
 * 
 * @see        SetOnChange
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSelector::SetValue(int16_t Value)
{
  if (mValue == Value || Value < -1 || Value >= mCount)
    return false;

  mValue = Value;
  setNewVal(-1);

  if (mOnChange != nullptr)
    mOnChange(this, mValue);

  return true;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the item delimiters to visible.
 * 
 * By default, item delimiters are not visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::ShowDelimiter(void)
{
  mOption |= SEPARATOR_ON;
  Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the item delimiters to not visible.
 * 
 * By default, item delimiters are not visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::HideDelimiter(void)
{
  mOption &= ~SEPARATOR_ON;
  Modified();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets a Bitmap on the Selector.
 * 
 * The Bitmap takes priority over text items. This means that if a Bitmap
 * is set, no text element will be displayed.
 *
 * @param[in]  *Bmp     The EvBmp type struct address pointer of the Bitmap.
 * @param[in]  Count    The numbers of items in the Bitmap.
 * 
 * @return     true on success, otherwise returns false.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSelector::SetBmp(const EvBmp *Bmp, uint16_t Count)
{
  if (mBmp != nullptr)
  {
    Disp->UnloadBmp(mDest);
    mDest = nullptr;
    mBmp = nullptr;
    Modified();
  }

  if (Bmp != nullptr && Bmp->Layout != PALETTED8 && (mDest = Disp->LoadBmp(Bmp)) != nullptr)
  {
    mCount = Count;
    mBmp = Bmp;
    Modified();
    return true;
  }

  return false;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the colors of the Selector.
 * 
 * The colors are defined in RGB555 format.
 *
 * @param[in]  ColorBg      The color of the unselected items.
 * @param[in]  ColorSelect  The color of the selected item.
 * @param[in]  ColorNew     The color of the coming selected item.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::SetColor(uint16_t ColorBg, uint16_t ColorSelect, uint16_t ColorNew)
{
  if (mColorBg.Set(ColorBg) | mColorSelect.Set(ColorSelect) | mColorNew.Set(ColorNew))
    Modified();

  BgColor(CL_NOCOLOR);
}

/// @copydoc EvButton::SetOnTouch()

void        EvSelector::SetOnTouch(void (*OnTouch)(EvSelector *Sender, const EvTouchEvent *Touch))
{
  mOnTouch = OnTouch;
}

/// @copydoc EvButton::SetOnChange()

void        EvSelector::SetOnChange(void (*OnChange)(EvSelector *Sender, int32_t Value))
{
  mOnChange = OnChange;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::SetDisplay(EvDisplay *Disp)
{
  if (EvObj::Disp != Disp)
  {
    if (mBmp != nullptr)
      EvObj::Disp->UnloadBmp(mDest);

    EvObj::SetDisplay(Disp);

    if (mBmp != nullptr)
      mDest = EvObj::Disp->LoadBmp(mBmp);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::drawEvent(void)
{
  int16_t   i, x, y, w, h, size;

  x = y = 0;
  w = mWidth << 4;
  h = mHeight << 4;

  if (mWidth >= mHeight)
  {
    if ((mOption & TAB_STYLE) != 0)
    {
      if (!(mOption & TAB_LEFT_TOP)) y = -h;
      h <<= 1;
    }

    FillRectangle2f(x, y, w, h, mColorBg.Get(), mBdRadius);

    if (mCount > 0)
    {
      size = w / mCount;

      if (mNewVal >= 0 && mNewVal != mValue)
        FillRectangle2f(mNewVal * size, y, size, h, mColorNew.Get(), mBdRadius);

      if (!drawBmp(mValue * size, y, size, h))
        for (i = 0; i < mCount; i++)
          if (i < mCount)
            DrawText((i * size) >> 4 , 0, size >> 4, mHeight, c_str(Items[i]), (i == mValue) ? mStyle.color2.Get() : mStyle.color.Get());

      if ((mOption & SEPARATOR_ON) != 0 && mCount > 2)
      {
        x = size;
        y = mHeight << 2;
        h = mHeight << 3;
        Disp->LineWidth(8);
        Disp->ColorRGB(mStyle.color.Get());
        Disp->Begin(LINES);

        for (i = 0; i < mCount - 1; i++, x += size)
          if (mValue != i && mValue != i + 1)
          {
            Disp->Vertex2f(x, y);
            Disp->Vertex2f(x, y + h);
          }
      }
    }
  }
  else
  {
    if ((mOption & TAB_STYLE) != 0)
    {
      if (!(mOption & TAB_LEFT_TOP)) x = -w;
      w <<= 1;
    }

    FillRectangle2f(x, y, w, h, mColorBg.Get(), mBdRadius);

    if (mCount > 0)
    {
      size = h / mCount;

      if (mNewVal >= 0 && mNewVal != mValue)
        FillRectangle2f(x, mNewVal * size, w, size, mColorNew.Get(), mBdRadius);

      if (!drawBmp(x, mValue * size, w, size))
        for (i = 0; i < mCount; i++)
          if (i < mCount)
            DrawText(0, (i * size) >> 4 , mWidth, size >> 4, c_str(Items[i]), (i == mValue) ? mStyle.color2.Get() : mStyle.color.Get());

      if ((mOption & SEPARATOR_ON) != 0 && mCount > 2)
      {
        y = size;
        x = mWidth << 2;
        w = mWidth << 3;
        Disp->LineWidth(8);
        Disp->ColorRGB(mStyle.color.Get());
        Disp->Begin(LINES);

        for (i = 0; i < mCount - 1; i++, y += size)
          if (mValue != i && mValue != i + 1)
          {
            Disp->Vertex2f(x, y);
            Disp->Vertex2f(x + w, y);
          }
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::touchEvent(const EvTouchEvent *Touch)
{
  int16_t   value;

  if (mOnTouch != nullptr)
    mOnTouch(this, Touch);

  if (mWidth >= mHeight)
    value = (Touch->x * mCount) / mWidth;
  else
    value = (Touch->y * mCount) / mHeight;

  if (value < 0)
    value = 0;
  else if (value >= mCount)
    value = mCount - 1;

  switch (Touch->event)
  {
    case TOUCH_START:
      setNewVal((value != mValue) ? value : -1);
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      if (Touch->x < -10 || Touch->y < -10 || Touch->x >= mWidth + 10 || Touch->y >= mHeight + 10)
      {
        setNewVal(-2);
        return;
      }

      if (mNewVal >= 0)
        setNewVal(value);
      else if (mNewVal == -1)
        SetValue(value);

      Touch->event = 0;
      break;

    case TOUCH_END:
      if (mNewVal >= 0)
        SetValue(mNewVal);

      Touch->event = 0;
      setNewVal(-2);
      break;

    case TOUCH_CANCEL:
      if (mNewVal >= 0)
        setNewVal(-2);

      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvSelector::drawBmp(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height)
{
  if (mBmp != nullptr)
  {
    Disp->Begin(BITMAPS);
    Disp->BitmapHandle(0);
    Disp->BitmapSource(mDest->addr + mBmp->PalSize);
    Disp->BitmapLayout(mBmp->Layout, mBmp->Width, mBmp->Height);
    Disp->BitmapSize(NEAREST, BORDER, BORDER, mBmp->Width, mBmp->Height);

    if (mBmp->PalSize > 0)
      Disp->PaletteSource(mDest->addr);

    Disp->ColorRGB(mStyle.color.Get());
    Disp->Vertex2ii(0, 0);
    Disp->SaveContext();
    Disp->StencilOp(INCR, INCR);
  }

  if (Left >= 0 && Top >= 0)
    FillRectangle2f(Left, Top, Width, Height, mColorSelect.Get(), mBdRadius);

  if (mBmp == nullptr)
    return false;

  Disp->RestoreContext();
  Disp->StencilFunc(EQUAL, 1, 255);
  Disp->ColorRGB(mStyle.color2.Get());
  Disp->Begin(BITMAPS);
  Disp->Vertex2ii(0, 0);
  Disp->StencilFunc(ALWAYS, 0, 255);
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::setNewVal(int16_t Value)
{
  mNewVal = Value;
  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSelector::sOnStringListChange(EvObj *Sender, int32_t Count)
{
  EvSelector  *obj = (EvSelector *)Sender;

  if ((uint16_t)obj->mValue >= Count)
    obj->SetValue(-1);

  obj->mCount = Count;
  obj->Modified();
}
