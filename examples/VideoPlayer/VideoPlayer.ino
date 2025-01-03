//
// EvEGL - VideoPlayer Demo
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

#define     FREE_CPU    0     // 0 = none, 1 = yes, 2 = to adjust 100% idle

#if (FREE_CPU != 0)
#include    <TeensyThreads.h>
uint        FreeCpu = 0;
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayQueue           AudioQueue;     //xy=547,225
AudioOutputI2S           AudioOut;       //xy=795,261
AudioAnalyzeFFT1024      AudioFFT;       //xy=796,174
AudioConnection          patchCord1(AudioQueue, AudioFFT);
AudioConnection          patchCord2(AudioQueue, 0, AudioOut, 0);
AudioConnection          patchCord3(AudioQueue, 0, AudioOut, 1);
AudioControlSGTL5000     AudioCtrl;      //xy=672,352
// GUItool: end automatically generated code

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDisplay		*Disp;
EvLabel 		*Title;
EvLabel     *Status;
EvPlayer    *Player;
EvSpectrum  *Spectrum;

EvShell     Shell(Serial, &SD);

uint8_t     fftVal[128];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void 				setup(void)
{
  Serial.begin(500000);
  SD.begin(BUILTIN_SDCARD);
  SystemColor.SelectTheme(1);

  AudioMemory(100);
  AudioCtrl.enable();
  AudioCtrl.volume(0);

  EvDisplay::SetFrameRate(59.94); // change default frame rate based on video (2 x 27.97Hz)

#ifdef DISP_1024
  Disp = EvDisplay::Create(1066, 600, "Disp", EvConfig_1066x600_70, CS, RST, 30000000, &SPI);
#else
  Disp = EvDisplay::Create(800, 480, "Disp", EvConfig_800x480_50, CS, RST, 30000000, &SPI);
#endif

  Title = EvLabel::Create(10, 0, "EvEGL VideoPlayer Demo", 27, Disp, "Title");
  Title->SetOnTouch(OnTouchTitle);

  Status = EvLabel::Create(390, 0, 400, 40, Disp, "Status");
  Status->TextAlign(RIGHT_CENTER);
  Status->TextFont(26);

  Player = EvPlayer::Create(10, 40, 780, 440, Disp, "Player");
  Player->Open("Big_buck_bunny_640x360.avi", SD);
  Player->Volume->SetOnChange(OnChangeVolume);
  Player->Video->SetOnLoadAudio(OnLoadAudio);
  Player->TimeLapse->SetOnTouch(nullptr);
  Player->Volume->SetValue(50);

  Spectrum = EvSpectrum::Create(30, 90, 260, 150, Disp, "Spectrum");
  Spectrum->SetDataSize(sizeof(fftVal), 255);
  Spectrum->SetOnTouch(OnTouchSpectrum);
  memset(fftVal, 255, sizeof(fftVal));

  Disp->SetOnUpdateFPS(OnUpdateFPS);

#if (FREE_CPU != 0)
  threads.setMicroTimer();
  threads.addThread(IdleThread);
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void 				loop(void)
{
#if (FREE_CPU != 2)
  if (Disp->Update())
  {
    // Synchronous process here - Every 16.683ms here for 29.97Hz video frame rate compatibility

    if (AudioFFT.available())
    {
      uint  i, j, val;

      for (i = j = 0; i < sizeof(fftVal); i++, j += 2)
      {
        if ((val = log10((AudioFFT.read(j, j+1) / 2)) * -60.0) > 255)
          val = 255;
        fftVal[i] = ((fftVal[i] * 192) + (val * 64)) / 256;
      }

      Spectrum->SetData(0, fftVal, sizeof(fftVal));
    }
  }

  while (EvIn->available())
    Shell.Input(EvIn->read());
#endif

  // Asynchronous process here

#if (FREE_CPU != 0)
  threads.yield();
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnTouchTitle(EvLabel *Sender, const EvTouchEvent *Touch)
{
  if (Touch->event == TOUCH_DOUBLE)
    SystemColor.SwitchTheme();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnLoadAudio(EvVideo *Sender, void *Data, uint16_t DataSize)
{
  if (Sender->AviInfo.AudioFmt == WAVE_FORMAT_PCM && Sender->AviInfo.SampleDepth == 16 && Sender->AviInfo.SampleRate == 44100)
    AudioQueue.play((int16_t *)Data, DataSize / 2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnTouchSpectrum(EvSpectrum *Sender, const EvTouchEvent *Touch)
{
  switch (Touch->event)
  {
    case TOUCH_START: Sender->BdColor(CL_RED); break;
    case TOUCH_MOVE: Sender->MoveRel(Touch->move.x, Touch->move.y); break;
    case TOUCH_CANCEL:
    case TOUCH_END: Sender->BdColor(CL_WHITE); break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnChangeVolume(EvSlider *Sender, int32_t Value)
{
  AudioCtrl.volume((float)Value * 0.008);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        OnUpdateFPS(EvDisplay *Sender, uint32_t TimeUsed, uint16_t FrameCount, uint16_t MaxDL)
{
  char      str[80];

#if (FREE_CPU == 0)
  snprintf(str, sizeof(str) - 1, "%u FPS  %4.2f msec/frame  DL=%04u", FrameCount, TimeUsed / (FrameCount * 1000.0), MaxDL);
#else
  snprintf(str, sizeof(str) - 1, "%u FPS  Free CPU=%u%%  DL=%04u", FrameCount, FreeCpu, MaxDL);
#endif
  Status->TextColor(MaxDL > 6500 ? CL_RED : CL_BLUE);
  Status->TextLabel(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Thanks to "mikaeltulldahl" for this suggestion to use TeensyThread to evaluate
 * the free CPU time. See "https://github.com/ftrias/TeensyThreads/issues/5"
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (FREE_CPU != 0)

void        IdleThread(void)
{
  static volatile uint32_t idleCounter = 0;
  static int32_t millisSinceCPULoadUpdate = millis();

  while (1) 
  {
    int32_t dt = (int32_t)millis() - millisSinceCPULoadUpdate;

    if (dt >= 1000)
    {
      FreeCpu = idleCounter / 1000;
      #if (FREE_CPU == 2)
        Serial.printf("Free CPU: %u%%\n", FreeCpu);
      #endif
      millisSinceCPULoadUpdate += 1000; // dt;
      idleCounter = 0;
    }

    // do some pointless heavy lifting to burn cpu cycles;
    for (volatile int i = 0; i < 161; i++)  // adjust loop time to 100% idle
    {
      volatile float a = 123;
      volatile float b = sqrtf(a);
      (void)b;  // to suppress [-Wunused-variable] warning
    }

    idleCounter++;
    threads.yield();
  }
}

#endif