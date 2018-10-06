#include <Arduino.h>

#define MESSAGE_INTERVAL 30000;

int AsciiToNumber(char number) {
	return (uint8_t)number - 48;
}