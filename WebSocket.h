#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include "CommandHandler.h";

class WebSocket
{
public:
	WebSocket();
	WebSocketsClient client;
	CommandHandler commandHandler;
	bool isConnected;
	void loop();
private:
	char ip [10];
	uint16_t port;
	uint64_t heartbeatTimestamp = 0;
	void eventHandler(WStype_t type, uint8_t * payload, size_t length);
	void connect();
	void disconnect();
	void handleMessage(JsonObject& payload);
};