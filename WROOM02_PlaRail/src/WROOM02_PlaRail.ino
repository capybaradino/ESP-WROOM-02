#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Arduino.h>
#include "WROOM02.h"

ESP8266WebServer server(80);

#define READY_LED_PIN 16

#define LEDDELAY_WHEN_CONNECTED 250


void handleRoot() {
    String msg = "<html><head><title>ESP8266WebSerber</title></head>";
    msg += "<body>";
    msg += "<h1>You are connected</h1>";
	msg += "<form action=\"/stop/\" target=\"result\" name=\"form_stop\" ></form>";
	msg += "<form action=\"/forw/\" target=\"result\" name=\"form_forw\" ></form>";
	msg += "<form action=\"/forf/\" target=\"result\" name=\"form_forf\" ></form>";
	msg += "<form action=\"/back/\" target=\"result\" name=\"form_back\" ></form>";
	msg += "<h2><a href=\"javascript:document.form_stop.submit()\">STOP</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_forw.submit()\">MOVE FORWARD</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_forf.submit()\">MOVE FORWARD FASTER</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_back.submit()\">MOVE BACK</a></h2>";
	msg += "<iframe src=\"\" name=\"result\"></iframe>";
    msg += "</body></html>";
    server.send(200, "text/html", msg);
}

void pla_stop(){
  server.send(200, "text/html", "<h1>STOPPED</h1>");
	//TODO gpio control
}

void pla_forw(){
  server.send(200, "text/html", "<h1>MOVE FORWARD</h1>");
	//TODO gpio control
}

void pla_forf(){
  server.send(200, "text/html", "<h1>MOVE FORWARD FASTER</h1>");
	//TODO gpio control
}

void pla_back(){
  server.send(200, "text/html", "<h1>MOVE BACK</h1>");
	//TODO gpio control
}

void setup() {
  pinMode(READY_LED_PIN,OUTPUT);
  digitalWrite(READY_LED_PIN,LOW);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
 
  Serial.println("");
  Serial.println("Start");
  Serial.print("Connecting to ");
  Serial.println(ssid);
// Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(3000);  //Needed for ?

  server.on("/", handleRoot);
  server.on("/stop/", pla_stop);
  server.on("/forw/", pla_forw);
  server.on("/forf/", pla_forf);
  server.on("/back/", pla_back);
  server.begin();
  Serial.println("HTTP server started");

  for(int i=0; i<2; i++)
  {
    digitalWrite(READY_LED_PIN, HIGH);
    delay(LEDDELAY_WHEN_CONNECTED);
    digitalWrite(READY_LED_PIN, LOW);      
    delay(LEDDELAY_WHEN_CONNECTED);
  }
}

boolean isConnected=false;
void loop() {
  server.handleClient();
  if (!isConnected && WiFi.status())
  {
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    delay(5000);
    Serial.println(WiFi.localIP());
    isConnected=true;
  }
}

