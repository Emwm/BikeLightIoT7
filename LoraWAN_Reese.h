/* Prepares the payload which is the data to be sent. 
Takes the LoraWAN port as a parameter (this variable is specified in the cpp file as a value of 2) */
static void prepareTxFrame( uint8_t port );

/* Intializes the lorawan board. Also Initializes lorawan settings by generating the device EUI from the chip and
setting the lora class, region, and defualt data rate. */
void initState();

/* Joins loraWAN network. Should be called after initState */ 
void joinState();

/* Sends the data and then goes to the cycleState function. 
While sending the data, depending on the paremeters set in the cpp file, it will wait for an acknowledgement 
from the server and retry sending the data a set number of times. */
void sendState();

/* Sets the amount of time the device will wait before sending data again and then goes to the sleepState function. It wakes itself up after the set amount of time.
This is important to minimize lora traffic and prevent collisions. */ 
void cycleState();

/* Puts the loraWan into sleep mode (not the whole device just the loraWan functionality) */
void sleepState();