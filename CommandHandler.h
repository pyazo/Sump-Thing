#include <ArduinoJson.h>

#include "Outlet.h"

class CommandHandler
{
public:
	CommandHandler();
	void batchUpdateOutlets(JsonObject& args);
	void updateOutlet(JsonObject& args);
private:
	Outlet * outlets [4];
};

