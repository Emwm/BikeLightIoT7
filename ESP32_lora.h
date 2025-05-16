#include <rn2xx3.h>
#include <HardwareSerial.h>

void initialize_radio(); //called within init_lora
void init_lora(); //initializes loraWAN and connects to TTN
void send_lora( int devMode, float batteryLevel, float longCoord, float latCoord ); //sends data with TTN
void recieve_lora(); //checks for downlink and prints it to serial