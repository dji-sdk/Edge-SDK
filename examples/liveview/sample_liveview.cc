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

#include "liveview/liveview.h"

#define BITRATE_CALCULATE_BITS_PER_BYTE 8
#define BITRATE_CALCULATE_INTERVAL_TIME_MS 2000

using namespace edge_sdk;

namespace edge_app {

LiveviewSample::LiveviewSample(const std::string& name) {
    liveview_ = edge_sdk::CreateLiveview();
    liveview_status_ = 0;
}

ErrorCode LiveviewSample::StreamCallback(const uint8_t* data, size_t len) {
    auto now =  std::chrono::system_clock::now();
    if (stream_processor_thread_) {
        stream_processor_thread_->InputStream(data, len);
    }

    // for bitrate calculate
    receive_stream_data_total_size_ += len;
    auto duration  = std::chrono::duration_cast<std::chrono::milliseconds>(now - receive_stream_data_time_).count();
    if (duration >= BITRATE_CALCULATE_INTERVAL_TIME_MS) {
        auto kbps = receive_stream_data_total_size_ * BITRATE_CALCULATE_BITS_PER_BYTE / (BITRATE_CALCULATE_INTERVAL_TIME_MS / 1000) / 1024;
        stream_bitrate_kbps_ = kbps;
        receive_stream_data_total_size_  = 0;
        receive_stream_data_time_  = now;
    }
    return kOk;
}

ErrorCode LiveviewSample::Init(
    Liveview::CameraType type, Liveview::StreamQuality quality,
    std::shared_ptr<StreamProcessorThread> processor) {
    stream_processor_thread_ = processor;

    auto stream_callback =
        std::bind(&LiveviewSample::StreamCallback, this, std::placeholders::_1,
                  std::placeholders::_2);
    Liveview::Options option = {type, quality, stream_callback};

    auto rc = liveview_->Init(option);

    liveview_->SubscribeLiveviewStatus(std::bind(
        &LiveviewSample::LiveviewStatusCallback, this, std::placeholders::_1));

    return rc;
}

ErrorCode LiveviewSample::Start() {
    if (stream_processor_thread_->Start() != 0) {
        ERROR("stream processor start failed");
        return kErrorInvalidOperation;
    }
    std::thread([&] {
        // Waiting for the liveview to be available before starting,
        // otherwise the StartH264Stream() will fail.
        while (liveview_status_ == 0) sleep(1);
        auto rc = liveview_->StartH264Stream();
        if (rc != kOk) {
            ERROR("Failed to start liveview: %d", rc);
        }
    }).detach();
    return kOk;
}

void LiveviewSample::LiveviewStatusCallback(
    const Liveview::LiveviewStatus& status) {
    liveview_status_ = status;
    DEBUG("status: %d", status);
}

int32_t InitLiveviewSample(std::shared_ptr<LiveviewSample>& liveview_sample, edge_sdk::Liveview::CameraType type,
    edge_sdk::Liveview::StreamQuality quality,
    std::shared_ptr<StreamDecoder> stream_decoder,
    std::shared_ptr<ImageProcessor> image_processor)
{
    auto image_processor_thread = std::make_shared<ImageProcessorThread>(stream_decoder->Name());
    image_processor_thread->SetImageProcessor(image_processor);

    auto stream_processor_thread =
        std::make_shared<StreamProcessorThread>(stream_decoder->Name());
    stream_processor_thread->SetStreamDecoder(stream_decoder);
    stream_processor_thread->SetImageProcessorThread(image_processor_thread);

    auto rc = liveview_sample->Init(type, quality, stream_processor_thread);
    if (rc != kOk) {
        ERROR("liveview sample init failed");
        return -1;
    }

    return 0;
}

ErrorCode LiveviewSample::SetCameraSource(
    edge_sdk::Liveview::CameraSource source) {
    return liveview_->SetCameraSource(source);
}

}  // namespace edge_app

