#include <rn2xx3.h>
#include <HardwareSerial.h>

// ---- Function Declarations ----
void initialize_radio(); //called within init_lora
void init_lora(); //initializes loraWAN and connects to TTN
void send_lora( int devMode, float batteryLevel, float longCoord, float latCoord ); //sends data with TTN
int recieve_lora(); //checks for downlink, checks if downlink is new, returns int value