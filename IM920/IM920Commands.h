#ifndef IM920_CMD_LIST_H
#define IM920_CMD_LIST_H

namespace IM920 {
namespace CMD {

enum TYPE
{
    INFO,
    CTRL,
    FUNC,
    SETTING,
    SEND
};

template<TYPE>
struct Cmd{};

template<>
struct Cmd<INFO>
{
    static constexpr const char* UNIQUE_ID = "RDID ";
    static constexpr const char* RSSI = "RDRS ";
    static constexpr const char* VERSION = "RDVR ";
    static constexpr const char* ALL = "RPRM ";
};

template<>
struct Cmd<CTRL>
{
    static constexpr const char* BAUDRATE = "SBRT ";
    static constexpr const char* ERASE_ID = "ERID ";
    static constexpr const char* RESET = "SRST ";
    static constexpr const char* CLEAR_SETTINGS = "PCLR ";
};

template<>
struct Cmd<FUNC>
{
    static constexpr const char* EN_SAVE = "ENWR ";
    static constexpr const char* DS_SAVE = "DSWR ";
    static constexpr const char* EN_CHAR_IO= "ECIO ";
    static constexpr const char* DS_CHAR_IO= "DCIO ";
    static constexpr const char* EN_SLEEP = "DSRX ";
    static constexpr const char* DS_SLEEP = "ENRX ";
    static constexpr const char* EN_ANSWER_BACK = "EABK ";
    static constexpr const char* DS_ANSWER_BACK = "DABK ";
    static constexpr const char* EN_REPEATER = "ERPT ";
    static constexpr const char* DS_REPEATER = "DRPT ";
};

template<>
struct Cmd<SETTING>
{
    static constexpr const char* W_NODE = "STNN ";
    static constexpr const char* R_NODE = "RDNN ";
    static constexpr const char* W_RECEIVE_ID = "SRID ";
    static constexpr const char* R_RECEIVE_ID = "RRID ";
    static constexpr const char* W_CHANNEL = "STCH ";
    static constexpr const char* R_CHANNEL = "RDCH ";
    static constexpr const char* W_RF_POWER = "STPO ";
    static constexpr const char* R_RF_POWER = "RDPO ";
    static constexpr const char* W_RF_RATE = "STRT ";
    static constexpr const char* R_RF_RATE = "RDRT ";
    static constexpr const char* W_SLEEP_TIME = "SSTM ";
    static constexpr const char* R_SLEEP_TIME = "RSTM ";
    static constexpr const char* W_WAIT_TIME = "SWTM ";
    static constexpr const char* R_WAIT_TIME = "RWTM ";
};

template<>
struct Cmd<SEND>
{
    static constexpr const char* EIGHT_BYTES = "TXDT ";
    static constexpr const char* MULTI_BYTES = "TXDA ";
};

}
}

#endif
