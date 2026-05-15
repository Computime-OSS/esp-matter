#pragma once

#include <cstdint>
#include <mutex>

#include "helpers.h"

namespace CT {
namespace Charger {

struct HwChargingMeter_t {
    uint32_t current_mA;
    uint32_t voltage_mV;
    uint32_t power_mW;
    uint64_t energy_mWh;
};

struct HwChargingLimit_t {
    int32_t current_mA;
};

enum class HwCableStatus_t : uint8_t {
    NOT_CONNECTED = 0,
    CONNECTED = 1,
};

enum class HwChargeCommand_t : uint8_t {
    STOP = 0,
    START = 1,
    PAUSE = 2,
};

struct HwControlState_t {
    HwCableStatus_t cable;
    HwChargingLimit_t limit;
    HwChargeCommand_t chargeCommand;
    HwChargingMeter_t meter;
};

class HardwareControlInterface {
public:
    static HardwareControlInterface &Instance() {
        static HardwareControlInterface inst;
        return inst;
    }

    HardwareControlInterface(const HardwareControlInterface &) = delete;
    HardwareControlInterface &operator=(const HardwareControlInterface &) = delete;

    void resetForTest() {
        std::lock_guard<std::mutex> lock(mutex_);
        state_ = {};
        is_charging_ = false;
        is_EV_drawing_ = false;
        faultCode_ = 0;
    }

    void resetMeterData() {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.meter = {};
    }

    void setCableStatus(HwCableStatus_t s) {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.cable = s;
    }

    HwCableStatus_t getCableStatus() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.cable;
    }

    void setChargeCommand(HwChargeCommand_t cmd) {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.chargeCommand = cmd;
    }

    HwChargeCommand_t getChargeCommand() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.chargeCommand;
    }

    void setMeter(const HwChargingMeter_t &m) {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.meter = m;
    }

    HwChargingMeter_t getMeter() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.meter;
    }

    void setLimit(const HwChargingLimit_t &m) {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.limit = m;
    }

    HwChargingLimit_t getLimit() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.limit;
    }

    void setFaultCode(uint8_t code) {
        std::lock_guard<std::mutex> lock(mutex_);
        faultCode_ = code;
    }

    uint8_t getFaultCode() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return faultCode_;
    }

    void setEVDrawing(bool drawing) {
        std::lock_guard<std::mutex> lock(mutex_);
        is_EV_drawing_ = drawing;
    }

    bool isEVDrawing() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return is_EV_drawing_;
    }

    bool isCharging() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return is_charging_;
    }

    void setChargingForTest(bool v) {
        std::lock_guard<std::mutex> lock(mutex_);
        is_charging_ = v;
    }

private:
    HardwareControlInterface() = default;

    mutable std::mutex mutex_;
    HwControlState_t state_{};
    bool is_charging_ = false;
    bool is_EV_drawing_ = false;
    uint8_t faultCode_ = 0;
};

} // namespace Charger
} // namespace CT
