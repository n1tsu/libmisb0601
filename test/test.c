#include <time.h>
#include <stdio.h>

#include "packet.h"

int main()
{
    struct Packet *packet = initialize_packet();
    unsigned long unix_time = (uint64_t)time(NULL);
    printf("UNIX TIMESTAMP is : %lun\n", unix_time);

    printf("Create KLV from it\n\n");
    packet = add_klv(packet, UNIX_TIME_STAMP, 8, (uint8_t *)&unix_time);

    char mission[9] = "MISSION01";
    printf("MISSION ID is : %s\n", mission);

    printf("Create KLV from it\n\n");
    packet = add_klv(packet, MISSION_ID, 9, (uint8_t *)mission);

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

    return 0;
}
