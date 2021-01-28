#include "packet.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct Packet *initialize_packet()
{
  struct Packet *packet = malloc(sizeof(struct Packet));

  // Arbitrary allocation to begin.
  // It will be doubled when more size is required.
  packet->content = malloc(256);

  if (!packet->content)
    return NULL;

  packet->available_size = 256;
  packet->size = 0;
  packet->ready = 0;

  // Add mandatory Timestamp KLV which needs to be the first klv
  uint64_t unix_time = get_timestamp();
  struct GenericValue unix_time_stamp_value = {UINT64, .uint64_value = unix_time};
  packet = add_klv(packet, F_UNIX_TIME_STAMP, unix_time_stamp_value);

  // Add mandatory KLV for UAS_LDS version which is ST0601.6
  // Add key tag
  packet->content[packet->size++] = UAS_LDS_VERSION_NUMBER;
  // Add size
  packet->content[packet->size++] = 1;
  // Add revision number
  packet->content[packet->size++] = 0x6;

  return packet;
}

void free_packet(struct Packet *packet)
{
  free(packet->content);
  free(packet);
}

int finalize_packet(struct Packet *packet)
{
  // We need to add Checksum KLV last since it needs
  // to checksum the whole frame. We know that the KLV
  // frame is 4 bytes long. (1 for tag, 1 for length,
  // 2 for value).
  packet->size += 1 + 1 + 2;
  int new_size = packet->size;

  int LDS_KL_size = 0;

  // LDS length field size is determined via BER short/long encoding.
  // LDS_K_size is the size of the key, LDS_L_size is the size of the
  // length field.
  int LDS_K_size = 16;
  int LDS_L_size = 0;

  // Short BER
  if (new_size <= 127)
    LDS_L_size = 1;
  // Long BER
  else if (new_size <= 255)
    LDS_L_size = 2;
  else
    LDS_L_size = 3;

  LDS_KL_size = LDS_K_size + LDS_L_size;

  // Create memory of the length of the whole packet, packet->content will be
  // appended to this buffer.
  uint8_t *LDS_KL = malloc((new_size + LDS_KL_size) * sizeof(uint8_t));
  memcpy(LDS_KL, LDS_UNIVERSAL_KEY, 16);

  if (LDS_L_size == 1)
    LDS_KL[LDS_K_size] = (uint8_t)new_size;
  else if (LDS_L_size == 2)
  {
    LDS_KL[LDS_K_size] = (1 << 7) + 1;
    LDS_KL[LDS_K_size + 1] = (uint8_t)new_size;
  }
  else
  {
    LDS_KL[LDS_K_size] = (1 << 7) + 2;
    LDS_KL[LDS_K_size + 1] = (uint8_t)new_size >> 8;
    LDS_KL[LDS_K_size + 2] = (uint8_t)new_size;
  }

  // Copy packet content after LDS KL memory
  if (!memcpy(LDS_KL + LDS_KL_size, packet->content, packet->size))
    return 1;

  free(packet->content);

  packet->content = LDS_KL;
  packet->size += LDS_KL_size;

  // Checksum KLV needs to be calculated and added
  packet->content[packet->size - 4] = CHECKSUM;
  packet->content[packet->size - 3] = 2;

  unsigned short checksum = bcc_16(packet->content, packet->size - 2);
  packet->content[packet->size - 2] = (uint8_t)(checksum >> 8);
  packet->content[packet->size - 1] = (uint8_t)checksum;

  packet->ready = 1;

  return 0;
}

struct Packet *add_klv(struct Packet *packet, const struct Field field,
                       struct GenericValue value)
{
  union Offset {
    float foffset;
    double doffset;
  } offset;

  union Res {
    uint16_t u16res;
    uint32_t u32res;
    int16_t s16res;
    int32_t s32res;
  } res;

  if (field.value_format != value.type)
    return NULL;

  // The tag field length follow BER-OID encoding to encode the length
  // of the KLV. Since in MISB0601 max tag value is 93, and is under
  // 2**7 - 1 = 127, we know that the length will be encoded in 1 byte.
  // Value length is assumed to be coded in 1 byte.

  if (packet->available_size <= packet->size + 2 + field.len)
  {
    packet->content = realloc(packet->content, packet->available_size * 2);
    if (!packet->content)
      return NULL;

    packet->available_size = packet->available_size * 2;
  }

  char *bytes_value = 0;
  // Conversion if needed
  if (field.value_format != field.encoded_format)
  {
    switch (field.encoded_format)
    {
    case UINT16:
      offset.foffset = (field.range.min.float_value < 0) ? field.range.min.float_value : 0;
      res.u16res = unsigned_dec_to_int16(value.float_value,
                                     fabs(field.range.min.float_value) +
                                     fabs(field.range.max.float_value),
                                     offset.foffset);
      bytes_value = (char *)&res.u16res;
      break;
    case UINT32:
      offset.doffset = (field.range.min.float_value < 0) ? field.range.min.float_value : 0;
      res.u32res = unsigned_dec_to_int32(value.double_value,
                                                   fabs(field.range.min.double_value) +
                                                   fabs(field.range.max.double_value),
                                                   offset.doffset);
      bytes_value = (char *)&res.u32res;
      break;
    case INT16:
      res.s16res = signed_dec_to_int16(value.float_value,
                                       fabs(field.range.min.float_value) +
                                       fabs(field.range.max.float_value));
      bytes_value = (char *)&res.s16res;
      break;
    case INT32:
      res.s32res = signed_dec_to_int16(value.double_value,
                                       fabs(field.range.min.double_value) +
                                       fabs(field.range.max.double_value));
      bytes_value = (char *)&res.s32res;
      break;
    default:
      // TODO implement missing conversions
      return NULL;
    }
  }
  else
  {
    switch (value.type)
    {
    case UINT8:
      bytes_value = (char *)&value.uint8_value;
      break;
    case UINT16:
      bytes_value = (char *)&value.uint16_value;
      break;
    case UINT32:
      bytes_value = (char *)&value.uint32_value;
      break;
    case UINT64:
      bytes_value = (char *)&value.uint64_value;
      break;
    case INT8:
      bytes_value = (char *)&value.int8_value;
      break;
    case INT16:
      bytes_value = (char *)&value.int16_value;
      break;
    case INT32:
      bytes_value = (char *)&value.int32_value;
      break;
    case INT64:
      bytes_value = (char *)&value.int64_value;
      break;
    default :
      return NULL;
      break;
    }
  }

  packet->content[packet->size++] = field.key;
  packet->content[packet->size++] = field.len;
  // We insert backward because of endianess
  for (int i = 0; i < field.len; i++)
    packet->content[packet->size++] = bytes_value[field.len - i - 1];

  return packet;
}
