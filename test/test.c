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

Test(Unpack, valid_unpack)
{
    struct Packet *packet = initialize_packet();

    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));
    finalize_packet(packet);

    int result = unpack_misb(packet->content, packet->size, klvmap);

    cr_assert_eq(result, 0, "got %d and expected %d",
                 result, 0);
    cr_assert_eq(klvmap->KLVs[CHECKSUM]->tag, CHECKSUM, "got %d and expected %d",
                 klvmap->KLVs[CHECKSUM]->tag, CHECKSUM);

    free_packet(packet);
    free(klvmap);
}

Test(Unpack, unpack_version)
{
    struct Packet *packet = initialize_packet();

    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));
    finalize_packet(packet);

    int result = unpack_misb(packet->content, packet->size, klvmap);

    cr_assert_eq(result, 0, "got %d and expected %d",
                 result, 0);
    cr_assert_eq(klvmap->KLVs[UAS_LDS_VERSION_NUMBER]->tag, UAS_LDS_VERSION_NUMBER, "got %d and expected %d",
                 klvmap->KLVs[UAS_LDS_VERSION_NUMBER]->tag, UAS_LDS_VERSION_NUMBER);
    cr_assert_eq(klvmap->KLVs[UAS_LDS_VERSION_NUMBER]->value.uint8_value, 6, "got %d and expected %d",
                 klvmap->KLVs[UAS_LDS_VERSION_NUMBER]->value.uint8_value, 6);

    free_packet(packet);
    free(klvmap);
}

Test(Unpack, unpack_present)
{
    struct Packet *packet = initialize_packet();

    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));
    finalize_packet(packet);

    int result = unpack_misb(packet->content, packet->size, klvmap);

    cr_assert_eq(result, 0, "got %d and expected %d",
                 result, 0);
    cr_assert_neq(klvmap->KLVs[UAS_LDS_VERSION_NUMBER], NULL, "got %d and not expected %d",
                 klvmap->KLVs[UAS_LDS_VERSION_NUMBER], NULL);
    cr_assert_eq(klvmap->KLVs[SLANT_RANGE], NULL, "got %d and expected %d",
                 klvmap->KLVs[SLANT_RANGE], NULL);

    free_packet(packet);
    free(klvmap);
}


Test(Unpack, unpack_mission)
{
    struct Packet *packet = initialize_packet();

    char *name = "MISSION01";
    struct GenericValue value = {CHAR_P, .charp_value = name};
    packet = add_klv(packet, FieldMap[MISSION_ID], value);
    finalize_packet(packet);

    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));
    int result = unpack_misb(packet->content, packet->size, klvmap);

    cr_assert_eq(result, 0, "got %d and expected %d",
                 result, 0);
    cr_assert_eq(klvmap->KLVs[MISSION_ID]->tag, MISSION_ID, "got %d and expected %d",
                 klvmap->KLVs[MISSION_ID]->tag, MISSION_ID);
    cr_assert_str_eq(klvmap->KLVs[MISSION_ID]->value.charp_value, "MISSION01", "got %s and expected %s",
                     klvmap->KLVs[MISSION_ID]->value.charp_value, "MISSION01");
    cr_assert_eq(klvmap->KLVs[MISSION_ID]->size, 9, "got %s and expected %s",
                 klvmap->KLVs[MISSION_ID]->size, 9);

    free_packet(packet);
    free(klvmap);
}

Test(Unpack, unpack_slant)
{
    struct Packet *packet = initialize_packet();

    float slant_range = 823.20;
    struct GenericValue value = {FLOAT, .float_value = slant_range};
    packet = add_klv(packet, FieldMap[SLANT_RANGE], value);
    finalize_packet(packet);

    struct KLVMap *klvmap = malloc(sizeof(struct KLVMap));
    int result = unpack_misb(packet->content, packet->size, klvmap);

    cr_assert_eq(result, 0, "got %d and expected %d",
                 result, 0);
    cr_assert_eq(klvmap->KLVs[SLANT_RANGE]->tag, SLANT_RANGE, "got %d and expected %d",
                 klvmap->KLVs[SLANT_RANGE]->tag, SLANT_RANGE);
    cr_assert_float_eq(klvmap->KLVs[SLANT_RANGE]->value.float_value, 0.1, 823.20, "got %f and expected %f",
                       klvmap->KLVs[SLANT_RANGE]->value.float_value, 0.1, 823.20);
    cr_assert_eq(klvmap->KLVs[SLANT_RANGE]->size, 4, "got %s and expected %s",
                 klvmap->KLVs[SLANT_RANGE]->size, 4);

    free_packet(packet);
    free(klvmap);
}
