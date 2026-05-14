#pragma once

// A simple structure to hold our results
typedef struct {
    float power_kw;
    float total_energy_kwh;
} charger_stats_t;

// The function we want to test
float calculate_energy(float amps, float volts, float hours);
