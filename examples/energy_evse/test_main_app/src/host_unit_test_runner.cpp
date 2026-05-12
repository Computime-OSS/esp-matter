#include "get_readable_time.h"
#include "matter_schedule_allow.h"

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
    std::fprintf(stderr, "[test_main_app] run_get_readable_time_tests: all checks passed (exit 0)\n");
#endif
    return 0;
}

int run_matter_schedule_allow_tests()
{
    // Logic extracted from MatterManager::IsChargingAllowedByTargets (matter_schedule_allow.cpp).
    using std::time_t;
    const time_t tStart  = 1000;
    const time_t tTarget = 2000;

    if (MatterScheduleIsChargingAllowedAtUnix(500, tStart, tTarget))
    {
        return 10;
    }
    if (!MatterScheduleIsChargingAllowedAtUnix(1000, tStart, tTarget))
    {
        return 11;
    }
    if (!MatterScheduleIsChargingAllowedAtUnix(1500, tStart, tTarget))
    {
        return 12;
    }
    if (!MatterScheduleIsChargingAllowedAtUnix(1999, tStart, tTarget))
    {
        return 13;
    }
    if (MatterScheduleIsChargingAllowedAtUnix(2000, tStart, tTarget))
    {
        return 14;
    }
    if (MatterScheduleIsChargingAllowedAtUnix(2500, tStart, tTarget))
    {
        return 15;
    }

#ifdef CT_HOST_TEST_LOG_GET_READABLE_TIME
    std::fprintf(stderr, "[test_main_app] run_matter_schedule_allow_tests: all checks passed (exit 0)\n");
#endif
    return 0;
}

void print_usage(const char * prog)
{
    std::fprintf(
        stderr,
        "Usage: %s [SUITE]\n"
        "  SUITE: get_readable_time | matter_schedule | all (default: all)\n",
        prog);
}

} // namespace

int main(int argc, char ** argv)
{
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    if (::setenv("TZ", "UTC", 1) != 0)
    {
        std::fprintf(stderr, "setenv TZ failed\n");
        return 3;
    }
#endif
    const char * suite = "all";
    if (argc >= 2)
    {
        suite = argv[1];
    }
    if (argc > 2)
    {
        print_usage(argv[0]);
        return 99;
    }

    if (std::strcmp(suite, "get_readable_time") == 0)
    {
        return run_get_readable_time_tests();
    }
    if (std::strcmp(suite, "matter_schedule") == 0)
    {
        return run_matter_schedule_allow_tests();
    }
    if (std::strcmp(suite, "all") == 0)
    {
        const int schedule_rc = run_matter_schedule_allow_tests();
        if (schedule_rc != 0)
        {
            return schedule_rc;
        }
        return run_get_readable_time_tests();
    }

    std::fprintf(stderr, "unknown suite: %s\n", suite);
    print_usage(argv[0]);
    return 99;
}
