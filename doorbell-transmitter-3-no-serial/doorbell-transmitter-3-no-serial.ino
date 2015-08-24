#include <ESP8266WiFi.h>
const char* ssid     = "SSID";
const char* password = "wifi-password";
const char* host = "192.168.1.57"; // addresss / URL of "server"
const int httpPort = 80;
String url = "/on";

IPAddress ip(192, 168, 1, 58); // address of this module
IPAddress gatewayip(192, 168, 1, 1);
IPAddress subnetmask(255, 255, 255, 0);

void setup() {
  pinMode(2, INPUT);
  WiFi.config(ip, gatewayip, subnetmask);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

bool button_state = LOW;

void loop() {
  
  button_state = digitalRead(2);
  if (button_state == LOW) {
    WiFiClient client;
    if (client.connect(host, httpPort)) {
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      while (client.available()) {
        String line = client.readStringUntil('\r');
        delay(10);
      }
      delay(10);
      client.stop();
    }
  }
  delay(1000);
}
