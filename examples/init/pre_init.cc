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
#include <cstring>
#include <iostream>

#include "app_info.h"
#include "init.h"
#include "key_store_default.h"
#include "logger.h"

using namespace edge_sdk;

static ErrorCode PrintConsoleFunc(const uint8_t* data, uint16_t dataLen) {
    printf("%s", data);
    return kOk;
}

static ErrorCode InitOptions(Options& option) {
    option.product_name = "Edge-1.0";
    option.vendor_name = "Vendor";
    option.serial_number = "SN0000100010101";
    option.firmware_version = {0, 1, 0, 0};

    AppInfo app_info;
    app_info.app_name =
        std::string((char*)USER_APP_NAME, strlen(USER_APP_NAME));
    app_info.app_id = std::string((char*)USER_APP_ID, strlen(USER_APP_ID));
    app_info.app_key = std::string((char*)USER_APP_KEY, strlen(USER_APP_KEY));
    app_info.app_license =
        std::string((char*)USER_APP_LICENSE, strlen(USER_APP_LICENSE));

    option.app_info = app_info;
    LoggerConsole console = {kLevelDebug, PrintConsoleFunc, true};
    option.logger_console_lists.push_back(console);

    option.key_store = std::make_shared<KeyStoreDefault>();

    return kOk;
}

ErrorCode ESDKInit() {
    Options option;
    InitOptions(option);
    auto rc = ESDKInit::Instance()->Init(option);
    if (rc != kOk) {
        std::cout << "Edge sdk init failed: " << rc << std::endl;
    }
    return rc;
}

ErrorCode ESDKDeInit() { return ESDKInit::Instance()->DeInit(); }

