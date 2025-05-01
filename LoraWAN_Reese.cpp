#include "LoRaWan_APP.h"
#include "LoraWAN_Reese.h"

/*
Below this point are the LoraWAN paremeters including
- OTAA parameters
- ABP parameters
- userChannelsMask
- loraWanRegion
- LoraWanClass
- appTxDutyCycle
- overTheAirActivation
- loraWanAdr
- isTxConfirmed
- appPort
- confirmedNbTrials
*/

/* OTAA (over the air activation) parameters*/
// device specific devEUI = AD AE 3D 98 00 00 24 DA
uint8_t devEui[] = { 0xAD, 0xAE, 0x3D, 0x98, 0x00, 0x00, 0x24, 0xDA };
// app EUI 70B3D57ED0070274
uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x02, 0x74 };
// app key B2E7E09AA34CC6BEABDA8899F57E9689
uint8_t appKey[] = {0xB2, 0xE7, 0xE0, 0x9A, 0xA3, 0x4C, 0xC6, 0xBE, 0xAB, 0xDA, 0x88, 0x99, 0xF5, 0x7E, 0x96, 0x89 };

/* ABP (activation by personalization) parameters*/
uint8_t nwkSKey[] = { 0xBB, 0x8F, 0x62, 0xB4, 0x5E, 0x4C, 0x66, 0xEF, 0x47, 0x51, 0x6F, 0x1C, 0xD9, 0x7B, 0x89, 0xDD };
uint8_t appSKey[] = { 0x91, 0x1C, 0xB0, 0x92, 0xE1, 0xD0, 0x9A, 0x0A, 0x55, 0xDF, 0x3B, 0x08, 0xF3, 0xCC, 0x91, 0x5C };
uint32_t devAddr =  ( uint32_t )0x01F768DD;

/*LoraWan channelsmask, default channels 0-7*/ 
// this is enabling channels 0 to 7 using a bitmask array,  eu868 uses channels 0-2 usually
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*
LoraWan Class, Class A and Class C are supported
We want to use Class A as it has lowest battery consumption (meaning the device will only listen for downlink after sending data)
*/
DeviceClass_t  loraWanClass = CLASS_A; 

/*
the application data transmission duty cycle.  value in [ms].
the transmission interval will be 15s
*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR (adaptive data rate) enable (which is used for power efficiency)*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* 
Application port
this port routes data to the correct application on the network server
enables multiple application to use the same lorawan connection
allows devices to send different types of data on different ports
Port 0 is for Lorawan MAC commands
Port 1-223 is for application-specific data
*/
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4; // how many times it tries again to receive the acknowledgment




/*
Below here are the functions:
- prepareTxFrame
- initState
- joinState
- sendState
- cycleState
- sleepState
*/

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
  *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
  *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
  *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
  *for example, if use REGION_CN470, 
  *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
    appDataSize = 4;
    appData[0] = 0x00;
    appData[1] = 0x01;
    appData[2] = 0x02;
    appData[3] = 0x03;
}

/* intializes lorawan settings by generating the device EUI from the chip, 
setting the lora class, region, and defualt data rate */
void initState(){
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE); //initialize lorawan board
  //Serial.println("initialized lorawan board");
  if (LORAWAN_DEVEUI_AUTO) LoRaWAN.generateDeveuiByChipID(); //get the DevEUI from the chip
  LoRaWAN.init(loraWanClass,loraWanRegion); //set the region and class (we want A) in the tools tab of arduino IDE
  LoRaWAN.setDefaultDR(3); //setting default data rate
  //Serial.println("initialized lorawan settings");
}

// joins loraWAN network
void joinState(){ 
  //Serial.println("tyring to join lorawan network");
  LoRaWAN.join();
}

/*
sends the data and then goes to the cycle state function. 
The prepareTXFrame function is called (which formats the packet). 
The LoRaWAN.send() function sends the data. This function uses the variable isTxConfirmed (to determine if it should 
send confirmed messages ie should it wait to receive an ACK) and confirmedNbTrials (the number of times it should try to send and wait for an ACK)
*/
void sendState(){ 
  //Serial.println("preparing and sending data");
  prepareTxFrame( appPort ); //calls function to prepare data packet
  LoRaWAN.send(); // sends data packet, waits to receive ACK, retries confirmedNBTrials amount of times
  cycleState(); //goes to cycle state function to delay before next data packet
}

/*
sets the random amount of time the device will wait before sending data again (important to minimize lora traffic and prevent collisions).
the LoRaWAN.cycle() function sets the amount of time before the device will wake up again.
then the sleep state function is called.
*/ 
void cycleState(){
  //Serial.println("start cycle delay");
  txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
  LoRaWAN.cycle(txDutyCycleTime); // this sets how long the device will enter the sleep state for before it wakes up again
  sleepState(); 
}

//puts the loraWan functionality into sleep mode (not the whole device)
void sleepState(){
  //Serial.println("entering sleep state");
  LoRaWAN.sleep(loraWanClass); // this puts the device in the sleep state for txDutyCycleTime
}