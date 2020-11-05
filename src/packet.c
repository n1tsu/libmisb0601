#include "packet.h"
#include <stdio.h>

const uint8_t LDS_UNIVERSAL_KEY[16] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x0b, 0x01, 0x01, 0x0e, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00};

// Checksum function from MISB0601 documentation
unsigned short bcc_16(uint8_t *buff, unsigned short len);

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

struct Packet *add_klv(struct Packet *packet, enum Tags id,
                       uint8_t value_length, uint8_t *value,
                       uint8_t invert)
{
  // The tag field length follow BER-OID encoding to encode the length
  // of the KLV. Since in MISB0601 max tag value is 93, and is under
  // 2**7 - 1 = 127, we know that the length will be encoded in 1 byte.
  // Value length is assumed to be coded in 1 byte.

  if (packet->available_size <= packet->size + 2 + value_length)
  {
    packet->content = realloc(packet->content, packet->available_size * 2);
    if (!packet->content)
      return NULL;

    packet->available_size = packet->available_size * 2;
  }

  packet->content[packet->size++] = id;
  packet->content[packet->size++] = value_length;
  for (int i = 0; i < value_length; i++)
    if (invert)
      packet->content[packet->size++] = value[value_length - i - 1];
    else
      packet->content[packet->size++] = value[i];

  return packet;
}

// `buff` is a pointer to the first byte in the 16-byte UAS LDS key.
// `len` is the length from 16-byte UDS key up to 1-byte checksum length.
unsigned short bcc_16(uint8_t *buff, unsigned short len)
{
  // Initialize Checksum and counter variables.
  unsigned short bcc = 0, i;

  // Sum each 16-bit chunk whitin the buffer into a checksum
  for (i = 0; i < len; i++)
    bcc += buff[i] << (8 * ((i + 1) % 2));
  return bcc;
} // end of bcc_16()
