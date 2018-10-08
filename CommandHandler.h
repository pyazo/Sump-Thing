#include <ArduinoJson.h>

#include "Outlet.h"

class CommandHandler
{
public:
	CommandHandler();
	void outlet(JsonObject& args);
private:
	Outlet * outlets [4];
};

