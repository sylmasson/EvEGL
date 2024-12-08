
#ifndef     _EV_VIDEO_H_
#define     _EV_VIDEO_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     VIDEO_RUN       (1 << 0)
#define     VIDEO_LOAD      (1 << 1)
#define     VIDEO_REWIND    (1 << 2)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct
{
  uint16_t  Width;
  uint16_t  Height;
  uint8_t   Stream;
  uint8_t   AudioFmt;
  uint8_t   AudioChan;
  uint8_t   SampleDepth;
  uint32_t  SampleRate;
  uint32_t  SampleCount;
  uint32_t  AudioBufSize;
  uint32_t  VideoBufSize;
  float     VideoLength;
  float     FrameRate;
  uint32_t  FrameCount;
  uint32_t  MoviBegin;
  uint32_t  MoviEnd;
  uint32_t  Idx1Begin;
  uint32_t  Idx1End;
  char      VideoFcc[5];
  char      AudioFcc[5];
} AVIINFO;

typedef struct
{
  uint32_t  Nbr;
  uint32_t  Ind;
  uint32_t  Offset;
  uint32_t  Length;
} AVIFRAME;

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
  protected:
    EvVideo(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvVideo(void);

    bool          Open(const char *FileName, SDClass &Dev = SD);
    bool          IsRunning(void);
    void          Close(void);
    bool          Play(void);
    bool          Stop(void);
    bool          Pause(void);
    bool          Rewind(uint8_t Speed);
    bool          Forward(uint8_t Speed);
    bool          Seek(uint32_t FrameNbr);
    void          SetOnLoadFrame(void (*OnLoadFrame)(EvVideo *Sender, uint32_t FrameNbr));

  protected:
    uint8_t       mCtrl;
    uint8_t       mSpeed;
    uint8_t       mSkip;
    File          mFidx;
    File          mFile;
    uint32_t      mFileInd;
    uint32_t      mFileSize;
    uint8_t       mFrameSync;
    AVIINFO       mAviInfo;
    AVIFRAME      mFrame;

    virtual void  refreshEvent(void);

  private:
    bool          loadFrame(void);
    bool          nextFrame(uint32_t FrameCnt);
    bool          rewind(uint32_t FrameCnt);
    bool          forward(uint32_t FrameCnt);
    bool          seek(uint32_t FrameNbr);
    bool          seekFirst(void);
    bool          seekLast(void);
    bool          readInfo(uint32_t BlkEnd, uint16_t TabCnt, uint32_t Limit);
    bool          readFile(void *Data, uint32_t Pos, uint32_t Count);
    bool          readFidx(void *Data, uint32_t Pos, uint32_t Count);
    void          printAddr(uint32_t Addr);
    void          printSize(uint32_t Size);

    static uint8_t  sVideoCount;

    void          (*mOnLoadFrame)(EvVideo *Sender, uint32_t FrameNbr);

  public:
    bool            Mute;
    const AVIINFO   &AviInfo = mAviInfo;
    const uint32_t  &FrameNbr = mFrame.Nbr;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvVideo *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_VIDEO_H_ */
