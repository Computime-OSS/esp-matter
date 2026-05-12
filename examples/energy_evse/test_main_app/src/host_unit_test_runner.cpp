#include "get_readable_time.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace {

int run_get_readable_time_tests()
{
#ifdef CT_HOST_TEST_LOG_GET_READABLE_TIME
    std::fprintf(
        stderr,
        "[test_main_app] host_unit_test_runner: exercising GetReadableTime() "
        "(object code linked from ../main/driver/ev_charger/get_readable_time.cpp)\n");
#endif

    char buf[128];

    GetReadableTime(0, buf, sizeof buf);
    if (std::strcmp(buf, "NOT SET") != 0)
    {
        std::fprintf(stderr, "expected NOT SET, got %s\n", buf);
        return 1;
    }

    // One day after CHIP epoch start; TZ=UTC in test harness for stable strftime.
    GetReadableTime(86400u, buf, sizeof buf);
    if (std::strcmp(buf, "02/01/2000 00:00") != 0)
    {
        std::fprintf(stderr, "expected 02/01/2000 00:00, got %s\n", buf);
        return 2;
    }

#ifdef CT_HOST_TEST_LOG_GET_READABLE_TIME
    std::fprintf(stderr, "[test_main_app] host_unit_test_runner: all checks passed (exit 0)\n");
#endif
    return 0;
}

} // namespace

int main()
{
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    if (::setenv("TZ", "UTC", 1) != 0)
    {
        std::fprintf(stderr, "setenv TZ failed\n");
        return 3;
    }
#endif
    return run_get_readable_time_tests();
}
