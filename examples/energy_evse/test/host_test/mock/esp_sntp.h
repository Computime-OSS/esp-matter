#pragma once

#include <cstdint>

#define SNTP_OPMODE_POLL 0

struct timeval;

typedef void (*sntp_sync_time_cb_t)(struct timeval *tv);

bool esp_sntp_enabled(void);
void esp_sntp_setoperatingmode(int mode);
void esp_sntp_setservername(int idx, const char *server);
void sntp_set_time_sync_notification_cb(sntp_sync_time_cb_t cb);
void esp_sntp_init(void);

void unit_test_time_sync_fire_callback(int64_t sec, int32_t usec);
const char *unit_test_time_sync_last_server(void);
