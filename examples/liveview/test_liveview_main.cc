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
using namespace edge_app;

ErrorCode ESDKInit();

int main(int argc, char **argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    int type = 0;
    while (argc < 3 || (type = atoi(argv[1])) > 2) {
        ERROR(
            "Usage: %s [CAMERA_TYPE] [QUALITY] [SOURCE] \nDESCRIPTION:\n "
            "CAMERA_TYPE: "
            "0-FPV. 1-Payload \n QUALITY: 1-540p. 2-720p. 3-720pHigh. "
            "4-1080p. 5-1080pHigh"
            "\n SOURCE: 1-wide 2-zoom 3-IR \n eg: \n %s 1 3 1",
            argv[0], argv[0]);
        sleep(1);
    }

    auto quality = atoi(argv[2]);

    const char type_to_str[2][16] = {"FPVCamera", "PayloadCamera"};

    // create liveview sample
    auto camera = std::string(type_to_str[type]);

    auto liveview_sample = std::make_shared<LiveviewSample>(std::string(camera));

    StreamDecoder::Options decoder_option = {.name = std::string("ffmpeg")};
    auto stream_decoder = CreateStreamDecoder(decoder_option);

    ImageProcessor::Options image_processor_option = {.name = std::string("display"),
                                       .alias = camera, .userdata = liveview_sample};
    auto image_processor = CreateImageProcessor(image_processor_option);

    if (0 != InitLiveviewSample(
        liveview_sample, (Liveview::CameraType)type, (Liveview::StreamQuality)quality,
        stream_decoder, image_processor)) {
        ERROR("Init %s liveview sample failed", camera.c_str());
    } else {
        liveview_sample->Start();
    }

    if (argc == 4) {
        auto src = atoi(argv[3]);
        INFO("set camera soure: %d", src);
        liveview_sample->SetCameraSource((edge_sdk::Liveview::CameraSource)src);
    }

    while (1) sleep(3);

    return 0;
}
