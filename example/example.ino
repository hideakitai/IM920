#include "IM920.h"

IM920 im920;
const size_t usb_baudrate = 115200;
const size_t im920_baudrate = 19200;

const uint32_t response_wait_ms = 500;
uint32_t prev_ms = 0;

void WaitForResponse(uint16_t ms = 0)
{
    delay(ms);

    im920.update();
    if (im920.available())
    {
        im920.print();
        im920.pop();
    }
}

void HandleResponse()
{
    im920.update();
    if (im920.available())
    {
        // get remote device info
        Serial.print(im920.remoteNode(), HEX); Serial.print(",");
        Serial.print(im920.remoteUID(),  HEX); Serial.print(",");
        Serial.print(im920.remoteRSSI(), HEX); Serial.print(":");

        // get & handle received data
        for (size_t i = 0; i < im920.size() - 1; ++i)
        {
            Serial.print(im920.data(i), HEX);
            Serial.print(",");
        }
        Serial.println(im920.data(im920.size() - 1), HEX);

        im920.pop();
    }
}

void setup()
{
    Serial.begin(usb_baudrate);

    // for Uno
    Serial.begin(im920_baudrate);
    im920.setup(Serial);

    // for Mega, Teensy, and other arduino which have multiple serial
    // Serial2.begin(im920_baudrate);
    // im920.setup(Serial2);

    delay(2000);

    Serial.println("write setting start");

    // write settings to IM920
    im920.writable(true);
    WaitForResponse(response_wait_ms);
    im920.clearSettings();
    WaitForResponse(response_wait_ms);
    im920.eraseID();
    WaitForResponse(response_wait_ms);
    im920.recvID(0x1234);
    WaitForResponse(response_wait_ms);
    im920.channel(IM920Param::CHANNEL::CH36_923_0MHZ);
    WaitForResponse(response_wait_ms);
    im920.power(IM920Param::RF_POWER::POWER_10_0_mW);
    WaitForResponse(response_wait_ms);
    im920.rate(IM920Param::RF_RATE::RATE_FAST);
    WaitForResponse(response_wait_ms);
    im920.reset();
    WaitForResponse(response_wait_ms);

    Serial.println("start test");
}

void loop()
{
    HandleResponse();

    if (millis() - prev_ms > 1000)
    {
        // append data to buffer & send
        im920 << "test" << 1;
        im920 << "data" << 2 << 3;
        im920.send();

        delay(100);

        // send in one line
        im920.send(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        prev_ms = millis();
    }
}
