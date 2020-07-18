#include "../lib/greatest.h"
#include "../src/logger.h"

TEST should_log_to_stderr(void) {
    logs(LOG_TRACE, "Tracing application %i", 2000);
    logs(LOG_INFO, "Ducky up and running");
    logs(LOG_DEBUG, "Send %i byte", 16);
    logs(LOG_WARN, "Warning cannot parse command");
    logs(LOG_ERROR, "Error, cannot SEND command");
    logs(LOG_FATAL, "Fatal error cannot: create cache data structure");
}

SUITE(suite) {
    RUN_TEST(should_log_to_stderr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}
