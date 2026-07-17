extern "C" void app_main(void);

#include "unity.h"

static void test_app_main_runs(void) { app_main(); }

void run_test_app_main_tests(void)
{
    RUN_TEST(test_app_main_runs);
}
