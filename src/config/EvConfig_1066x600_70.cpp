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

#include    <include/system/EvEVE.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const uint32_t  EvConfig_1066x600_70[] =
{
  REG_FREQUENCY, 72000000,
  REG_TRIM, 28,
  REG_PCLK_FREQ, 0xD12,
  REG_PCLK_2X, 0,
  REG_HSIZE, 1066,
  REG_VSIZE, 600,
  REG_HCYCLE, 1344,
  REG_HOFFSET, 160,
  REG_HSYNC0, 0,
  REG_HSYNC1, 70,
  REG_VCYCLE, 635,
  REG_VOFFSET, 23,
  REG_VSYNC0, 0,
  REG_VSYNC1, 10,
  REG_CSPREAD, 0,
  REG_DITHER, 0,
  REG_OUTBITS, 0666,
  REG_ROTATE, 0,
  REG_PWM_DUTY, 0,
  REG_SWIZZLE, 0,
  REG_PCLK_POL, 1,
  RAM_DL + 0, EV_CLEAR_COLOR_RGB(0, 0, 0),
  RAM_DL + 4, EV_CLEAR(1, 1, 1),
  RAM_DL + 8, EV_DISPLAY(),
  REG_DLSWAP, DLSWAP_FRAME,
  REG_GPIOX_DIR, 0x8000,
  REG_GPIOX, 0x8000,
  REG_PCLK, 1,
  REG_TOUCH_TRANSFORM_A, s15f16(1.051),
  REG_TOUCH_TRANSFORM_B, s15f16(0.000),
  REG_TOUCH_TRANSFORM_C, s15f16(7.137),
  REG_TOUCH_TRANSFORM_D, s15f16(0.000),
  REG_TOUCH_TRANSFORM_E, s15f16(1.000),
  REG_TOUCH_TRANSFORM_F, s15f16(-15.000),
  REG_CMDB_WRITE, CMD_HSF,
  REG_CMDB_WRITE, 1024,
  REG_CTOUCH_EXTENDED, 0,
  0
};
