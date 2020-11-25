#include <math.h>

#include "utils.h"

unsigned short signed_dec_to_int16(float value, int range)
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

float int16_to_signed_dec(unsigned short value, int range)
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