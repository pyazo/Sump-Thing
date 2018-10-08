#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

#include "WebSocket.h"

ESP8266WiFiMulti WiFiMulti;
WebSocket * socket;


void setup() {
	socket = new WebSocket();

	Serial.begin(115200);

	Serial.setDebugOutput(true);

	for (uint8_t t = 4; t > 0; t--) {
		Serial.flush();
		delay(1000);
	}

	WiFiMulti.addAP("Porter's Wifi", "cope1234");

	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

}

void loop() {
	socket->loop();
}