#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>


static const uint8_t LDS_UNIVERSAL_KEY[16] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x0b, 0x01, 0x01, 0x0e, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00};


enum Format {
  UINT8,
  UINT16,
  UINT32,
  UINT64,
  INT8,
  INT16,
  INT32,
  INT64,
  FLOAT,
  DOUBLE,
  CHAR_P,
};


struct GenericValue {
  enum Format type;
  union {
    uint8_t uint8_value;
    uint16_t uint16_value;
    uint32_t uint32_value;
    uint64_t uint64_value;
    int8_t int8_value;
    int16_t int16_value;
    int32_t int32_value;
    int64_t int64_value;
    float float_value;
    double double_value;
    char *charp_value;
  };
};


struct Range {
  struct GenericValue min;
  struct GenericValue max;
};


struct Field {
  uint8_t key;
  enum Format encoded_format;
  enum Format value_format;
  uint8_t len;
  struct Range range;
};


// All KLVs tags from the MISB0601
enum Tags {
  CHECKSUM = 1,
  UNIX_TIME_STAMP = 2,
  MISSION_ID = 3,
  PLATFORM_TAIL_NUMBER = 4,
  PLATFORM_HEADING_ANGLE = 5,
  PLATFORM_PITCH_ANGLE = 6,
  PLATFORM_ROLL_ANGLE = 7,
  PLATFORM_TRUE_AIRSPEED = 8,
  PLATFORM_INDICATED_AIRSPEED = 9,
  PLATFORM_DESIGNATION = 10,
  IMAGE_SOURCE_SENSOR = 11,
  IMAGE_COORDINATE_SYSTEM = 12,
  SENSOR_LATITUDE = 13,
  SENSOR_LONGITUDE = 14,
  SENSOR_TRUE_ALTITUDE = 15,
  SENSOR_HORIZONTAL_FOV = 16,
  SENSOR_VERTICAL_FOV = 17,
  SENSOR_RELATIVE_AZIMUTH_ANGLE = 18,
  SENSOR_RELATIVE_ELEVATION_ANGLE = 19,
  SENSOR_RELATIVE_ROLL_ANGLE = 20,
  SLANT_RANGE = 21,
  TARGET_WIDTH = 22,
  FRAME_CENTER_LATITUDE = 23,
  FRAME_CENTER_LONGITUDE = 24,
  FRAME_CENTER_ELEVATION = 25,
  OFFSET_CORNER_LATITUDE_POINT_1 = 26,
  OFFSET_CORNER_LONGITUDE_POINT_1 = 27,
  OFFSET_CORNER_LATITUDE_POINT_2 = 28,
  OFFSET_CORNER_LONGITUDE_POINT_2 = 29,
  OFFSET_CORNER_LATITUDE_POINT_3 = 30,
  OFFSET_CORNER_LONGITUDE_POINT_3 = 31,
  OFFSET_CORNER_LATITUDE_POINT_4 = 32,
  OFFSET_CORNER_LONGITUDE_POINT_4 = 33,
  ICING_DETECTED = 34,
  WIND_DIRECTION = 35,
  WIND_SPEED = 36,
  STATIC_PRESSURE = 37,
  DENSITY_ALTITUDE = 38,
  OUTSIDE_AIR_TEMPERATURE = 39,
  TARGET_LOCATION_LATITUDE = 40,
  TARGET_LOCATION_LONGITUDE = 41,
  TARGET_LOCATION_ELEVATION = 42,
  TARGET_TRACK_GATE_WIDTH = 43,
  TARGET_TRACK_GATE_HEIGHT = 44,
  TARGET_ERROR_ESTIMATE_CE90 = 45,
  TARGET_ERROR_ESTIMATE_LE90 = 46,
  GENERIC_FLAG_DATA1 = 47,
  SECURITY_LOCAL_METADATA_SET = 48,
  DIFFERENTIAL_PRESSURE = 49,
  PLATFORM_ANGLE_OF_ATTACK = 50,
  PLATFORM_VERTICAL_SPEED = 51,
  PLATFORM_SIDESLIP_ANGLE = 52,
  AIRFIELD_BAROMETRIC_PRESSURE = 53,
  AIRFIELD_ELEVATION = 54,
  RELATIVE_HUMIDITY = 55,
  PLATFORM_GROUND_SPEED = 56,
  GROUND_RANGE = 57,
  PLATFORM_FUEL_REMAINING = 58,
  PLATFORM_CALL_SIGN = 59,
  WEAPON_LOAD = 60,
  WEAPON_FIRED = 61,
  LASER_PRF_CODE = 62,
  SENSOR_FOV_NAME = 63,
  PLATFORM_MAGNETIC_HEADING = 64,
  UAS_LDS_VERSION_NUMBER = 65,
  TARGET_LOCATION_COVARIANCE_MATRIX = 66,
  ALTERNATE_PLATFORM_LATITUDE = 67,
  ALTERNATE_PLATFORM_LONGITUDE = 68,
  ALTERNATE_PLATFORM_ALTITUDE = 69,
  ALTERNATE_PLATFORM_NAME = 70,
  ALTERNATE_PLATFORM_HEADING = 71,
  EVENT_START_TIME_UTC = 72,
  RVT_LOCAL_DATA_SET = 73,
  VMTI_LOCAL_DATA_SET = 74,
  SENSOR_ELLIPSOID_HEIGHT = 75,
  ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT = 76,
  OPERATIONAL_MODE = 77,
  FRAME_CENTER_HEIGHT_ABOVE_ELLIPSOID = 78,
  SENSOR_NORTH_VELOCITY = 79,
  SENSOR_EAST_VELOCITY = 80,
  IMAGE_HORIZON_PIXEL_PACK = 81,
  CORNER_LATITUDE_POINT_1 = 82,
  CORNER_LONGITUDE_POINT_1 = 83,
  CORNER_LATITUDE_POINT_2 = 84,
  CORNER_LONGITUDE_POINT_2 = 85,
  CORNER_LATITUDE_POINT_3 = 86,
  CORNER_LONGITUDE_POINT_3 = 87,
  CORNER_LATITUDE_POINT_4 = 88,
  CORNER_LONGITUDE_POINT_4 = 89,
  PLATFORM_PITCH_ANGLE_F = 90,
  PLATFORM_ROLL_ANGLE_F = 91,
  PLATFORM_ANGLE_OF_ATTACK_F = 92,
  PLATFORM_SIDESLIP_ANGLE_F = 93,
};


