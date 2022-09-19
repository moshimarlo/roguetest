#include "logger.h"

#include <string.h>

#define DEBUG_LOGGER

static logfile_t *lf_HEAD;

static logfile_t* find_logfile(const char* filename);

int init_logging(void)
{
    lf_HEAD = malloc(sizeof(lf_HEAD));
    if (lf_HEAD == NULL) return RET_NOK;
    lf_HEAD->fp = NULL;
    lf_HEAD->next = NULL;
    return RET_OK;
}

int free_logs(logfile_t* p)
{
    while (p->next != NULL) {
        free_logs(p->next);
    }
    free(p);
    return RET_OK;
}

static logfile_t* find_logfile(const char* filename)
{
    for (logfile_t *p = lf_HEAD; p->next != NULL; p = p->next) {
        if (strncmp(filename, p->name, NAME_MAX) == 0) return p;
    }
    return RET_FNF;
}

int write_to_logfile(const char* filename, const char* string)
{
    logfile_t *lf = find_logfile(filename);
    if (lf == RET_FNF) {
#ifdef DEBUG_LOGGER
        printf("Logger: file not found\n");
#endif
        return RET_FNF;
    }
	lf->fp = fopen(lf->name, "a");
    if (lf->fp == NULL) {
#ifdef DEBUG_LOGGER
        printf("Logger: fopen failed\n");
#endif
        return RET_NOK;
    }
	fprintf(lf->fp, "%s", string);
    fclose(lf->fp);
}

int add_logfile(const char* filename)
{
    logfile_t *p;
    for (p = lf_HEAD; p->next != NULL; p = p->next) {}
    p->next = malloc(sizeof(p));
    strncpy(p->next->name, filename, NAME_MAX);
}