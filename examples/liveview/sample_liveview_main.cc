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
#include "sample_liveview.h"
#include <unistd.h>

#include "image_processor.h"
#include "error_code.h"
#include "logger.h"

using namespace edge_sdk;

ErrorCode ESDKInit();

int main(int argc, char** argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    auto liveview = edge_app::LiveviewSample::CreateLiveview(
        "Payload", Liveview::kCameraTypePayload, Liveview::kStreamQuality1080p,
        edge_app::CreateImageProcessor("PayloadCNN"));
    liveview->Start();

    auto liveview_fpv = edge_app::LiveviewSample::CreateLiveview(
        "Fpv", Liveview::kCameraTypeFpv, Liveview::kStreamQuality1080p, nullptr);
    liveview_fpv->Start();

    while (1) sleep(3);

    return 0;
}
