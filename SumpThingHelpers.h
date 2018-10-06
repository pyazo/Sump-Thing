#include <Arduino.h>

#define MESSAGE_INTERVAL 30000
#define HEARTBEAT_INTERVAL 25000

int AsciiToNumber(uint8_t number) {
	return ((int) number) - 48;
}