#ifndef IM920_SENDER_H
#define IM920_SENDER_H

#if defined (TEENSYDUINO)
#include "Arduino.h"
#include <cstdint>
#include <type_traits>
#elif defined(__AVR__)
#include "Arduino.h"
#elif defined OF_VERSION_MAJOR
#include "ofMain.h"
#include <string>
#else
#error THIS PLATFORM IS NOT SUPPORTED
#endif
#include "IM920Commands.h"
#include "IM920Settings.h"

namespace IM920Ctrl {

template <typename S>
class Sender
{
public:
    Sender() {}
    ~Sender() {}

    bool setup(S& s)
    {
        stream = &s;
        stream->flush();
        return true;
    }


    // IM92j0 info
    void uniqueID()
    {
        append(CMD::Cmd<CMD::TYPE::INFO>::UNIQUE_ID, 5);
        write();
    }

    void rssi()
    {
        append(CMD::Cmd<CMD::TYPE::INFO>::RSSI, 5);
        write();
    }

    void version()
    {
        append(CMD::Cmd<CMD::TYPE::INFO>::VERSION, 5);
        write();
    }

    void dump()
    {
        append(CMD::Cmd<CMD::TYPE::INFO>::ALL, 5);
        write();
    }

    // IM920 control
    void baudrate(const BAUDRATE& baud)
    {
        append(CMD::Cmd<CMD::TYPE::CTRL>::BAUDRATE, 5);
        append(baud);
        write();
    }

    void eraseID()
    {
        append(CMD::Cmd<CMD::TYPE::CTRL>::ERASE_ID, 5);
        write();
    }

    void reset()
    {
        append(CMD::Cmd<CMD::TYPE::CTRL>::RESET, 5);
        write();
    }

    void clearSettings()
    {
        append(CMD::Cmd<CMD::TYPE::CTRL>::CLEAR_SETTINGS, 5);
        write();
    }

    // enable / disable function
    void writable(const bool b)
    {
        if (b) append(CMD::Cmd<CMD::TYPE::FUNC>::EN_SAVE, 5);
        else   append(CMD::Cmd<CMD::TYPE::FUNC>::DS_SAVE, 5);
        write();
    }

    void charIO(const bool b)
    {
        if (b) append(CMD::Cmd<CMD::TYPE::FUNC>::EN_CHAR_IO, 5);
        else   append(CMD::Cmd<CMD::TYPE::FUNC>::DS_CHAR_IO, 5);
        write();
    }

    void sleep(const bool b)
    {
        if (b) append(CMD::Cmd<CMD::TYPE::FUNC>::EN_SLEEP, 5);
        else   append(CMD::Cmd<CMD::TYPE::FUNC>::DS_SLEEP, 5);
        write();
    }

    void answerback(const bool b)
    {
        if (b) append(CMD::Cmd<CMD::TYPE::FUNC>::EN_ANSWER_BACK, 5);
        else   append(CMD::Cmd<CMD::TYPE::FUNC>::DS_ANSWER_BACK, 5);
        write();
    }

    void repeater(const bool b)
    {
        if (b) append(CMD::Cmd<CMD::TYPE::FUNC>::EN_REPEATER, 5);
        else   append(CMD::Cmd<CMD::TYPE::FUNC>::DS_REPEATER, 5);
        write();
    }

