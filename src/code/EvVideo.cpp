
#include    <EvGUI.h>

// #define     VERBOSE

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

EvVideo     *EvVideo::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvVideo *)EvObj::TryCreate(new EvVideo(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvVideo" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVideo::EvVideo(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvImage(Left, Top, Width, Height, Disp, Tag, State)
{
  mFrame = nullptr;
  mNextLoad = false;
  mRun = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvVideo::~EvVideo(void)
{
  Unload();
  free(mFrame);

  if (mFile)
    mFile.close();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Open(const char *Filename, SDClass &Dev)
{
  uint32_t  data[3];

  if (mFile)
    mFile.close();

  Unload();
  free(mFrame);
  mFrame = nullptr;

  if (!Dev.mediaPresent() || !(mFile = Dev.open(Filename)))
  {
    #ifdef VERBOSE
      Serial.printf("\nDevice or file %s not found\n", Filename);
    #endif
    return false;
  }

  if ((mFileSize = mFile.size()) > 0 && read(data, 0, sizeof(data)) == true)
    if (data[0] == *(uint32_t *)"RIFF" && data[1] == (mFileSize - 8) && data[2] == *(uint32_t *)"AVI ")
    {
      #ifdef VERBOSE
        Serial.printf("\nRIFF %u AVI\n", data[1]);
      #endif

      mFrameCount = 0;
      mRun = false;
      mIndex = 12;

      return NextFrame();
    }

  #ifdef VERBOSE
    Serial.printf("%s is not valid AVI file\n", Filename);
  #endif

  mFile.close();
  return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::NextFrame(void)
{
  uint8_t   *jpeg;
  uint32_t  size, data[3];
  char      str[] = "????";

  if (!mFile || mFrame != nullptr)
    return false;

  for (; mIndex < mFileSize; mIndex += size)
  {
    if (!read(data, mIndex, 8))
      break;

    mIndex += 8;
    size = data[1];
    memcpy(str, data, 4);

    if (strcmp(str, "LIST") == 0)
    {
      if (!read(&data[2], mIndex, 4))
        break;

      memcpy(str, &data[2], 4);

      #ifdef VERBOSE
        Serial.printf("LIST %u %s\n", size, str);
      #endif

      mIndex += 4;
      size = 0;
    }
    else
    {
      #ifdef VERBOSE
        Serial.printf("%s %u @%lX\n", str, size, index);
      #endif

      if (strcmp(&str[2], "dc") == 0 && (mFrame = (EvBmp *)malloc(sizeof(EvBmp) + size)) != nullptr)
      {
        jpeg = ((uint8_t *)mFrame) + sizeof(EvBmp);

        if (!mFile.seek(mIndex) || mFile.read(jpeg, size) != size || !IsValidJPEG(jpeg, size, mFrame, "VideoFrame"))
          break;

        mFrameCount++;

        #ifdef VERBOSE
          Serial.printf("Frame %u\n", mFrameCount);
        #endif

        if (size & 1)
          size++;

        mIndex += size;
//        Disp->CmdSync();

/*        if (Load(mFrame))
        {
          SetMode(RESIZE_PROPORTIONAL, BILINEAR);
          return true;
        }

        mRun = false;
        break; */
        mNextLoad = true;
        return true;
      }

      if (size & 1)
        size++;
    }
  }

  return (mIndex == mFileSize);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::Play(void)
{
  if (!mFile)
    return false;

  Unload();
  free(mFrame);
  mFrame = nullptr;
  mFrameCount = 0;
  mIndex = 12;
  mRun = true;

  return NextFrame();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvVideo::refreshEvent(void)
{
  if (mRun)
  {
    if (mNextLoad)
      load();
    else
    {
      NextFrame();
      EvImage::refreshEvent();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::load(void)
{
  bool      result = false;

  if (mFrame != nullptr)
  {
    mFrame->Format |= BMP_MALLOC;

    if (Load(mFrame, OPT_MEDIAFIFO) == nullptr)
      free(mFrame);
    else
    {
      SetMode(RESIZE_PROPORTIONAL, BILINEAR);
      mNextLoad = false;
      result = true;
    }

    mFrame = nullptr;
  }

  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvVideo::read(uint32_t *Data, uint32_t Pos, uint32_t Count)
{
  return (mFile.seek(Pos) != 0 && mFile.read(Data, Count) == Count);
}
