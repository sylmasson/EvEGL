
#ifndef     _EV_SCROLLBAR_H_
#define     _EV_SCROLLBAR_H_

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
  public:
    int16_t       Value(void);
    void          WakeUp(void);
    bool          TouchBar(void);
    int16_t       Scrolling(void);
    void          ToBeginning(bool Wakeup = true);
    void          ToTheEnd(bool Wakeup = true);
    bool          SetValue(int16_t Value, bool Wakeup = true);
    void          SetScrollBarSync(EvScrollBar *ScrollBarSync);
    void          SetPageSize(uint16_t PageSize, uint16_t ViewSize);
    void          SetBarColor(uint16_t ColorKnob, uint16_t ColorTouch, uint8_t Opacity = 128);
    void          SetBarStyle(uint8_t Mode, uint8_t Padding = 6, uint16_t Delay = 1500);
    void          SetOnChange(void (*OnChange)(EvScrollBar *Sender, int16_t Value));

  protected:
    EvScrollBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    int16_t       mValue;
    int16_t       mSpeed;
    uint16_t      mPageSize;
    uint16_t      mViewSize;
    uint16_t      mDelay;
    uint16_t      mColorKnob;
    uint16_t      mColorTouch;
    uint16_t      mKnobZone;
    uint16_t      mKnobSize;
    uint8_t       mOpacityKnob;
    uint8_t       mMode;
    uint8_t       mPadding;
    uint8_t       mAlpha;
    uint8_t       mScrolling;
    bool          mTouchBar;
    uint16_t      mTimer;
    EvKinScroll   mKinScroll;
    EvScrollBar   *mScrollBarSync;

    virtual void  drawEvent(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnChange)(EvScrollBar *Sender, int16_t Value);

    void          wakeup(void);
    void          resize(void);

  public:
    static EvScrollBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SCROLLBAR_H_ */
