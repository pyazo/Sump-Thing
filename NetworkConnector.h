#include <Arduino.h>

class NetworkConnector
{
public:
	NetworkConnector();
	bool hasNetworkConnection;
	void loop();
private:
	char *ssid;
	char *password;
	void broadcastWiFi();
	static void handleRoot();
	static void handleNotFound();
};