    // settings
    void node()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_NODE, 5);
        write();
    }
    void node(uint8_t node)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_NODE, 5);
        append(node);
        write();
    }

    void recvID()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_RECEIVE_ID, 5);
        write();
    }
    void recvID(uint16_t id)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_RECEIVE_ID, 5);
        append(id);
        write();
    }

    void channel()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_CHANNEL, 5);
        write();
    }
    void channel(const CHANNEL channel)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_CHANNEL, 5);
        if (channel < 10) append("0", 1);
        append((uint8_t)channel, 10);
        write();
    }

    void power()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_RF_POWER, 5);
        write();
    }
    void power(const RF_POWER power)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_RF_POWER, 5);
        append((uint8_t)power, 10);
        write();
    }

    void rate()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_RF_RATE, 5);
        write();
    }
    void rate(RF_RATE rate)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_RF_RATE, 5);
        append((uint8_t)rate, 10);
        write();
    }

    void sleepTime()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_SLEEP_TIME, 5);
        write();
    }
    void sleepTime(uint16_t ms)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_SLEEP_TIME, 5);
        append(ms);
        write();
    }

    void waitTime()
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::R_WAIT_TIME, 5);
        write();
    }
    void waitTime(uint16_t ms)
    {
        append(CMD::Cmd<CMD::TYPE::SETTING>::W_WAIT_TIME, 5);
        append(ms);
        write();
    }

    // send data
    void send()
    {
        write();
    }

    template <typename First, typename ...Rest>
    void send(First&& first, Rest&& ...args)
    {
        if (empty()) append(CMD::Cmd<CMD::TYPE::SEND>::MULTI_BYTES, 5);
        append(first);
        send(args...);
    }

    template <typename T>
    Sender<S>& operator<< (const T& arg)
    {
        if (empty()) append(CMD::Cmd<CMD::TYPE::SEND>::MULTI_BYTES, 5);
        append(arg);
        return *this;
    }

    Sender<S>& operator<< (const char* arg)
    {
        if (empty()) append(CMD::Cmd<CMD::TYPE::SEND>::MULTI_BYTES, 5);
        append(arg, sizeof(arg));
        return *this;
    }

protected:

#if defined (TEENSYDUINO)

    using StringType = String;

    template <typename T, typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>::value>::type* = nullptr>
    String toHex(const T& value)
    {
        size_t size = sizeof(T) * 2;
        String format = "%0" + String(size) + "X";
        char hex[size + 1];
        sprintf(hex, format.c_str(), value);
        return String(hex);
    }

    bool empty() { return (asc_buffer.length() == 0); }

#elif defined(__AVR__)

    using StringType = String;

    // works correctly only for integer
    template <typename T>
    String toHex(const T& value)
    {
        size_t size = sizeof(T) * 2;
        String format = "%0" + String(size) + "X";
        char hex[size + 1];
        sprintf(hex, format.c_str(), value);
        return String(hex);
    }

    bool empty() { return (asc_buffer.length() == 0); }

#elif defined OF_VERSION_MAJOR

    using StringType = std::string;

    bool empty() { return asc_buffer.empty(); }

#else

#error THIS PLATFORM IS NOT SUPPORTED

#endif

    template <typename T>
    void append(const T& n, uint8_t base = 16);
    void append(const char* c, const uint8_t size) { asc_buffer += c; }

    void delimiter() { append("\r\n", 2); }

    void write();

    S* stream;
    StringType asc_buffer {""};

};

#if defined (TEENSYDUINO) || defined(__AVR__)

template <>
void Sender<Stream>::write()
{
    delimiter();
    stream->write(asc_buffer.c_str(), asc_buffer.length());
    asc_buffer.remove(0, asc_buffer.length());
}

template <>
template <typename T>
void Sender<Stream>::append(const T& n, uint8_t base)
{
    String s;
    if      (base == 16) s = toHex(n);
    else if (base == 10) s = String(n, DEC);
    else    { Serial.println("invalid base parameter"); return; }
    asc_buffer += s;
}

#elif defined (OF_VERSION_MAJOR)

template <>
void Sender<ofSerial>::write()
{
    delimiter();
    stream->writeBytes((unsigned char*)asc_buffer.c_str(), asc_buffer.length());
    asc_buffer.clear();
}

template <>
template <typename T>
void Sender<ofSerial>::append(const T& n, uint8_t base)
{
    stringstream ss;
    if      (base == 16) ss << std::hex;
    else if (base == 10) ss << std::dec;
    else    { ofLogError("invalid base parameter"); return; }
    ss << std::setw(sizeof(T) * 2) << std::setfill('0') << (size_t)n;
    asc_buffer += ss.str();
}

#else

#error THIS PLATFORM IS NOT SUPPORTED

#endif

} // namespace IM920Ctrl

#endif /* IM920_SENDER_H */
