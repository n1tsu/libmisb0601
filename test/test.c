#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "packet.h"
#include "unpack.h"
#include "utils.h"

int main()
{
    // Create a packet
    struct Packet *packet = initialize_packet();

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

    printf("Unpacking :\n");
    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));

    for (int i = 0; i < 94; i++)
        klvmap->KLVs[i] = NULL;

    int res = unpack_misb(packet->content, packet->size, klvmap);
    if (res)
      fprintf(stderr, "Error unpacking the packet : %d\n", res);

    for (int i = 0; i < 94; i++)
    {
        if (klvmap->KLVs[i])
            printf("Tag %d - Size %zu\n", klvmap->KLVs[i]->tag, klvmap->KLVs[i]->size);
    }

    free_packet(packet);
    free(klvmap);

    // Test utils conversions
    printf("\nTest conversion\n");
    printf("Encoding -61.88693\n");
    printf("%u\n", signed_dec_to_int16(-61.88693, 360));

    printf("Decoding 0xD3FE\n");
    printf("%f\n", int16_to_signed_dec(0xD3FE, 360));

    return 0;
}
