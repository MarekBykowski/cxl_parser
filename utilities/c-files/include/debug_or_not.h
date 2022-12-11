#ifndef __DEBUG_OR_NOT__
#define __DEBUG_OR_NOT__

#ifdef DEBUG
#define pr_debug(fmt, ...) printf("debug: " fmt, ##__VA_ARGS__)
#else
#define pr_debug(fmt, ...) do {} while(0)
#endif

#endif
