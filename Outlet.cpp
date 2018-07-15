#include <Outlet.h>

Outlet::Outlet(uint8_t pin) {
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

void Outlet::ChangeState(bool newState) {
	digitalWrite(pin, newState);

	isOn = newState;

	return;
}

uint8_t Outlet::getPin() {
	return pin;
}