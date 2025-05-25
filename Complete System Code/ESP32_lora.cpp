/*
 Includes example code from the rn2xx3.h library. 
 The example code comes from the "ArduinoUnoNano-basic.ino" and "ArduinoUnoNano-downlink.ino" files.
 Also includes code from chatgpt to help with the downlink payload parsing. 
 */

#include <rn2xx3.h>
#include <HardwareSerial.h>

// ---- Define UART Pins ----
#define RXD2 18
#define TXD2 19
#define RESET 23

// ---- Global Variables ----
HardwareSerial loraSerial(1); //initiailizing the hardware UART for our lora communication

//create an instance of the rn2xx3 library, giving the software UART as stream to use, and using LoRa WAN
rn2xx3 myLora(loraSerial);

String prevStr = ""; // holds value of the previous downlink

// ---- Downlink Parsing Functions ----
// Converts a hex character to its integer value
int hex_char_to_int(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Converts a hex string to ASCII
void hex_to_ascii(const char* hex, char* ascii) {
    while (*hex && hex[1]) {
        int hi = hex_char_to_int(*hex++);
        int lo = hex_char_to_int(*hex++);
        if (hi == -1 || lo == -1) break;
        *ascii++ = (hi << 4) | lo;
    }
    *ascii = '\0';
}

// Parses the ASCII string and extracts the value for "value"
int get_c_value_from_ascii(const char* ascii) {
    const char* key = "\"value\":";
    const char* pos = strstr(ascii, key);
    if (pos) {
        int value;
        if (sscanf(pos + strlen(key), " %d", &value) == 1) {
            return value;
        }
    }
    return -1; // Error code
}

// receives hex string, returns the C value
int get_c_value_from_hex(const char* hex) {
    // Buffer for decoded ASCII (hex string length / 2 + 1)
    size_t len = strlen(hex) / 2 + 1;
    char* ascii = (char*)malloc(len);
    if (!ascii) return -1;
    hex_to_ascii(hex, ascii);
    int value = get_c_value_from_ascii(ascii);
    free(ascii);
    return value;
}

// ---- Function: Initializes Network Connection ----
// this function is called within the init_lora() function during setup of the main code
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
  join_result = myLora.initOTAA("70B3D57ED0070C35", "840F5A2B838953A66D49D02B7A6BD1D4");

  while(!join_result) //need to fix this so it does not get stuck
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
}

// ---- Function: Initializes LoRaWAN ----
void init_lora(){
  // initialize lora serial
  loraSerial.begin(57600, SERIAL_8N1, RXD2, TXD2);
  delay(1000); //wait for the arduino ide's serial console to open
  Serial.println("Startup");
  
  // initialize lorawan communication
  initialize_radio();
}

// ---- Function: Sends Uplink ----
void send_lora( int devMode, float batteryLevel, float longCoord, float latCoord ){  
  Serial.println("TXing");
  // myLora.tx("Hello there"); //one byte, blocking function, uncomfirmed
  // ^ the function sends the data byte-by-byte internally, blocking (waiting) until each byte is fully sent before sending the next.

  // Build a comma-separated string
  String payload = String(devMode) + "," +
                   String(batteryLevel, 2) + "," +   // 2 decimal places
                   String(longCoord, 2) + "," +
                   String(latCoord, 2);
  Serial.println("Payload: " + payload);

  myLora.tx(payload); //one byte, blocking function

  // below is for sending raw data, ran out of time to fully implement

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

// ---- Function: Recieves Downlink ----
int recieve_lora(){
  String str = myLora.getRx(); // checks if there is a recieved downlink message waiting in RN modules buffer
  //Serial.println("Received str: " + str);
  //Serial.println("Previous str: " + prevStr);
  if (str.length() != 0 ){
    if (str != prevStr){ // not the same
      //Serial.println("New string Received: " + str);
      int mode = get_c_value_from_hex(str.c_str());
      Serial.println("Value integer:" + String(mode));
      prevStr = str;
      return mode;
    }
  } else {
    prevStr = "";
    return 0; // theres no recieved message
  }
}