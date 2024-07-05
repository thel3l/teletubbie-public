#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <WiFi.h>
#include <WebServer.h>

// IR Transmitter Setup
const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);     // Set the GPIO to be used to sending the message.

// WiFi Setup
const char *ssid = "XXXXXX";     // Replace with your WiFi SSID
const char *password = "XXXXXX"; // Replace with your WiFi password
WebServer server(80);

void setup()
{
  Serial.begin(115200);
  irsend.begin(); // Start the IR transmitter

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up HTTP server
  server.on("/post", HTTP_POST, []()
            {
    if (server.hasArg("plain")) {
      String message = server.arg("plain");
      Serial.println("Received POST data:");
      Serial.println(message);

      // Convert the received hex string to unsigned long
      unsigned long hexCode = strtoul(message.c_str(), NULL, 16);
      Serial.print("Sending IR code: ");
      Serial.println(hexCode, HEX);
      irsend.sendRC6(hexCode);

      server.send(200, "text/plain", "Data received and IR code sent");
    } else {
      server.send(500, "text/plain", "Bad POST request: no data found");
    } });

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}