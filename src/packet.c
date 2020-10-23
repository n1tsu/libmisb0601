#include "packet.h"

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

  return packet;
}

int finalize_packet(struct Packet *packet)
{
  // We need to add Checksum KLV last since it needs
  // to checksum the whole frame. We know that the KLV
  // frame is 4 bytes long. (1 for tag, 1 for length,
  // 2 for value).
  packet->size += 1 + 1 + 2;
  int new_size = packet->size;

  // This buffer will contains Key and Length of the LDS
  // frame, since we will insert it at the beginning of the
  // KLV, we need to populate another buffer.
  uint8_t *LDS_KL = NULL;
  int LDS_KL_size = 0;

  // TODO : Refactorize this part of code

  // LDS length field size is determined via BER short/long
  // encoding.
  if (new_size <= 127) {
    // It is a short BER encoding
    LDS_KL = malloc((new_size + 16 + 1) * sizeof(uint8_t));
    memcpy(LDS_KL, LDS_UNIVERSAL_KEY, 16);
    LDS_KL[16] = (uint8_t)new_size;
    LDS_KL_size = 16 + 1;
  }
  else {
    // It is a long BER encoding
    // We set first bit of first byte
    uint8_t first_length_byte = 1 << 7;

    if (new_size <= 255) {
      first_length_byte += 1;
      LDS_KL = malloc((new_size + 16 + 2) * sizeof(uint8_t));
      memcpy(LDS_KL, LDS_UNIVERSAL_KEY, 16);
      LDS_KL[16] = first_length_byte;
      LDS_KL[17] = (uint8_t)new_size;
      LDS_KL_size = 16 + 2;
    }
    else {
      // Assuming that we will need 2 bytes at most
      first_length_byte += 2;
      LDS_KL = malloc((new_size + 16 + 3) * sizeof(uint8_t));
      memcpy(LDS_KL, LDS_UNIVERSAL_KEY, 16);
      LDS_KL[16] = first_length_byte;
      LDS_KL[17] = (uint8_t)new_size >> 8;
      LDS_KL[18] = (uint8_t)new_size;
      LDS_KL_size = 16 + 3;
    }
  }

  // Move packet content after LDS KL memory
  packet->content = memmove(LDS_KL + LDS_KL_size, packet->content, packet->size);
  if (!packet->content)
    return 1;

  packet->size += LDS_KL_size;

  // Checksum KLV needs to be calculated and added
  packet->content[packet->size - 4] = CHECKSUM;
  packet->content[packet->size - 3] = 2;

  short checksum = bcc_16(packet->content, packet->size - 3);
  packet->content[packet->size - 2] = (uint8_t)checksum >> 8;
  packet->content[packet->size - 1] = (uint8_t)checksum;

  return 0;
}

struct Packet *add_klv(struct Packet *packet, enum Tags id,
                       uint8_t value_length, uint8_t *value)
{
  // The tag field length follow BER-OID encoding to encode the length
  // of the KLV. Since in MISB0601 max tag value is 93, and is under
  // 2**7 - 1 = 127, we know that he length will be encoded in 1 byte.
  // Value length is assumed to be coded in 1 byte.

  if (packet->available_size <= packet->size)
  {
    packet->content = realloc(packet->content, packet->available_size * 2);
    if (!packet->content)
      return NULL;

    packet->available_size = packet->available_size * 2;
  }

  packet->content[packet->size++] = id;
  packet->content[packet->size++] = value_length;
  for (int i = 0; i < value_length; i++)
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
