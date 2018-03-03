#include "com.h"
#include <signal.h>

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

static int servSock; //server socket descripter

class ESP8266WebServer
{
	public:
	ESP8266WebServer(int);
	void handleClient();
	void begin();
	void on(const char*, void (*func)(void));
	void send(int, const char*, String);
	private:
	int clitSock; //client socket descripter
	struct sockaddr_in servSockAddr; //server internet socket address
	struct sockaddr_in clitSockAddr; //client internet socket address
	static const unsigned short servPort = ESPWEB_PORT; //server port number
	unsigned int clitLen; // client internet socket address length
	char inbuf[ESPWEB_BUFSIZE];
	unordered_map<string, func_void> map;
	char method_path[ESPWEB_METHOD_PATHSIZE + 1];
};

void sig_handler(int signo)
{
	if (signo == SIGINT) {
		printf("received SIGINT. close socket.\n");
		close(servSock);
	}
	exit(0);
}

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

	clitLen = sizeof(clitSockAddr);
	if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0)
	{
		perror("accept() failed.");
		exit(EXIT_FAILURE);
	}

	printf("connected from %s.", inet_ntoa(clitSockAddr.sin_addr));

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

	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		printf("\ncan't catch SIGUSR1\n");
	}
}

void ESP8266WebServer::on(const char* PATH, func_void func)
{
	map[PATH] = func;
}

void ESP8266WebServer::send(int HTTP_CODE, const char* MIME_TYPE, String BODY)
{

//Response HTTP message
	string str = "";
	str += "HTTP/1.0 " + to_string(HTTP_CODE);
	switch(HTTP_CODE)
	{
		case 200:
		str += " OK"; break;
		case 404:
		str += " Not Found"; break;
		default: break;
	}
	str += "\r\n";
	str += "Content-Length: " + to_string(BODY.length()) + "\r\n";
	str += "Content-Type: text/html\r\n";
	str += "\r\n";
	str += BODY;
//Response HTTP message

	::send(clitSock, str.c_str(), str.length(), 0);
}

