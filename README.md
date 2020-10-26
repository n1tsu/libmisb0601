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

[Standard Documentation](https://gwg.nga.mil/misb/docs/standards/ST0601.6.pdf)
