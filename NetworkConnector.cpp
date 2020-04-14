#include "NetworkConnector.h"

#include "ESP8266WiFi.h";
#include "DNSServer.h";
#include "ESP8266WebServer.h";
#include "ESP8266mDNS.h";
#include "FS.h";
#include "EEPROM.h";

using namespace std;

ESP8266WebServer server(80);
DNSServer dnsServer;

String broadcastedSSID;
String savedSSID;
String savedPassword;

NetworkConnector::NetworkConnector()
{
	broadcastedSSID = "Sump Thing";

	readCredentials();

	if (savedSSID.length() == 0)
	{

		broadcastWiFi();
	}
	else {
		connectToWiFi();
	}
}

void NetworkConnector::readCredentials()
{
	File ssidFile = SPIFFS.open("/ssid.txt", "r");

	if (!ssidFile) {
		Serial.println("Unable to locate saved WiFi credientials, starting WiFi...");
		return;
	}

	while (ssidFile.available()) {
		savedSSID += (char) ssidFile.read();
	}

	ssidFile.close();

	File passwordFile = SPIFFS.open("/password.txt", "r");

	while (passwordFile.available()) {
		savedPassword += (char) passwordFile.read();
	}

	passwordFile.close();

	return;
}

void NetworkConnector::connectToWiFi()
{
	WiFi.begin(savedSSID.c_str(), savedPassword.c_str());

	int wifiTryCount = 1;

	while (WiFi.status() != WL_CONNECTED) {
		Serial.println("Connecting to " + savedSSID + "...");

		if (WiFi.status() == WL_CONNECT_FAILED || wifiTryCount >= 60) {
			Serial.println("Invalid saved WiFi credentials...");

			savedSSID = "";
			savedPassword = "";

			broadcastWiFi();

			return;
		}

		wifiTryCount += 1;
		delay(1000);
	}

	Serial.println("Successfully connected to " + savedSSID + "!");
}

void NetworkConnector::broadcastWiFi()
{
	IPAddress localhost(192, 168, 1, 1);
	const byte DNS_PORT = 53;

	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(localhost, localhost, IPAddress(255, 255, 255, 0));
	WiFi.softAP(broadcastedSSID.c_str());

	dnsServer.start(DNS_PORT, "*", localhost);

	server.on("/", HTTP_GET, handleRoot);
	server.on("/main.js", HTTP_GET, handleJS);
	server.on("/networks", HTTP_GET, handleNetworks);
	server.on("/connect", HTTP_POST, handleConnect);
	server.on("/stop-wifi", HTTP_GET, handleStopWifi);

	server.onNotFound(handleNotFound);

	server.begin();

	return;
}

void NetworkConnector::handleRoot()
{
	File file = SPIFFS.open("/index.html", "r");

	server.streamFile(file, "text/html");

	file.close();
}

void NetworkConnector::handleJS()
{
	File file = SPIFFS.open("/main.js", "r");

	server.streamFile(file, "text/javascript");

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

void NetworkConnector::handleConnect()
{
	Serial.println("Hit /connect");

	String ssid = server.arg("ssid");
	String password = server.arg("password");

	Serial.println("Recieved ssid " + ssid + " and password " + password);

	WiFi.begin(ssid.c_str(), password.c_str());

	int wifiTryCount = 1;

	while (WiFi.status() != WL_CONNECTED) {
		Serial.println("Connecting to wifi...");

		if (WiFi.status() == WL_CONNECT_FAILED || wifiTryCount >= 10) {
			server.send(401, "text/plain", "Wrong password");
			return;
		}

		wifiTryCount += 1;
		delay(1000);
	}

	File ssidFile = SPIFFS.open("/ssid.txt", "w");

	if (ssidFile.print(ssid)) {
		Serial.println("SSID file written.");
	}
	else {
		Serial.println("Failed to write SSID file.");
	}
	ssidFile.close();

	File passwordFile = SPIFFS.open("/password.txt", "w");

	if (passwordFile.print(password)) {
		Serial.println("Password file written.");
	}
	else {
		Serial.println("Failed to write password file");
	}

	passwordFile.close();

	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(200, "application/json", "{ \"connected\": true, \"uid\": \"" + WiFi.macAddress() + "\" }");
}

void NetworkConnector::handleNotFound()
{
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(404, "text/plain", "You've left the path...");
}

void NetworkConnector::handleStopWifi()
{
	WiFi.softAPdisconnect(true);
	server.stop();
}

// Values from here: http://www.veris.com/docs/whitePaper/vwp18_RSSI_RevA.pdf
String NetworkConnector::parseRSSI(int rssi)
{
	if (rssi > -40) return "4";
	if (rssi > -55 && rssi <= -40) return "3";
	if (rssi > -70 && rssi <= -55) return "2";
	if (rssi > -80 && rssi <= -70) return "1";
}

void NetworkConnector::loop()
{
	dnsServer.processNextRequest();
	server.handleClient();
}