#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <FS.h>;

#include "WebSocket.h"
#include "NetworkConnector.h";

ESP8266WiFiMulti WiFiMulti;
WebSocket * socket;
NetworkConnector * connector;


void setup() {
	socket = new WebSocket();
	connector = new NetworkConnector();

	Serial.begin(115200);

	SPIFFS.begin();

	Serial.setDebugOutput(true);

	for (uint8_t t = 4; t > 0; t--) {
		Serial.flush();
		delay(1000);
	}

}

void loop() {
	socket->loop();
	connector->loop();
}