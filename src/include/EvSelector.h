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

#ifndef     _EV_SELECTOR_H_
#define     _EV_SELECTOR_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvSelector class is used to create a standard Selector.
 *
 * Use the public \b Items member to add new text items to the list. \b Items is a pointer
 * to the EvStringList class object.
 * 
 * You can also set a Bitmap on the Selector instead of text items.
 * 
 * The shape of the new Selector is set to FIXED_CORNERS by default.
 * 
 * @par Example
 * ~~~
 * EvSelector *mySelector = EvSelector::Create(50, 50, 200, 50, myPanel, "MySelector");
 * mySelector->Items.Add("A|B|C|D", '|');
 * mySelector->Items.AutoSort = true;
 * mySelector->ShowDelimiter();
 * mySelector->SetValue(2);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSelector : public EvObj
{
  protected:
    EvSelector(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvSelector(void);

    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          ShowDelimiter(void);
    void          HideDelimiter(void);
    bool          SetBmp(const EvBmp *Bmp, uint16_t Count);
    void          SetColor(uint16_t ColorBg, uint16_t ColorSelect = CL_SELECTOR_TRUE, uint16_t ColorNew = CL_SELECTOR_NEW);
    void          SetOnTouch(void (*OnTouch)(EvSelector *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvSelector *Sender, int32_t Value));

    virtual void  SetDisplay(EvDisplay *Disp);

  protected:
    int16_t       mValue;
    uint16_t      mCount;
    int16_t       mNewVal;
    uint16_t      mOption;
    color16       mColorBg;
    color16       mColorNew;
    color16       mColorSelect;
    const EvMem   *mDest;
    const EvBmp   *mBmp;

    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSelector *Sender, const EvTouchEvent *Touch);
    void          (*mOnChange)(EvSelector *Sender, int32_t Value);

    void          setNewVal(int16_t Value);
    bool          drawBmp(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height);

    static void   sOnStringListChange(EvObj *Sender, int32_t Count);

  public:
    EvStringList  Items;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvSelector *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvTab class is used to create a standard Tab selector.
 *
 * The EvTab class is a child of the EvSelector class. The difference is only on
 * the display. The EvTab always has one side with two square corners. The other
 * two corners are set by the EvObj::BdShape member function.
 * 
 * The delimiters are visible by default.
 * 
 * @par Example
 * ~~~
 * EvTab *myTab = EvTab::Create(50, 50, 200, 50, myPanel, "MyTab");
 * myTab->Items.Add("Tab1|Tab2|Tab3", '|');
 * myTab->Items.AutoSort = true;
 * myTab->SetValue(0);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTab : public EvSelector
{
  protected:
    EvTab(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    void          SetTabLeftTop(void);
    void          SetTabRightBottom(void);
    void          SetOnTouch(void (*OnTouch)(EvTab *Sender, const EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvTab *Sender, int32_t Value));

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvTab  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | FILTER_DIS_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SELECTOR_H_ */

