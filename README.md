# EvEGL Project

EvEGL is a new graphics library for Bridgetek's EVE graphics controller. It supports EVE2 / EVE3 / EVE4 generations.

This library is designed for embedded applications for the Arduino IDE. The project was entirely developed around the PJRC's Teensy boards. Preliminary tests were done with development boards using the popular ESP32.

## Presentation and History

My name is Sylvain Masson. I have a background as an electronics technician. I have always worked in R&D in real-time embedded applications. I have experience with several microcontrollers and DSP. The C language and assembler have been my main programming languages.

Now retired since 2016, I had fun with the Arduino environment to learn C++. I discovered a very powerful object-oriented language with the class inheritance and polymorphism. I am now a fan of C++.

It was after studying graphic displays using a Brigetek"s EVE graphics controller with the Gameduino library (thanks to James Bowman) that I decided to start the EvEGL project. I wanted to develop a new library that would use the power of the EVE chips, but without necessarily having to know how they work. **This way, a programmer could create graphical user interfaces (GUI) more faster.**

Now I would like to share my work for the benefit of the Open Source community.

## Hardware (tested)

#### Development boards

 - Teensy boards 4.0 and 4.1 [(PJRC)](https://www.pjrc.com/teensy/)
 - ESP32 boards (preliminary)

#### LCD Display
 - Riverdi, 7", 1024x600px, EVE4, [(RVT70HSBNWC00-B)](https://riverdi.com/product/eve4-intelligent-display-rvt70hsbnwc00-b-7-inch-projected-capacitive-touch-panel-optical-bonding-uxtouch)
 - Newhaven Display, 5", 800x480px, EVE2, [(NHD-5.0-800480FT-CTXL-CTP)](https://newhavendisplay.com/5-0-inch-premium-eve-capacitive-tft/)

## Features
 - Dynamic allocation of EVE chip memory (RAM_G)
 - MultiTouch support with event callback function
 - Built-in Shell command via SerialMonitor for debugging
 - Built-in Editor to modify and/or resize a visual object live
 - Optimized display list by tracking last send to EVE controller
 - Cache memory implemented for each visual object's display list
 - Multiple display capability on the same SPI (up to four)
 - SPI optimization with DMA for large memory transfer
 - Dark theme support using indexed colors implementation
 - Images and Fonts static can be integrated in code. (see EvBmp utility)

#### Shell command features
 - Dump display memory (programmable radix)
 - Show display memory (disassemble display list of visual object)
 - Show object information (position, size, shape, status, ...)
 - Trace all touchscreen events
 - Change display orientation
 - Open/close live editor
 - Show SD card contents

## How it works
### Important note
| *I assume that the reader has some knowledge of the C++ language with Arduino environment* |
|:---:|

The EvEGL library is based on the creation of visual objects that are refreshed as needed at a given frequency. This default frequency is 60Hz and can be changed downwards.

The primary object classes are EvObj and EvPanel. The EvObj class is the base class from which all other visual object classes inherit. The EvPanel class of course inherits from the EvObj class, but above all allows you to add a list of visual objects that will be refreshed in order of creation. The EvPanel class can also contain other EvPanel classes. In this way, several chains of visual objects can be created.

The EvObj class provides a long list of methods to interact with the object. This visual object class also generates several events when it is refreshed. These main events are: drawEvent, resizeEvent, refreshEvent and touchEvent. Each object can then react to these events according to its needs.

Each visual object must be created dynamically. In other words, with the keyword « new ». To ensure that an object is created dynamically, the object's constructor must not be declared public. In this way, no visual object can be statically created. However, a static method named « Create » must be available and it will manage the allocation  of the object itself. This method must return a pointer to the object created. If creation fails, a null pointer should be returned. 

All visual objects will therefore be manipulated using pointers. The programmer should validate the creation of visual objects before use. There is no need to release visual objects that will be created in an EvPanel visual object. An EvPanel object type is designed to destroy all visual objects that contain it upon its own destruction.

A list of visual objects is included in this library (see below). A programmer can of course create his own objects from scratch but the easiest way is to start with already existing objects. In this way, development time is considerably reduced.

| List of visual objects included | Object category |
|:---:|:---:|
| EvObj | Primary object
| EvDisplay | Main display object |
| EvSlider, EvProgress | Progress event object |
| EvButton, EvToggle, EvCheckBox, EvSelector, EvTab | Control button object |
| EvLabel, EvNumLabel, EvNumInt, EvTextBox | Single line text object |
| EvTextBlock, EvTextCursor, EvTerminal | Multi-line text object |
| EvPanel, EvScrollbar, EvScrollBox, EvSideBar | Panel type object |
| EvVuBar, EvSmeter, EvSpectrum, EvPixelBox | Dynamic graphic object |
| EvImage, EvViewer, EvVideo, EvPlayer | Image and video object |
| EvKbd, EvEditor | System object |

## How to use EvEGL
To illustrate the simplicity of this library, let's take a look at the « TextBlock Demo », which you can find in the Examples section of the Arduino IDE. You can see what this demo can do with so few lines of code.

https://www.youtube.com/watch?v=ltXed7DeuFM

[![](https://img.youtube.com/vi/ltXed7DeuFM/0.jpg)](https://www.youtube.com/watch?v=ltXed7DeuFM)

### Header section
First you need to include the « EvEGL.h » file, which defines all the library classes. Then define the CS and RST constants corresponding to your display configuration.

```C++
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

```

### Variables section
Declaration of all visual objects pointers and of the static command Shell object.

```C++
EvDisplay   *Disp;
EvSelector  *AlignX;
EvLabel     *Title, *Status;
EvLabel     *FontLab, *SpLab, *PadLab;
EvNumInt    *FontSize, *Spacing, *Padding;
EvCheckBox  *Wrap;
EvTextBlock *Page;

EvShell     Shell(Serial);
```

Declaration of constant variables. You will notice the declaration of « AlignIconsX_160x40 » which was generated by the EvBmp utility from the « AlignIconsX_160x40.png » file. This file is also included in the library. You will also find the « Segoeprb_24_L4 » variable in an external file that was generated by the EvBmp utility.
```C++
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
```
### Code section

#### Setup() function
In this demo, all visual objects are created in the program's setup() function. The first object to be created is always EvDisplay, since all other objects are added to it. Once each object has been created, it can be configured using the available methods.

***Please note that in order to simplify the demo, validation of object creation has not been performed. I assume that all objects can be created, since there are few of them.***

```C++
void        setup(void)
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
```

#### Loop() function
Now all you have to do is call the Update method in the Loop function. This method refreshes all created objects at the configured frequency. By default, this is 60Hz. Afterwards, each character received from the SerialMonitor is transmitted to the command Shell.

```C++
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
```

#### Other functions
The following functions are callback functions for reacting to various object events.

```C++
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

```
## More examples

#### You can see other examples provided with the library
 - EvEGL Terminal Demo
 - EvEGL VideoPlayer Demo (https://www.youtube.com/watch?v=yxi0nsAX0lU)
- EvEGL ImageViewer Demo with ***Shell and live editing***

https://www.youtube.com/watch?v=mzpa-61yCAY

[![](https://img.youtube.com/vi/mzpa-61yCAY/0.jpg)](https://www.youtube.com/watch?v=mzpa-61yCAY)

I invite you to consult the source code to learn more. Some documentation has been started in the source code, but there is still a lot to do. To be continued...
