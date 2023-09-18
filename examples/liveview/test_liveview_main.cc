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
#include "sample_liveview.h"

using namespace edge_sdk;

ErrorCode ESDKInit();

int main(int argc, char **argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }
    while (argc < 3) {
        ERROR(
            "Usage: %s [CAMERA_TYPE] [QUALITY] [SOURCE] \nDESCRIPTION:\n "
            "CAMERA_TYPE: "
            "1-FPV. 2-Payload \n QUALITY: 1-540p. 2-720p. 3-720pHigh. 4-1080p "
            "\n SOURCE: 1-wide 2-zoom 3-IR \n eg: \n %s 1 3 1",
            argv[0], argv[0]);
        sleep(3);
    }

    auto type = atoi(argv[1]);
    auto quality = atoi(argv[2]);

    auto liveview = edge_app::LiveviewSample::CreateLiveview(
        std::to_string(type), (edge_sdk::Liveview::CameraType)type,
        (edge_sdk::Liveview::StreamQuality)quality, nullptr);

    if (argc == 4) {
        auto src = atoi(argv[3]);
        INFO("set camera soure: %d", src);
        liveview->SetCameraSource((edge_sdk::Liveview::CameraSource)src);
    }

    liveview->Start();

    while (1) sleep(3);

    return 0;
}
