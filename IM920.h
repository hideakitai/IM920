#ifndef ARDUINO_IM920_H
#define ARDUINO_IM920_H

#if defined (TEENSYDUINO) || defined(__AVR__)
#include "Arduino.h"
using SerialStream = Stream;
#elif defined OF_VERSION_MAIJOR
#include "ofMain.h"
using SerialStream = ofSerial;
#endif
#include "IM920/IM920Commands.h"
#include "IM920/IM920Settings.h"
#include "IM920/Sender.h"
#include "IM920/Receiver.h"

template <typename S>
class IM920_ : public IM920Ctrl::Sender<S>, public IM920Ctrl::Receiver<S>
{
public:

	bool setup(S& s)
    {
		IM920Ctrl::Sender<S>::setup(s);
		IM920Ctrl::Receiver<S>::setup(s);
        return true;
    }

private:

};

using IM920 = IM920_<SerialStream>;
namespace IM920Param = IM920Ctrl;

#endif /* ARDUINO_IM920_H */