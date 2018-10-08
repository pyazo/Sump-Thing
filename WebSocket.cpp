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
		// Payload starts with 42 so we don't care about those.
		int offset = 2;

		char * json = new char[length + 6]();

		string jsonCoersion = "{\"0\":";

		strcpy(json, jsonCoersion.c_str());

		int index = offset;

		Serial.printf("%s\n", payload);

		while (index <= length)
		{
			json[(index - offset) + 5] = (char) payload[index];
			index++;
		}

		json[length + 5] = *"}";

		DynamicJsonBuffer jsonBuffer;

		JsonObject& payloadJson = jsonBuffer.parseObject(json);

		handleMessage(payloadJson);
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
	payload.printTo(Serial);
	return;
}