#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

// IR Receiver Setup
#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 23; // 14 on a ESP32-C3 causes a boot loop.
#else                         // ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 23;
#endif                        // ARDUINO_ESP32C3_DEV
IRrecv irrecv(kRecvPin);
decode_results results;

// IR Transmitter Setup
const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);     // Set the GPIO to be used to sending the message.

// tatasky power button
uint16_t power[49] = {2596, 986, 362, 530, 362, 506, 388, 974, 364, 956, 834, 508, 390, 504, 388, 504, 388, 504, 390, 506, 388, 504, 388, 506, 390, 502, 390, 502, 388, 504, 388, 506, 388, 504, 388, 506, 388, 532, 360, 532, 360, 506, 834, 504, 386, 952, 388, 532, 360};
void setup()
{
    Serial.begin(115200);
    irrecv.enableIRIn(); // Start the receiver
    irsend.begin();      // Start the transmitter

    while (!Serial) // Wait for the serial connection to be established.
        delay(50);

    Serial.println();
    Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
    Serial.println(kRecvPin);
}

void loop()
{
    // Receive IR signals
    if (irrecv.decode(&results))
    {
        serialPrintUint64(results.value, HEX);
        Serial.println("");
        irrecv.resume(); // Receive the next value
    }

    // Transmit IR signals
    // Serial.println("RC6 transmission");
    // irsend.sendRC6(0xC0000C);
    // delay(2000);

    // Serial.println("rawData capture");
    // irsend.sendRaw(power, 49, 38);  // Send a raw data capture at 38kHz.

    // Serial.println("power");
    // irsend.sendNEC(0x499B750A);

    irsend.sendRaw(volume_up, 49, 38); // Send a raw data capture at 38kHz.
    Serial.println("volume up");
    delay(2000);

    //     irsend.sendRaw(five, 47, 38);  // Send a raw data capture at 38kHz.
    // Serial.println("five");
    // delay(2000);

    irsend.sendRaw(eight, 51, 38); // Send a raw data capture at 38kHz.
    Serial.println("eight");
    delay(2000);
}