//
// EvEGL - Terminal Demo
//

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

#define     DISP_1024

// Change CS and RST according to your configuration
#ifdef DISP_1024
  #define   CS          16
  #define   RST         14
#else
  #define   CS          9
  #define   RST         14
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay   *Disp;
EvLabel     *Title;
EvLabel     *Status;
EvButton    *Clear;
EvTextBox   *TermIn;
EvTerminal  *TermOut;
String      LastInputCmd;

EvShell     Shell(Serial, &SD);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        setup(void)
{
  Serial.begin(500000);
  SD.begin(BUILTIN_SDCARD);
  SystemColor.SelectTheme(1);

#ifdef DISP_1024
  Disp = EvDisplay::Create(1024, 600, "Disp", EvConfig_1024x600_70, CS, RST, 30000000, &SPI);
  Disp->Kbd->SetKeyboard(&Keyboard1024x276, &Keyboard600x252);
#else
  Disp = EvDisplay::Create(800, 480, "Disp", EvConfig_800x480_50, CS, RST, 30000000, &SPI);
  Disp->Kbd->SetKeyboard(&Keyboard800x220, &Keyboard480x230);
#endif

  Title = EvLabel::Create(10, 5, "EvEGL Terminal Demo", 27, Disp, "Title");
  Title->SetOnTouch(OnTouchTitle);

  Status = EvLabel::Create(390,  5, 400, 45, Disp, "Status");
  Status->TextAlign(RIGHT_CENTER);
  Status->TextFont(26);

  TermIn = EvTextBox::Create(10, 50, 700, 45, Disp, "TermIn");
  TermIn->SetOnKbdFocus(OnInputSetFocus, OnInputLostFocus);
  TermIn->SetOnReturn(OnInputReturn);
  OnInputLostFocus(TermIn);

  Clear = EvButton::Create(715, 50, 75, 47, Disp, "Clear");
  Clear->TextLabel("Clear");
  Clear->TextFont(26);
  Clear->SetOnChange(OnClearBuffer);

  TermOut = EvTerminal::Create(10, 100, 780, 370, Disp, "TermOut");
  TermOut->SetBufferSize(32 * 1024);
  TermOut->SetOnTouch(OnTouchTermOut);
  EvOut = TermOut;  // Set Shell output redirection to TermOut

  Disp->SetOnUpdateFPS(OnUpdateFPS);
  Shell.Input("\nhelp\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        loop(void)
{
  if (Disp->Update())
  {
    // Synchronous process here - Every 16.67 msec by default
  }

  while (EvIn->available())
    Shell.Input(EvIn->read());

  // Asynchronous process here
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnTouchTitle(EvLabel *Sender, const EvTouchEvent *Touch)
{
  if (Touch->event == TOUCH_DOUBLE)
    SystemColor.SwitchTheme();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnInputSetFocus(EvTextBox *Sender)
{
  Sender->Disp->Kbd->SetLayout(LAYOUT_ALPHA);
  Sender->TextLabel(LastInputCmd);
  Sender->TextColor(CL_DEFAULT_TEXT);
  Sender->SelectAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnInputLostFocus(EvTextBox *Sender)
{
  Sender->TextLabel("Input command");
  Sender->TextColor(CL_GRAY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnInputReturn(EvTextBox *Sender, const String &Str)
{
  Shell.Input(Str + '\n');
  LastInputCmd = Str;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnClearBuffer(EvButton *Sender, int32_t Value)
{
  if (Value == false)
  {
    TermIn->LostKbdFocus();
    TermOut->ClearBuffer();
    Shell.Input('\n');
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnTouchTermOut(EvTextBlock *Sender, const EvTouchEvent *Touch)
{
  if (Touch->event == TOUCH_START)
    TermIn->LostKbdFocus();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnUpdateFPS(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL)
{
  char      str[80];

  sprintf(str, "%u FPS  %4.2f msec/frame  DL=%04u", FrameCount, TimeUsed / (FrameCount * 1000.0), MaxDL);
  Status->TextColor(MaxDL > 6500 ? CL_RED : CL_BLUE);
  Status->TextLabel(str);
}
