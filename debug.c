#include <stdio.h>
#ifdef _DEBUG
void log_debug(const char *msg) {
    fprintf(stderr, "debug: %s\n", msg);
}
void log_info(const char *msg) {
    fprintf(stderr, "info: %s\n", msg);
}
#else
#include <syslog.h>
void log_debug(const char *msg) {
    return;
}
void log_info(const char *msg) {
    syslog(LOG_INFO, "%s", msg);
}
#endif
