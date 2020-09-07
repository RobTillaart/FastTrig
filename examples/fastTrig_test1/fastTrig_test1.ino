//
//    FILE: fastTrig.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.1
// PURPOSE: testing the fastTrigonio functions
//    DATE: 2020-08-30
//    (c) : MIT
//

/*
   On AVR
   ISIN 2.72 x faster
   ICOS 2.19 x faster
   ITAN 1.14 x faster (rel error < 1% for 0 .. 89.8°)

   On ESP32
   6 - 9 x faster
*/

#include "FastTrig.h"

uint32_t start, d1, d2;

volatile float x;
int i;

void setup()
{
  Serial.begin(115200);
  Serial.println("start");

  test_hw_support();

  test_sin_cos_tan();
  test_isin_icos_itan();

  test_isin_error_1(false);
  test_icos_error_1(false);

  // fsin is expensive...
  test_fsin();
  test_fsin_error_3(false);
  test_fsin_error_4(false);

  test_itan_error_1(true);  // TODO not happy on AVR

  Serial.println("done...\n");
}

bool test_hw_support()
{
  Serial.println(__FUNCTION__);
  int n = random(350);
  start = micros();
  volatile float x = sin(n);
  d1 = micros() - start;
  start = micros();
  volatile float y = x / 3.14;
  d2 = micros() - start;
  Serial.println(d1);
  Serial.println(d2);
  Serial.println();
  return (d1 / d2) < 1.5; // just a guess.
}

void test_sin_cos_tan()
{
  Serial.println(__FUNCTION__);
  Serial.println("SIN\tCOS\tTAN\t360 calls" );
  delay(10);
  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = sin(i);
  }
  Serial.print((micros() - start) / 360.0);
  Serial.print('\t');
  delay(10);

  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = cos(i);
  }
  Serial.print((micros() - start) / 360.0);
  Serial.print('\t');
  delay(10);

  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = tan(i);
  }
  Serial.println((micros() - start) / 360.0);
  Serial.println();
  Serial.println();
  delay(10);
}

void test_isin_icos_itan()
{
  Serial.println(__FUNCTION__);
  Serial.println("ISIN\tICOS\tITAN\t360 calls");
  delay(10);
  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = isin(i);
  }
  Serial.print((micros() - start) / 360.0);
  Serial.print('\t');
  delay(10);

  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = icos(i);
  }
  Serial.print((micros() - start) / 360.0);
  Serial.print('\t');
  delay(10);

  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = itan(i);
  }
  Serial.println((micros() - start) / 360.0);
  Serial.println();
  Serial.println();
  delay(10);
}

void test_fsin()
{
  Serial.println(__FUNCTION__);
  Serial.print("FSIN 360 calls: \t");
  start = micros();
  for ( i = 0; i < 360; i++)
  {
    x = fsin(i);
  }
  Serial.println((micros() - start) / 360.0);
  Serial.println();
  delay(10);
}

void test_isin_error_1(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("ISIN 0-3600 calls: \t");
  delay(10);

  float mx = 0;
  float z = 0;
  for ( i = 0; i < 3600; i++)
  {
    float a = sin(i * 0.1 * PI / 180);
    float b = isin(i * 0.1);
    float y = abs(a - b);
    z += y;
    if (mx < y)
    {
      mx = y;
      if (show)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(a, 6);
        Serial.print("\t");
        Serial.print(b, 6);
        Serial.print("\t");
        Serial.println(a - b, 6);
      }
    }
  }
  Serial.print("max error: ");
  Serial.println(mx, 8);
  Serial.print("avg error: ");
  Serial.println(z / 3600, 8);
  Serial.println();
  delay(10);
}

void test_icos_error_1(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("ICOS 0-3600 calls:\t");
  delay(10);

  float mx = 0;
  float z = 0;
  for ( i = 0; i < 3600; i++)
  {
    float a = cos(i * 0.1 * PI / 180);
    float b = icos(i * 0.1);
    float y = abs(a - b);
    z += y;
    if (mx < y)
    {
      mx = y;
      if (show)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(a, 6);
        Serial.print("\t");
        Serial.print(b, 6);
        Serial.print("\t");
        Serial.println(a - b, 6);
      }
    }
  }

  Serial.print("max error: ");
  Serial.println(mx, 8);
  Serial.print("avg error: ");
  Serial.println(z / 3600, 8);
  Serial.println();
  Serial.println();
  delay(10);
}

void test_fsin_error_3(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("FSIN 0-3600 calls: \t");
  delay(10);

  float mx = 0;
  float z = 0;
  for ( i = 0; i < 3600; i++)
  {
    float a = sin(i * 0.1 * PI / 180);
    float b = fsin(i * 0.1);
    float y = abs(a - b);
    z += y;
    if (mx < y)
    {
      mx = y;
      if (show)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(a, 6);
        Serial.print("\t");
        Serial.print(b, 6);
        Serial.print("\t");
        Serial.println(a - b, 6);
      }
    }
  }

  Serial.print("max error: ");
  Serial.println(mx, 8);
  Serial.print("avg error: ");
  Serial.println(z / 3600, 8);
  Serial.println();
  delay(10);
}

void test_fsin_error_4(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("FSIN 360-720 calls: \t");
  delay(10);

  float mx = 0;
  float z = 0;
  for ( i = 360; i < 720; i++)
  {
    float a = sin(i * PI / 180);
    float b = fsin(i);
    float y = abs(a - b);
    z += y;
    if (mx < y)
    {
      mx = y;
      if (show)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(a, 6);
        Serial.print("\t");
        Serial.print(b, 6);
        Serial.print("\t");
        Serial.println(a - b, 6);
      }
    }
  }

  Serial.print("max error: ");
  Serial.println(mx, 8);
  Serial.print("avg error: ");
  Serial.println(z / 360, 8);
  Serial.println();
  Serial.println();
  delay(10);
}


void test_itan_error_1(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("ITAN 0-3600 calls: \t");
  delay(10);

  float mx = 0;
  float z = 0;
  for ( i = -0; i < 3600; i++)
  {
    if ((i + 900 )% 1800 == 0) continue;
    float a = tan(i * 0.1 * PI / 180);
    float b = itan(i * 0.1);
    float y = abs(a - b);  // abs error - rel error ~ 1%
    z += y;
    if (mx < y)
    {
      mx = y;
      if (show)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(a, 6);
        Serial.print("\t");
        Serial.print(b, 6);
        Serial.print("\t");
        Serial.print(a-b, 6);
        Serial.print("\t");
        Serial.println((a-b)/a, 6);
      }
    }
  }
  Serial.print("max error: ");
  Serial.println(mx, 8);
  Serial.print("avg error: ");
  Serial.println(z / 3600, 8);
  Serial.println();
  delay(10);
}


void loop()
{
}

// -- END OF FILE --
