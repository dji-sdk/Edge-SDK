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

#include "error_code.h"
#include "image_processor.h"
#include "logger.h"
#include "sample_liveview.h"

using namespace edge_sdk;
using namespace edge_app;

ErrorCode ESDKInit();

int main(int argc, char** argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    // create payload liveview
    auto payload_liveview = std::make_shared<LiveviewSample>("Payload");

    StreamDecoder::Options decoder_option = {.name = std::string("ffmpeg")};
    // create payload decoder
    auto payload_decoder = CreateStreamDecoder(decoder_option);

    // create payload image processor
    ImageProcessor::Options image_processor_option = {
        .name = std::string("yolovfastest"),
        .alias = std::string("PlayloadCamera: Yolovfastest")};
    auto payload_image_processor = CreateImageProcessor(image_processor_option);

    if (0 != InitLiveviewSample(
        payload_liveview, Liveview::kCameraTypePayload, Liveview::kStreamQuality1080pHigh,
        payload_decoder, payload_image_processor)) {
        ERROR("Init fpv liveview sample failed");
    } else {
        // start payload liveview
        payload_liveview->Start();
    }

    // create fpv liveview
    auto fpv_liveview = std::make_shared<LiveviewSample>("Fpv");

    // create fpv stream decoder
    auto fpv_decoder = CreateStreamDecoder(decoder_option);

    // create fpv image processor
    ImageProcessor::Options fpv_image_processor_option = {
        .name = std::string("display"), .alias = std::string("FPVCamera"), .userdata = fpv_liveview};
    auto fpv_image_processor = CreateImageProcessor(fpv_image_processor_option);

    if (0 != InitLiveviewSample(
        fpv_liveview, Liveview::kCameraTypeFpv, Liveview::kStreamQuality720p,
        fpv_decoder, fpv_image_processor)) {
        ERROR("Init fpv liveview sample failed");
    } else {
        // start fpv liveview
        fpv_liveview->Start();
    }

    while (1) sleep(3);

    return 0;
}
