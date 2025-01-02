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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const EvFont    sEvRomFont16 =
{
  { 8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8},
    1,   1,   8,   8, 0x2ff7fc,  16,   2,   0, 128,   0,   0,   0, "RomFont16"
};

static const EvFont    sEvRomFont17 =
{
  { 8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8},
    1,   1,   8,   8, 0x2ffbfc,  17,   2,   0, 128,   0,   0,   0, "RomFont17"
};

static const EvFont    sEvRomFont18 =
{
  { 8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8},
    1,   1,   8,  16, 0x2fe7fc,  18,   2,   0, 128,   0,   0,   0, "RomFont18"
};

static const EvFont    sEvRomFont19 =
{
  { 8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8},
    1,   1,   8,  16, 0x2feffc,  19,   2,   0, 128,   0,   0,   0, "RomFont19"
};

static const EvFont    sEvRomFont20 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    3,   3,   4,   6,   6,   9,   8,   2,   4,   4,   4,   6,   3,   4,   3,   3,
    6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   3,   3,   6,   5,   6,   6,
   11,   7,   7,   8,   8,   7,   6,   8,   8,   3,   5,   7,   6,   9,   8,   8,
    7,   8,   7,   7,   5,   8,   7,   9,   7,   7,   7,   3,   3,   3,   6,   6,
    3,   5,   6,   5,   6,   5,   4,   6,   6,   2,   2,   5,   2,   8,   6,   6,
    6,   6,   4,   5,   4,   5,   6,   8,   6,   5,   5,   3,   3,   3,   7,   0},
    1,   2,  10,  13, 0x2fdafc,  20,   1,  32,  96,   0,   0,   0, "RomFont20"
};

static const EvFont    sEvRomFont21 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    4,   4,   5,   8,   8,  12,  10,   3,   5,   5,   7,   9,   3,   4,   3,   4,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   3,   4,   8,   9,   8,   8,
   13,   9,   9,  10,  10,   9,   8,  11,  10,   4,   7,   9,   8,  12,  10,  11,
    9,  11,  10,   9,   9,  10,   9,  13,   9,   9,   9,   4,   4,   4,   7,   8,
    5,   8,   7,   7,   8,   8,   4,   8,   8,   3,   3,   7,   3,  11,   8,   8,
    8,   8,   5,   7,   4,   7,   7,  10,   7,   7,   7,   5,   3,   5,   8,   0},
    1,   2,  13,  17, 0x2fcd3c,  21,   1,  32,  96,   0,   0,   0, "RomFont21"
};

static const EvFont    sEvRomFont22 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    5,   5,   6,   9,   9,  14,  11,   3,   6,   6,   6,  10,   4,   5,   4,   5,
    9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   4,   4,  10,  10,  10,   9,
   17,  11,  11,  12,  12,  11,  10,  13,  12,   4,   8,  11,   9,  13,  12,  13,
   11,  13,  12,  11,  10,  12,  11,  15,  11,  11,  10,   5,   5,   5,   8,   9,
    6,   9,   9,   8,   9,   9,   5,   9,   9,   3,   4,   8,   3,  14,   9,   9,
    9,   9,   5,   8,   5,   9,   8,  12,   8,   8,   8,   6,   4,   6,  10,   0},
    1,   2,  14,  20, 0x2fbd7c,  22,   1,  32,  96,   0,   0,   0, "RomFont22"
};

static const EvFont    sEvRomFont23 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    5,   6,   5,  10,  10,  16,  13,   3,   6,   6,   7,  10,   5,   6,   5,   5,
   10,  10,  10,  10,  10,  10,  10,  10,  10,  10,   5,   5,  10,  11,  10,  10,
   18,  13,  13,  14,  14,  13,  12,  15,  14,   6,  10,  13,  11,  16,  14,  15,
   13,  15,  14,  13,  12,  14,  13,  18,  13,  13,  12,   5,   5,   5,   9,  11,
    4,  11,  11,  10,  11,  10,   6,  11,  10,   4,   4,   9,   4,  16,  10,  11,
   11,  11,   6,   9,   6,  10,  10,  14,  10,  10,   9,   6,   5,   6,  10,   0},
    1,   3,  17,  22, 0x2fa17c,  23,   1,  32,  96,   0,   0,   0, "RomFont23"
};

