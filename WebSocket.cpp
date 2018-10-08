#include <ArduinoJson.h>

#include "WebSocket.h"
#include "SumpThingHelpers.h"

using namespace std;
using namespace std::placeholders;

WebSocket::WebSocket()
{
	strcpy(ip, SERVER_IP);
	port = SERVER_PORT;

	std::function<void(WStype_t type, uint8_t * payload, size_t length)> handleEvents = std::bind(&WebSocket::eventHandler, this, _1, _2, _3);

	client.beginSocketIO(ip, port);
	client.onEvent(handleEvents);
}

void WebSocket::loop()
{
	client.loop();

	if (isConnected) {
		uint64_t now = millis();

		if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
			heartbeatTimestamp = now;
			// socket.io heartbeat message
			client.sendTXT("2");
		}
	}
}

void WebSocket::eventHandler(WStype_t type, uint8_t * payload, size_t length)
{
	switch (type) {
	case WStype_CONNECTED:
		connect();
		break;
	case WStype_DISCONNECTED:
		disconnect();
		break;
	case WStype_TEXT:
		// Don't care about heartbeats
		if (length > 3)
		{
			int offset = 2;

			char * json = new char[length + 4]();

			string jsonCoersion = "{\"0\":";

			strcpy(json, jsonCoersion.c_str());

			int index = offset;

			Serial.printf("%s\n", payload);

			while (index <= length)
			{
				json[(index - offset) + 5] = (char)payload[index];
				index++;
			}

			json[length + 3] = *"}";

			DynamicJsonBuffer jsonBuffer;

			JsonObject& payloadJson = jsonBuffer.parseObject(json);

			handleMessage(payloadJson);
		}
	}
}

void WebSocket::disconnect()
{
	isConnected = false;
}

void WebSocket::connect()
{
	isConnected = true;
	// Socket.IO upgrade confirmation
	client.sendTXT("5");
}

void WebSocket::handleMessage(JsonObject& payload)
{
	String command = payload["0"][0];

	JsonObject& args = payload["0"][1];

	if (command == "outlet")
	{
		commandHandler.outlet(args);
	}

	return;
}