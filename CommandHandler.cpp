#include "SumpThingHelpers.h"
#include "CommandHandler.h"

CommandHandler::CommandHandler()
{
	outlets[0] = new Outlet(PIN_1);
	outlets[1] = new Outlet(PIN_2);
	outlets[2] = new Outlet(PIN_3);
	outlets[3] = new Outlet(PIN_4);
}

void CommandHandler::outlet(JsonObject& args)
{
	int loop = 3;

	args.printTo(Serial);

	while (loop >= 0) {
		outlets[loop]->toggle((uint8_t) args["power"][loop]);
		loop--;
	}
}
