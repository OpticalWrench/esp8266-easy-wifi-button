#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "SSID";
const char* password = "wifi-password";

IPAddress ip(192, 168, 1, 57); // address of this module (the "server")
IPAddress gatewayip(192, 168, 1, 1);
IPAddress subnetmask(255, 255, 255, 0);

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleOn() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "button pressed.");
  delay(500);
  digitalWrite(led, 0);
}

void handleOff() {
  digitalWrite(led, 0);
  server.send(200, "text/plain", "button is off.");
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
 
void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  
  WiFi.config(ip, gatewayip, subnetmask);
  WiFi.begin(ssid, password);
  
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
