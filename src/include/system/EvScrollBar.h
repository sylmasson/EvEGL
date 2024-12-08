
#ifndef     _EV_SCROLLBAR_H_
#define     _EV_SCROLLBAR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     SCROLL_BAR_OFF  0
#define     SCROLL_BAR_ON   1
#define     SCROLL_BAR_AUTO 2

#define     SCROLL_MOVING   (1 << 0)
#define     SCROLL_TIMER    (1 << 1)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvScrollBar class is used to create a standard ScrollBar.
 *
 * The user can add one or more ScrollBar in his own Panel object. However, take a
 * look at the EvScrollBox class which makes it easy to implement these mechanisms.
 * 
 * @par Example
 * ~~~
 * EvPanel     *myPanel = EvPanel::Create(50, 50, 150, 250, myDisplay, "MyPanel");
 * EvScrollBar *myScrollBar = EvScrollBar::Create(150 - 15, 0, 15, 250, myPanel, "MyScrollBar");
 * myScrollBar->SetOnChange(MyScrollBarChange);
 * myScrollBar->SetPageSize(500, 250);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvScrollBar : public EvObj
{
  friend class EvScrollBox;

  protected:
    EvScrollBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    int16_t       Value(void);
    void          WakeUp(void);
    bool          TouchBar(void);
    int16_t       Scrolling(void);
    void          ToBeginning(bool Wakeup = true);
    void          ToTheEnd(bool Wakeup = true);
    bool          SetValue(int16_t Value);
    bool          SetValue(int16_t Value, bool Wakeup, bool Change = false);
    void          SetScrollBarSync(EvScrollBar *ScrollBarSync);
    void          SetPageSize(uint16_t PageSize, uint16_t ViewSize);
    void          SetBarColor(uint16_t ColorKnob, uint8_t Opacity = 128);
    void          SetBarStyle(uint8_t Mode, uint8_t Padding = 6, uint16_t Delay = 1500);
    void          SetOnChange(void (*OnChange)(EvScrollBar *Sender, int32_t Value));

  protected:
    int16_t       mValue;
    uint16_t      mTimer;
    int16_t       mSpeed;
    uint16_t      mPageSize;
    uint16_t      mViewSize;
    uint16_t      mDelay;
    color16       mColorKnob;
    uint16_t      mKnobZone;
    uint16_t      mKnobSize;
    uint8_t       mOpacityKnob;
    uint8_t       mMode;
    uint8_t       mPadding;
    uint8_t       mAlpha;
    uint8_t       mScrolling;
    bool          mTouchBar;
    EvKinScroll   mKinScroll;
    EvScrollBar   *mScrollBarSync;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnChange)(EvScrollBar *Sender, int32_t Value);

    void          wakeup(void);
    void          resize(void);

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvScrollBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SCROLLBAR_H_ */
