/*
 * Author: JP Meijers
 * Date: 2016-10-20
 *
 * Transmit a one byte packet via TTN. This happens as fast as possible, while still keeping to
 * the 1% duty cycle rules enforced by the RN2483's built in LoRaWAN stack. Even though this is
 * allowed by the radio regulations of the 868MHz band, the fair use policy of TTN may prohibit this.
 *
 * CHECK THE RULES BEFORE USING THIS PROGRAM!
 *
 * CHANGE ADDRESS!
 * Change the device address, network (session) key, and app (session) key to the values
 * that are registered via the TTN dashboard.
 * The appropriate line is "myLora.initABP(XXX);" or "myLora.initOTAA(XXX);"
 * When using ABP, it is advised to enable "relax frame count".
 *
 * Connect the RN2xx3 as follows:
 * RN2xx3 -- ESP8266
 * Uart TX -- GPIO4
 * Uart RX -- GPIO5
 * Reset -- GPIO15
 * Vcc -- 3.3V
 * Gnd -- Gnd
 *
 */
#include <rn2xx3.h>
#include <HardwareSerial.h>

#define RXD2 18
#define TXD2 19
#define RESET 23

HardwareSerial loraSerial(1); //initiailizing the hardware UART for our lora communication

//create an instance of the rn2xx3 library, giving the software UART as stream to use, and using LoRa WAN
rn2xx3 myLora(loraSerial);


void initialize_radio()
{
  //reset RN2xx3
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  loraSerial.flush();

  //check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(hweui); //theis is a 16 hexadecimal identificatio number we need to add to the Cibicom server
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  // We are not using activation by personalization
  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  //join_result = myLora.initABP("02017201", "8D7FFEF938589D95AAD928C2E2E7E48F", "AE17E567AECC8787F749A62F5541D522");

  // We are using over the air
  // OTAA: initOTAA(AppEUI, AppKey);
  join_result = myLora.initOTAA("70B3D57ED0070B8E", "FBA8024835EEE54FB9C1F11060FF9BAB");

  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
}

void init_lora(){
  // initialize lora serial
  loraSerial.begin(57600, SERIAL_8N1, RXD2, TXD2);
  delay(1000); //wait for the arduino ide's serial console to open
  Serial.println("Startup");
  
  // initialize lorawan communication
  initialize_radio();

  //transmit a startup message
  myLora.tx("TTN Mapper on ESP32 node");
}

void send_lora( int devMode, float batteryLevel, float longCoord, float latCoord ){  
  Serial.println("TXing");
  myLora.tx("Hello there"); //one byte, blocking function, uncomfirmed
  // ^ the function sends the data byte-by-byte internally, blocking (waiting) until each byte is fully sent before sending the next.

  // Build a comma-separated string
  String payload = String(devMode) + "," +
                   String(batteryLevel, 2) + "," +   // 2 decimal places
                   String(longCoord, 2) + "," +
                   String(latCoord, 2);
  Serial.println("Payload: " + payload);

  myLora.tx(payload); //one byte, blocking function

  // below is for sending raw data

  // // create payload buffer (1 + 4 + 4 + 4 = 13 bytes)
  // uint8_t payload[13];

  // // add devMode (1 byte)
  // payload[0] = (uint8_t)devMode;

  // // add batteryLevel (float) into payload[1–4]
  // memcpy(&payload[1], &batteryLevel, sizeof(float));

  // // add longitude (float) into payload[5–8]
  // memcpy(&payload[5], &longCoord, sizeof(float));

  // // add latitude (float) into payload[9–12]
  // memcpy(&payload[9], &latCoord, sizeof(float));

  // // Convert payload to hex string
  // String hexPayload = "";
  // for (int i = 0; i < sizeof(payload); i++) {
  //   if (payload[i] < 0x10) hexPayload += "0";
  //   hexPayload += String(payload[i], HEX);
  // }

  // // Send payload using raw command
  // String response = myLora.sendRawCommand("mac tx uncnf 1 " + hexPayload);
  // Serial.println("Response: " + response);
}

void recieve_lora(){
  String str = myLora.getRx(); // checks if there is a recieved downlink message waiting in RN modules buffer
  Serial.println("Recieved: " + str);
}