static const EvFont    sEvRomFont24 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    6,   6,   8,  14,  13,  22,  17,   6,   8,   8,  10,  14,   6,   8,   6,   7,
   13,  13,  13,  13,  13,  13,  13,  13,  13,  13,   6,   6,  15,  15,  15,  12,
   25,  17,  17,  18,  18,  16,  14,  19,  18,   8,  13,  18,  14,  21,  18,  18,
   16,  18,  17,  16,  16,  18,  17,  22,  17,  16,  15,   7,   7,   7,  12,  14,
    7,  13,  14,  12,  14,  13,   8,  14,  13,   6,   6,  12,   6,  20,  14,  13,
   14,  14,   9,  12,   8,  14,  13,  18,  12,  13,  12,   8,   6,   8,  14,   0},
    1,   3,  24,  29, 0x2f7e3c,  24,   2,  32,  96,   0,   0,   0, "RomFont24"
};

static const EvFont    sEvRomFont25 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    9,   9,  12,  19,  18,  29,  22,   6,  11,  11,  13,  19,   9,  11,   9,   9,
   18,  18,  18,  18,  18,  18,  18,  18,  18,  18,   9,   9,  19,  19,  19,  18,
   34,  22,  22,  24,  24,  22,  20,  25,  24,   9,  16,  22,  18,  27,  24,  25,
   22,  26,  24,  22,  20,  24,  22,  31,  22,  22,  20,   9,   9,   9,  16,  18,
   11,  18,  18,  16,  18,  18,   9,  18,  18,   7,   7,  16,   7,  27,  18,  18,
   18,  18,  11,  16,   9,  18,  16,  23,  16,  16,  16,  11,   9,  11,  19,   0},
    1,   4,  30,  38, 0x2f3d1c,  25,   3,  32,  96,   0,   0,   0, "RomFont25"
};

static const EvFont    sEvRomFont26 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    3,   3,   5,  10,   8,  11,   9,   3,   5,   5,   7,   9,   3,   6,   3,   6,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   3,   3,   8,   8,   8,   7,
   13,   9,   9,   9,   9,   7,   7,   9,   9,   4,   8,   9,   7,  11,   9,  10,
    9,  10,   9,   9,  10,   9,   9,  12,   9,   9,   9,   4,   6,   4,   6,   8,
    4,   8,   8,   8,   8,   8,   6,   8,   8,   3,   3,   7,   3,  11,   8,   8,
    8,   8,   5,   7,   6,   8,   7,  11,   7,   7,   8,   5,   3,   5,  10,   3},
    2,   7,  14,  16, 0x2f181c,  26,   1,  32,  96,   0,   0,   0, "RomFont26"
};

static const EvFont    sEvRomFont27 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    4,   4,   6,  11,  10,  13,  11,   4,   6,   6,   8,  10,   4,   7,   4,   7,
   10,  10,  10,  10,  10,  10,  10,  10,  10,  10,   4,   4,   9,   9,   9,   9,
   15,  11,  10,  11,  11,   9,   9,  11,  11,   5,   9,  11,   9,  14,  11,  12,
   10,  12,  11,  11,  12,  11,  11,  15,  11,  10,  11,   5,   7,   5,   7,  10,
    5,   9,   9,   9,  10,   9,   7,  10,   9,   4,   4,   9,   4,  15,   9,  10,
    9,  10,   6,   9,   7,   9,   9,  13,   9,   9,   9,   6,   4,   6,  11,   4},
    2,   8,  16,  20, 0x2ed61c,  27,   1,  32,  96,   0,   0,   0, "RomFont27"
};

static const EvFont    sEvRomFont28 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    5,   6,   7,  14,  11,  16,  14,   4,   7,   8,  10,  12,   4,  10,   6,   9,
   12,  12,  12,  12,  12,  12,  12,  12,  12,  12,   6,   6,  11,  13,  11,  10,
   19,  13,  14,  13,  14,  12,  12,  14,  15,   6,  12,  14,  12,  19,  15,  14,
   14,  14,  13,  12,  14,  13,  14,  18,  13,  14,  13,   6,   9,   7,   9,  11,
    7,  11,  11,  11,  12,  11,   8,  11,  11,   6,   6,  11,   6,  18,  11,  12,
   11,  12,   7,  11,   8,  12,  11,  16,  11,  11,  11,   8,   5,   7,  14,   5},
    2,   9,  18,  25, 0x2e799c,  28,   1,  32,  96,   0,   0,   0, "RomFont28"
};

static const EvFont    sEvRomFont29 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    6,   6,   8,  15,  15,  17,  15,   5,   9,   8,  11,  14,   5,  11,   7,  10,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,   6,   6,  12,  14,  13,  12,
   21,  15,  15,  15,  17,  13,  13,  16,  17,   7,  13,  16,  13,  21,  17,  16,
   15,  17,  15,  14,  15,  17,  15,  21,  15,  15,  14,   7,  10,   7,  10,  13,
    8,  13,  14,  12,  14,  12,  10,  14,  14,   6,   6,  13,   6,  21,  14,  13,
   14,  13,   9,  12,   9,  14,  12,  18,  12,  12,  12,   8,   6,   9,  15,   6},
    2,  11,  22,  28, 0x2dfbbc,  29,   2,  32,  96,   0,   0,   0, "RomFont29"
};

