#include <Arduino.h>

/* Socket.IO stuff */
#define MESSAGE_INTERVAL 30000
#define HEARTBEAT_INTERVAL 25000

/* Define our default pins */
#define PIN_1 4
#define PIN_2 2
#define PIN_3 0
#define PIN_4 5

/* Begin Helper Functions */
int AsciiToNumber(uint8_t number) {
	return ((int) number) - 48;
}