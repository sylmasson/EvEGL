
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
  public:
    ~EvSelector(void);

    int16_t       Value(void);
    bool          SetValue(int16_t Value);
    void          ShowDelimiter(void);
    void          HideDelimiter(void);
    bool          SetBmp(const EvBmp *Bmp, uint16_t Count);
    void          SetColor(uint16_t ColorNone, uint16_t ColorSelect, uint16_t ColorNew);
    void          SetOnTouch(void (*OnTouch)(EvSelector *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvSelector *Sender, int16_t Value));

    virtual void  SetDisp(EvDisplay *Disp);

  protected:
    EvSelector(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    uint16_t      mCount;
    int16_t       mNewVal;
    uint16_t      mOption;
    uint16_t      mColorNew;
    uint16_t      mColorNone;
    uint16_t      mColorSelect;
    const EvMem   *mDest;
    const EvBmp   *mBmp;

    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvSelector *Sender, EvTouchEvent *Touch);
    void          (*mOnChange)(EvSelector *Sender, int16_t Value);

    void          setNewVal(int16_t Value);
    bool          drawBmp(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height);

    static void   sOnStringListChange(EvObj *Sender, int16_t Count);

  public:
    EvStringList  Items;

    static EvSelector *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
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
  public:
    void          SetTabLeftTop(void);
    void          SetTabRightBottom(void);
    void          SetOnTouch(void (*OnTouch)(EvTab *Sender, EvTouchEvent *Touch));
    void          SetOnChange(void (*OnChange)(EvTab *Sender, int16_t Value));

  protected:
    EvTab(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

  public:
    static EvTab  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SELECTOR_H_ */
