#include "esp_sntp.h"
#include "nvs.h"
#include "TimeSync.h"

#include "unity.h"

static void test_init_configures_sntp(void)
{
    unit_test_nvs_reset();
    chip::Esp32TimeSync::Init("pool.ntp.org", 1);
    TEST_ASSERT_TRUE(esp_sntp_enabled());
    TEST_ASSERT_EQUAL_STRING("pool.ntp.org", unit_test_time_sync_last_server());
}

static void test_init_rejects_zero_interval(void)
{
    chip::Esp32TimeSync::Init("pool.ntp.org", 0);
}

static void test_time_sync_callback_persists_nvs(void)
{
    unit_test_nvs_reset();
    chip::Esp32TimeSync::Init("pool.ntp.org", 1);
    unit_test_time_sync_fire_callback(1704067201LL, 0);
}

void run_test_time_sync_tests(void)
{
    RUN_TEST(test_init_configures_sntp);
    RUN_TEST(test_init_rejects_zero_interval);
    RUN_TEST(test_time_sync_callback_persists_nvs);
}
