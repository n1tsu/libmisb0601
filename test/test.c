#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>

#include "packet.h"
#include "unpack.h"

uint64_t get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

int main()
{
    // Create a packet
    struct Packet *packet = initialize_packet();
    uint64_t unix_time = get_timestamp();
    printf("UNIX TIMESTAMP is : %lu\n", unix_time);

    printf("Create KLV from it\n\n");
    packet = add_klv(packet, UNIX_TIME_STAMP, 8, (uint8_t *)&unix_time, 1);

    char mission[9] = "MISSION01";
    printf("MISSION ID is : %s\n", mission);

    printf("Create KLV from it\n\n");
    packet = add_klv(packet, MISSION_ID, 9, (uint8_t *)mission, 0);

    finalize_packet(packet);

    // Save into a file
    FILE * file;
    file = fopen("klv_test.bin", "w+");
    if(!file)
    {
        fprintf(stderr, "Unable to create file.\n");
        return 1;
    }
    fwrite(packet->content, 1, packet->size, file);
    fclose(file);

    // Unpack the packet
    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));

    for (int i = 0; i < 94; i++)
        klvmap->KLVs[i] = NULL;

    int res = unpack_misb(packet->content, packet->size, klvmap);
    if (res)
      fprintf(stderr, "Error unpacking the packet : %d\n", res);

    return 0;
}
