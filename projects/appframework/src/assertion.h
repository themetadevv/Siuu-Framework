#pragma once

#include "core.h"
#include "utils.h"
#include "Application/Logger.h"

#if defined(ENABLE_ASSERTION)
#define AF_ASSERT(X, ...) \
    do { \
        if (!(X)) { \
            AF_ERROR( \
                "Assertion Failed\n" \
                "  Expression : {}\n" \
                "  Message    : {}\n"  \
                "  Location   : {}::{}", \
                #X, \
                fmt::format(__VA_ARGS__), \
                util::get_file_name_from_path(__FILE__), \
                __LINE__ \
            ); \
            DEBUG_BREAK(); \
        } \
    } while (0)
#else
#define AF_ASSERT(X, ...) \
    do { \
        if (!(X)) { \
            AF_ERROR( \
                "Runtime Error\n" \
                "  Expression : {}\n" \
                "  Message    : {}\n"  \
                "  Location   : {}::{}", \
                #X, \
                fmt::format(__VA_ARGS__), \
                util::get_file_name_from_path(__FILE__), \
                __LINE__ \
            ); \
            DEBUG_BREAK(); \
        } \
    } while (0)
#endif


