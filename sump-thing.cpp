/*
* WebSocketClientSocketIO.ino
*
*  Created on: 06.06.2016
*
*/

// External Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>


// Our libraries
#include <SumpThingHelpers.h>
#include <Outlet.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define MESSAGE_INTERVAL 30000
#define HEARTBEAT_INTERVAL 25000

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

Outlet *outlets[4];

void UpdateOutlets(uint8_t outletStateArr[])
{
	Serial.printf("[On_Off]: %s\n", outletStateArr);

	int loop = sizeof(outletStateArr) - 1;

	while (loop >= 0) {
		outlets[loop]->ChangeState((bool) outletStateArr[loop]);
	}
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	char index;
	char jIndex;
	uint8_t j[4];
	switch (type) {
	case WStype_DISCONNECTED:
		Serial.printf("[WSc] Disconnected!\n");
		isConnected = false;
		break;
	case WStype_CONNECTED:
	{
		Serial.printf("[WSc] Connected to url: %s\n", payload);
		isConnected = true;

		// send message to server when Connected
		// socket.io upgrade confirmation message (required)
		webSocket.sendTXT("5");
	}
	break;
	case WStype_TEXT:
		Serial.printf(" %s\n", payload);

		if (length > 22)
		{
			jIndex = 0;
			index = 22;

			while (index <= 28)
			{
				j[jIndex] = (bool) AsciiToNumber(payload[index]);
				index += 2;
				jIndex++;
			}

			UpdateOutlets(j);
		}
		// send message to server
		// webSocket.sendTXT("message here");
		break;
	case WStype_BIN:
		Serial.printf("[WSc] get binary length: %u\n", length);
		hexdump(payload, length);

		// send data to server
		// webSocket.sendBIN(payload, length);
		break;
	}

}

void setup() {
	
	// Define our outlets here in the array
	outlets[0] = new Outlet(0);
	outlets[1] = new Outlet(4);
	outlets[2] = new Outlet(2);
	outlets[3] = new Outlet(5);


	Serial.begin(115200);

	Serial.setDebugOutput(true);

	for (uint8_t t = 4; t > 0; t--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}

	WiFiMulti.addAP("Porter's Wifi", "cope1234");

	//WiFi.disconnect();
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

	webSocket.beginSocketIO("10.0.0.80", 4000);
	//webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
	webSocket.onEvent(webSocketEvent);

}

void loop() {
	webSocket.loop();

	if (isConnected) {

		uint64_t now = millis();

		if (now - messageTimestamp > MESSAGE_INTERVAL) {
			messageTimestamp = now;
			// example socket.io message with type "messageType" and JSON payload
			webSocket.sendTXT("42[\"messageType\",{\"greeting\":\"hello\"}]");
		}
		if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
			heartbeatTimestamp = now;
			// socket.io heartbeat message
			webSocket.sendTXT("2");
		}
	}
}