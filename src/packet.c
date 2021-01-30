#include "packet.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// This routine is creating a new packet with Universal Key, final size of the
// packet, and allocate enough memory to contains `new_size` klv data.
uint8_t *create_final_packet(int len_size, int new_size)
{
  int key_size = 16;
  int LDS_KL_size = key_size + len_size;
  uint8_t *LDS_KL = malloc((new_size + LDS_KL_size) * sizeof(uint8_t));
  memcpy(LDS_KL, LDS_UNIVERSAL_KEY, 16);

  if (len_size == 1)
    LDS_KL[key_size] = (uint8_t)new_size;
  else if (len_size == 2)
  {
    LDS_KL[key_size] = (1 << 7) + 1;
    LDS_KL[key_size + 1] = (uint8_t)new_size;
  }
  else
  {
    LDS_KL[key_size] = (1 << 7) + 2;
    LDS_KL[key_size + 1] = (uint8_t)new_size >> 8;
    LDS_KL[key_size + 2] = (uint8_t)new_size;
   }

  return LDS_KL;
}


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
  packet = add_klv(packet, FieldMap[UNIX_TIME_STAMP], unix_time_stamp_value);

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


  uint8_t *LDS_KL = create_final_packet(LDS_L_size, new_size);

  // Copy packet content into final packet (after Universal Key and size)
  int LDS_KL_size = LDS_K_size + LDS_L_size;
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


char *encode_value(const struct Field field, struct GenericValue *value)
{
  union Offset {
    float foffset;
    double doffset;
  } offset;

  // Conversion if needed
  if (field.value_format != field.encoded_format)
  {
    switch (field.encoded_format)
    {
    case UINT16:
      offset.foffset = (field.range.min.float_value < 0) ? -field.range.min.float_value : 0;
      value->uint16_value = unsigned_dec_to_int16(value->float_value,
                                                  fabs(field.range.min.float_value) +
                                                  fabs(field.range.max.float_value),
                                                  offset.foffset);
      return (char*)&value->uint16_value;
    case UINT32:
      offset.doffset = (field.range.min.float_value < 0) ? -field.range.min.float_value : 0;
      value->uint32_value = unsigned_dec_to_int32(value->double_value,
                                                  fabs(field.range.min.double_value) +
                                                  fabs(field.range.max.double_value),
                                                  offset.doffset);
      return (char*)&value->uint32_value;
    case INT16:
      value->int16_value = signed_dec_to_int16(value->float_value,
                                               fabs(field.range.min.float_value) +
                                               fabs(field.range.max.float_value));
      return (char*)&value->int16_value;

    case INT32:
       value->int32_value = signed_dec_to_int16(value->double_value,
                                                fabs(field.range.min.double_value) +
                                                fabs(field.range.max.double_value));
       return (char*)&value->int32_value;
    default:
      // TODO implement missing conversions
      return NULL;
    }
  }
  else
  {
    switch (value->type)
    {
    case UINT8:
      return (char*)&value->uint8_value;
    case UINT16:
      return (char*)&value->uint16_value;
    case UINT32:
      return (char*)&value->uint32_value;
    case UINT64:
      return (char*)&value->uint64_value;
    case INT8:
      return (char*)&value->int8_value;
    case INT16:
      return (char*)&value->int16_value;
    case INT32:
      return (char*)&value->int32_value;
    case INT64:
      return (char*)&value->int64_value;
    case CHAR_P:
      return value->charp_value;
    default :
      return NULL;
    }
  }
}



struct Packet *add_klv(struct Packet *packet, const struct Field field,
                       struct GenericValue value)
{
  uint8_t field_len = field.len;
  if (value.type == CHAR_P)
    field_len = strlen(value.charp_value);

  if (field.value_format != value.type)
    return NULL;

  // Increase allocated size if necessary
  if (packet->available_size <= packet->size + 2 + field_len)
  {
    packet->content = realloc(packet->content, packet->available_size * 2);
    if (!packet->content)
      return NULL;

    packet->available_size = packet->available_size * 2;
  }

  char *bytes_value = encode_value(field, &value);
  if (!bytes_value)
    return NULL;

  packet->content[packet->size++] = field.key;
  packet->content[packet->size++] = field_len;
  for (int i = 0; i < field_len; i++)
  {
    // Because of endianess we insert backward, except when dealing with char*
    if (value.type != CHAR_P)
      packet->content[packet->size++] = bytes_value[field_len - i - 1];
    else
    {
      packet->content[packet->size++] = bytes_value[i];
      if (bytes_value[i] == '\0')
        break;
    }
  }

  return packet;
}
