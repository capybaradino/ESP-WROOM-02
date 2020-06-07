#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Arduino.h>
#include "WROOM02.h"

ESP8266WebServer server(80);

#define READY_LED_PIN 14
#define LEDDELAY_WHEN_CONNECTED 250
#define RBPI_RESET_PIN 16

void handleRoot() {
	String msg = "<html><head><title>ESP8266WebSerber</title></head>";
	msg += "<body>";
	msg += "<h1>You are connected</h1>";
	msg += "<form action=\"/pson/\" target=\"result\" name=\"form_pson\" ></form>";
	msg += "<h2><a href=\"javascript:document.form_pson.submit()\">PSON</a></h2>";
	msg += "<iframe src=\"\" name=\"result\"></iframe>";
	msg += "</body></html>";
	server.send(200, "text/html", msg);
}

void rbpi_pson(){
	server.send(200, "text/html", "<h1>Powering on</h1>");
	digitalWrite(RBPI_RESET_PIN,LOW);
	delay(1000);
	digitalWrite(RBPI_RESET_PIN,HIGH);
}

void setup() {
  pinMode(READY_LED_PIN,OUTPUT);
  digitalWrite(READY_LED_PIN,LOW);
  pinMode(RBPI_RESET_PIN,OUTPUT);
  digitalWrite(RBPI_RESET_PIN,HIGH);

  Serial.print("ssid=");
  Serial.println(ssid);
  Serial.print("ssid password=");
  Serial.println(password);

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
  server.on("/pson/", rbpi_pson);
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

