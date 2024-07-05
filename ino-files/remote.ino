#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <queue>

// IR Transmitter Setup
const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);     // Set the GPIO to be used to sending the message.
std::queue<String> irCodeQueue;

// WiFi Setup
const char *ssid = "XXXXXX";     // Replace with your WiFi SSID
const char *password = "XXXXXX"; // Replace with your WiFi password
WebServer server(80);

// IR codes (your existing code arrays)
uint16_t power[49] = {2596, 986, 362, 530, 362, 506, 388, 974, 364, 956, 834, 508, 390, 504, 388, 504, 388, 504, 390, 506, 388, 504, 388, 506, 390, 502, 390, 502, 388, 504, 388, 506, 388, 504, 388, 506, 388, 532, 360, 532, 360, 506, 834, 504, 386, 952, 388, 532, 360};
uint16_t one[49] = {2622, 960, 388, 506, 386, 506, 386, 950, 388, 954, 836, 510, 388, 536, 356, 506, 386, 530, 362, 506, 386, 506, 388, 506, 388, 502, 390, 530, 362, 532, 360, 504, 388, 504, 390, 504, 390, 504, 388, 506, 388, 502, 390, 502, 390, 506, 386, 506, 838};
uint16_t two[49] = {2642, 892, 390, 492, 390, 494, 414, 908, 440, 882, 832, 490, 390, 492, 414, 468, 390, 492, 414, 466, 416, 466, 412, 470, 412, 468, 414, 468, 390, 492, 442, 440, 390, 492, 390, 494, 388, 494, 390, 490, 390, 492, 398, 486, 414, 468, 828, 934, 388};
uint16_t three[49] = {2598, 984, 360, 536, 360, 532, 360, 976, 362, 954, 836, 510, 388, 504, 388, 532, 360, 532, 360, 532, 360, 510, 384, 508, 386, 504, 388, 504, 388, 506, 388, 532, 360, 534, 360, 506, 386, 530, 362, 532, 360, 534, 360, 530, 362, 506, 834, 530, 362};
uint16_t four[49] = {2622, 960, 390, 502, 390, 504, 388, 948, 388, 958, 834, 508, 386, 506, 388, 502, 390, 502, 390, 504, 390, 500, 390, 504, 390, 502, 390, 502, 390, 506, 388, 504, 390, 504, 388, 502, 390, 504, 388, 506, 388, 504, 390, 504, 836, 944, 390, 504, 390};
uint16_t five[47] = {2622, 916, 390, 492, 388, 492, 418, 904, 388, 936, 854, 468, 416, 464, 418, 464, 416, 466, 414, 468, 416, 466, 388, 494, 388, 494, 442, 438, 444, 440, 388, 494, 444, 438, 388, 492, 446, 436, 416, 468, 386, 494, 446, 436, 832, 932, 830};
uint16_t six[49] = {2620, 962, 390, 502, 390, 502, 390, 948, 388, 956, 836, 508, 390, 502, 388, 504, 390, 504, 388, 504, 388, 504, 388, 504, 388, 504, 392, 502, 388, 504, 390, 502, 390, 502, 390, 502, 390, 504, 390, 500, 390, 504, 390, 504, 836, 500, 392, 948, 388};
uint16_t seven[49] = {2600, 956, 392, 504, 388, 502, 392, 948, 390, 952, 838, 510, 388, 500, 392, 506, 388, 504, 390, 502, 388, 504, 390, 506, 388, 502, 390, 502, 392, 504, 390, 502, 390, 502, 392, 504, 390, 500, 392, 504, 390, 502, 390, 502, 838, 504, 388, 500, 390};
uint16_t eight[51] = {228, 36734, 2670, 860, 440, 442, 442, 440, 440, 882, 442, 880, 882, 440, 468, 416, 440, 442, 466, 416, 466, 414, 464, 416, 468, 414, 442, 442, 442, 442, 494, 386, 440, 440, 442, 440, 440, 442, 440, 442, 464, 418, 442, 440, 906, 854, 466, 416, 442, 442, 442};
uint16_t nine[49] = {276, 49466, 2644, 892, 442, 444, 412, 466, 442, 880, 442, 882, 908, 416, 440, 440, 440, 442, 416, 466, 440, 444, 410, 470, 442, 442, 466, 414, 416, 466, 466, 418, 442, 438, 442, 440, 494, 386, 470, 412, 470, 414, 442, 440, 936, 826, 494, 388, 856};
uint16_t zero[51] = {2590, 946, 388, 498, 410, 468, 388, 934, 386, 936, 828, 498, 384, 496, 384, 498, 386, 492, 414, 470, 384, 496, 388, 494, 412, 470, 386, 494, 388, 494, 414, 466, 414, 470, 410, 470, 386, 496, 412, 470, 414, 468, 410, 472, 386, 498, 408, 472, 412, 468, 388};
uint16_t channel_up[51] = {130, 43070, 2614, 918, 408, 472, 408, 476, 462, 860, 408, 912, 880, 442, 436, 446, 410, 474, 408, 472, 412, 470, 410, 472, 410, 470, 410, 472, 410, 470, 414, 470, 436, 444, 388, 496, 436, 442, 496, 390, 878, 884, 386, 496, 438, 446, 410, 470, 414, 470, 412};
uint16_t channel_down[47] = {2614, 922, 386, 496, 386, 496, 386, 938, 382, 938, 826, 496, 410, 470, 410, 474, 410, 468, 438, 446, 410, 470, 408, 474, 410, 472, 410, 474, 438, 444, 408, 470, 410, 474, 384, 496, 438, 444, 878, 884, 438, 442, 412, 472, 410, 472, 850};
uint16_t volume_down[49] = {202, 32740, 2614, 914, 412, 472, 384, 498, 386, 936, 408, 912, 826, 498, 408, 472, 412, 470, 412, 470, 386, 498, 386, 498, 384, 498, 436, 442, 414, 472, 384, 498, 438, 444, 386, 494, 388, 496, 384, 496, 386, 494, 826, 936, 440, 442, 386, 496, 828};
uint16_t volume_up[51] = {200, 38354, 2634, 914, 464, 418, 412, 470, 488, 834, 386, 938, 850, 470, 466, 418, 462, 418, 388, 494, 410, 470, 386, 496, 386, 496, 410, 474, 410, 470, 410, 472, 412, 472, 438, 442, 388, 496, 434, 446, 412, 472, 848, 914, 410, 472, 384, 496, 436, 446, 462};
uint16_t mute[47] = {2640, 890, 386, 494, 386, 496, 412, 912, 412, 910, 908, 414, 412, 470, 412, 470, 388, 492, 388, 496, 386, 498, 410, 472, 386, 494, 414, 470, 412, 468, 414, 472, 410, 472, 410, 468, 412, 470, 412, 470, 388, 494, 852, 470, 414, 910, 878};

