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
	server.on("/networks", handleNetworks);

	server.onNotFound(handleNotFound);

	server.begin();
}

void NetworkConnector::handleRoot()
{
	File file = SPIFFS.open("/test.html", "r");

	server.streamFile(file, "text/html");

	file.close();
}

void NetworkConnector::handleNetworks()
{
	int networks = WiFi.scanNetworks();

	String json = "{\"networks\":[";

	for (int i; i < networks; i++)
	{
		String ssid = WiFi.SSID(i);
		int rssi = WiFi.RSSI(i);
		int encryption_type = WiFi.encryptionType(i);

		if (rssi < -80) continue;

		json += "{";
		json += "\"name\":\"";
		json += ssid;
		json += "\",";
		json += "\"strength\":\"";
		json += parseRSSI(rssi);
		json += "\",";
		json += "\"hasPassword\":";

		if (encryption_type == 7)
		{
			json += "false";
		}
		else
		{
			json += "true";
		}

		if (i + 1 == networks)
		{
			json += "}";
		}
		else
		{
			json += "},";
		}
	}
	
	json += "]}";

	// Remove trailing comma
	json.replace("},]", "}]");

	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(200, "application/json", json);
}

void NetworkConnector::handleNotFound()
{
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(404, "text/plain", "You've left the path...");
}

void NetworkConnector::loop()
{
	dnsServer.processNextRequest();
	server.handleClient();
}

// Values from here: http://www.veris.com/docs/whitePaper/vwp18_RSSI_RevA.pdf
String NetworkConnector::parseRSSI(int rssi)
{
	if (rssi > -40) return "4";
	if (rssi > -55 && rssi <= -40) return "3";
	if (rssi > -70 && rssi <= -55) return "2";
	if (rssi > -80 && rssi <= -70) return "1";
}