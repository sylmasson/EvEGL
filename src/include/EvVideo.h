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

#ifndef     _EV_VIDEO_H_
#define     _EV_VIDEO_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     VIDEO_RUN               (1 << 0)
#define     VIDEO_LOAD              (1 << 1)
#define     VIDEO_REWIND            (1 << 2)

#define     WAVE_FORMAT_UNKNOWN     0
#define     WAVE_FORMAT_PCM         1
#define     WAVE_FORMAT_MULAW       2
#define     WAVE_FORMAT_IMA_ADPCM   3

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
  uint32_t  FrameRate;
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
    uint16_t      FPS(void) { return mFps; };
    void          SetOnLoadFrame(void (*OnLoadFrame)(EvVideo *Sender, uint32_t FrameNbr));
    void          SetOnLoadAudio(void (*OnLoadAudio)(EvVideo *Sender, void *Data, uint16_t DataSize));

  protected:
    uint8_t       mCtrl;
    uint8_t       mSpeed;
    uint8_t       mFrameSync;
    uint8_t       mFps;
    uint8_t       mFpsCnt;
    uint16_t      mFpsTimer;
    File          mFidx;
    File          mFile;
    uint32_t      mFileInd;
    uint32_t      mFileSize;
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
    void          (*mOnLoadAudio)(EvVideo *Sender, void *Data, uint16_t DataSize);

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
