#include <Arduino.h>

#define MESSAGE_INTERVAL 30000;

int AsciiToNumber(uint8_t number) {
	return ((int) number) - 48;
}