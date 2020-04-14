#include <Arduino.h>

class NetworkConnector
{
public:
	NetworkConnector();
	bool hasNetworkConnection;
	void loop();
private:
	String broadcastedSSID;
	String savedSSID;
	String savedPassword;
	void connectToWiFi();
	void broadcastWiFi();
	void readCredentials();
	static void handleRoot();
	static void handleJS();
	static void handleNetworks();
	static void handleConnect();
	static void handleNotFound();
	static void handleStopWifi();
	static String parseRSSI(int rssi);
};
