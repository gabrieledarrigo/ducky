#ifndef DUCKY_LOGGER_H
#define DUCKY_LOGGER_H

typedef enum log_level {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} log_level;

void logs(log_level level, const char *fmt, ...);

#endif
