#include "com.h"

class ESP8266WebServer
{
	public:
	ESP8266WebServer(int);
	void handleClient();
	void begin();
	void on(const char*, void (*func)(void));
	void send(int, const char*, String);
	private:
};

ESP8266WebServer::ESP8266WebServer(int PORT)
{
}

void ESP8266WebServer::handleClient()
{
	std::cout << "handleClient()" << "\n";
	sleep(1);
}

void ESP8266WebServer::begin()
{
}

void ESP8266WebServer::on(const char* PATH, void (*func)(void))
{
}

void ESP8266WebServer::send(int HTTP_CODE, const char* MIME_TYPE, String BODY)
{
}

