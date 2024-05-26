#ifndef CAN_H
#define CAN_H

#include <Arduino.h>
#include <CANmsg.h>
#include "can_defs.h"
#include "hardware_defs.h"
#include "packets.h"

bool CAN_start_device(void);
mqtt_packet_t setupVolatilePacket(void); // volatile packet default values
void Send_SOT_msg(uint8_t _msg);
mqtt_packet_t update_packet(void);
/* Interrupt */
void canISR(CAN_FRAME* rxMsg);

#endif