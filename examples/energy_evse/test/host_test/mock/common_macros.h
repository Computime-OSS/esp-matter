#pragma once

#define ABORT_APP_ON_FAILURE(x, ...)                                                                                   \
    do {                                                                                                               \
        if (!(x)) {                                                                                                    \
            __VA_ARGS__;                                                                                               \
        }                                                                                                              \
    } while (0)
