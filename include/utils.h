#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

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
unsigned short signed_dec_to_int16(float value, float range);

/**
 *  convert an int16 value to a signed decimal.
 *  int16 value should be encoded using two's complement.
 *
 *  @param value is the value to decode.
 *  @param range is the range of the value.
 *  @return the decoded value.
 */
float int16_to_signed_dec(unsigned short value, float range);
/**
 *  Convert an unsigned decimal value to int16.
 *
 *  @param value is the value to encode.
 *  @param range is the range of the value.
 *  @param offset is the offset of the value.
 *  @return the encoded value.
 */
unsigned short unsigned_dec_to_int16(float value, float range, float offset);

/**
 *  convert an int16 value to an unsigned decimal.
 *
 *  @param value is the value to decode.
 *  @param range is the range of the value.
 *  @param offset of the value.
 *  @return the decoded value.
 */
float int16_to_unsigned_dec(unsigned short value, float range, float offset);

/**
 *  Convert signed decimal value to int32.
 *  int32 value will be encoded using two's complement.
 *
 *  @param value is the value to encode.
 *  @param range is the range of the value.
 *  @return the encoded value.
 */
int signed_dec_to_int32(double value, float range);

/**
 *  Convert an int32 value to a signed decimal.
 *  int32 value should be encoded using two's complement.
 *
 *  @param value is the value to decode.
 *  @param range is the range of the value.
 *  @return the decoded value.
 */
double int32_to_signed_dec(int value, float range);

/**
 *  Convert unsigned decimal value to int32.
 *
 *  @param value is the value to encode.
 *  @param range is the range of the value.
 *  @param range is the range of the value.
 *  @param offset is the offset of the value.
 *  @return the encoded value.
 */
int unsigned_dec_to_int32(double value, float range, double offset);

/**
 *  Convert an int32 value to an unsigned decimal.
 *
 *  @param value is the value to decode.
 *  @param range is the range of the value.
 *  @param offset is the offset of the value.
 *  @return the decoded value.
 */
double int32_to_unsigned_dec(int value, float range, double offset);

// Checksum function from MISB0601 documentation
unsigned short bcc_16(uint8_t *buff, unsigned short len);

uint64_t get_timestamp();

#endif /* !UTILS_H */
