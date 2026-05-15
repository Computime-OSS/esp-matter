#include "chip_system_layer.h"

namespace chip {
namespace DeviceLayer {

SystemLayerImpl & SystemLayerImpl::Get()
{
    static SystemLayerImpl layer;
    return layer;
}

CHIP_ERROR SystemLayerImpl::StartTimer(System::Clock::Seconds32 delay, TimerCallback onComplete, void * appState)
{
    if (start_timer_fail_) {
        return CHIP_ERROR_INTERNAL;
    }
    timers_.push_back(Entry{ delay, onComplete, appState });
    return CHIP_NO_ERROR;
}

void SystemLayerImpl::CancelTimer(TimerCallback onComplete, void * appState)
{
    for (auto it = timers_.begin(); it != timers_.end();) {
        if (it->callback == onComplete && it->context == appState) {
            it = timers_.erase(it);
        } else {
            ++it;
        }
    }
}

void SystemLayerImpl::FireExpiredTimersForTest()
{
    const auto pending = timers_;
    timers_.clear();
    for (const auto & entry : pending) {
        if (entry.callback != nullptr) {
            entry.callback(this, entry.context);
        }
    }
}

void SystemLayerImpl::ResetForTest()
{
    timers_.clear();
    start_timer_fail_ = false;
}

} // namespace DeviceLayer
} // namespace chip
