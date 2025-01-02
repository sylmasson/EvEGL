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

#ifndef     _EV_PLAYER_H_
#define     _EV_PLAYER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPlayer : public EvPanel
{
  class EvPlayerBtn : public EvLabel
  {
    public:
      EvPlayerBtn(uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag) : EvLabel(0, 0, Width, Height, Disp, Tag, VISIBLE_OBJ) {};

    protected:
      virtual void  drawEvent(void);
  
    public:
      static const  char *const TypeName;
      virtual const char *TypeObj(void) { return TypeName; };
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
    uint8_t       mPrevFPS;
    uint16_t      mTimerHide;
    int16_t       mSmallLeft;
    int16_t       mSmallTop;
    uint16_t      mSmallWidth;
    uint16_t      mSmallHeight;
    EvPanel       *mSmallOwner;

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
    EvSlider      *Volume;
    EvLabel       *LiveFPS;
    EvPlayerBtn   *TimeLapse;
    EvPlayerBtn   *PlayButton;
    EvPlayerBtn   *FullButton;
    EvPlayerBtn   *SpeedButton;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvPlayer   *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PLAYER_H_ */
