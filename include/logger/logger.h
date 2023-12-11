/**
 ********************************************************************
 *
 * @copyright (c) 2023 DJI. All rights reserved.
 *
 * All information contained herein is, and remains, the property of DJI.
 * The intellectual and technical concepts contained herein are proprietary
 * to DJI and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of DJI.
 *
 * If you receive this source code without DJI’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify DJI of its removal. DJI reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <cstdint>
#include <functional>

#include "error_code.h"

namespace edge_sdk {

/* Exported types
 * ------------------------------------------------------------*/

/**
 * @brief Log Level
 */
enum LogLevel {
    kLevelError = 0,
    kLevelWarn = 1,
    kLevelInfo = 2,
    kLevelDebug = 3,
};

/**
 * @brief Log Console Configuration
 */
struct LoggerConsole {
    /*! Log output function definition */
    using OutputFunc = std::function<ErrorCode(const uint8_t *, uint32_t)>;

    /*! Filters log level. Only logs with this level or lower will be output. */
    LogLevel level;

    /*! Log output process function */
    OutputFunc output_func;

    /*! Determines if the console supports color display */
    bool is_support_color;
};

/* Exported functions
 * --------------------------------------------------------*/

void EdgeSDKLoggerOutput(LogLevel level, const char *fmt, ...);

#define DEBUG(fmt, ...)                                                      \
    EdgeSDKLoggerOutput(edge_sdk::kLevelDebug, "[%s:%d) " fmt, __FUNCTION__, \
                        __LINE__, ##__VA_ARGS__)

#define INFO(fmt, ...)                                                      \
    EdgeSDKLoggerOutput(edge_sdk::kLevelInfo, "[%s:%d) " fmt, __FUNCTION__, \
                        __LINE__, ##__VA_ARGS__)

#define WARN(fmt, ...)                                                      \
    EdgeSDKLoggerOutput(edge_sdk::kLevelWarn, "[%s:%d) " fmt, __FUNCTION__, \
                        __LINE__, ##__VA_ARGS__)

#define ERROR(fmt, ...)                                                      \
    EdgeSDKLoggerOutput(edge_sdk::kLevelError, "[%s:%d) " fmt, __FUNCTION__, \
                        __LINE__, ##__VA_ARGS__)

}  // namespace edge_sdk

#endif  // __LOGGER_H__
