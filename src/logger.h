#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdio.h>  // for FILE and print functions
#include <stdarg.h> // for varargs in write_to_logfile
#include <limits.h> // for NAME_MAX (maximum filename length)

#define ENABLE_LOGGING 1

#if ENABLE_LOGGING
#define ASTAR_LOG "astar.log"
#endif // ENABLE_LOGGING

typedef struct logFile {
    char name[NAME_MAX];
    FILE *fp;
    struct logFile *next;
} logfile_t;

enum {
    RET_OK  =   0x00,   //  Operation succeeded
    RET_NOK =   0x02,   //  Operation failed
    RET_FNF =   0x04    //  File not found
};

int init_logging(void);
int free_logs(void);
int add_logfile(const char* filename);
int write_to_logfile(const char* filename, const char* string, ...);

#endif // __LOGGER_H_