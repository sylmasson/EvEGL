
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

  protected:
    EvPlayer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    bool          Open(const char *FileName, SDClass &Dev = SD);
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
    bool          IsFullScreen(void);
    bool          IsRunning(void);
    bool          IsMovable(void);
    void          SetMovable(bool Value);
    void          TouchInfo(const EvTouchEvent *Touch);

  protected:
    bool          mRun;
    bool          mLock;
    bool          mMovable;
    bool          mFullScreen;
    uint8_t       mSpeedIndex;
    uint8_t       mTouchCount;
    uint16_t      mTimerHide;
    int16_t       mSmallLeft;
    int16_t       mSmallTop;
    uint16_t      mSmallWidth;
    uint16_t      mSmallHeight;

    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          resize(void);

  public:
    EvVideo       *Video;
    EvSideBar     *TopBar;
    EvSideBar     *BottomBar;
    EvSlider      *TimeLine;
    Button        *TimeLapse;
    Button        *PlayButton;
    Button        *FullButton;
    Button        *SpeedButton;

    static EvPlayer   *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PLAYER_H_ */
