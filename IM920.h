#ifndef ARDUINO_IM920_H
#define ARDUINO_IM920_H

#if defined (TEENSYDUINO) || defined(__AVR__)
#include "Arduino.h"
#elif defined OF_VERSION_MAIJOR
#include "ofMain.h"
#endif
#include "IM920/IM920Commands.h"
#include "IM920/IM920Settings.h"
#include "IM920/Sender.h"
#include "IM920/Receiver.h"

namespace IM920 {

template <typename S>
class IM920 : public Sender<S>, public Receiver<S>
{
public:

	bool setup(S& s)
    {
		Sender<S>::setup(s);
		Receiver<S>::setup(s);
        return true;
    }

private:

};

}

#endif /* ARDUINO_IM920_H */