static const EvFont    sEvRomFont30 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    8,   9,  12,  19,  18,  23,  19,   7,  11,  10,  14,  17,   7,  15,   8,  13,
   17,  17,  17,  17,  17,  17,  17,  17,  17,  17,   7,   8,  16,  18,  16,  15,
   28,  20,  19,  20,  22,  16,  17,  22,  23,   9,  17,  19,  17,  26,  23,  22,
   19,  22,  19,  20,  19,  21,  20,  27,  20,  19,  18,   9,  13,   9,  13,  16,
   10,  17,  17,  16,  17,  16,  12,  18,  17,   7,   8,  16,   7,  27,  17,  17,
   17,  17,  11,  17,  11,  17,  16,  23,  16,  16,  15,  11,   7,  10,  21,   8},
    2,  14,  28,  36, 0x2d263c,  30,   2,  32,  96,   0,   0,   0, "RomFont30"
};

static const EvFont    sEvRomFont31 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   10,  11,  15,  26,  25,  31,  26,  10,  15,  14,  18,  24,   9,  18,  11,  17,
   24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  10,  10,  21,  23,  22,  20,
   37,  27,  27,  26,  28,  23,  22,  28,  29,  12,  23,  26,  22,  35,  29,  28,
   26,  29,  27,  26,  26,  28,  27,  36,  27,  26,  25,  12,  18,  12,  18,  21,
   13,  23,  24,  22,  24,  22,  15,  24,  24,  10,  11,  22,  10,  36,  24,  24,
   24,  24,  15,  22,  14,  24,  21,  32,  21,  21,  22,  15,  10,  15,  29,  10},
    2,  18,  36,  49, 0x2bac3c,  31,   3,  32,  96,   0,   0,   0, "RomFont31"
};

static const EvFont    sEvRomFont32 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   13,  15,  19,  33,  31,  40,  34,  11,  18,  18,  24,  30,  12,  24,  14,  22,
   30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  13,  14,  28,  30,  29,  26,
   49,  34,  34,  34,  36,  29,  29,  37,  37,  15,  30,  34,  29,  46,  37,  37,
   34,  38,  33,  33,  32,  37,  34,  46,  34,  34,  32,  15,  22,  15,  23,  26,
   17,  30,  31,  28,  31,  29,  19,  31,  31,  13,  14,  28,  13,  47,  31,  31,
   31,  31,  19,  29,  17,  31,  27,  41,  27,  27,  27,  18,  14,  18,  36,  13},
    2,  23,  46,  63, 0x2945fc,  32,   4,  32,  96,   0,   0,   0, "RomFont32"
};

static const EvFont    sEvRomFont33 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   18,  19,  25,  44,  41,  52,  44,  15,  24,  24,  31,  41,  16,  32,  19,  29,
   40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  18,  18,  36,  40,  37,  34,
   63,  45,  45,  45,  48,  39,  39,  48,  50,  20,  40,  45,  39,  62,  50,  49,
   45,  50,  45,  43,  42,  48,  45,  61,  45,  45,  42,  19,  29,  19,  30,  34,
   22,  39,  40,  37,  40,  37,  25,  41,  41,  18,  18,  36,  18,  63,  41,  40,
   40,  40,  25,  38,  23,  41,  36,  54,  36,  36,  36,  24,  18,  24,  47,  18},
    2,  30,  60,  83, 0x251e1c,  33,   6,  32,  96,   0,   0,   0, "RomFont33"
};

static const EvFont    sEvRomFont34 =
{
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   23,  25,  33,  57,  54,  68,  57,  20,  31,  31,  40,  52,  20,  41,  24,  38,
   52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  23,  23,  46,  52,  48,  44,
   82,  58,  58,  58,  63,  50,  50,  62,  65,  26,  50,  58,  51,  79,  65,  63,
   58,  64,  58,  56,  56,  62,  58,  79,  58,  58,  55,  25,  38,  25,  38,  43,
   29,  50,  52,  48,  52,  48,  31,  52,  52,  23,  23,  47,  23,  80,  52,  52,
   51,  52,  32,  48,  29,  52,  46,  70,  46,  46,  46,  31,  23,  31,  63,  23},
    2,  39,  78, 108, 0x1e1b5c,  34,   7,  32,  96,   0,   0,   0, "RomFont34"
};

