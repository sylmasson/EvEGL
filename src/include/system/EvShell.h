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

#ifndef     _EV_SHELL_H_
#define     _EV_SHELL_H_

#define     DL_STACK_SIZE   4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum CmdId : uint8_t
{
  CD, DUMP, LIST, EDITOR, RADIX, TRACE, THEME, ROTATE, CALIB, INFO, FONT, ROMFONT, LISTSD, CLRCACHE, HELP
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

    void          Input(const String &Str);
    void          Input(const char *Str);
    void          Input(const char C);
    void          ShowPrompt(void);

  protected:
    void          dumpEveMemory(EvDisplay *Disp, uint32_t Addr, int32_t Count, uint16_t Radix);
    void          displayObjectRamG(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL, int32_t Count);
    void          displayListCommand(EvDisplay *Disp, uint32_t Addr, uint32_t RamDL);
    void          displayMallocBlock(EvDisplay *Disp);
    void          displayObjectInformation(EvDisplay *Disp, EvObj *Obj, int16_t SpacesCnt, bool Detailed);
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
