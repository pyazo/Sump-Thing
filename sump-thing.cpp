#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <FS.h>;
#include <EEPROM.h>;

#include "WebSocket.h"
#include "NetworkConnector.h";

ESP8266WiFiMulti WiFiMulti;
WebSocket * socket;
NetworkConnector * connector;


void setup() {
	Serial.begin(74880);

	SPIFFS.begin();

	Serial.setDebugOutput(true);

	socket = new WebSocket();
	connector = new NetworkConnector();

	for (uint8_t t = 4; t > 0; t--) {
		Serial.flush();
		delay(1000);
	}
}

void loop() {
	socket->loop();
	connector->loop();
}