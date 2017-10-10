# IM920
Interplan [IM920 series](http://www.interplan.co.jp/solution/wireless/im920.php) wrapper for Arduino and openFrameworks

## Usage
### Set up IM920

```c++

#include "IM920.h"

IM920 im920;

// setup IM920 for Arduino
Serial1.begin(im920_baudrate);
im920.setup(Serial1);

// setup IM920 for openFrameworks
ofSerial serial;
serial.setup("your serial device", im920_baudrate);
im920.setup(serial);

// write settings to IM920
im920.writable(true);
im920.clearSettings(); // set to factory default
im920.recvID(0x1234);
im920.channel(IM920Param::CHANNEL::CH36_923_0MHZ);
im920.power(IM920Param::RF_POWER::POWER_10_0_mW);
im920.rate(IM920Param::RF_RATE::RATE_FAST);
im920.reset();
```

### Reading Data
``` c++
im920.update();

if (im920.available())
{
    // get remote device info
    Serial.print(im920.remoteNode(), HEX); Serial.print(",");
    Serial.print(im920.remoteUID(),  HEX); Serial.print(",");
    Serial.print(im920.remoteRSSI(), HEX); Serial.print(":");

    // get & handle received data
    for (size_t i = 0; i < im920.size(); ++i)
    {
        Serial.print(im920.data(i), HEX);
        Serial.print(",");
    }

    // go to next data
    im920.pop();
}
```

### Sending Data

``` c++
// send in one line
im920.send(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

// append data to buffer & send
im920 << "test" << 1;
im920 << "data" << 2 << 3;
im920.send();

// same as above
im920.append("test");
im920.append(1);
im920.append("data");
im920.append(2);
im920.append(3);
im920.send();
```

## Parameters

``` c++
IM920Param::BAUDRATE::BPS_1200
IM920Param::BAUDRATE::BPS_2400
IM920Param::BAUDRATE::BPS_4800
IM920Param::BAUDRATE::BPS_9600
IM920Param::BAUDRATE::BPS_19200 // default
IM920Param::BAUDRATE::BPS_38400
IM920Param::BAUDRATE::BPS_57600
IM920Param::BAUDRATE::BPS_115200

IM920Param::CHANNEL::CH24_920_6MHZ // default
IM920Param::CHANNEL::CH25_920_8MHZ
IM920Param::CHANNEL::CH26_921_0MHZ
IM920Param::CHANNEL::CH27_921_2MHZ
IM920Param::CHANNEL::CH28_921_4MHZ
IM920Param::CHANNEL::CH29_921_6MHZ
IM920Param::CHANNEL::CH30_921_8MHZ
IM920Param::CHANNEL::CH31_922_0MHZ
IM920Param::CHANNEL::CH32_922_2MHZ
IM920Param::CHANNEL::CH33_922_4MHZ
IM920Param::CHANNEL::CH34_922_6MHZ
IM920Param::CHANNEL::CH35_922_8MHZ
IM920Param::CHANNEL::CH36_923_0MHZ
IM920Param::CHANNEL::CH37_923_2MHZ
IM920Param::CHANNEL::CH38_923_4MHZ

IM920Param::RF_POWER::POWER_0_1_mW
IM920Param::RF_POWER::POWER_1_0_mW
IM920Param::RF_POWER::POWER_10_0_mW // default

IM920Param::RF_RATE::RATE_FAST
IM920Param::RF_RATE::RATE_LONG // default
```

## Supported Platform

- NON-AVR Arduino Platforms
- openFrameworks (tested on 0.9.8)


## Notation

If you want to build the oF example, add IM920/IM920.h and IM920/IM920/* to your oF project.


## License

MIT