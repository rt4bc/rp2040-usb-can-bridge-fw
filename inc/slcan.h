#ifndef _SLCAN_H
#define _SLCAN_H

// maximum rx buffer len: extended CAN frame with timestamp
#define SLCAN_MTU 30 // (sizeof("T1111222281122334455667788EA5F\r")+1)

#define SLCAN_STD_ID_LEN 3
#define SLCAN_EXT_ID_LEN 8

#endif // _SLCAN_H