
#ifndef     _EV_PLAYER_H_
#define     _EV_PLAYER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPlayer : public EvPanel
{
  public:
    bool          Open(const char *FileName, SDClass &Dev = SD);
    void          Close(void);
    void          ShowInfo(void);
    void          HideInfo(void);
    void          TouchInfo(EvTouchEvent *Touch);
    void          SetOnTouch(void (*OnTouch)(EvPlayer *Sender, EvTouchEvent *Touch));

  protected:
    EvPlayer(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    bool          mLock;
    uint8_t       mTouchCount;
    uint16_t      mTimerHide;

    void          resize(void);
    virtual void  resizeEvent(void);
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvPlayer *Sender, EvTouchEvent *Touch);

  public:
    EvVideo       *Video;
    EvLabel       *Timelapse;
    EvSlider      *Timeline;
    EvSideBar     *TopBar;
    EvSideBar     *BottomBar;

    static EvPlayer   *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PLAYER_H_ */
