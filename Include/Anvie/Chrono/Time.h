/**
 * @file Time.h
 * @date Wed, 9th Aug, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2023 Siddharth Mishra
 * @copyright Copyright 2023 Anvie Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @brief Get time in various resolutions
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
