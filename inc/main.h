#ifndef _MAIN_H
#define _MAIN_H

#ifdef DEBUG_PRINT
#define DEBUG_LOG(fmt, ...) printf("DEBUG: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt, ...) ((void)0) // 什么都不做
#endif

#endif