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

#include    <EvEGL.h>

#define     VERBOSE                 1       // 0 = None, 1 = AviInfo only, 2 = Full

#define     AVIF_HASINDEX           0x00000010
#define     AVIF_MUSTUSEINDEX       0x00000020
#define     AVIF_ISINTERLEAVED      0x00000100
#define     AVIF_TRUSTCKTYPE        0x00000800
#define     AVIF_WASCAPTUREFILE     0x00010000
#define     AVIF_COPYRIGHTED        0x00020000

#define     AVIMAINHEADER_SIZE      56
#define     AVISTREAMHEADER_SIZE    56
#define     BITMAPINFOHEADER_SIZE   40
#define     WAVEFORMATEX_SIZE       18
#define     WAVEFORMAT_SIZE         14
#define     AVIOLDINDEX_SIZE        16

#define     LIMIT_INFO_DEFAULT      20

#define     FOURCC(str)             (*(uint32_t *)str)

#define     printFmt                EvOut->printf

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct
{
  uint32_t  dwMicroSecPerFrame;
  uint32_t  dwMaxBytesPerSec;
  uint32_t  dwPaddingGranularity;
  uint32_t  dwFlags;
  uint32_t  dwTotalFrames;
  uint32_t  dwInitialFrames;
  uint32_t  dwStreams;
  uint32_t  dwSuggestedBufferSize;
  uint32_t  dwWidth;
  uint32_t  dwHeight;
  uint32_t  dwReserved[4];
} AVIMAINHEADER;  // 'avih'

typedef struct
{
  uint32_t  fccType;
  uint32_t  fccHandler;
  uint32_t  dwFlags;
  uint16_t  wPriority;
  uint16_t  wLanguage;
  uint32_t  dwInitialFrames;
  uint32_t  dwScale;
  uint32_t  dwRate;
  uint32_t  dwStart;
  uint32_t  dwLength;
  uint32_t  dwSuggestedBufferSize;
  uint32_t  dwQuality;
  uint32_t  dwSampleSize;
  struct
  {
    int16_t left;
    int16_t top;
    int16_t right;
    int16_t bottom;
  } rcFrame;
} AVISTREAMHEADER;  // 'strh'

typedef struct
{
  uint32_t  biSize;
  int32_t   biWidth;
  int32_t   biHeight;
  uint16_t  biPlanes;
  uint16_t  biBitCount;
  uint32_t  biCompression;
  uint32_t  biSizeImage;
  int32_t   biXPelsPerMeter;
  int32_t   biYPelsPerMeter;
  uint32_t  biClrUsed;
  uint32_t  biClrImportant;
} BITMAPINFOHEADER; // 'strf' for video

typedef struct
{
  uint16_t  wFormatTag;
  uint16_t  nChannels;
  uint32_t  nSamplesPerSec;
  uint32_t  nAvgBytesPerSec;
  uint16_t  nBlockAlign;
  uint16_t  wBitsPerSample;
  uint16_t  cbSize;
} WAVEFORMATEX;     // 'strf' for audio

typedef struct
{
  uint32_t  dwChunkId;
  uint32_t  dwFlags;
  uint32_t  dwOffset;
  uint32_t  dwSize;
} AVIOLDINDEX;      // 'idx1'

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char * const EvVideo::TypeName = "EvVideo";

uint8_t     EvVideo::sVideoCount = 0;

#if (VERBOSE > 0)
  static const char *sWaveFormat[4] = {"Unknown format", "PCM signed integer format", "Mu-law encoded format", "IMA ADPCM format"};
