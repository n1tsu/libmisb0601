#ifndef UTILS_H
#define UTILS_H

/**
 *  Those conversions are from the MISB0601.6 check the official document
 *  for further information.
 *
 *  It follows the formula : LDS_dec = (LDS_range / uint_range) * LDS_uint
 *  with LDS_uint encoded following two's complement
 *
 *  For example the conversion for the Platform Vertical Speed, Tag 51 :
 *  Its value is between the range -180 and 180, and is encoded into an int16.
 *
 *  Decode : (360 / 65534) * encoded_int16
 *  Encode : decimal_value * (65534 / 360)
 */

/**
 *  Convert signed decimal value to int16.
 *  int16 value will be encoded using two's complement.
 *
 *  @param value is the value to encode.
 *  @param range is the range of the value.
 *  @return the encoded value.
 */
unsigned short signed_dec_to_int16(float value, int range);

/**
 *  Convert an int16 value to a signed decimal.
 *  int16 value should be encoded using two's complement.
 *
 *  @param value is the value to decode.
 *  @param range is the range of the value.
 *  @return the decoded value.
 */
float int16_to_signed_dec(unsigned short value, int range);

#endif /* !UTILS_H */
