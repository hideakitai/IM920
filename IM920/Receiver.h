#ifndef RECEIVER_H
#define RECEIVER_H

#if defined (TEENSYDUINO)
#include "Arduino.h"
#include <cstdint>
#include <algorithm>
#elif defined(__AVR__)
#include "Arduino.h"
#elif defined OF_VERSION_MAJOR
#include "ofMain.h"
#else
#error THIS PLATFORM IS NOT SUPPORTED
#endif

namespace IM920Ctrl {

template <typename S>
class Receiver
{
    // response format

    // OK<CR><LF>
    // NG<CR><LF>
    // VALUE<CR><LF>
    // aa,bbbb,cc:XX,YY,ZZ<CR><LF>
    // aa: NODE, bbbb: UID, cc: RSSI

public:

    inline Receiver()
    : stream(), asc_buffer(), bin_buffer(), read_count(), is_available(false)
    {};
    inline ~Receiver() {};

    bool setup(S& s)
    {
        stream = &s;
        stream->flush();
        return true;
    }

	void update()
    {
        while (stream->available())
        {
            asc_buffer.bytes[read_count] = readByte();
            if (asc_buffer.bytes[read_count] == '\n')
            {
                parse();
                memset(asc_buffer.bytes, 0, sizeof(asc_buffer.bytes));
                read_count = 0;
                break;
            }
            else
            {
                ++read_count;
            }
        }
    }

    uint8_t remoteNode() { return bin_buffer.node; }
    uint16_t remoteUID() { return bin_buffer.uid; }
    uint8_t remoteRSSI() { return bin_buffer.rssi; }

    const char* data() { return (const char*)bin_buffer.data; }
    uint8_t data(const uint8_t i) { return bin_buffer.data[i]; }

    bool available() { return is_available; }

    void pop()
    {
        is_available = false;
        memset(bin_buffer.data, 0, sizeof(bin_buffer.data));
    }

    uint8_t size() { return bin_buffer.size; }

	void print();

protected:

    uint8_t readByte();

    void verbose(const char* c);

    void parse()
    {
        if (asc_buffer.bytes[3] == '\n')
        {
            verbose(asc_buffer.bytes);
            verbose("\n");
        }
        else if (asc_buffer.bytes[10] == ':')
        {
            char* endptr = nullptr;
            char* token_ptr = strtok(asc_buffer.bytes, " ,:\r\n");

            bin_buffer.node = (uint8_t)strtol(token_ptr, &endptr, 16);
            token_ptr = strtok(nullptr, " ,:\r\n");
            bin_buffer.uid = (uint16_t)strtol(token_ptr, &endptr, 16);
            token_ptr = strtok(nullptr, " ,:\r\n");
            bin_buffer.rssi = (uint16_t)strtol(token_ptr, &endptr, 16);

            uint8_t count = 0;
            token_ptr = strtok(nullptr, " ,:\r\n");
            while (token_ptr != nullptr)
            {
                bin_buffer.data[count] = (uint8_t)strtol(token_ptr, &endptr, 16);
                ++count;
                token_ptr = strtok(nullptr, " ,:\r\n");
            }
            bin_buffer.size = count;

            if (*endptr)
            {
                verbose("token parse error\n");
            }
            else
            {
                is_available = true;
            }
        }
        else
        {
            char* token_ptr = strtok(asc_buffer.bytes, " ,:\r\n");
            if ((token_ptr != nullptr) && (!strcmp(token_ptr, "IM920")))
            {
                verbose(token_ptr);
                verbose(" ");
                token_ptr = strtok(nullptr, " ,:\r\n");
                verbose(token_ptr);
            }
            else
            {
                verbose(asc_buffer.bytes);
                verbose("\n");
            }
        }
    }

    union RecvAsciiBuffer
    {
        // OK, NG
        struct {
            char result[4]; // 2 + <CR><LF>
            char empty[200]; // <CR><LF> + empty
        };
        // TXDT, TXDA
        struct {
            char node[3]; // 2 + 1
            char uid[5]; // 4 + 1
            char rssi[3]; // 2 + 1
            char data[193]; // (2 + 1) * n - 1 + <CR><LF>
        };
        // others
        char bytes[204];
    };

    struct RecvBinBuffer
    {
        uint16_t uid;
        uint8_t node;
        uint8_t rssi;
        uint8_t data[64];
        uint8_t size;
    };

    S* stream;
    RecvAsciiBuffer asc_buffer;
    RecvBinBuffer bin_buffer;
    uint8_t read_count {0};
    bool is_available {false};

};


#if defined (TEENSYDUINO) || defined(__AVR__)

    template <>
    void Receiver<Stream>::print()
    {
        Serial.print(remoteNode(), HEX); Serial.print(",");
        Serial.print(remoteUID(),  HEX); Serial.print(",");
        Serial.print(remoteRSSI(), HEX); Serial.print(":");
        for (uint8_t i = 0; i < size() - 1; ++i)
        {
            Serial.print(data(i), HEX);
            Serial.print(",");
        }
        Serial.println(data(size() - 1), HEX);
    }

	template <>
	uint8_t Receiver<Stream>::readByte() { return (uint8_t)stream->read(); }

    template <>
    void Receiver<Stream>::verbose(const char* c) { Serial.print(c); }

#elif defined (OF_VERSION_MAJOR)

	template <>
	void Receiver<ofSerial>::print()
	{
		std::stringstream ss;
        ss << hex << std::setw(2) << std::setfill('0');
		ss << (int)remoteNode() << ",";
		ss << (int)remoteUID()  << ",";
		ss << (int)remoteRSSI() << ":";
        for (size_t i = 1; i < size(); ++i) ss << (int)data(i - 1) << ",";
		ss << (int)data(size() - 1) << endl;
        cout << ss.str() << endl;
	}

	template <>
	uint8_t Receiver<ofSerial>::readByte() { return (uint8_t)stream->readByte(); }

    template <>
    void Receiver<ofSerial>::verbose(const char* c) { cout << c; }

#else

    #error THIS PLATFORM IS NOT SUPPORTED

#endif


} // namespace IM920Ctrl

#endif /* RECEIVER_H */