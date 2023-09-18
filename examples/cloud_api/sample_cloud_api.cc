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
#include <unistd.h>

#include <cstring>

#include "cloud_api.h"
#include "logger.h"

using namespace edge_sdk;

ErrorCode ESDKInit();

void CloudAPICustomSerivesMessageHandle(const uint8_t* data, uint32_t len) {
    INFO("cloud services message incoming: %s, len = %d", (char*)data, len);
}

int main(int argc, char** argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    rc = CloudAPI_RegisterCustomServicesMessageHandler(
        CloudAPICustomSerivesMessageHandle);
    if (rc != kOk) {
        ERROR("register custom services message handler failed");
        return -2;
    }

    uint8_t message[] = "message from edge sdk";
    while (1) {
        INFO("send message to cloud: %s", message);
        CloudAPI_SendCustomEventsMessage(message, sizeof(message));
        sleep(1);
    }

    return 0;
}
