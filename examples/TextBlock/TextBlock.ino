//
// EvEGL - TextBlock Demo
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

#include		<EvEGL.h>

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

EvDisplay		*Disp;
EvSelector  *AlignX;
EvLabel 		*Title, *Status;
EvLabel     *FontLab, *SpLab, *PadLab;
EvNumInt    *FontSize, *Spacing, *Padding;
EvCheckBox  *Wrap;
EvTextBlock *Page;

EvShell     Shell(Serial);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char  MIT_License[] =
"The MIT License (MIT)\n\n\
Copyright (c) 2024 Sylvain Masson\n\n\
Permission is hereby granted, free of charge, to any person obtaining a copy of this \
software and associated documentation files (the \"Software\"), to deal in the Software \
without restriction, including without limitation the rights to use, copy, modify, merge, \
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons \
to whom the Software is furnished to do so, subject to the following conditions:\n\n\
The above copyright notice and this permission notice shall be included in all \
copies or substantial portions of the Software.\n\n\
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING \
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND \
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, \
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const uint8_t  sAlignIconsX_160x40_data[85] =
{
  0x78, 0x01, 0xED, 0xD6, 0x31, 0x0A, 0xC0, 0x30, 0x0C, 0x04, 0xC1, 0xEC, 0x0B, 0xF2, 0xFF, 0xD7,
  0x3A, 0x7D, 0x02, 0xC2, 0x07, 0x01, 0x61, 0xB4, 0x53, 0x2F, 0x46, 0xC5, 0x15, 0x5E, 0xF7, 0x5A,
  0x97, 0x24, 0x4D, 0x46, 0xC9, 0xEE, 0xF4, 0x4E, 0xEA, 0x44, 0x32, 0x4E, 0x92, 0xB8, 0xAE, 0x29,
  0x6D, 0x3E, 0xF2, 0x46, 0x12, 0xCF, 0xB8, 0x4F, 0xEA, 0xC4, 0xE6, 0x4E, 0xB1, 0x3B, 0xB2, 0x93,
  0x3A, 0x91, 0x8C, 0x93, 0x24, 0xF6, 0xFF, 0xF2, 0x41, 0x12, 0xFF, 0x73, 0x9F, 0x24, 0x0D, 0xF4,
  0x00, 0x9D, 0x8E, 0x07, 0xE7
};

static const EvBmp    sAlignIconsX_160x40 = {ZIP_DATA, PALETTED4444, 160, 40, 4, 6400, 85, sAlignIconsX_160x40_data, "AlignIconsX_160x40"};

const EvBmp   *AlignIconsX_160x40 = &sAlignIconsX_160x40;

extern const EvFont  *Segoeprb_24_L4;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void 				setup(void)
{
  Serial.begin(500000);

#ifdef DISP_1024
  Disp = EvDisplay::Create(1024, 600, "Disp", EvConfig_1024x600_70, CS, RST, 30000000, &SPI);
#else
  Disp = EvDisplay::Create(800, 480, "Disp", EvConfig_800x480_50, CS, RST, 30000000, &SPI);
#endif

  Title = EvLabel::Create(10, 5, "EvEGL TextBlock Demo", 27, Disp, "Title");
  Title->SetOnTouch(OnTouchTitle);

  Status = EvLabel::Create(390,  5, 400, 45, Disp, "Status");
  Status->TextAlign(RIGHT_CENTER);
  Status->TextFont(26);

  Page = EvTextBlock::Create(10, 100, 780, 370, Disp, "Page");
  Page->TextLabel(MIT_License);
  Page->TextAlign(LEFT_TOP);
  Page->TextFont(26);

  AlignX = EvSelector::Create(10, 50, 160, 40, Disp, "AlignX");
  AlignX->SetColor(CL_DEFAULT_BG);
  AlignX->SetBmp(AlignIconsX_160x40, 4);

  FontLab = EvLabel::Create(180, 55, "Font:", 26, Disp, "FontLab");
  FontSize = EvNumInt::Create(240, 50, 70, 40, Disp, "FontSize");
  FontSize->SetRange(20, 31);
  FontSize->TextFont(26);
  FontSize->SetValue(26);

  SpLab = EvLabel::Create(320, 55, "Spacing:", 26, Disp, "SpLab");
  Spacing = EvNumInt::Create(420, 50, 0, FontSize, Disp, "Spacing");
  Spacing->SetRange(-5, 25);
  Spacing->SetFormat("%+ld");

  PadLab = EvLabel::Create(500, 55, "Padding:", 26, Disp, "PadLab");
  Padding = EvNumInt::Create(600, 50, 10, Spacing, Disp, "Padding");
  Page->TextPadding(10, 10);

  Wrap = EvCheckBox::Create(690, 55, "Wrap", 26, Disp, "Wrap");

  Disp->SetOnUpdateFPS(OnUpdateFPS);
  AlignX->SetOnChange(OnChangeAlignX);
  FontSize->SetOnChange(OnChangeFont);
  Spacing->SetOnChange(OnChangeSpacing);
  Padding->SetOnChange(OnChangePadding);
  Wrap->SetOnChange(OnChangeWrap);

  Disp->UnloadFont(31);
  Disp->LoadFont(Segoeprb_24_L4, 31);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void 				loop(void)
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

void        OnChangeAlignX(EvSelector *Sender, int32_t Value)
{
  Page->TextAlign(Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeFont(EvNumInt *Sender, int32_t Value)
{
  Page->TextFont(Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeSpacing(EvNumInt *Sender, int32_t Value)
{
  Page->LineSpacing((int)Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangePadding(EvNumInt *Sender, int32_t Value)
{
  Page->TextPadding(Value, Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeWrap(EvCheckBox *Sender, int32_t Value)
{
  Page->WrapText(Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnUpdateFPS(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL)
{
  char      str[80];

  snprintf(str, sizeof(str) - 1, "%u FPS  %4.2f msec/frame  DL=%04u", FrameCount, TimeUsed / (FrameCount * 1000.0), MaxDL);
  Status->TextColor(MaxDL > 6500 ? CL_RED : CL_BLUE);
  Status->TextLabel(str);
}
