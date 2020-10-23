#pragma once

#include <stddef.h>
#include <stdlib.h>
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
  char *content;
  size_t available_size;
  size_t size;
  char ready;
};

/**
 *  Initialize a packet structure.
 *  It is caller responsibility to free the data after use.
 */
struct Packet *initialize_packet();

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
 *
 *  `value` is the bytes of the value.
 */
struct Packet *add_klv(struct Packet *packet, enum Tags id, int value_length, char *value);
