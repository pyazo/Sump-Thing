#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

#include <SumpThingHelpers.h>
#include <Outlet.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

Outlet *outlets[4];

void UpdateOutlets(int outletStateArr [])
{
	int loop = 3;

	while (loop >= 0) {
		outlets[loop]->toggle((uint8_t) outletStateArr[loop]);
		loop--;
	}
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	int index;
	int jIndex;
	int j [4];
	switch (type) {
	case WStype_DISCONNECTED:
		isConnected = false;
		break;
	case WStype_CONNECTED:
	{
		isConnected = true;

		// send message to server when Connected
		// socket.io upgrade confirmation message (required)
		webSocket.sendTXT("5");
	}
	break;
	case WStype_TEXT:
		if (length > 22)
		{
			jIndex = 0;
			index = 22;

			while (index <= 28)
			{
				j[jIndex] = AsciiToNumber((uint8_t) payload[index]);

				Serial.println(j[jIndex]);

				index += 2;
				jIndex++;
			}

			UpdateOutlets(j);
		}
		break;
	}

}

void setup() {
	
	// Define our outlets here in the array
	outlets[0] = new Outlet(PIN_1);
	outlets[1] = new Outlet(PIN_2);
	outlets[2] = new Outlet(PIN_3);
	outlets[3] = new Outlet(PIN_4);


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

	webSocket.beginSocketIO("10.0.0.80", 4000);
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