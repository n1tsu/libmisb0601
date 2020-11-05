# MISB0601 library

C library to encode and decode MISB STD 0601.

## Encode

Usage :

```c
#include "packet.h"

// Initialize an UDS packet
struct Packet *packet = initialize_packet();

// Get UNIX timestamp
unsigned long unix_time = (uint64_t)time(NULL);

// Add Timestamp KLV to packet
// See packet.h to understand parameters
packet = add_klv(packet, UNIX_TIME_STAMP, 8, (uint8_t *)&unix_time, 1);

// Finalize packet to be ready to being sent
finalize_packet(packet);

// We can then access data with `packet->content` and `packet->size`
```

## Decode

Usage :

``` c
#include "unpack.h"

// Initialize the map that will hold the KLVs
struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));

// Set all pointers to NULL
for (int i = 0; i < 94; i++)
    klvmap->KLVs[i] = NULL;

// Trying to unpack the misb, check header to see error code
int res = unpack_misb(data, size, klvmap);
if (res)
    fprintf(stderr, "Error unpacking the packet : %d\n", res);

// Iterating over the map to retrieve KLVs
for (int i = 0; i < 94; i++)
{
    if (klvmap->KLVs[i])
        printf("Tag %d - Size %ld\n", klvmap->KLVs[i]->tag, klvmap->KLVs[i]->size);
}

free(klvmap);
```

[Standard Documentation](https://gwg.nga.mil/misb/docs/standards/ST0601.6.pdf)
