#include <Arduino.h>
#include <Outlet.h>

Outlet::Outlet(unsigned char pin) {
	pin = pin;

	pinMode(pin, OUTPUT);
}

void Outlet::TurnOff() {
	digitalWrite(pin, 0);

	isOn = false;

	return;
}

void Outlet::TurnOn() {
	digitalWrite(pin, 1);

	isOn = true;

	return;
}

void Outlet::ChangeState(uint8_t newState) {
	digitalWrite(pin, newState);

	isOn = (bool) newState;

	return;
}

uint8_t Outlet::getPin() {
	return pin;
}