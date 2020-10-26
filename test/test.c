#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>

#include "packet.h"

uint64_t get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

int main()
{
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
    int space = 0;
    int line = 0;

    printf("\n");
    for (size_t i = 0 ; i < packet->size; i++)
    {
        space += 1;
        line += 1;
        printf("%02hhx", packet->content[i]);
        if (space == 2)
        {
            printf(" ");
            space = 0;
        }
        if (line == 8)
        {
            printf("\n");
            line = 0;
        }
    }
    printf("\n");
    printf("Size is : %ld\n", packet->size);

    return 0;
}
