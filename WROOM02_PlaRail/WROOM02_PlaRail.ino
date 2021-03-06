#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Arduino.h>
#include "WROOM02.h"

ESP8266WebServer server(80);

#define READY_LED_PIN 14
#define LEDDELAY_WHEN_CONNECTED 250

#define MP4212_PIN_2 13
#define MP4212_PIN_4 16
#define MP4212_PIN_6 5
#define MP4212_PIN_8 4
#define ANALOG_MIDSPEED 800
#define ANALOG_TOPSPEED 850

#define USE_ANALOG_2_4
#define USE_ANALOG_6_8

void handleRoot() {
	String msg = "<html><head><title>ESP8266WebSerber</title></head>";
	msg += "<body>";
	msg += "<h1>You are connected</h1>";
#ifdef USE_ANALOG_2_4
  msg += "<h2>USE_ANALOG_2_4 ON " + String(ANALOG_MIDSPEED) + "/" + String(ANALOG_TOPSPEED) + "</h2>";
#else
  msg += "<h2>USE_ANALOG_2_4 OFF</h2>";
#endif
#ifdef USE_ANALOG_6_8
  msg += "<h2>USE_ANALOG_6_8 ON " + String(ANALOG_MIDSPEED) + "/" + String(ANALOG_TOPSPEED) + "</h2>";
#else
  msg += "<h2>USE_ANALOG_6_8 OFF</h2>";
#endif
	msg += "<form action=\"/stop/\" target=\"result\" name=\"form_stop\" ></form>";
	msg += "<form action=\"/forw/\" target=\"result\" name=\"form_forw\" ></form>";
	msg += "<form action=\"/forf/\" target=\"result\" name=\"form_forf\" ></form>";
	msg += "<form action=\"/back/\" target=\"result\" name=\"form_back\" ></form>";
  msg += "<form action=\"/bacf/\" target=\"result\" name=\"form_bacf\" ></form>";
	msg += "<h2><a href=\"javascript:document.form_stop.submit()\">STOP</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_forw.submit()\">MOVE FORWARD</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_forf.submit()\">MOVE FORWARD FASTER</a></h2>";
	msg += "<h2><a href=\"javascript:document.form_back.submit()\">MOVE BACK</a></h2>";
  msg += "<h2><a href=\"javascript:document.form_bacf.submit()\">MOVE BACK FASTER</a></h2>";
	msg += "<iframe src=\"\" name=\"result\"></iframe>";
	msg += "</body></html>";
	server.send(200, "text/html", msg);
}

void pla_stop(){
	server.send(200, "text/html", "<h1>STOPPED</h1>");
#ifdef USE_ANALOG_2_4
	analogWrite(MP4212_PIN_2,0);
	analogWrite(MP4212_PIN_4,0);
#else
	digitalWrite(MP4212_PIN_2,LOW);
	digitalWrite(MP4212_PIN_4,LOW);
#endif
#ifdef USE_ANALOG_6_8
	analogWrite(MP4212_PIN_6,1023);
	analogWrite(MP4212_PIN_8,1023);
#else
	digitalWrite(MP4212_PIN_6,HIGH);
	digitalWrite(MP4212_PIN_8,HIGH);
#endif
}

void pla_forw(){
	server.send(200, "text/html", "<h1>MOVE FORWARD</h1>");
#ifdef USE_ANALOG_2_4
	analogWrite(MP4212_PIN_2,0);
	analogWrite(MP4212_PIN_4,ANALOG_MIDSPEED);
#else
	digitalWrite(MP4212_PIN_2,LOW);
	digitalWrite(MP4212_PIN_4,HIGH);
#endif
#ifdef USE_ANALOG_6_8
	analogWrite(MP4212_PIN_6,1023-ANALOG_MIDSPEED);
	analogWrite(MP4212_PIN_8,1023);
#else
	digitalWrite(MP4212_PIN_6,LOW);
	digitalWrite(MP4212_PIN_8,HIGH);
#endif
}

void pla_forf(){
	server.send(200, "text/html", "<h1>MOVE FORWARD FASTER</h1>");
#ifdef USE_ANALOG_2_4
	analogWrite(MP4212_PIN_2,0);
	analogWrite(MP4212_PIN_4,ANALOG_TOPSPEED);
#else
	digitalWrite(MP4212_PIN_2,LOW);
	digitalWrite(MP4212_PIN_4,HIGH);
#endif
#ifdef USE_ANALOG_6_8
	analogWrite(MP4212_PIN_6,1023-ANALOG_TOPSPEED);
	analogWrite(MP4212_PIN_8,1023);
#else
	digitalWrite(MP4212_PIN_6,LOW);
	digitalWrite(MP4212_PIN_8,HIGH);
#endif
}

void pla_back(){
	server.send(200, "text/html", "<h1>MOVE BACK</h1>");
#ifdef USE_ANALOG_2_4
	analogWrite(MP4212_PIN_2,ANALOG_MIDSPEED);
	analogWrite(MP4212_PIN_4,0);
#else
	digitalWrite(MP4212_PIN_2,HIGH);
	digitalWrite(MP4212_PIN_4,LOW);
#endif
#ifdef USE_ANALOG_6_8
	analogWrite(MP4212_PIN_6,1023);
	analogWrite(MP4212_PIN_8,1023-ANALOG_MIDSPEED);
#else
	digitalWrite(MP4212_PIN_6,HIGH);
	digitalWrite(MP4212_PIN_8,LOW);
#endif
}

void pla_bacf(){
  server.send(200, "text/html", "<h1>MOVE BACK FASTER</h1>");
#ifdef USE_ANALOG_2_4
  analogWrite(MP4212_PIN_2,ANALOG_TOPSPEED);
  analogWrite(MP4212_PIN_4,0);
#else
  digitalWrite(MP4212_PIN_2,HIGH);
  digitalWrite(MP4212_PIN_4,LOW);
#endif
#ifdef USE_ANALOG_6_8
  analogWrite(MP4212_PIN_6,1023);
  analogWrite(MP4212_PIN_8,1023-ANALOG_TOPSPEED);
#else
  digitalWrite(MP4212_PIN_6,HIGH);
  digitalWrite(MP4212_PIN_8,LOW);
#endif
}

void setup() {
  pinMode(READY_LED_PIN,OUTPUT);
  digitalWrite(READY_LED_PIN,LOW);

	pinMode(MP4212_PIN_2,OUTPUT);
	pinMode(MP4212_PIN_4,OUTPUT);
	pinMode(MP4212_PIN_6,OUTPUT);
	pinMode(MP4212_PIN_8,OUTPUT);
#ifdef USE_ANALOG_2_4
	analogWrite(MP4212_PIN_2,0);
	analogWrite(MP4212_PIN_4,0);
#else
	digitalWrite(MP4212_PIN_2,LOW);
	digitalWrite(MP4212_PIN_4,LOW);
#endif
#ifdef USE_ANALOG_6_8
	analogWrite(MP4212_PIN_6,1023);
	analogWrite(MP4212_PIN_8,1023);
#else
	digitalWrite(MP4212_PIN_6,HIGH);
	digitalWrite(MP4212_PIN_8,HIGH);
#endif

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
  server.on("/bacf/", pla_bacf);
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