static const struct Field FieldMap[94] = {
  // First one is dummy to align index with tag number
  {
    .key = CHECKSUM,
    .encoded_format = UINT16,
    .value_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 65535}
    },
  },
  {
    .key = CHECKSUM,
    .encoded_format = UINT16,
    .value_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 65535}
    },
  },
  {
    .key = UNIX_TIME_STAMP,
    .encoded_format = UINT64,
    .value_format = UINT64,
    .len = 8,
    .range = {
      {UINT64, .uint64_value = 0},
      {UINT64, .uint64_value = 18446744073709551615U}
    },
  },
  {
    .key = MISSION_ID,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = PLATFORM_TAIL_NUMBER,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = PLATFORM_HEADING_ANGLE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 360}
    },
  },
  {
    .key = PLATFORM_PITCH_ANGLE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -20},
      {FLOAT, .float_value = 20}
    },
  },
  {
    .key = PLATFORM_ROLL_ANGLE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -50},
      {FLOAT, .float_value = 50}
    },
  },
  {
    .key = PLATFORM_TRUE_AIRSPEED,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = PLATFORM_INDICATED_AIRSPEED,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = PLATFORM_DESIGNATION,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = IMAGE_SOURCE_SENSOR,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = IMAGE_COORDINATE_SYSTEM,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = SENSOR_LATITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = SENSOR_LONGITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = SENSOR_TRUE_ALTITUDE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {DOUBLE, .double_value = -900},
      {DOUBLE, .double_value = 19000}
    },
  },
  {
    .key = SENSOR_HORIZONTAL_FOV,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = SENSOR_VERTICAL_FOV,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = SENSOR_RELATIVE_AZIMUTH_ANGLE,
    .value_format = DOUBLE,
    .encoded_format = UINT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 360}
    },
  },
  {
    .key = SENSOR_RELATIVE_ELEVATION_ANGLE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = SENSOR_RELATIVE_ROLL_ANGLE,
    .value_format = DOUBLE,
    .encoded_format = UINT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 360}
    },
  },
  {
    .key = SLANT_RANGE,
    .value_format = FLOAT,
    .encoded_format = UINT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 5000000}
    },
  },
  {
    .key = TARGET_WIDTH,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 10000}
    },
  },
  {
    .key = FRAME_CENTER_LATITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = FRAME_CENTER_LONGITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = FRAME_CENTER_ELEVATION,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = OFFSET_CORNER_LATITUDE_POINT_1,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LONGITUDE_POINT_1,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LATITUDE_POINT_2,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LONGITUDE_POINT_2,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LATITUDE_POINT_3,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LONGITUDE_POINT_3,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LATITUDE_POINT_4,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = OFFSET_CORNER_LONGITUDE_POINT_4,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -0.075},
      {FLOAT, .float_value = 0.075}
    },
  },
  {
    .key = ICING_DETECTED,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = WIND_DIRECTION,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 360}
    },
  },
  {
    .key = WIND_SPEED,
    .value_format = FLOAT,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 100}
    },
  },
  {
    .key = STATIC_PRESSURE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 5000}
    },
  },
  {
    .key = DENSITY_ALTITUDE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = OUTSIDE_AIR_TEMPERATURE,
    .value_format = INT8,
    .encoded_format = INT8,
    .len = 1,
    .range = {
      {INT8, .int8_value = -128},
      {INT8, .int8_value = 127}
    },
  },
  {
    .key = TARGET_LOCATION_LATITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = TARGET_LOCATION_LONGITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = TARGET_LOCATION_ELEVATION,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = TARGET_TRACK_GATE_WIDTH,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 512}
    },
  },
  {
    .key = TARGET_TRACK_GATE_HEIGHT,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 512}
    },
  },
  {
    .key = TARGET_ERROR_ESTIMATE_CE90,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 4095}
    },
  },
  {
    .key = TARGET_ERROR_ESTIMATE_LE90,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 4095}
    },
  },
  {
    .key = GENERIC_FLAG_DATA1,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 255}
    },
  },
  // TODO really not ready to be used, information are dummies
  {
    .key = SECURITY_LOCAL_METADATA_SET,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 255}
    },
  },
  {
    .key = DIFFERENTIAL_PRESSURE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 5000}
    },
  },
  {
    .key = PLATFORM_ANGLE_OF_ATTACK,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -20},
      {FLOAT, .float_value = 20}
    },
  },
  {
    .key = PLATFORM_VERTICAL_SPEED,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -180},
      {FLOAT, .float_value = 180}
    },
  },
  {
    .key = PLATFORM_SIDESLIP_ANGLE,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -20},
      {FLOAT, .float_value = 20}
    },
  },
  {
    .key = AIRFIELD_BAROMETRIC_PRESSURE,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 5000}
    },
  },
  {
    .key = AIRFIELD_ELEVATION,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = RELATIVE_HUMIDITY,
    .value_format = FLOAT,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 100}
    },
  },
  {
    .key = PLATFORM_GROUND_SPEED,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 255}
    },
  },
  {
    .key = GROUND_RANGE,
    .value_format = DOUBLE,
    .encoded_format = UINT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 5000000}
    },
  },
  {
    .key = PLATFORM_FUEL_REMAINING,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {DOUBLE, .double_value = 0},
      {DOUBLE, .double_value = 10000}
    },
  },
  {
    .key = PLATFORM_CALL_SIGN,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 1},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = WEAPON_LOAD,
    .value_format = UINT16,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 65535}
    },
  },
  {
    .key = WEAPON_FIRED,
    .value_format = UINT16,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 65535}
    },
  },
  {
    .key = LASER_PRF_CODE,
    .value_format = UINT16,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 65535}
    },
  },
  {
    .key = SENSOR_FOV_NAME,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = PLATFORM_MAGNETIC_HEADING,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {UINT16, .uint16_value = 0},
      {UINT16, .uint16_value = 360}
    },
  },
  {
    .key = UAS_LDS_VERSION_NUMBER,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  // Parameters unused in the MISB0601.6
  {
    .key = TARGET_LOCATION_COVARIANCE_MATRIX,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_LATITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_LONGITUDE,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_ALTITUDE,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_NAME,
    .value_format = CHAR_P,
    .encoded_format = CHAR_P,
    .len = 127,
    .range = {
      {UINT8, .uint8_value = 1},
      {UINT8, .uint8_value = 127}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_HEADING,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = 0},
      {FLOAT, .float_value = 360}
    },
  },
  {
    .key = EVENT_START_TIME_UTC,
    .value_format = UINT64,
    .encoded_format = UINT64,
    .len = 4,
    .range = {
      {UINT64, .uint64_value = 0},
      {UINT64, .uint64_value = 18446744073709551615U}
    },
  },
  // TODO this parameter is not handled
  {
    .key = RVT_LOCAL_DATA_SET,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  // TODO this parameter is not handled
  {
    .key = VMTI_LOCAL_DATA_SET,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = SENSOR_ELLIPSOID_HEIGHT,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = OPERATIONAL_MODE,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = FRAME_CENTER_HEIGHT_ABOVE_ELLIPSOID,
    .value_format = FLOAT,
    .encoded_format = UINT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -900},
      {FLOAT, .float_value = 19000}
    },
  },
  {
    .key = SENSOR_NORTH_VELOCITY,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -357},
      {FLOAT, .float_value = 357}
    },
  },
  {
    .key = SENSOR_EAST_VELOCITY,
    .value_format = FLOAT,
    .encoded_format = INT16,
    .len = 2,
    .range = {
      {FLOAT, .float_value = -357},
      {FLOAT, .float_value = 357}
    },
  },
  {
    .key = IMAGE_HORIZON_PIXEL_PACK,
    .value_format = UINT8,
    .encoded_format = UINT8,
    .len = 1,
    .range = {
      {UINT8, .uint8_value = 0},
      {UINT8, .uint8_value = 255}
    },
  },
  {
    .key = CORNER_LATITUDE_POINT_1,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = CORNER_LONGITUDE_POINT_1,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = CORNER_LATITUDE_POINT_2,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = CORNER_LONGITUDE_POINT_3,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = CORNER_LATITUDE_POINT_4,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = CORNER_LONGITUDE_POINT_4,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = CORNER_LATITUDE_POINT_1,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = CORNER_LONGITUDE_POINT_1,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  },
  {
    .key = PLATFORM_PITCH_ANGLE_F,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = PLATFORM_ROLL_ANGLE_F,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = PLATFORM_ANGLE_OF_ATTACK,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -90},
      {DOUBLE, .double_value = 90}
    },
  },
  {
    .key = PLATFORM_SIDESLIP_ANGLE_F,
    .value_format = DOUBLE,
    .encoded_format = INT32,
    .len = 4,
    .range = {
      {DOUBLE, .double_value = -180},
      {DOUBLE, .double_value = 180}
    },
  }
};


#endif /* !KEYS_H */
