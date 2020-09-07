//
//    FILE: fastTrig_plot.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: optimize isinTable16[] 
//    DATE: 2020-09-06

#include "FastTrig.h"

float getError(int i)
{
  float error = 0;
  for (float f = i - 1; f < i + 1; f += 0.0001)
  {
    error += abs(sin(f / 180 * PI) - isin(f));
  }
  return error;
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  for (int i = 0; i < 90; i++)
  {
    Serial.print(isinTable16[i]);
    Serial.print(", ");
    if (i % 10 == 0) Serial.println();
  }
  Serial.println();
  Serial.println();
  test_isin_error_1(false);
  Serial.println();

  while(optimize());
  Serial.println("\n\ndone...");
}

int optimize()
{
  int rv = 0;
  for (int i = 1; i < 90; i++)
  {
    int t = isinTable16[i];
    int idx = 0;
    float minError = getError(i);
    bool flag = false;
    for (int j = -1; j < 1; j++)
    {
      if (j == 0) continue;
      isinTable16[i] = t + j;
      float e = getError(i);
      if (e < minError)
      {
        idx = j;
        minError = e;
        rv++;
        flag = true;
      }
    }
    if (flag) Serial.print('*');
    isinTable16[i] = t + idx;
    Serial.print(isinTable16[i]);
    Serial.print(", ");
    if (i % 10 == 0) Serial.println();
  }
  Serial.println();
  Serial.println();
  test_isin_error_1(false);

  return rv;
}


void test_isin_error_1(bool show)
{
  Serial.println(__FUNCTION__);
  Serial.println("ISIN 0-3600 calls: \t");
  delay(10);

  float mx = 0;
  float z = 0;
  for (int i = 0; i < 3600; i++)
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



void loop()
{
}

// -- END OF FILE --
