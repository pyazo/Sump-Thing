﻿#include <Arduino.h>

class Outlet
{
public:
	Outlet(uint8_t pin);
	bool isOn;
	void changeState(uint8_t newState);
	uint8_t getPin();
private:
	uint8_t pin;
};