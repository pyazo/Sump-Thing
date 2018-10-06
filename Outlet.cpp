#include <Arduino.h>
#include <Outlet.h>

Outlet::Outlet(uint8_t pin) 
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
}

void Outlet::turnOff() 
{
	digitalWrite(pin, LOW);
	Serial.println(pin);
	isOn = false;

	return;
}

void Outlet::turnOn() {
	digitalWrite(pin, HIGH);
	Serial.println(pin);
	isOn = true;

	return;
}

void Outlet::changeState(uint8_t newState) 
{

	if (newState == 1) {
		turnOn();
	}
	else if(newState == 0) {
		turnOff();
	}
	else {
		Serial.println("wut");
	}

	return;
}

uint8_t Outlet::getPin() {
	return pin;
}