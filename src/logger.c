#include "logger.h"

#include <string.h>
#include <stdlib.h>

#define DEBUG_LOGGER 1

#define MAX_STRLEN 256

static logfile_t *lf_HEAD;

static int find_logfile(const char* filename, logfile_t** out);
static int free_log(logfile_t* p);

int init_logging(void)
{
    lf_HEAD = malloc(sizeof(*lf_HEAD));
    if (lf_HEAD == NULL) return RET_NOK;
    lf_HEAD->fp = NULL;
    lf_HEAD->next = NULL;
    return RET_OK;
}

int free_logs(void)
{
    free_log(lf_HEAD);
    return RET_OK;
}

static int free_log(logfile_t* p)
{
    while (p->next != NULL) {
        free_log(p->next);
    }
    free(p);
    return RET_OK;
}

static int find_logfile(const char* filename, logfile_t** out)
{
    for (logfile_t *p = lf_HEAD; p != NULL; p = p->next) {
        if (strncmp(filename, p->name, NAME_MAX) == 0) {
            *out = p;
            return RET_OK;
        }
    }
    return RET_FNF;
}

int write_to_logfile(const char* filename, const char* string, ...)
{
    logfile_t *lf;
    char buf[MAX_STRLEN];

    va_list vl;
    va_start(vl, string);
    vsnprintf(buf, sizeof(buf), string, vl);
    va_end(vl);

    if (find_logfile(filename, &lf) == RET_FNF) {
#if DEBUG_LOGGER
        fprintf(stderr, "Logger: file not found (%s)\n", filename);
#endif
        return RET_FNF;
    }
	lf->fp = fopen(lf->name, "a");
    if (lf->fp == NULL) {
#if DEBUG_LOGGER
        fprintf(stderr, "Logger: fopen failed\n");
#endif
        return RET_NOK;
    }
	fprintf(lf->fp, "%s", buf);
    fclose(lf->fp);
    return RET_OK;
}

int add_logfile(const char* filename)
{
    logfile_t *p = lf_HEAD;
    while (p->next != NULL) p = p->next;
    p->next = malloc(sizeof(*p->next));
    if (p->next == NULL) {
#if DEBUG_LOGGER
        fprintf(stderr, "Logger: couldn't allocate memory for new logfile\n");
#endif
        return RET_NOK;
    }
    strncpy(p->next->name, filename, NAME_MAX);
	p->next->fp = fopen(p->next->name, "a");
    fclose(p->next->fp);
    return RET_OK;
}