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

#include "logger.h"
#include "media_manager.h"

using namespace edge_sdk;

ErrorCode ESDKInit();

int main(int argc, char **argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    if (argc < 3) {
        ERROR("Usage: ./%s UPLOAD(0..1) AUTO-DELETE(0..1)\n", argv[0]);
        exit(0);
    }

    auto upload = atoi(argv[1]);
    auto del = atoi(argv[2]);
    INFO("Set cloud upload: %d, auto delete: %d\n", upload, del);

    rc = MediaManager::Instance()->SetDroneNestUploadCloud(upload ? true : false);
    if (rc != kOk) {
        WARN("set upload cloud strategy failed");
    }

    rc = MediaManager::Instance()->SetDroneNestAutoDelete(del ? true : false);
    if (rc != kOk) {
        WARN("set auto delete strategy failed");
    }

    while (1) sleep(3);

    return 0;
}
