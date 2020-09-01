#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

static const char *log_levels[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

/**
 * A variadic function that logs to stderr.
 *
 * @param level     the desired log level
 * @param fmt       a C string that contains the text to be written to stderr. It can optionally contain embedded format specifiers
 * @param ...
 */
void logs(log_level level, const char *fmt, ...) {
    char buffer[100];
    time_t t = time(NULL);
    size_t len = strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&t));
    buffer[len] = '\0';

    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "%s %-5s ", buffer, log_levels[level]);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);

    va_end(ap);
}