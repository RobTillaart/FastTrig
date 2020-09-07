#pragma once
//
//    FILE: fastTrig.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.3
// PURPOSE: Arduino library for a faster approximation of sin() and cos()
//    DATE: 2011-08-18
//     URL: https://github.com/RobTillaart/fastTrig
//          https://forum.arduino.cc/index.php?topic=69723.0
//
// HISTORY:
// 0.1.00   2011-08-18 initial version
// 0.1.01   2011-08-18 improved tables a bit + changed param to float
// 0.1.02   2011-08-20 added interpolation
//          eons passed
// 0.1.1    2020-08-30 refactor, create a library. isin() & icos() are OK.   itan() is bad.
// 0.1.2    2020-09-06 optimize 16 bit table with example sketch
// 0.1.3    2020-09-07 initial release.   itan() still bad

#include "Arduino.h"

// 91 x 2 bytes ==> 182 bytes
unsigned int isinTable16[] = {
  0,
  1145, 2289, 3435, 4571, 5715, 6852, 7988, 9125, 10254, 11385,
  12508, 13630, 14745, 15859, 16963, 18067, 19165, 20253, 21342, 22416,
  23488, 24553, 25610, 26659, 27699, 28730, 29754, 30773, 31777, 32771,
  33755, 34734, 35697, 36649, 37594, 38523, 39445, 40350, 41247, 42127,
  42998, 43856, 44697, 45527, 46344, 47146, 47931, 48708, 49461, 50205,
  50933, 51645, 52341, 53022, 53686, 54333, 54969, 55578, 56180, 56759,
  57322, 57866, 58394, 58908, 59399, 59871, 60327, 60767, 61184, 61584,
  61969, 62330, 62677, 63000, 63304, 63592, 63857, 64108, 64333, 64544,
  64731, 64902, 65049, 65177, 65289, 65376, 65449, 65501, 65527, 65535,
  65535
};

uint8_t isinTable8[] = { 
  0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44,
  49, 53, 57, 62, 66, 70, 75, 79, 83, 87,
  91, 96, 100, 104, 108, 112, 116, 120, 124, 128,
  131, 135, 139, 143, 146, 150, 153, 157, 160, 164,
  167, 171, 174, 177, 180, 183, 186, 190, 192, 195,
  198, 201, 204, 206, 209, 211, 214, 216, 219, 221,
  223, 225, 227, 229, 231, 233, 235, 236, 238, 240,
  241, 243, 244, 245, 246, 247, 248, 249, 250, 251,
  252, 253, 253, 254, 254, 254, 255, 255, 255, 255,
  255
};

float isin(float f)
{
  boolean pos = true;  // positive
  if (f < 0)
  {
    f = -f;
    pos = !pos;
  }

  long x = f;
  uint8_t r = (f - x) * 256;

  if (x >= 360) x %= 360;

  int y = x; // 16 bit math is faster than 32 bit

  if (y >= 180)
  {
    y -= 180;
    pos = !pos;
  }

  if (y >= 90)
  {
    y = 180 - y;
    if (r != 0)
    {
      r = 256 - r;
      y--;
    }
  }

  uint16_t v = isinTable16[y];
  // interpolate if needed
  if (r > 0) v = v + ((isinTable16[y + 1] - v)/8 * r) /32;   //  == * r / 256
  if (pos) return v * 0.0000152590219; // = /65535.0
  return v * -0.0000152590219 ;
}


float icos(float x)
{
  return isin(x + 90);
}

// x = -90..90
float itan(float x)
{
  // reference
  return isin(x)/icos(x);
}

// experimental floating point interpolation
float fsin(float d)
{
  // interpolate
  int dd = int(d);
  float a = isin(dd);
  float frac = d - dd;
  if (frac > 0)
  {
    float b = isin(d + 1);
    a += frac * (b - a);
  }
  return a;
}

/////////////////////////////////////////////////////////////
//
// Code for generating the initial tables   (use the example to optimize for interpolation)
//
//  Serial.println("unsigned int isinTable[] = { ");
//  for (int i = 0; i <= 90; i++)
//  {
//    uint16_t  x = round(sin(i * PI /180) * 65535);
//    Serial.print(x);
//    Serial.print(", ");
//    if (i % 10 == 0) { Serial.println(); Serial.print("  "); }
//  }
//  Serial.println("}; ");

//  Serial.println("uint8_t isinTable[] = { ");
//  for (int i = 0; i <= 90; i++)
//  {
//    uint16_t  x = round(sin(i * PI /180) * 255);
//    Serial.print(x);
//    Serial.print(", ");
//    if (i % 10 == 0) { Serial.println(); Serial.print("  "); }
//  }
//  Serial.println("}; ");


// -- END OF FILE --
