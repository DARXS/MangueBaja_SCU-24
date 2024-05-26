#include "CAN.h"

bool mode = false;
mqtt_packet_t can_receive_packet = setupVolatilePacket();
CANmsg txMsg(CAN_RX_id, CAN_TX_id, CAN_BPS_1000K);

bool CAN_start_device()
{
  txMsg.Set_Debug_Mode(false);
  if(!txMsg.init(canISR))
  {
    log_e("CAN ERROR!! SYSTEM WILL RESTART IN 2 SECONDS...");
    vTaskDelay(2000);
    return false;
  }
  /* if you needed apply a Mask and id to filtering write this:
   * txMsg.init(canISR, ID); or txMsg.init(canISR, ID, Mask);
      * if you ID is bigger then 0x7FF the extended mode is activated
   * you can set the filter in this function too:
   * txMsg.Set_Filter(uint32_t ID, uint32_t Mask, bool Extended);  */ 
  return true;
}

mqtt_packet_t setupVolatilePacket()
{ 
  mqtt_packet_t t;
  memset(&t, 0x00, sizeof(mqtt_packet_t));
  return t;
}

void Send_SOT_msg(uint8_t _msg)
{
  vTaskDelay(1);
  /* Sent State of Telemetry (SOT) */
  txMsg.clear(SOT_ID);
  txMsg << _msg;
  txMsg.write();

  /*
    * If you send a buffer message you can use this function:
    * SendMsgBuffer(uint32_t Id, bool ext, uint8_t length, unsigned char* data);
    * or you use this:
    * txMsg << data1 << data2 << data3 ...; 
  */
}

mqtt_packet_t update_packet()
{
  return can_receive_packet;
}

/* CAN functions */
void canISR(CAN_FRAME* rxMsg)
{ 
  mode = !mode; digitalWrite(EMBEDDED_LED, mode);

  can_receive_packet.timestamp = millis();

  if(rxMsg->id==IMU_ACC_ID)
  {
    memcpy(&can_receive_packet.imu_acc, (imu_acc_t*)&rxMsg->data.uint8, sizeof(imu_acc_t));
    //Serial.printf("ACC X = %f\r\n", (float)((can_receive_packet.imu_acc.acc_x*0.061)/1000));
    //Serial.printf("ACC Y = %f\r\n", (float)((can_receive_packet.imu_acc.acc_y*0.061)/1000));
    //Serial.printf("ACC Z = %f\r\n", (float)((can_receive_packet.imu_acc.acc_z*0.061)/1000));
  }

  if(rxMsg->id==IMU_DPS_ID)
  {
    memcpy(&can_receive_packet.imu_dps, (imu_dps_t*)&rxMsg->data.uint8, sizeof(imu_dps_t));
    //Serial.printf("DPS X = %d\r\n", can_receive_packet.imu_dps.dps_x);
    //Serial.printf("DPS Y = %d\r\n", can_receive_packet.imu_dps.dps_y);
    //Serial.printf("DPS Z = %d\r\n", can_receive_packet.imu_dps.dps_z);
  }

  if(rxMsg->id==ANGLE_ID)
  {
    memcpy(&can_receive_packet.Angle, (Angle_t*)&rxMsg->data.uint8, sizeof(Angle_t));
    //Serial.printf("Angle Roll = %d\r\n", can_receive_packet.Angle.Roll);
    //Serial.printf("Angle Pitch = %d\r\n", can_receive_packet.Angle.Pitch);
  }

  if(rxMsg->id==RPM_ID)
  {
    memcpy(&can_receive_packet.rpm, (uint16_t*)&rxMsg->data.uint8, sizeof(uint16_t));
    //Serial.printf("RPM = %d\r\n", can_receive_packet.rpm);
  }

  if(rxMsg->id==SPEED_ID)
  {
    memcpy(&can_receive_packet.speed, (uint16_t*)&rxMsg->data.uint8, sizeof(uint16_t));
    //Serial.printf("Speed = %d\r\n", can_receive_packet.speed);
  }

  if(rxMsg->id==TEMPERATURE_ID)
  {
    memcpy(&can_receive_packet.temperature, (uint8_t*)&rxMsg->data.uint8, sizeof(uint8_t));
    //Serial.printf("Motor = %d\r\n", can_receive_packet.temperature);
  }

  if(rxMsg->id==FLAGS_ID)
  {
    memcpy(&can_receive_packet.flags, (uint8_t*)&rxMsg->data.uint8, sizeof(uint8_t));
    //Serial.printf("Flags = %d\r\n", can_receive_packet.flags);
  }

  if(rxMsg->id==SOC_ID)
  {
    memcpy(&can_receive_packet.SOC, (uint8_t*)&rxMsg->data.uint8, sizeof(uint8_t));
    //Serial.printf("SOC = %d\r\n", can_receive_packet.SOC);
  }

  if(rxMsg->id==CVT_ID)
  {
    memcpy(&can_receive_packet.cvt, (uint8_t*)&rxMsg->data.uint8, sizeof(uint8_t));
    //Serial.printf("CVT = %d\r\n", can_receive_packet.cvt);
  }

  if(rxMsg->id==VOLTAGE_ID)
  {
    memcpy(&can_receive_packet.volt, (float*)&rxMsg->data.uint8, sizeof(float));
    //Serial.printf("Volt = %f\r\n", can_receive_packet.volt);
  }

  if(rxMsg->id==CURRENT_ID)
  {
    memcpy(&can_receive_packet.current, (float*)&rxMsg->data.uint8, sizeof(float));
    //Serial.printf("Current = %f\r\n", can_receive_packet.current);
  }

  if(rxMsg->id==LAT_ID)
  {
    memcpy(&can_receive_packet.latitude, (double*)&rxMsg->data.uint8, sizeof(double));
    //Serial.printf("Latitude (LAT) = %lf\r\n", can_receive_packet.latitude);
  }  

  if(rxMsg->id==LNG_ID)
  {
    memcpy(&can_receive_packet.longitude, (double*)&rxMsg->data.uint8, sizeof(double));
    //Serial.printf("Longitude (LNG) = %lf\r\n", can_receive_packet.longitude);
  }
}
