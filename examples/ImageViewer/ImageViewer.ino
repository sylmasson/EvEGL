//
// EvEGL - ImageViewer Demo
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
EvLabel 		*Title, *Status;
EvLabel     *ScaleLab, *AngleLab, *RGB;
EvNumLabel  *ScaleVal;
EvNumInt    *AngleVal;
EvCheckBox  *Bilinear;
EvSlider    *Red, *Green, *Blue;
EvViewer    *Viewer;

EvShell     Shell(Serial, &SD);

extern const EvBmp   *Canard_brachu_3483_720x480;
extern const EvBmp   *Cardinal_a_poitrine_rose_4459_720x480;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void 				setup(void)
{
  Serial.begin(500000);
  SD.begin(BUILTIN_SDCARD);
  SystemColor.SelectTheme(1);

#ifdef DISP_1024
  Disp = EvDisplay::Create(1066, 600, "Disp", EvConfig_1066x600_70, CS, RST, 30000000, &SPI);
#else
  Disp = EvDisplay::Create(800, 480, "Disp", EvConfig_800x480_50, CS, RST, 30000000, &SPI);
#endif

  Title = EvLabel::Create(10, 5, "EvEGL ImageViewer Demo", 27, Disp, "Title");
  Title->SetOnTouch(OnTouchTitle);

  Status = EvLabel::Create(390,  5, 400, 45, Disp, "Status");
  Status->TextAlign(RIGHT_CENTER);
  Status->TextFont(26);

  Viewer = EvViewer::Create(10, 50, 630, 420, Disp, "Viewer");

  ScaleLab = EvLabel::Create(660, 50, 120, 40, Disp, "ScaleLab");
  ScaleLab->TextLabel("Scale");
  ScaleLab->TextAlign(CENTER);
  ScaleLab->TextFont(26);

  ScaleVal = EvNumLabel::Create(660, 90, 120, 40, Disp, "ScaleVal");
  ScaleVal->BgColor(CL_DEFAULT_BG);
  ScaleVal->SetFormat("%.2f");
  ScaleVal->TextFont(27);
  ScaleVal->SetValue(1.0);

  AngleLab = EvLabel::Create(660, 135, "Angle", ScaleLab, Disp, "AngleLab");

  AngleVal = EvNumInt::Create(660, 175, 120, 40, Disp, "AngleVal");
  AngleVal->SetRange(-179, 180);
  AngleVal->SetTurnOver(true);
  AngleVal->TextFont(27);

  Bilinear = EvCheckBox::Create(660, 240, "Bilinear", 26, Disp, "Bilinear");
  Viewer->Image->SetMode(SCALE_TO_FIT | RESIZE_ON_LOAD, BILINEAR);

  RGB = EvLabel::Create(660, 295, "FFFFFF", ScaleVal, Disp, "RGB");
  RGB->BgColor(CL_DEFAULT_BG);
  RGB->TextAlign(CENTER);

  Red = EvSlider::Create(660, 350, 38, 120, Disp, "Red");
  Red->SetColor(CL_RED, CL_GRAY);
  Red->BdShape(RATIO_CORNERS);
  Red->SetRange(192, 255);
  Red->SetValue(255);

  Green = EvSlider::Create(701, 350, 255, Red, Disp, "Green");
  Green->SetColor(CL_GREEN, CL_GRAY);

  Blue = EvSlider::Create(742, 350, 255, Red, Disp, "Blue");
  Blue->SetColor(CL_BLUE, CL_GRAY);

  Disp->SetOnUpdateFPS(OnUpdateFPS);
  Bilinear->SetOnChange(OnChangeBilinear);
  Red->SetOnChange(OnChangeColorRGB);
  Green->SetOnChange(OnChangeColorRGB);
  Blue->SetOnChange(OnChangeColorRGB);
  AngleVal->SetOnChange(OnChangeAngleVal);
  Viewer->SetOnScaling(OnScalingImage);
  Viewer->SetOnRotate(OnRotateImage);

  // Viewer->Load("Cardinal_a_poitrine_rose_4459_720x480_ASTC_6x6.astc", SD);
  // Viewer->Load("Cardinal_a_poitrine_rose_4459_720x480.jpg", SD);

  // Viewer->Load("Canard_brachu_3483_1026x684_ASTC_6x6.astc", SD);
  // Viewer->Load("Canard_brachu_3483_720x480_ASTC_5x5.astc", SD);

  // Viewer->Load(Cardinal_a_poitrine_rose_4459_720x480);
  Viewer->Load(Canard_brachu_3483_720x480);
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

void        OnChangeAngleVal(EvNumInt *Sender, int32_t Angle)
{
  Viewer->SetAngle(Angle);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnScalingImage(EvViewer *Sender, float Scale)
{
  ScaleVal->SetValue(Scale);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnRotateImage(EvViewer *Sender, float Angle)
{
  AngleVal->SetValue(Angle);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeBilinear(EvCheckBox *Sender, int32_t Value)
{
  Viewer->Image->SetMode(SCALE_TO_FIT | RESIZE_ON_LOAD, Value == true ? BILINEAR : NEAREST);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeColorRGB(EvSlider *Sender, int32_t Value)
{
  char      str[80];
  uint8_t   red, green, blue;

  Viewer->Image->SetColor(red = Red->Value(), green = Green->Value(), blue = Blue->Value());
  snprintf(str, sizeof(str) - 1, "%02X%02X%02X", red, green, blue);
  RGB->TextLabel(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnUpdateFPS(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL)
{
  char      str[80];

  snprintf(str, sizeof(str) - 1, "%u FPS  %4.2f msec/frame  DL=%04u", FrameCount, TimeUsed / (FrameCount * 1000.0), MaxDL);
  Status->TextColor(MaxDL > 6500 ? CL_RED : CL_BLUE);
  Status->TextLabel(str);
}
