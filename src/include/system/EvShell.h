
#ifndef     _EV_SHELL_H_
#define     _EV_SHELL_H_

#define     DL_STACK_SIZE   8

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum CmdId : uint8_t
{
  CD, DUMP, LIST, EDITOR, RADIX, TRACE, ROTATE, CALIB, INFO, FONT, ROMFONT, CLRCACHE, HELP
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
    EvShell(void);

    void          ShowPrompt(void);
    void          Input(const char C);

  protected:
    void          DumpEveMemory(EvDisplay *Disp, uint32_t Addr, int32_t Count, uint16_t Radix);
    void          DisplayObjectRamG(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL, int32_t Count);
    void          DisplayListCommand(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL);
    void          DisplayMallocBlock(EvDisplay *Disp);
    void          DisplayFontMetrixBlock(EvDisplay *Disp);
    void          DisplayRomFont(EvDisplay *Disp);
    bool          ChangeDisplay(char *Str);
    bool          SetTrace(char *Str, uint16_t *Flags);
    bool          SetRadix(char *Str);

    uint32_t      mAddr;
    uint32_t      mSize;
    uint8_t       mRadix;
    uint8_t       mActDisp;
    uint8_t       mBufCount;
    char          mBuffer[64];

    int8_t        mSp;
    Vertex        mVt;
    Vertex        mStack[DL_STACK_SIZE];
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SHELL_H_ */
