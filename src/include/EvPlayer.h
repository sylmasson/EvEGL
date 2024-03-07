
#ifndef     _EV_PLAYER_H_
#define     _EV_PLAYER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPlayer : public EvPanel
{
  class Button : public EvLabel
  {
    public:
      Button(uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag) : EvLabel(0, 0, Width, Height, Disp, Tag, VISIBLE_OBJ) {};

    protected:
      virtual void  drawEvent(void);
  };

  public:
    bool          Open(const char *FileName, SDClass &Dev = SD);
    bool          IsRunning(void);
    void          Close(void);
    bool          Play(void);
    bool          Stop(void);
    bool          Pause(void);
    bool          Faster(void);
    void          ShowInfo(void);
    void          HideInfo(void);
    void          ScreenSize(void);
    void          FullScreen(void);
    void          SmallScreen(void);
    void          TouchInfo(EvTouchEvent *Touch);

  protected:
    EvPlayer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    bool          mRun;
    bool          mLock;
    bool          mFullScreen;
    uint8_t       mSpeedIndex;
    uint8_t       mTouchCount;
    uint16_t      mTimerHide;
    int16_t       mSmallLeft;
    int16_t       mSmallTop;
    uint16_t      mSmallWidth;
    uint16_t      mSmallHeight;

    void          resize(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  public:
    EvVideo       *Video;
    EvLabel       *TimeLapse;
    EvSlider      *TimeLine;
    EvSideBar     *TopBar;
    EvSideBar     *BottomBar;
    Button        *PlayButton;
    Button        *FullButton;
    Button        *SpeedButton;

    static EvPlayer   *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PLAYER_H_ */
