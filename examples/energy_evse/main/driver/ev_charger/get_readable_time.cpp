#include "get_readable_time.h"

#include <cstdio>
#include <ctime>

namespace {
// Matter spec: CHIP epoch starts at 2000-01-01 00:00:00 UTC (same as chip::kChipEpochSecondsSinceUnixEpoch).
constexpr uint32_t kChipEpochSecondsSinceUnixEpoch = 946684800u;
} // namespace

void GetReadableTime(uint32_t matterEpoch_s, char * outStr, size_t outSize)
{
    if (matterEpoch_s == 0)
    {
        snprintf(outStr, outSize, "NOT SET");
#ifdef CT_HOST_TEST_LOG_GET_READABLE_TIME
        ::fprintf(::stderr, "[energy_evse/main get_readable_time.cpp] GetReadableTime(%u) -> \"%s\"\n", matterEpoch_s, outStr);
#endif
        return;
    }

    time_t unixTime = static_cast<time_t>(matterEpoch_s + kChipEpochSecondsSinceUnixEpoch);

    struct tm timeInfo;
    localtime_r(&unixTime, &timeInfo);

    strftime(outStr, outSize, "%d/%m/%Y %H:%M", &timeInfo);
#ifdef CT_HOST_TEST_LOG_GET_READABLE_TIME
    ::fprintf(::stderr, "[energy_evse/main get_readable_time.cpp] GetReadableTime(%u) -> \"%s\"\n", matterEpoch_s, outStr);
#endif
}
