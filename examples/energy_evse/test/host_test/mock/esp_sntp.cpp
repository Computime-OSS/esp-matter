#include "esp_sntp.h"

#include <cstring>
#include <string>
#include <sys/time.h>

namespace {
bool g_enabled = false;
sntp_sync_time_cb_t g_cb = nullptr;
std::string g_server;
} // namespace

bool esp_sntp_enabled(void) { return g_enabled; }

void esp_sntp_setoperatingmode(int mode)
{
    (void) mode;
}

void esp_sntp_setservername(int idx, const char *server)
{
    if (idx == 0 && server != nullptr) {
        g_server = server;
    }
}

void sntp_set_time_sync_notification_cb(sntp_sync_time_cb_t cb) { g_cb = cb; }

void esp_sntp_init(void) { g_enabled = true; }

void unit_test_time_sync_fire_callback(int64_t sec, int32_t usec)
{
    if (g_cb == nullptr) {
        return;
    }
    timeval tv{};
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    g_cb(&tv);
}

const char *unit_test_time_sync_last_server(void) { return g_server.c_str(); }
