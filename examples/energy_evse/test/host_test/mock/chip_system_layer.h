#pragma once

#include "chip_support.h"

#include <cstdint>
#include <vector>

namespace chip {
namespace System {
namespace Clock {
using Seconds32 = int32_t;
} // namespace Clock
} // namespace System

namespace DeviceLayer {

class SystemLayerImpl;

using TimerCallback = void (*)(SystemLayerImpl *, void *);

class SystemLayerImpl {
public:
    static SystemLayerImpl & Get();

    CHIP_ERROR StartTimer(System::Clock::Seconds32 delay, TimerCallback onComplete, void * appState);
    void CancelTimer(TimerCallback onComplete, void * appState);

    void FireExpiredTimersForTest();
    void ResetForTest();

    void SetStartTimerFailForTest(bool fail) { start_timer_fail_ = fail; }

private:
    struct Entry {
        System::Clock::Seconds32 delay;
        TimerCallback callback;
        void * context;
    };

    std::vector<Entry> timers_;
    bool start_timer_fail_ = false;
};

// Matches CHIP PlatformManager: DeviceLayer::SystemLayer()
inline SystemLayerImpl & SystemLayer()
{
    return SystemLayerImpl::Get();
}

} // namespace DeviceLayer

namespace System {
using Layer = DeviceLayer::SystemLayerImpl;
} // namespace System

} // namespace chip