#endif

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of playback Video object.
 * 
 * A new playback Video object is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of Video object.
 * @param[in]  Top     The top position of Video object.
 * @param[in]  Width   The width of Video object.
 * @param[in]  Height  The height of Video object.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Video object. If nullptr, the default tag name is "EvVideo".
 * @param[in]  State   The initial state of the Video object. Default is set to VISIBLE_OBJ.
 *
 * @return     EvVideo address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVideo      *EvVideo::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvVideo *)EvObj::TryCreate(new EvVideo(Left, Top, Width, Height, Dest->Disp, Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVideo::EvVideo(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvImage(Left, Top, Width, Height, Disp, !Tag ? TypeName : Tag, State),
  mCtrl(0),
  mSpeed(1),
  mFrameSync(++sVideoCount & 1),
  mFps(0),
  mFpsCnt(0),
  mFpsTimer(millis()),
  mFileInd(0),
  mFileSize(0),
  mOnLoadFrame(nullptr),
  mOnLoadAudio(nullptr),
  Mute(false)
{
  SetMode(SCALE_TO_FIT, BILINEAR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVideo::~EvVideo(void)
{
  Close();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Open(const char *FileName, SDClass &Dev)
{
  struct
  {
    uint32_t  fcc;
    uint32_t  cb;
    uint32_t  type;
  } hdr;

  Close();

  if (!Dev.mediaPresent() || !(mFile = Dev.open(FileName)) || !(mFidx = Dev.open(FileName)))
  {
    #if (VERBOSE > 0)
      printFmt("\nDevice or file \"%s\" not found\n", FileName);
    #endif
  }
  else
  {
    if ((mFileSize = mFile.size()) > 0 && readFile(&hdr, mFileInd = 0, sizeof(hdr)) == true)
      if (hdr.fcc == FOURCC("RIFF") && hdr.cb == (mFileSize - 8) && hdr.type == FOURCC("AVI "))
      {
      #if (VERBOSE > 1)
        printFmt("\nReading file \"%s\"\n", FileName);
        printAddr(mFileInd);
        printFmt("'RIFF'('AVI '");
        printSize(hdr.cb);
      #endif

        mFileInd = 12;

        if (readInfo(mFileSize, 1, LIMIT_INFO_DEFAULT) == true)
        {
        #if (VERBOSE > 1)
          printAddr(mFileInd);
          printFmt("End of file \"%s\"\n", FileName);
        #endif
        #if (VERBOSE > 0)
          printFmt("\nAviInfo \"%s\"\n", FileName);
          printFmt("{\n");
          printFmt("  Width = %u\n", mAviInfo.Width);
          printFmt("  Height = %u\n", mAviInfo.Height);
          printFmt("  Stream = %u\n", mAviInfo.Stream);
          printFmt("  AudioFmt = %u (%s)\n", mAviInfo.AudioFmt, sWaveFormat[mAviInfo.AudioFmt]);
          printFmt("  AudioChan = %u\n", mAviInfo.AudioChan);
          printFmt("  SampleDepth = %u bits\n", mAviInfo.SampleDepth);
          printFmt("  SampleRate  = %u Hz\n", mAviInfo.SampleRate);
          printFmt("  SampleCount = %u\n", mAviInfo.SampleCount);
          printFmt("  AudioBufSize = %u bytes\n", mAviInfo.AudioBufSize);
          printFmt("  VideoBufSize = %u bytes\n", mAviInfo.VideoBufSize);
          printFmt("  VideoLength  = %.2f sec\n", mAviInfo.VideoLength);
          printFmt("  FrameRate  = %.3f Hz\n", (float)mAviInfo.FrameRate / 1000.0);
          printFmt("  FrameCount = %u\n", mAviInfo.FrameCount);
          printFmt("  MoviBegin = 0x%08X\n", mAviInfo.MoviBegin);
          printFmt("  MoviEnd   = 0x%08X\n", mAviInfo.MoviEnd);
          printFmt("  Idx1Begin = 0x%08X\n", mAviInfo.Idx1Begin);
          printFmt("  Idx1End   = 0x%08X\n", mAviInfo.Idx1End);
          printFmt("  VideoFcc = '%s'\n", mAviInfo.VideoFcc);
          printFmt("  AudioFcc = '%s'\n", mAviInfo.AudioFcc);
          printFmt("}\n");
        #endif
        #if (VERBOSE > 1)
          printFmt("\nmFrame\n");
          printFmt("{\n");
          printFmt("  Nbr    = #%u\n", mFrame.Nbr);
          printFmt("  Ind    = 0x%08X\n", mFrame.Ind);
          printFmt("  Offset = 0x%08X\n", mFrame.Offset);
          printFmt("  Length = %u\n", mFrame.Length);
          printFmt("}\n");
        #endif

          if (EvDisplay::FrameRate() / 2 != mAviInfo.FrameRate)
            EvErr->printf("EvVideo Warning: Incompatibility between display FPS (%.3f) and video FPS (%.3f)\n", (float)EvDisplay::FrameRate() / 1000.0, (float)mAviInfo.FrameRate / 1000.0);

          mCtrl = 0;

          if (!seekFirst() || !loadFrame())
          {
            Close();
            return false;
          }

          ScaleToFit(mWidth, mHeight, mAviInfo.Width, mAviInfo.Height);
          mFps = mFpsCnt = 0;
          return true;
        }
      }

    #if (VERBOSE > 0)
      printFmt("\n\"%s\" is not valid AVI file\n", FileName);
    #endif
  }

  Close();
  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::IsRunning(void)
{
  return (mCtrl & VIDEO_RUN) ? true : false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::Close(void)
{
  Unload();
  memset(&mFrame, 0, sizeof(mFrame));
  memset(&mAviInfo, 0, sizeof(mAviInfo));

  if (mFile)
    mFile.close();

  if (mFidx)
    mFidx.close();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Play(void)
{
  if (!mFile)
    return false;

  mCtrl = VIDEO_RUN;
  mFpsTimer = millis();
  mFps = mFpsCnt = 0;
  return (mFrame.Nbr >= mAviInfo.FrameCount) ? seekFirst() : true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Stop(void)
{
  if (!mFile)
    return false;

  mCtrl = VIDEO_LOAD;
  return seekFirst();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Pause(void)
{
  if (!mFile)
    return false;

  mCtrl = 0;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Rewind(uint8_t Speed)
{
  if (!mFile)
    return false;

  mSpeed = Speed;
  mCtrl |= VIDEO_REWIND;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Forward(uint8_t Speed)
{
  if (!mFile)
    return false;

  mSpeed = Speed;
  mCtrl &= ~VIDEO_REWIND;
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Seek(uint32_t FrameNbr)
{
  if (!mFile)
    return false;

  mCtrl |= VIDEO_LOAD;
  return (seek(FrameNbr));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::SetOnLoadFrame(void (*OnLoadFrame)(EvVideo *Sender, uint32_t FrameNbr))
{
  mOnLoadFrame = OnLoadFrame;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::SetOnLoadAudio(void (*OnLoadAudio)(EvVideo *Sender, void *Data, uint16_t DataSize))
{
  mOnLoadAudio = OnLoadAudio;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::refreshEvent(void)
{
  if ((uint16_t)(millis() - mFpsTimer) >= 1000)
  {
    mFps = mFpsCnt;
    mFpsTimer += 1000;
    mFpsCnt = 0;
  }

  if ((Disp->FrameNumber() & 1) == mFrameSync)
  {
    if (mCtrl & (VIDEO_RUN | VIDEO_LOAD) && loadFrame() == true)
    {
      if (mFrame.Nbr >= mAviInfo.FrameCount || mSpeed == 0)
        mCtrl = 0;
      else
      {
        uint32_t  Nbr = mFrame.Nbr;

        mCtrl &= ~VIDEO_LOAD;

        if (mCtrl & VIDEO_REWIND)
          rewind(mSpeed);
        else if (!Mute && mSpeed == 1 && mCtrl & VIDEO_RUN)
          nextFrame(1);
        else
          forward(mSpeed);

        if (Nbr == mFrame.Nbr)
          mCtrl = 0;
      }
    }
  }

  EvImage::refreshEvent();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::loadFrame(void)
{
  EvBmp     *bmp;
  uint8_t   *jpeg;

  if ((bmp = (EvBmp *)malloc(sizeof(EvBmp) + mFrame.Length)) != nullptr)
  {
    jpeg = ((uint8_t *)bmp) + sizeof(EvBmp);

    if (readFile(jpeg, mFrame.Offset, mFrame.Length) && IsValidJPEG(jpeg, mFrame.Length, bmp, "VideoFrame"))
    {
      bmp->Format |= BMP_MALLOC;

      if (Load(bmp, OPT_MEDIAFIFO) != nullptr)
      {
        if (mOnLoadFrame != nullptr)
          mOnLoadFrame(this, mFrame.Nbr);

        mFpsCnt++;
        return true;
      }

      free(bmp);
    }
  }

  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::nextFrame(uint32_t FrameCnt)
{
  uint32_t  length = mFrame.Length;
  struct
  {
    uint32_t  fcc;
    uint32_t  cb;
  } chunk;

//  digitalWrite(38, HIGH);
  mFileInd = mFrame.Offset;

  while (FrameCnt > 0)
  {
    if (length & 1)
      length++;

    mFileInd += length;

    if (!readFile(&chunk, mFileInd, 8))
      return false;

    mFileInd += 8;
    length = chunk.cb;

    if (chunk.fcc == FOURCC(mAviInfo.AudioFcc))
    {
      uint32_t  ind, len, size;
      int16_t   buf[mAviInfo.AudioBufSize > 2048 ? 2048 : mAviInfo.AudioBufSize];

      for (ind = mFileInd, len = length; len > 0; len -= size, ind += size)
      {
        size = len < sizeof(buf) ? len : sizeof(buf);

        if (!readFile(buf, ind, size))
          return false;

        if (mOnLoadAudio != nullptr)
          mOnLoadAudio(this, buf, size);
      }
    }
    else if (chunk.fcc == FOURCC(mAviInfo.VideoFcc))
    {
      mFrame.Nbr++;
      mFrame.Length = length;
      mFrame.Offset = mFileInd;
      FrameCnt--;
    }

    mFrame.Ind += AVIOLDINDEX_SIZE;
  }

//  digitalWrite(38, LOW);
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::rewind(uint32_t FrameCnt)
{
  AVIOLDINDEX   idx;
  uint32_t      ind = mFrame.Ind;

  if (!ind)
    return false;

  while (FrameCnt)
  {
    if ((ind -= AVIOLDINDEX_SIZE) < mAviInfo.Idx1Begin)
      return seekFirst();

    if (!readFidx(&idx, ind, AVIOLDINDEX_SIZE))
      return false;

    if (idx.dwChunkId == FOURCC(mAviInfo.VideoFcc))
    {
      mFrame.Nbr--;
      mFrame.Ind = ind;
      mFrame.Offset = idx.dwOffset + mAviInfo.MoviBegin + 4;
      mFrame.Length = idx.dwSize;
      FrameCnt--;
    }
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::forward(uint32_t FrameCnt)
{
  AVIOLDINDEX   idx;
  uint32_t      ind = mFrame.Ind;

  if (!ind)
    return false;

  while (FrameCnt)
  {
    if ((ind += AVIOLDINDEX_SIZE) >= mAviInfo.Idx1End)
      return seekLast();

    if (!readFidx(&idx, ind, AVIOLDINDEX_SIZE))
      return false;

    if (idx.dwChunkId == FOURCC(mAviInfo.VideoFcc))
    {
      mFrame.Nbr++;
      mFrame.Ind = ind;
      mFrame.Offset = idx.dwOffset + mAviInfo.MoviBegin + 4;
      mFrame.Length = idx.dwSize;
      FrameCnt--;
    }
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::seek(uint32_t FrameNbr)
{
  int32_t   rel;

  if ((FrameNbr == 0 || mFrame.Ind == 0) && !seekFirst())
    return false;

  if (FrameNbr > mAviInfo.FrameCount)
    FrameNbr = mAviInfo.FrameCount;

  return ((rel = FrameNbr - mFrame.Nbr) < 0) ? rewind(-rel) : forward(rel);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::seekFirst(void)
{
  mFrame.Nbr = 0;
  mFrame.Ind = mAviInfo.Idx1Begin - AVIOLDINDEX_SIZE;
  return forward(1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::seekLast(void)
{
  mFrame.Nbr = mAviInfo.FrameCount + 1;
  mFrame.Ind = mAviInfo.Idx1End;
  return rewind(1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::readInfo(uint32_t BlkEnd, uint16_t TabCnt, uint32_t Limit)
{
  char        str[] = "????";
#if (VERBOSE > 1)
  char        tab[] = "\t\t\t\t\t\t\t\t\t\t";
  const char  begin[] = " (", end[] = "%s\t       )\n";
#endif
  uint32_t    size, limit = Limit;
  struct
  {
    uint32_t  fcc;
    uint32_t  cb;
  } chunk;

  if (BlkEnd > mFileSize)
    return false;

#if (VERBOSE > 1)
  if (TabCnt < sizeof(tab))
    tab[TabCnt] = 0;
#endif

  for (; mFileInd < BlkEnd; mFileInd += size)
  {
    if (!readFile(&chunk, mFileInd, 8))
      return false;

    mFileInd += 8;
    size = chunk.cb;

  #if (VERBOSE > 1)
    FOURCC(str) = chunk.fcc;
    printAddr(mFileInd - 8);
    printFmt("%s'%s'", tab, str);
  #endif

    if (chunk.fcc == FOURCC("LIST"))
    {
      if (!readFile(str, mFileInd, 4))
        return false;

    #if (VERBOSE > 1)
      printFmt(begin);
      printFmt("'%s'", str);
      printSize(size);
    #endif

      mFileInd += 4;

      if (FOURCC(str) == FOURCC("movi"))
      {
        mAviInfo.MoviBegin = mFileInd;
        mAviInfo.MoviEnd = mFileInd + size - 4;
      }

      if (readInfo(mFileInd + size - 4, TabCnt + 1, (FOURCC(str) == FOURCC("movi")) ? Limit : (uint32_t)-1) == false)
        return false;

    #if (VERBOSE > 1)
      printFmt(end, tab);
    #endif
      size = 0;
    }
    else if (chunk.fcc == FOURCC("avih") && size == AVIMAINHEADER_SIZE)
    {
      AVIMAINHEADER   avih;

      if (!readFile(&avih, mFileInd, AVIMAINHEADER_SIZE))
        return false;

      mAviInfo.Width = avih.dwWidth;
      mAviInfo.Height = avih.dwHeight;
      mAviInfo.FrameCount = avih.dwTotalFrames;

    #if (VERBOSE > 1)
      printFmt(begin);
      printSize(size);
      printFmt("%s\t\tdwMicroSecPerFrame = %u\n", tab, avih.dwMicroSecPerFrame);
      printFmt("%s\t\tdwMaxBytesPerSec = %u\n", tab, avih.dwMaxBytesPerSec);
      printFmt("%s\t\tdwPaddingGranularity = %u\n", tab, avih.dwPaddingGranularity);
      printFmt("%s\t\tdwFlags = 0x%x%s", tab, avih.dwFlags, avih.dwFlags & AVIF_HASINDEX ? " (has index)\n" : "/n");
      printFmt("%s\t\tdwTotalFrames = %u\n", tab, avih.dwTotalFrames);
      printFmt("%s\t\tdwInitialFrames = %u\n", tab, avih.dwInitialFrames);
      printFmt("%s\t\tdwStreams = %u\n", tab, avih.dwStreams);
      printFmt("%s\t\tdwSuggestedBufferSize = %u\n", tab, avih.dwSuggestedBufferSize);
      printFmt("%s\t\tdwWidth = %u\n", tab, avih.dwWidth);
      printFmt("%s\t\tdwHeight = %u\n", tab, avih.dwHeight);
      printFmt(end, tab);
    #endif
    }
    else if (chunk.fcc == FOURCC("strh") && size == AVISTREAMHEADER_SIZE)
    {
      AVISTREAMHEADER   strh;

      if (!readFile(&strh, mFileInd, AVISTREAMHEADER_SIZE))
        return false;

      if (strh.fccType == FOURCC("auds"))
      {
        mAviInfo.AudioBufSize = strh.dwSuggestedBufferSize;
        mAviInfo.SampleRate = strh.dwRate / strh.dwScale;
        mAviInfo.SampleCount = strh.dwLength;

        if (!FOURCC(mAviInfo.AudioFcc))
          snprintf(mAviInfo.AudioFcc, sizeof(mAviInfo.AudioFcc), "%02uwb", mAviInfo.Stream & 0x0F);
      }
      else if (strh.fccType == FOURCC("vids") && strh.fccHandler == FOURCC("MJPG"))
      {
        mAviInfo.VideoBufSize = strh.dwSuggestedBufferSize;
        mAviInfo.FrameRate = (strh.dwScale == 0) ? 0 : ((float)strh.dwRate * 1000.0) / (float)strh.dwScale;
        mAviInfo.VideoLength = (mAviInfo.FrameRate == 0) ? 0 : ((float)mAviInfo.FrameCount * 1000.0) / mAviInfo.FrameRate;

        if (!FOURCC(mAviInfo.VideoFcc))
          snprintf(mAviInfo.VideoFcc, sizeof(mAviInfo.VideoFcc), "%02udc", mAviInfo.Stream & 0x0F);
      }

      mAviInfo.Stream++;

    #if (VERBOSE > 1)
      printFmt(begin);
      printSize(size);
      FOURCC(str) = strh.fccType;
      printFmt("%s\t\tfccType = \"%s\"\n", tab, str);
      FOURCC(str) = strh.fccHandler;
      printFmt("%s\t\tfccHandler = \"%s\"\n", tab, str);
      printFmt("%s\t\tdwFlags = 0x%x\n", tab, strh.dwFlags);
      printFmt("%s\t\twPriority = %u\n", tab, strh.wPriority);
      printFmt("%s\t\twLanguage = %u\n", tab, strh.wLanguage);
      printFmt("%s\t\tdwInitialFrames = %u\n", tab, strh.dwInitialFrames);
      printFmt("%s\t\tdwScale = %u\n", tab, strh.dwScale);
      printFmt("%s\t\tdwRate = %u\n", tab, strh.dwRate);
      printFmt("%s\t\tdwStart = %u\n", tab, strh.dwStart);
      printFmt("%s\t\tdwLength = %u\n", tab, strh.dwLength);
      printFmt("%s\t\tdwSuggestedBufferSize = %u\n", tab, strh.dwSuggestedBufferSize);
      printFmt("%s\t\tdwQuality = %d\n", tab, strh.dwQuality);
      printFmt("%s\t\tdwSampleSize = %u\n", tab, strh.dwSampleSize);
      printFmt("%s\t\trcFrame.left = %d\n", tab, strh.rcFrame.left);
      printFmt("%s\t\trcFrame.top = %d\n", tab, strh.rcFrame.top);
      printFmt("%s\t\trcFrame.right = %d\n", tab, strh.rcFrame.right);
      printFmt("%s\t\trcFrame.bottom = %d\n", tab, strh.rcFrame.bottom);
      printFmt(end, tab);
    #endif
    }
    else if (chunk.fcc == FOURCC("strf"))
    {
      if (size == BITMAPINFOHEADER_SIZE)
      {
        BITMAPINFOHEADER  strf;

        if (!readFile(&strf, mFileInd, BITMAPINFOHEADER_SIZE))
          return false;

      #if (VERBOSE > 1)
        printFmt(begin);
        printSize(size);
        printFmt("%s\t\tbiSize = %u\n", tab, strf.biSize);
        printFmt("%s\t\tbiWidth = %d\n", tab, strf.biWidth);
        printFmt("%s\t\tbiHeight = %d\n", tab, strf.biHeight);
        printFmt("%s\t\tbiPlanes = %u\n", tab, strf.biPlanes);
        printFmt("%s\t\tbiBitCount = %u\n", tab, strf.biBitCount);
        printFmt("%s\t\tbiCompression = %u\n", tab, strf.biCompression);
        printFmt("%s\t\tbiSizeImage = %u\n", tab, strf.biSizeImage);
        printFmt("%s\t\tbiXPelsPerMeter = %d\n", tab, strf.biXPelsPerMeter);
        printFmt("%s\t\tbiYPelsPerMeter = %d\n", tab, strf.biYPelsPerMeter);
        printFmt("%s\t\tbiClrUsed = %d\n", tab, strf.biClrUsed);
        printFmt("%s\t\tbiClrImportant = %d\n", tab, strf.biClrImportant);
        printFmt(end, tab);
      #endif
      }
      else
      {
        WAVEFORMATEX  strf;
        int16_t       strfSize;

        strf.cbSize = 0;
        strfSize = (size > WAVEFORMATEX_SIZE) ? WAVEFORMATEX_SIZE : size;

        if (!readFile(&strf, mFileInd, strfSize))
          return false;

        if (strfSize == WAVEFORMAT_SIZE)
          strf.wBitsPerSample = 8 * strf.nAvgBytesPerSec / strf.nSamplesPerSec / strf.nChannels;

        switch (strf.wFormatTag)
        {
          case 0x01: mAviInfo.AudioFmt = WAVE_FORMAT_PCM; break;
          case 0x07: mAviInfo.AudioFmt = WAVE_FORMAT_MULAW; break;
          case 0x11: mAviInfo.AudioFmt = WAVE_FORMAT_IMA_ADPCM; break;
          default: mAviInfo.AudioFmt = WAVE_FORMAT_UNKNOWN;
        }

        mAviInfo.AudioChan = strf.nChannels;
        mAviInfo.SampleDepth = strf.wBitsPerSample;

      #if (VERBOSE > 1)
        printFmt(begin);
        printSize(size);
        printFmt("%s\t\twFormatTag = %u (%s)\n", tab, strf.wFormatTag, sWaveFormat[mAviInfo.AudioFmt]);
        printFmt("%s\t\tnChannels = %u\n", tab, strf.nChannels);
        printFmt("%s\t\tnSamplesPerSec = %u\n", tab, strf.nSamplesPerSec);
        printFmt("%s\t\tnAvgBytesPerSec = %u\n", tab, strf.nAvgBytesPerSec);
        printFmt("%s\t\tnBlockAlign = %u\n", tab, strf.nBlockAlign);
        printFmt("%s\t\twBitsPerSample = %u\n", tab, strf.wBitsPerSample);
        printFmt(end, tab);
      #endif
      }
    }
    else if (chunk.fcc == FOURCC("idx1"))
    {
      AVIOLDINDEX   idx;

      if ((size & 3) != 0)
        return false;

      mAviInfo.Idx1Begin = mFileInd;
      mAviInfo.Idx1End = mFileInd + size;

    #if (VERBOSE > 1)
      printFmt(begin);
      printSize(size);

      uint32_t frame = 0;
    #endif
      if (Limit < (limit = size / 4))
        limit = Limit;

      for (; limit > 0 && mFileInd < BlkEnd; mFileInd += AVIOLDINDEX_SIZE, limit--)
      {
        if (!readFile(&idx, mFileInd, AVIOLDINDEX_SIZE))
          return false;

      #if (VERBOSE > 1)
        printAddr(mFileInd);
        FOURCC(str) = idx.dwChunkId;
        printFmt("%s\t'%s' +%08X%7u", tab, str, idx.dwOffset, idx.dwSize);

        if (idx.dwChunkId == FOURCC(mAviInfo.VideoFcc))
          printFmt("  #%u\n", ++frame);
        else
          printFmt("\n");
      #endif
      }

    #if (VERBOSE > 1)
      if (limit == 0)
      {
        printAddr(mFileInd);
        printFmt("%s\t ....", tab);
        printSize(BlkEnd - mFileInd);
      }

      printFmt(end, tab);
    #endif
      mFileInd = BlkEnd;
      size = 0;
    }
    else if (chunk.fcc == FOURCC("ISFT"))
    {
    #if (VERBOSE > 1)
      char    package[128];

      if (size > sizeof(package) - 1)
        size = sizeof(package) - 1;

      if (!readFile(package, mFileInd, size))
        return false;

      package[size + 1] = 0;
      printFmt(" %s\n", package);
    #endif
      mFileInd = BlkEnd;
      size = 0;
    }
    else
    {
    #if (VERBOSE > 1)
      printSize(size);
    #endif
      if (!--limit)
      {
      #if (VERBOSE > 1)
        printAddr(mFileInd);
        printFmt("%s ....", tab);
        printSize(BlkEnd - mFileInd);
      #endif
        mFileInd = BlkEnd;
        size = 0;
      }
    }

    if (size & 1)
      size++;
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::readFile(void *Data, uint32_t Pos, uint32_t Count)
{
  return (mFile.seek(Pos) != 0 && mFile.read(Data, Count) == Count);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::readFidx(void *Data, uint32_t Pos, uint32_t Count)
{
  return (mFidx.seek(Pos) != 0 && mFidx.read(Data, Count) == Count);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::printAddr(uint32_t Addr)
{
#if (VERBOSE > 1)
  printFmt("%08X ", Addr);
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::printSize(uint32_t Size)
{
#if (VERBOSE > 1)
  printFmt(" %u\n", Size);
#endif
}

