#include <math.h>

#include "utils.h"


// INT16

unsigned short signed_dec_to_int16(float value, float range)
{
  char negative = 0;
  unsigned short result = 0;

  // pack using two's complement if needed
  if (value < 0)
  {
    value = -value;
    negative = 1;
  }

  // range of an uint16 is 65534
  result = (unsigned short) round((value * 65534) / range);

  if (negative)
    result = (~result) + 1;

  return result;
}

float int16_to_signed_dec(unsigned short value, float range)
{
  char negative = 0;
  float result = 0;

  // unpack two's complement value
  if ((value >> 15) & 1)
  {
    negative = 1;
    value = (~value) + 1;
  }

  // range of an uint16 is 65534
  result = ((float)range / 65534.0) * (float)value;
  if (negative)
    result *= -1;

  return result;
}

// INT32

int signed_dec_to_int32(double value, float range)
{
  char negative = 0;
  int result = 0;

  // pack using two's complement if needed
  if (value < 0)
  {
    value = -value;
    negative = 1;
  }

  // range of an uint32 is 4294967294
  result = round((value * 4294967294.0) / range);

  if (negative)
    result = (~result) + 1;

  return result;
}

double int32_to_signed_dec(int value, float range)
{
  char negative = 0;
  double result = 0;

  // unpack two's complement value
  if ((value >> 31) & 1)
  {
    negative = 1;
    value = (~value) + 1;
  }

  // range of an uint16 is 4294967294
  result = ((double)range / 4294967294.0) * (double)value;
  if (negative)
    result *= -1;

  return result;
}