void sendIRCode(const String &code)
{
  if (code == "power")
  {
    irsend.sendRaw(power, 49, 38);
    Serial.println("power");
  }
  else if (code == "one")
  {
    irsend.sendRaw(one, 49, 38);
    Serial.println("one");
  }
  else if (code == "two")
  {
    irsend.sendRaw(two, 49, 38);
    Serial.println("two");
  }
  else if (code == "three")
  {
    irsend.sendRaw(three, 49, 38);
    Serial.println("three");
  }
  else if (code == "four")
  {
    irsend.sendRaw(four, 49, 38);
    Serial.println("four");
  }
  else if (code == "five")
  {
    irsend.sendRaw(five, 47, 38);
    Serial.println("five");
  }
  else if (code == "six")
  {
    irsend.sendRaw(six, 49, 38);
    Serial.println("six");
  }
  else if (code == "seven")
  {
    irsend.sendRaw(seven, 49, 38);
    Serial.println("seven");
  }
  else if (code == "eight")
  {
    irsend.sendRaw(eight, 51, 38);
    Serial.println("eight");
  }
  else if (code == "nine")
  {
    irsend.sendRaw(nine, 49, 38);
    Serial.println("nine");
  }
  else if (code == "zero")
  {
    irsend.sendRaw(zero, 51, 38);
    Serial.println("zero");
  }
  else if (code == "channel_up")
  {
    irsend.sendRaw(channel_up, 51, 38);
    Serial.println("channel_up");
  }
  else if (code == "channel_down")
  {
    irsend.sendRaw(channel_down, 47, 38);
    Serial.println("channel_down");
  }
  else if (code == "volume_down")
  {
    irsend.sendRaw(volume_down, 49, 38);
    Serial.println("volume_down");
  }
  else if (code == "volume_up")
  {
    irsend.sendRaw(volume_up, 51, 38);
    Serial.println("volume_up");
  }
  else if (code == "mute")
  {
    irsend.sendRaw(mute, 47, 38);
    Serial.println("mute");
  }
  else
  {
    Serial.println("Unknown code: " + code);
  }
  delay(500);
}

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
  server.on("/post", HTTP_POST, handlePost);
  server.begin();
  Serial.println("HTTP server started");
}

void handlePost()
{
  if (server.hasArg("plain"))
  {
    String message = server.arg("plain");
    Serial.println("Received POST data:");
    Serial.println(message);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, message);

    if (!error)
    {
      JsonArray keycodes = doc["keycodes"];
      for (JsonVariant keycode : keycodes)
      {
        String code = keycode["code"].as<String>();
        irCodeQueue.push(code);
      }
    }
    else
    {
      message.trim();
      if (message.length() > 0)
      {
        irCodeQueue.push(message);
      }
    }

    server.send(200, "text/plain", "IR codes queued successfully");
  }
  else
  {
    server.send(400, "text/plain", "Bad POST request: no data found");
  }
}

void processIRQueue()
{
  if (!irCodeQueue.empty())
  {
    String code = irCodeQueue.front();
    sendIRCode(code);
    irCodeQueue.pop();
    delay(500); // Delay between sending codes
  }
}

void loop()
{
  server.handleClient();
  processIRQueue();
}