#include <criterion/criterion.h>

#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "packet.h"
#include "unpack.h"
#include "utils.h"

/**************/
/* CONVERSION */
/**************/

Test(Conversion, signed_dec_to_int16)
{
    float to_convert = -61.88693;
    int16_t expected = (int16_t)0xD3FE;

    int16_t result = signed_dec_to_int16(to_convert, 360);
    cr_assert_eq(result, expected, "got 0x%x and expected 0x%x",
                 result, expected);
}

Test(Conversion, int16_to_signed_dec)
{
    int16_t to_convert = (int16_t)0xD3FE;
    float expected = -61.88693;

    float result = int16_to_signed_dec(to_convert, 360);
    cr_assert_float_eq(result, expected, 0.1, "got %f and expected %f",
                       result, expected);
}

Test(Conversion, unsigned_dec_to_int16)
{
    float to_convert = 14190.72;
    uint16_t expected = 0xC220;

    uint16_t result = unsigned_dec_to_int16(to_convert, 19900, 900);
    cr_assert_eq(result, expected, "got 0x%x and expected 0x%x",
                 result, expected);
}

Test(Conversion, int16_to_unsigned_dec)
{
    uint16_t to_convert = 0xC221;
    float expected = 14190.72;

    float result = int16_to_unsigned_dec(to_convert, 19900, 900);
    cr_assert_float_eq(result, expected, 0.3, "got %f and expected %f",
                       result, expected);
}

Test(Conversion, signed_dec_to_int32)
{
    float to_convert = 60.1768229669783;
    int32_t expected = (int32_t)0x5595B666;

    int32_t result = signed_dec_to_int32(to_convert, 180);
    cr_assert_eq(result, expected, "got 0x%x and expected 0x%x",
                 result, expected);
}

Test(Conversion, int32_to_signed_dec)
{
    int32_t to_convert = (int32_t)0x5595B66D;
    float expected = 60.1768229669783;

    int32_t result = int32_to_signed_dec(to_convert, 180);
    cr_assert_float_eq(result, expected, 0.3, "got %f and expected %f",
                       result, expected);
}


/***********/
/* PACKETS */
/***********/

Test(Packets, initialize)
{
    struct Packet *packet = initialize_packet();
    cr_assert_not_null(packet);
    cr_assert_eq(packet->ready, 0);
    // UAS LDS version and timestamp
    cr_assert_eq(packet->size, 3 + 10, "got %d and expected %d",
                 packet->size, 3 + 10);
    free_packet(packet);
}

Test(Packets, finalize)
{
    struct Packet *packet = initialize_packet();
    int result = finalize_packet(packet);
    cr_assert_eq(result, 0);
    cr_assert_eq(packet->ready, 1);
    // Initialize packet + LDS Key and checksum
    cr_assert_eq(packet->size, 13 + 4 + 17, "got %d and expected %d",
                 packet->size, 13 + 4 + 17);
    free_packet(packet);
}

Test(Packets, add_klv)
{
    struct Packet *packet = initialize_packet();
    uint64_t timestamp = get_timestamp();

    struct GenericValue value;
    value.type = UINT64;
    value.uint64_value = timestamp;

    packet = add_klv(packet, FieldMap[UNIX_TIME_STAMP], value);
    cr_assert_not_null(packet);
    free_packet(packet);
}

Test(Packets, save_file)
{
    struct Packet *packet = initialize_packet();

    {
      struct GenericValue value = {FLOAT, .float_value = 159.9744};
      packet = add_klv(packet, FieldMap[PLATFORM_HEADING_ANGLE], value);
    }

    {
      char *name = "MISSION01";
      struct GenericValue value = {CHAR_P, .charp_value = name};
      packet = add_klv(packet, FieldMap[MISSION_ID], value);
    }

    int result = finalize_packet(packet);
    cr_assert_eq(result, 0);

    FILE * file;
    file = fopen("klv_test.bin", "w+");
    if(!file)
    {
        fprintf(stderr, "Unable to create file.\n");
        return;
    }
    fwrite(packet->content, 1, packet->size, file);
    fclose(file);
    free_packet(packet);
}



/**********/
/* UNPACK */
/**********/

/*
Test(Unpack, valid_unpack)
{
    struct Packet *packet = initialize;
    int result = unpack_misb(data, size, kilvmap);
}
*/

/*
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

void save_file(struct Packet *packet)
{

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
*/
