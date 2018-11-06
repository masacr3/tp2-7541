#define _GNU_SOURCE
#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"
#include <time.h>

time_t iso8601_to_time(const char* iso8601);
