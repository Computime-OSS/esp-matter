#include "calculate_energy.h"

float calculate_energy(float amps, float volts, float hours) {
    // Basic P = I * V
    float power_watts = amps * volts;
    // Convert to kWh: (Watts * hours) / 1000
    float energy_kwh = (power_watts * hours) / 1000.0f;
    return energy_kwh;
}