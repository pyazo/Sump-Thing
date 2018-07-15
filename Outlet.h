class Outlet
{
public:
	Outlet(uint8_t pin);
	bool isOn;
	void TurnOff();
	void TurnOn();
	void ChangeState(bool newState);
	uint8_t getPin();
private:
	uint8_t pin;
};