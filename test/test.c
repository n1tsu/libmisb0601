#include <time.h>
#include <stdio.h>

#include "packet.h"

int main()
{
    struct Packet *packet = initialize_packet();
    unsigned long unix_time = (unsigned long)time(NULL);
    printf("UNIX TIMESTAMP is : %lun\n", unix_time);

    printf("Create KLV from it\n");
    packet = add_klv(packet, UNIX_TIME_STAMP, 8, (uint8_t *)unix_time);

    for (size_t i = 0 ; i < packet->size; i++)
    {
        printf("%hhx", packet->content[i]);
        if (i % 2)
            printf(" ");
    }

    return 0;
}
