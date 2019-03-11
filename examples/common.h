#include "../private/common.h"

#include <stdio.h>
#include <time.h>

double time_diff(struct timespec *start, struct timespec *end)
{
    struct timespec result;

    if (end->tv_nsec < start->tv_nsec) {
        result.tv_sec = end->tv_sec - start->tv_sec - 1;
        result.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
    } else {
        result.tv_sec = end->tv_sec - start->tv_sec;
        result.tv_nsec = end->tv_nsec - start->tv_nsec;
    }
    return (double) result.tv_sec + (double) result.tv_nsec / 1000000000.0;
}
