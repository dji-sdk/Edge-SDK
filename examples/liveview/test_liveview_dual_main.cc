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

    auto payload_quality = atoi(argv[1]);
    auto fpv_quality = atoi(argv[2]);

    while (argc < 3 || payload_quality > 5 || fpv_quality > 5) {
        ERROR(
            "Usage: %s [PAYLOAD_QUOLITY] [FPV_QUALITY] [SOURCE] \nDESCRIPTION:\n "
            "QUALITY: 1-540p. 2-720p. 3-720pHigh. "
            "4-1080p. 5-1080pHigh"
            "\n SOURCE: 1-wide 2-zoom 3-IR \n eg: \n %s 1 3 1",
            argv[0], argv[0]);
        sleep(1);
    }

    // create payload liveview sample
    auto payload_liveview_sample = std::make_shared<LiveviewSample>(std::string("PayloadCamera"));

    StreamDecoder::Options decoder_option = {.name = std::string("ffmpeg")};
    auto payload_stream_decoder = CreateStreamDecoder(decoder_option);

    ImageProcessor::Options payload_image_processor_option = {.name = std::string("display"),
                                       .alias = "PayloadCamera", .userdata = payload_liveview_sample};

    auto payload_image_processor = CreateImageProcessor(payload_image_processor_option);

    if (0 != InitLiveviewSample(
        payload_liveview_sample, Liveview::kCameraTypePayload, (Liveview::StreamQuality)payload_quality,
        payload_stream_decoder, payload_image_processor)) {
        ERROR("Init liveview sample failed");
    } else {
        payload_liveview_sample->Start();
    }

    // create fpv liveview sample
    auto fpv_liveview_sample = std::make_shared<LiveviewSample>(std::string("FPV"));

    auto fpv_stream_decoder = CreateStreamDecoder(decoder_option);

    ImageProcessor::Options fpv_image_processor_option = {.name = std::string("display"),
                                       .alias = "FPVCamera", .userdata = fpv_liveview_sample};

    auto fpv_image_processor = CreateImageProcessor(fpv_image_processor_option);

    if (0 != InitLiveviewSample(
        fpv_liveview_sample, Liveview::kCameraTypeFpv, (Liveview::StreamQuality)fpv_quality,
        fpv_stream_decoder, fpv_image_processor)) {
        ERROR("Init liveview sample failed");
    } else {
        fpv_liveview_sample->Start();
    }

    if (argc == 4) {
        auto src = atoi(argv[3]);
        INFO("set camera soure: %d", src);
        payload_liveview_sample->SetCameraSource((edge_sdk::Liveview::CameraSource)src);
    }

    while (1) sleep(3);

    return 0;
}
