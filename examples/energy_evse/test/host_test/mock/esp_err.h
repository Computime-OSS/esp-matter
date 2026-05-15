#pragma once

#include <cstdint>

typedef int esp_err_t;

#define ESP_OK 0
#define ESP_ERR_INVALID_ARG 0x102
#define ESP_ERR_NOT_SUPPORTED 0x106

inline const char *esp_err_to_name(int err) {
    switch (err) {
    case ESP_OK:
        return "ESP_OK";
    case ESP_ERR_INVALID_ARG:
        return "ESP_ERR_INVALID_ARG";
    case ESP_ERR_NOT_SUPPORTED:
        return "ESP_ERR_NOT_SUPPORTED";
    default:
        return "UNKNOWN";
    }
}
