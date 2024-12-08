
#ifndef     _EV_SCROLLBOX_H_
#define     _EV_SCROLLBOX_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvScrollBox class is used to create a standard ScrollBox.
 *
 * The EvScrollBox class is a child class of EvPanel. The operation of scrolling a
 * page larger than possible view has been added. A vertical and horizontal ScrollBar
 * has also been added. These operate in SCROLL_BAR_AUTO mode by default.
 * 
 * @par Example
 * ~~~
 * EvScrollBox *myScrollBox = EvScrollBox::Create(50, 50, 150, 250, myPanel, "MyScrollBox");
 * myScrollBox->SetPageSize(200, 500);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvScrollBox : public EvPanel
{
  protected:
    EvScrollBox(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | CONTROL_OBJ);

  public:
    bool          Scrolling(void);
    void          SetScrollSpeed(uint8_t Speed, uint16_t Period = 640);
    void          SetPageSize(uint16_t PageWidth, uint16_t PageHeight);
    void          SetPageOffset(int16_t PageOffsetX, int16_t PageOffsetY);
    void          SetBarColor(uint16_t ColorKnob, uint8_t Opacity = 128);
    void          SetBarStyle(uint8_t Mode, uint8_t Thickness = 8, uint8_t Padding = 6, uint16_t Delay = 1500);
    void          SetOnTouch(void (*OnTouch)(EvScrollBox *Sender, const EvTouchEvent *Touch));

    virtual EvObj *Touching(const EvTouchEvent *Touch);
    virtual void  Draw(void);

  protected:
    int16_t       mSpeedX;
    int16_t       mSpeedY;
    uint16_t      mPageWidth;
    uint16_t      mPageHeight;
    int16_t       mPageOffsetX;
    int16_t       mPageOffsetY;
    uint8_t       mBarThickness;
    uint8_t       mBarPadding;
    bool          mTouchFlag;
    EvScrollBar   *mScrollBarX;
    EvScrollBar   *mScrollBarY;
    EvKinScroll   mKinScrollX;
    EvKinScroll   mKinScrollY;

    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          resize(void);
    void          scrollX(int16_t X);
    void          scrollY(int16_t Y);

    static void   sOnChangeScrollBarX(EvScrollBar *Sender, int32_t Value);
    static void   sOnChangeScrollBarY(EvScrollBar *Sender, int32_t Value);

    void          (*mOnTouch)(EvScrollBox *Sender, const EvTouchEvent *Touch);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvScrollBox *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ | CONTROL_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SCROLLBOX_H_ */
