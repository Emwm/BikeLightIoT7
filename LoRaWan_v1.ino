/* Heltec Automation LoRaWAN communication example
 *
 * Function:
 * 1. Upload node data to the server using the standard LoRaWAN protocol.
 *  
 * Description:
 * 1. Communicate using LoRaWAN protocol.
 * 
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * 
 */

// use wireless mini shell board name so lora options are in the tools

#include "LoRaWan_APP.h"

// we can use Cibicom to generate all parameters
/* OTAA (over the air activation) para*/
// cibicom generated dev EUI = BE-7A-00-00-00-00-02-8A (if you go to the devices tab you can see the device EUI in a good format)
// I also tried device generated devEUI = AD AE 3D 98 00 00 24 DA
uint8_t devEui[] = { 0xAD, 0xAE, 0x3D, 0x98, 0x00, 0x00, 0x24, 0xDA };
// app EUI = join EUI = BE7A000000001465
uint8_t appEui[] = { 0xBE, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x14, 0x65 };
// in lorawan parameters tab = C0 F7 0B 05 FF 25 DB 7A 8D FB 25 FF 12 44 13 62
uint8_t appKey[] = { 0xC0, 0xF7, 0x0B, 0x05, 0xFF, 0x25, 0xDB, 0x7A, 0x8D, 0xFB, 0x25, 0xFF, 0x12, 0x44, 0x13, 0x62 };

//we do not need these
/* ABP para*/
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
We want to use Class A as it has lowest battery consumption
The device will oly listen after sneding data
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

//if true, next uplink will add MOTE_MAC_DEVICE_TIME_REQ 

void setup() {
  Serial.begin(115200);
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE); //initialize lorawan board
  //Serial.println("initialized lorawan board");
}

void loop()
{
  switch( deviceState )
  {
    //initializes the lorawan settings
    case DEVICE_STATE_INIT: 
    {
#if(LORAWAN_DEVEUI_AUTO) // this can be set in the tools tab of arduino IDE
      LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass,loraWanRegion); //set the region in the tools tab of arduino IDE
      //both set join DR and DR when ADR off 
      LoRaWAN.setDefaultDR(3);
      //Serial.println("initialized lorawan settings");
      break;
    }
    //joins the lorawan network
    case DEVICE_STATE_JOIN:
    {
      //Serial.println("tyring to join lorawan network");
      LoRaWAN.join();
      break;
    }
    //prepares and sends data
    case DEVICE_STATE_SEND:
    {
      //Serial.println("preparing to send data");
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    // waits before next transmission (waits for a randomized duty cycle delay and moves to sleep mode)
    case DEVICE_STATE_CYCLE:
    {
      //Serial.println("waiting in duty cycle delay");
      // Schedule next packet transmission
      // the randomized duty cycle delay helps prevent collisions with other lorawan devices
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    //enters sleep mode
    case DEVICE_STATE_SLEEP:
    {
      //Serial.println("entering sleep mode");
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    // if the stat is undefined, reinitialize the device
    default:
    {
      //Serial.println("undefined state, reinitializing the device");
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}