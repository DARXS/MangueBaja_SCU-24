#include <Arduino.h>
/* CAN Libraries */
#include <CAN.h>
/* OTA functions Librarie */
#include <ota.h>
/* Libraries of SD and Conectivity state Machine */
#include <SD_state_machine.h>
#include <CON_state_machine.h>

TaskHandle_t SDlogging = NULL, ConectivityState = NULL;
bool _sd = false; // flag to check if SD module compile
uint8_t _sot = DISCONNECTED;

/* States Machines */
void SdStateMachine(void* pvParameters);
void ConnStateMachine(void* pvParameters);

void setup()
{
  Serial.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
  pinConfig(); // Hardware and Interrupt Config

  /* CAN-BUS Initialize */
  if(!CAN_start_device()) esp_restart();
  
  /* Tasks */
  xTaskCreatePinnedToCore(SdStateMachine, "SDStateMachine", 4096, NULL, 5, &SDlogging, 0);
  // This state machine is responsible for the Basic CAN logging
  xTaskCreatePinnedToCore(ConnStateMachine, "ConnectivityStateMachine", 4096, NULL, 5, &ConectivityState, 1);
  // This state machine is responsible for the GPRS connection          
}

void loop() {/**/} 

/* SD State Machine */
void SdStateMachine(void* pvParameters)
{
  _sd = start_SD_device();

  /* For synchronization between ECU and panel */
  Send_SOT_msg(_sot);

  while(1)
  {
    Check_SD_for_storage();

    vTaskDelay((_sd ? 1 : 100));
  }
}

/* Connectivity State Machine */
void ConnStateMachine(void* pvParameters)
{
  _sot = Initialize_GSM(); 
  if((_sot & 0x04)==ERROR_CONECTION)
  { // enable the error bit
    Send_SOT_msg(_sot);
    delay(DELAY_ERROR(_sot));
    esp_restart();
  } 
  Send_SOT_msg(_sot);

  while(1)
  {
    if(!Check_mqtt_client_conection())
    {
      _sot==CONNECTED ? _sot ^= (DISCONNECTED|0x01) : 0; // disable online flag 
      Send_SOT_msg(_sot);
      gsmReconnect(_sot);
      //while(_sot==DISCONNECTED) { _sot = gsmReconnect(); vTaskDelay(5); }
      Send_SOT_msg(_sot); 
    }
    Send_msg_MQTT();

    vTaskDelay(1);
  }
}
