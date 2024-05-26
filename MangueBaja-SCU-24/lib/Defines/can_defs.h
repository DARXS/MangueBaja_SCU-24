#ifndef CAN_DEFS_H
#define CAN_DEFS_H

/* IDs */
#define BUFFER_SIZE     50
#define THROTTLE_MID    0x00
#define THROTTLE_RUN    0x01
#define THROTTLE_CHOKE  0x02

#define SYNC_ID         0x001       // message for bus sync
#define THROTTLE_ID     0x100       // 1by = throttle state (0x00, 0x01 or 0x02)
#define FLAGS_ID        0x101       // 1by
#define IMU_ACC_ID      0x200       // 6by           
#define IMU_DPS_ID      0x201       // 6by          
#define ANGLE_ID        0X205       // 4by
#define SPEED_ID        0x300       // 2by          
#define SOC_ID          0x302       // 1by
#define RPM_ID          0x304       // 2by           
#define SOT_ID          0x305       // 1by
#define TEMPERATURE_ID  0x400       // 1by          
#define CVT_ID          0x401       // 1by
#define FUEL_ID         0x500       // 2by           
#define VOLTAGE_ID      0x502       // 4by
#define CURRENT_ID      0x505       // 4by 
#define LAT_ID          0x600       // 8by
#define LNG_ID          0x700       // 8by

typedef struct
{
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t dps_x;
    int16_t dps_y;
    int16_t dps_z;
    
} imu_t;
    
typedef struct
{  
    imu_t imu;
    uint16_t rpm;
    uint16_t speed;
    uint8_t tempMOTOR;
    uint8_t flags; // LOW_BATTERY | DANGER_CVT | DANGER_MOTOR | LOW_FUEL_LEVEL | MQTT_ON | SERVOR_ERROR | CHOKE | RUN -- LSB
    uint16_t fuel;
    uint8_t soc;
    uint8_t tempCVT;
    float voltage;
    double latitude;
    double longitude;
    uint32_t timestamp;

} packet_t;

#endif