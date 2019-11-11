#include "NetworkConnector.h"

#include "ESP8266WiFi.h";
#include "DNSServer.h";
#include "ESP8266WebServer.h";
#include "ESP8266mDNS.h";
#include "FS.h";

using namespace std;

ESP8266WebServer server(80);
DNSServer dnsServer;

NetworkConnector::NetworkConnector()
{
	hasNetworkConnection = false;

	if (!hasNetworkConnection)
	{
		ssid = "Sump Thing";

		broadcastWiFi();
	}
}

void NetworkConnector::broadcastWiFi()
{
	IPAddress localhost(192, 168, 1, 1);
	const byte DNS_PORT = 53;

	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(localhost, localhost, IPAddress(255, 255, 255, 0));
	WiFi.softAP(ssid);

	dnsServer.start(DNS_PORT, "*", localhost);

	server.on("/", handleRoot);
	server.onNotFound(handleNotFound);

	server.begin();
}

void NetworkConnector::handleRoot()
{
	File file = SPIFFS.open("/test.html", "r");

	server.streamFile(file, "text/html");

	file.close();
}

void NetworkConnector::handleNotFound()
{
	server.send(404, "text/plain", "You've left the path...");
}

void NetworkConnector::loop()
{
	dnsServer.processNextRequest();
	server.handleClient();
}