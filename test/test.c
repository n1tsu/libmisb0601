#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "packet.h"
#include "unpack.h"
#include "utils.h"

static char mission[9] = "MISSION01";
static double latitude = 60.1768229669783;
static double longitude = 128.426759042045;
static double altitude = 14190.72;


void test_unpack(struct KLVMap *klvmap)
{
    for (int i = 0; i < 94; i++)
    {
        if (klvmap->KLVs[i])
        {
            printf("Tag %d - Size %zu\n", klvmap->KLVs[i]->tag, klvmap->KLVs[i]->size);
            int data;
            unsigned short s_data;
            switch (i)
            {
            case 13:
              data = (*klvmap->KLVs[i]->data << 24) + (*(klvmap->KLVs[i]->data + 1) << 16) +
                     (*(klvmap->KLVs[i]->data + 2) << 8) + *(klvmap->KLVs[i]->data + 3);
              printf("  * Sensor latitude is %f\n", int32_to_signed_dec(data, 180));
              printf("  * Expected is %f\n", latitude);
              break;
            case 14:
              data = (*klvmap->KLVs[i]->data << 24) + (*(klvmap->KLVs[i]->data + 1) << 16) +
                     (*(klvmap->KLVs[i]->data + 2) << 8) + *(klvmap->KLVs[i]->data + 3);
              printf("  * Sensor longitude is %f\n", int32_to_signed_dec(data, 360));
              printf("  * Expected is %f\n", longitude);
              break;
            case 15:
              s_data =  (*klvmap->KLVs[i]->data << 8) + *(klvmap->KLVs[i]->data + 1);
              printf("  * Sensor altitude is %f\n", int16_to_unsigned_dec(s_data, 19900, 900));
              printf("  * Expected is %f\n", altitude);
              break;
            default:
              break;
            }
        }
    }
}


void test_conversion()
{
    // Test utils conversions
    printf("\nTest conversion\n");
    printf("Encoding -61.88693 : ");
    printf("0x%x\n", signed_dec_to_int16(-61.88693, 360));

    printf("Decoding 0xD3FE : ");
    printf("%f\n\n", int16_to_signed_dec(0xD3FE, 360));

    printf("Encoding 14190.72 : ");
    printf("0x%x\n", unsigned_dec_to_int16(14190.72, 19900, 900));

    printf("Decoding 0xC221 : ");
    printf("%f\n\n", int16_to_unsigned_dec(0xC221, 19900, 900));

    printf("Encoding 60.1768229669783 : ");
    printf("0x%x\n", signed_dec_to_int32(60.1768229669783, 180));

    printf("Decoding 0x5595B66D : ");
    printf("%f\n", int32_to_signed_dec(0x5595B66D, 180));
}


void save_file(struct Packet *packet)
{
    FILE * file;
    file = fopen("klv_test.bin", "w+");
    if(!file)
    {
        fprintf(stderr, "Unable to create file.\n");
        return;
    }
    fwrite(packet->content, 1, packet->size, file);
    fclose(file);
}


struct Packet *add_klvs(struct Packet *packet)
{
    printf("KLVS :\n");
    printf("Mission name is %s\n", mission);
    printf("Latitude is %f\n", latitude);
    printf("Longitude is %f\n", longitude);
    printf("Altitude is %f\n", altitude);

    packet = add_klv(packet, MISSION_ID, 9, (uint8_t *)mission, 0);
    int encoded_latitude_degree = signed_dec_to_int32(latitude, 180);
    packet = add_klv(packet, SENSOR_LATITUDE, 4, (uint8_t*)&encoded_latitude_degree, 1);
    int encoded_longitude_degree = signed_dec_to_int32(longitude, 360);
    packet = add_klv(packet, SENSOR_LONGITUDE, 4, (uint8_t*)&encoded_longitude_degree, 1);
    unsigned short encoded_altitude = unsigned_dec_to_int16(altitude, 19900, 900);
    packet = add_klv(packet, SENSOR_TRUE_ALTITUDE, 2, (uint8_t*)&encoded_altitude, 1);

    return packet;
}


int main()
{
    // Create a packet
    struct Packet *packet = initialize_packet();
    packet = add_klvs(packet);
    finalize_packet(packet);

    // Save into file
    save_file(packet);

    // Unpack the packet
    printf("\nUnpacking :\n");
    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));

    for (int i = 0; i < 94; i++)
        klvmap->KLVs[i] = NULL;

    int res = unpack_misb(packet->content, packet->size, klvmap);
    if (res)
      fprintf(stderr, "Error unpacking the packet : %d\n", res);

    test_unpack(klvmap);

    free_packet(packet);
    free(klvmap);

    test_conversion();

    return 0;
}
