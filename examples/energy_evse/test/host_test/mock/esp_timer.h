#pragma once

#include <cstdint>

/// Host test monotonic clock (microseconds); tests may adjust via reference.
inline int64_t &unit_test_esp_timer_now_us() {
    static int64_t v = 1'000'000LL;
    return v;
}

inline int64_t esp_timer_get_time(void) {
    return unit_test_esp_timer_now_us();
}
