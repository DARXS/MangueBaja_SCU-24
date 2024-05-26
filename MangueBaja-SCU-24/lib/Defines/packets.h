#ifndef PACKETS_H
#define PACKETS_H

#include <stdio.h>
#include <string.h>

#define MB1_ID  11
#define MB2_ID  22

typedef struct
{
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
} imu_acc_t;

typedef struct
{
    int16_t dps_x;
    int16_t dps_y;
    int16_t dps_z;
} imu_dps_t;

typedef struct 
{
    int16_t Roll;
    int16_t Pitch;
} Angle_t;

typedef struct
{
    /* REAR DATAS */
    float volt;
    uint8_t SOC;
    uint8_t cvt;
    //uint16_t fuel;
    float current;
    uint8_t temperature;
    uint16_t speed;
    /* FRONT DATAS */
    imu_acc_t imu_acc;
    imu_dps_t imu_dps;
    Angle_t Angle;
    uint16_t rpm;
    uint8_t flags; // MSB - BOX | BUFFER FULL | NC | NC | FUEL_LEVEL | SERVO_ERROR | CHK | RUN - LSB
    /* MPU DATAS */
    double latitude;
    double longitude;
    /* DEBUG DATA */
    uint32_t timestamp;

} mqtt_packet_t;

#endif