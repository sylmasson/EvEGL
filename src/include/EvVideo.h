
#ifndef     _EV_VIDEO_H_
#define     _EV_VIDEO_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvVideo class is used to playback Video.
 * 
 * @par Example
 * ~~~
 * EvVideo *myVideo = EvVideo::Create(50, 50, 150, 50, myPanel, "MyVideo");
 * myVideo->Load(...)
 * ~~~
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvVideo : public EvImage
{
  public:
    ~EvVideo(void);

    bool          Open(const char *Filename, SDClass &Dev = SD);
    bool          NextFrame(void);
    bool          Play(void);

  protected:
    EvVideo(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);

    virtual void  refreshEvent(void);

    File          mFile;
    uint32_t      mFileSize;
    uint32_t      mIndex;
    uint32_t      mFrameCount;
    EvBmp         *mFrame;
    bool          mNextLoad;
    bool          mRun;

  private:
    void          (*mOnTouch)(EvVideo *Sender, EvTouchEvent *Touch);

    bool          load(void);
    bool          read(uint32_t *Data, uint32_t Pos, uint32_t Count);

  public:
    static EvVideo  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = NULL, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_VIDEO_H_ */
