#ifndef IM920_SETTINGS_H
#define IM920_SETTINGS_H

namespace IM920Ctrl {

enum BAUDRATE
{
    BPS_1200 = 0,
    BPS_2400 = 1,
    BPS_4800 = 2,
    BPS_9600 = 3,
    BPS_19200 = 4, // default
    BPS_38400 = 5,
    BPS_57600 = 6,
    BPS_115200 = 7
};

enum CHANNEL
{
    CH24_920_6MHZ = 1, // default
    CH25_920_8MHZ = 2,
    CH26_921_0MHZ = 3,
    CH27_921_2MHZ = 4,
    CH28_921_4MHZ = 5,
    CH29_921_6MHZ = 6,
    CH30_921_8MHZ = 7,
    CH31_922_0MHZ = 8,
    CH32_922_2MHZ = 9,
    CH33_922_4MHZ = 10,
    CH34_922_6MHZ = 11,
    CH35_922_8MHZ = 12,
    CH36_923_0MHZ = 13,
    CH37_923_2MHZ = 14,
    CH38_923_4MHZ = 15
};

enum RF_POWER
{
    POWER_0_1_mW = 1,
    POWER_1_0_mW = 2,
    POWER_10_0_mW = 3 // default
};

enum RF_RATE
{
    RATE_FAST = 1,
    RATE_LONG = 2 // default
};

} // namespace IM920Ctrl


#endif /* IM920_SETTINGS_H */

