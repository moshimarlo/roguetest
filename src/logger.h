#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdio.h>
#include <limits.h> // for NAME_MAX (maximum filename length)

typedef struct logFile {
    char name[NAME_MAX];
    FILE *fp;
    logfile_t *next;
} logfile_t;

enum {
    RET_OK  =   0x00,   //  Operation succeeded
    RET_NOK =   0x02,   //  Operation failed
    RET_FNF =   0x04    //  File not found
};

int init_logging(void);
int add_logfile(const char* filename);
int write_to_logfile(const char* filename, const char* string);

#endif // __LOGGER_H_