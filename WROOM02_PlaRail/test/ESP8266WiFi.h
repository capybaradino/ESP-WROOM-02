#include "com.h"
#define WIFI_STA (0)

class WiFi
{
	public:
	int status();
	char* localIP();
//	void begin();
	void begin(String, String);
	void mode(int);
	private:
};

int WiFi::status()
{
	return 0;
}

char* WiFi::localIP()
{
	return NULL;
}

//void WiFi::begin()
//{
//}

void WiFi::mode(int mode)
{
}

void WiFi::begin(String ssid, String pw)
{
}

WiFi WiFi;
