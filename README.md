# MISB0601 library

C library to encode and decode MISB STD 0601.

## Encode

Usage :

```c
#include "packet.h"

// Initialize packet (add mandatories KLVs)
struct Packet *packet = initialize_packet();

// Instantiate values with union
char *name = "MISSION01";
struct GenericValue mission_value = {CHAR_P, .charp_value = name};
float slant_range = 823.20;
struct GenericValue range_value = {FLOAT, .float_value = slant_range};

// Add KLVs into packet
packet = add_klv(packet, FieldMap[MISSION], mission_value);
packet = add_klv(packet, FieldMap[SLANT_RANGE], range_value);

// Finalize packet by adding checksum and total length
finalize_packet(packet);

// We can then access data with `packet->content` and `packet->size`
```

## Decode

Usage :

``` c
#include "unpack.h"

// Initialize the map that will hold the KLVs
struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));

// Trying to unpack the misb, check header to see error code
if (unpack_misb(data, size, klvmap))
  fprintf(stderr, "Error unpacking the packet : %d\n", res);

// Iterating over the map to retrieve KLVs
for (int i = 0; i < 94; i++)
{
  if (klvmap->KLVs[i])
    printf("Tag %d - Size %ld\n", klvmap->KLVs[i]->tag, klvmap->KLVs[i]->size);
}

// Or access wanted klv directly
struct KLV timestamp_klv = klvmap[UNIX_TIME_STAMP];

// Don't forget to free klvs when you are done
free(klvmap);
```

[Standard Documentation](https://gwg.nga.mil/misb/docs/standards/ST0601.6.pdf)
