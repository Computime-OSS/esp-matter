#pragma once

#include <cstdint>
#include <cstring>
#include <string>

namespace chip {

constexpr uint32_t kChipEpochSecondsSinceUnixEpoch = 10957U * 24U * 60U * 60U;

enum CHIP_ERROR : int {
    CHIP_NO_ERROR = 0,
    CHIP_ERROR_BUFFER_TOO_SMALL = 1,
    CHIP_ERROR_PROVIDER_LIST_EXHAUSTED = 2,
    CHIP_ERROR_NOT_FOUND = 3,
    CHIP_ERROR_INVALID_ARGUMENT = 4,
    CHIP_ERROR_INCORRECT_STATE = 5,
    CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE = 6,
    CHIP_ERROR_INTERNAL = 7,
    CHIP_ERROR_NO_MEMORY = 8,
    CHIP_ERROR_WRONG_KEY_TYPE = 9,
};

struct CHIP_ERROR_FORMAT {
    int code;
};

#define CHIP_ERROR_FORMAT "%d"

inline bool UnixEpochToChipEpochTime(uint32_t unixEpochTimeSeconds, uint32_t & outChipEpochTimeSeconds)
{
    if (unixEpochTimeSeconds < kChipEpochSecondsSinceUnixEpoch) {
        return false;
    }
    outChipEpochTimeSeconds = unixEpochTimeSeconds - kChipEpochSecondsSinceUnixEpoch;
    return true;
}

template <typename EnumType>
class BitMask {
public:
    BitMask() : mValue(0) {}
    explicit BitMask(uint8_t v) : mValue(v) {}
    uint8_t Raw() const { return mValue; }

private:
    uint8_t mValue;
};

struct MutableByteSpan {
    uint8_t * data = nullptr;
    size_t length = 0;
};

namespace Platform {
inline void CopyString(char * buf, size_t bufSize, const char * src)
{
    if (bufSize > 0) {
        std::strncpy(buf, src, bufSize - 1);
        buf[bufSize - 1] = '\0';
    }
}
} // namespace Platform

#define ChipLogError(module, ...) ((void)0)

namespace System {
namespace Clock {
using Milliseconds64 = int64_t;
using Seconds32 = int32_t;
} // namespace Clock

class SystemClock {
public:
    void SetRealTimeMsForTest(int64_t ms) { real_time_ms_ = ms; }

    CHIP_ERROR GetClock_RealTimeMS(Clock::Milliseconds64 & ms) const
    {
        ms = real_time_ms_;
        return CHIP_NO_ERROR;
    }

private:
    int64_t real_time_ms_ = 0;
};

inline SystemClock & SystemClockInstance()
{
    static SystemClock clock;
    return clock;
}

inline SystemClock & SystemClock() { return SystemClockInstance(); }

} // namespace System

#define VerifyOrDie(expr)                                                                                              \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
        }                                                                                                              \
    } while (0)

#define ReturnErrorCodeIf(cond, err)                                                                                   \
    do {                                                                                                               \
        if (cond) {                                                                                                    \
            return (err);                                                                                              \
        }                                                                                                              \
    } while (0)

#define ReturnErrorOnFailure(expr)                                                                                     \
    do {                                                                                                               \
        const CHIP_ERROR _err = (expr);                                                                                \
        if (_err != CHIP_NO_ERROR) {                                                                                   \
            return _err;                                                                                               \
        }                                                                                                              \
    } while (0)

#define VerifyOrReturnError(cond, err)                                                                                   \
    do {                                                                                                               \
        if (!(cond)) {                                                                                                 \
            return (err);                                                                                              \
        }                                                                                                              \
    } while (0)

#define VerifyOrReturnValue(cond, val)                                                                                 \
    do {                                                                                                               \
        if (!(cond)) {                                                                                                 \
            return (val);                                                                                              \
        }                                                                                                              \
    } while (0)

inline const char * ErrorStr(CHIP_ERROR) { return "CHIP_ERROR"; }

} // namespace chip

using chip::CHIP_ERROR;
using chip::CHIP_NO_ERROR;
using chip::CHIP_ERROR_BUFFER_TOO_SMALL;
using chip::CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
using chip::CHIP_ERROR_NOT_FOUND;
using chip::CHIP_ERROR_INVALID_ARGUMENT;
using chip::CHIP_ERROR_INCORRECT_STATE;
using chip::CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
using chip::CHIP_ERROR_INTERNAL;
using chip::CHIP_ERROR_NO_MEMORY;
using chip::CHIP_ERROR_WRONG_KEY_TYPE;
using chip::MutableByteSpan;
