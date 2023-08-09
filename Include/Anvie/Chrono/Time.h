/**
 * @file Time.h
 * @date Wed, 9th Aug, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Get time in various resolutions
 * */

#ifndef ANVIE_CHRONO_TIME_H
#define ANVIE_CHRONO_TIME_H

#include <Anvie/Types.h>

#ifdef __linux__

#include <sys/time.h>

Size anv_chrono_get_time_as_seconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((Size)tv.tv_sec)+((Size)tv.tv_usec)/1000000;
}

Size anv_chrono_get_time_as_milliseconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((Size)tv.tv_sec)*1000+((Size)tv.tv_usec)/1000;
}

Size anv_chrono_get_time_as_microseconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((Size)tv.tv_sec)*1000000+((Size)tv.tv_usec);
}

Size anv_chrono_get_time_as_nanoseconds(void) {
    return 0;
    // TODO:
}


#else
#error Platform not supported for time
#endif

#endif // ANVIE_CHRONO_TIME_H