const EvFont   *sEvRomFont[19] =
{
  &sEvRomFont16,
  &sEvRomFont17,
  &sEvRomFont18,
  &sEvRomFont19,
  &sEvRomFont20,
  &sEvRomFont21,
  &sEvRomFont22,
  &sEvRomFont23,
  &sEvRomFont24,
  &sEvRomFont25,
  &sEvRomFont26,
  &sEvRomFont27,
  &sEvRomFont28,
  &sEvRomFont29,
  &sEvRomFont30,
  &sEvRomFont31,
  &sEvRomFont32,
  &sEvRomFont33,
  &sEvRomFont34,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvSysFont::EvSysFont(EvDisplay *Disp) :
  Disp(Disp)
{
  InitSystemFont();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvSysFont::FindFont(const EvFont *Fnt)
{
  for (int font = LAST_FONT; font > 0; font--)
    if (SystemFont[font] == Fnt)
      return font;

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvSysFont::LoadFont(const EvFont *Fnt)
{
  for (int font = LAST_FONT; font > 0; font--)
    if (SystemFont[font] == nullptr)
      return LoadFont(Fnt, font);

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvSysFont::LoadFont(const EvFont *Fnt, uint8_t Font)
{
  uint32_t  Addr;
  EvMem     *Dest;

  if (Font == 0 || Font > LAST_FONT || SystemFont[Font] != nullptr)
    return 0;

  if ((Dest = (EvMem *)Disp->RAM_G.FindByOwner(Fnt)) == nullptr)
  {
    if ((Dest = (EvMem *)Disp->RAM_G.Malloc(FMB_SIZE + Fnt->BmpSize, Fnt)) == nullptr)
      return 0;

    Addr = Dest->addr + FMB_SIZE;

    Disp->CmdMemWrite(Dest->addr, FMB_SIZE);
    Disp->wrCmdBufData((const uint8_t *)Fnt, FMB_SIZE - 4);
    Disp->wrCmdBuf32(Addr);

    if (Fnt->BmpSize == Fnt->DataSize)
      Disp->CmdMemWrite(Addr, Fnt->DataSize);
    else
      Disp->CmdInflate(Addr);

    Disp->wrCmdBufData(Fnt->Data, Fnt->DataSize);
    Disp->wrCmdBufAlign();
  }

  Disp->CmdSetFont2(Font, Dest->addr, Fnt->CharBeg);
  Disp->CmdRomFont(0, FIRST_ROM_FONT);   // dummy to finish CmdSetFont2 ???
  SystemFont[Font] = Fnt;
  Dest->count++;
  return Font;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint8_t     EvSysFont::LoadRomFont(uint8_t Font, uint8_t RomFont)
{
  if (RomFont < FIRST_ROM_FONT || RomFont > LAST_ROM_FONT || Font == 0 || Font > LAST_FONT || SystemFont[Font] != nullptr)
    return 0;

  SystemFont[Font] = sEvRomFont[RomFont - FIRST_ROM_FONT];
  Disp->CmdRomFont(Font, RomFont);
  Disp->CmdRomFont(0, FIRST_ROM_FONT);   // dummy to finish last CmdRomFont ???
  return Font;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSysFont::UnloadFont(const EvFont *Fnt)
{
  int       font = FindFont(Fnt);

  if (font > 0 && Fnt->RomFont == 0)
  {
    EvMem   *Dest = (EvMem *)Disp->RAM_G.FindByOwner(Fnt);

    if (Dest != nullptr && --Dest->count == 0)
      Disp->RAM_G.Free(Dest);

    SystemFont[font] = nullptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSysFont::UnloadFont(uint8_t Font)
{
  const EvFont  *fnt;

  if (Font != 0 && Font <= LAST_FONT && (fnt = SystemFont[Font]) != nullptr)
  {
    if (fnt->RomFont == 0)
    {
      EvMem     *Dest = (EvMem *)Disp->RAM_G.FindByOwner(fnt);

      if (Dest != nullptr && --Dest->count == 0)
        Disp->RAM_G.Free(Dest);
    }

    SystemFont[Font] = nullptr;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvSysFont::InitSystemFont(void)
{
  int       font, romFont;

  Disp->CmdSetScratch(0);      // Relocate scratch handle to 0.

  for (font = 0; font < FIRST_ROM_FONT; font++)
    SystemFont[font] = nullptr;

  for (romFont = font; font <= LAST_FONT; font++)
  {
    SystemFont[font] = sEvRomFont[romFont - FIRST_ROM_FONT];
    Disp->CmdRomFont(font, romFont);

    if (++romFont == 23)
      romFont += 3;
  }

  Disp->CmdRomFont(0, FIRST_ROM_FONT);   // dummy to finish last CmdRomFont ???
}
