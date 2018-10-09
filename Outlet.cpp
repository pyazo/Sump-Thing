#include <Arduino.h>

#include "Outlet.h"

Outlet::Outlet(uint8_t pin) 
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
}

void Outlet::changeState(uint8_t newState) 
{
	bool isOn = newState == 1 ? true : false;

	int pull = isOn ? HIGH : LOW;

	digitalWrite(pin, pull);

	this->isOn = isOn;

	return;
}

uint8_t Outlet::getPin() {
	return pin;
}