#include "com.h"

#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <unordered_map> //unordered_map(HashMap)
typedef void (*func_void)(void);
using namespace std;

#define QUEUELIMIT 5
#define ESPWEB_PORT 30080
#define ESPWEB_BUFSIZE 2048
#define ESPWEB_FUNCNUM 16
#define ESPWEB_METHOD_PATHSIZE 20

class ESP8266WebServer
{
	public:
	ESP8266WebServer(int);
	void handleClient();
	void begin();
	void on(const char*, void (*func)(void));
	void send(int, const char*, String);
	private:
	int servSock; //server socket descripter
	int clitSock; //client socket descripter
	struct sockaddr_in servSockAddr; //server internet socket address
	struct sockaddr_in clitSockAddr; //client internet socket address
	static const unsigned short servPort = ESPWEB_PORT; //server port number
	unsigned int clitLen; // client internet socket address length
	char buf[ESPWEB_BUFSIZE];
	char inbuf[ESPWEB_BUFSIZE];
	unordered_map<string, func_void> map;
	char method_path[ESPWEB_METHOD_PATHSIZE + 1];
};

ESP8266WebServer::ESP8266WebServer(int PORT)
{
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	memset(&servSockAddr, 0, sizeof(servSockAddr));
	servSockAddr.sin_family      = AF_INET;
	servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servSockAddr.sin_port        = htons(servPort);
}

void ESP8266WebServer::handleClient()
{
	cout << "handleClient()" << "\n";
	while(1)
	{
		clitLen = sizeof(clitSockAddr);
		if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0)
		{
			perror("accept() failed.");
			exit(EXIT_FAILURE);
		}

		printf("connected from %s.", inet_ntoa(clitSockAddr.sin_addr));

//Response HTTP message
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf),
		"HTTP/1.0 200 OK\r\n"
		"Content-Length: 20\r\n"
		"Content-Type: text/html\r\n"
		"\r\n");
//Response HTTP message

		memset(inbuf, 0, sizeof(inbuf));
		recv(clitSock, inbuf, sizeof(inbuf), 0);
		//printf("%s", inbuf);

		//parse request message
		memset(method_path, '\0', sizeof(method_path));
		memcpy(method_path, inbuf, ESPWEB_METHOD_PATHSIZE);
		string str = method_path;
		vector<string> strarr = split(str, ' ');
		string http_method = strarr[0];
		string request_path = strarr[1];
		cout << " HTTP method = " << http_method << ", request path = " << request_path << "\n";

		//TODO send dummy header
		::send(clitSock, buf, (int)strlen(buf), 0);

		func_void func = map[request_path];
		if (func == NULL) func = map[request_path + "/"];
		if (func == NULL)
		{
			send(404, "text/html", "<h1>404 Not Found</h1>");
		}
		else
		{
			func();
		}

		close(clitSock);
	}
	sleep(1);
}

void ESP8266WebServer::begin()
{
	if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr) ) < 0 )
	{
		perror("bind() failed.");
		exit(EXIT_FAILURE);
	}

	if (listen(servSock, QUEUELIMIT) < 0)
	{
		perror("listen() failed.");
		exit(EXIT_FAILURE);
	}
}

void ESP8266WebServer::on(const char* PATH, func_void func)
{
	map[PATH] = func;
}

void ESP8266WebServer::send(int HTTP_CODE, const char* MIME_TYPE, String BODY)
{
	::send(clitSock, BODY.c_str(), BODY.length(), 0);
}

