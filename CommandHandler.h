#include <ArduinoJson.h>

#include "Outlet.h"

class CommandHandler
{
public:
	CommandHandler();
	void batchUpdate(JsonObject& args);
	void update(JsonObject& args);
private:
	Outlet * outlets [4];
};

