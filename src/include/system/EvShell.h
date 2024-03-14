
#ifndef     _EV_SHELL_H_
#define     _EV_SHELL_H_

#define     DL_STACK_SIZE   4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum CmdId : uint8_t
{
  DC, DUMP, LIST, EDITOR, RADIX, TRACE, ROTATE, CALIB, INFO, FONT, ROMFONT, LISTSD, CLRCACHE, HELP
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct EvCmd
{
  const CmdId     id;
  const uint8_t   argMax;
  const char      *shortName;
  const char      *fullName;
  const char      *help;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvShell
{
  struct Vertex
  {
    uint8_t       fmt;
    float         x;
    float         y;
  };

  public:
    EvShell(Stream &SerialMonitor, SDClass *SDCard = nullptr);

    void          Input(const char *Str);
    void          Input(const char C);
    void          ShowPrompt(void);

  protected:
    void          dumpEveMemory(EvDisplay *Disp, uint32_t Addr, int32_t Count, uint16_t Radix);
    void          displayObjectRamG(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL, int32_t Count);
    void          displayListCommand(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL);
    void          displayMallocBlock(EvDisplay *Disp);
    void          displayFontMetrixBlock(EvDisplay *Disp);
    void          displayRomFont(EvDisplay *Disp);
    bool          displayChange(char *Str);
    bool          setTrace(char *Str, uint16_t *Flags);
    bool          setRadix(char *Str);
    void          listDirectory(File Dir, int16_t SpacesCnt = 0);

    uint32_t      mAddr;
    uint32_t      mSize;
    uint8_t       mRadix;
    uint8_t       mActDisp;
    uint8_t       mBufCount;
    char          mBuffer[64];

    int8_t        mSp;
    Vertex        mVt;
    Vertex        mStack[DL_STACK_SIZE];

    SDClass       *mSDCard;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SHELL_H_ */
