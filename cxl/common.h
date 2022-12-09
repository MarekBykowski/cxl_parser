// Description: Extern definitions of the misc utility functions

#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <cmath>

#include <ctime>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>            // standard unix functions, like getpid()
#include <sys/types.h>         // various type definitions, like pid_t
#include <csignal>             // signal name macros, and the kill() prototype
#include <cstdlib>
#include <cstring>
#include <endian.h>

using namespace std;

// File-IO
extern void open_file(ifstream &file, const char *fname);
extern void open_file(ofstream &file, const char *fname);

// Utility
extern int    better_sleep(double sleep_time);
extern void   setup_signal_handlers(char *pgmname);
extern double diff_time(struct timeval *start, struct timeval *end);

// Command line parameters
extern void cmd_line_parse(int, char**);
extern const char *inp_file;
extern unsigned verbose;

#define PROC_CXL_IO_TLP      0x1
#define PROC_CXL_IO_DLLP     0x2
#define PROC_CXL_ALMP        0x4
#define PROC_CXL_CM_CNTRL    0x8
#define PROC_CXL_CM_PROTOCOL 0x10

#endif
