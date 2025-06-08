#ifndef _CAN_H
#define _CAN_H

enum can_bitrate
{
    CAN_BITRATE_10K = 0,
    CAN_BITRATE_20K,
    CAN_BITRATE_50K,
    CAN_BITRATE_100K,
    CAN_BITRATE_125K,
    CAN_BITRATE_250K,
    CAN_BITRATE_500K,
    CAN_BITRATE_750K,
    CAN_BITRATE_1000K,

    CAN_BITRATE_INVALID,
};

typedef enum can_bus_state
{
    OFF_BUS = 0,
    ON_BUS = 1,
} can_bus_state_t;

// CAN transmit buffering
#define TXQUEUE_LEN 28    // Number of buffers allocated
#define TXQUEUE_DATALEN 8 // CAN DLC length of data buffers

void can_init(void);

#endif // _CAN_H