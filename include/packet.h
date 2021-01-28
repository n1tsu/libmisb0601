#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "keys.h"

/**
 *  UAS LDS packet containing MISB0601 KLVs.
 * 
 *  Representation = [LDS Universal Key : Length : MISB0601 KLVs]
 *
 *  `content` is the bytes contained within the packet.
 *
 *  `size` represent the total size of the packet, including
 *  LDS Universal Key and Length field. It might be used to
 *  iterate the content of the packet.
 *
 *  `available_size` is the remaining allocated memory not used.
 *
 *  `ready` flag indicates if the packet is ready to be sent or not.
 *  Call `finalize_packet()` to set this flag.
 *  If this flag is set, it is not possible to add KLV to the packet.
 */
struct Packet {
  uint8_t *content;
  size_t available_size;
  size_t size;
  uint8_t ready;
};

/**
 *  Initialize a packet structure.
 *  It is caller responsibility to free the data after use.
 */
struct Packet *initialize_packet();

/**
 *  Release the memory allocated during initialization
 */
void free_packet(struct Packet *packet);

/**
 *  Finalize the packet.
 *
 *  Add inside the `packet`, in this order :
 *    - LDS Universal Key and LDS Length
 *    - Checksum KLV
 *
 *  These data are mandatories but needs the final size of
 *  the packet to be added. It will return 1 if it failed.
 */
int finalize_packet(struct Packet *packet);

/**
 *
 *  Add KLV metadata frame inside the packet.
 *
 *  Representation : [Key (Tag) | Length | Value]
 *
 *  `id` is the tag of the metadata, MISB0601 tags can be found
 *  inside the file `keys.h`
 *
 *  `packet` is the packet into the KLV while be added.
 *
 *  `value_length` represents the size of the value.
 *  Refer to the documentation to know length.
 *
 *  `value` is the bytes of the value.
 *
 *  `invert` is a flag to determine if we want to store
 *  the value inverting the bytes.
 *
 *  return NULL on error.
 */
struct Packet *add_klv(struct Packet *packet, const struct Field field,
                       struct GenericValue value);

// For KLV that contains a variable size (example : Mission ID with a string)
struct Packet *add_variable_klv(struct Packet *packet, const struct Field field,
                                struct GenericValue value, size_t size);

#endif /* !PACKET_H */
