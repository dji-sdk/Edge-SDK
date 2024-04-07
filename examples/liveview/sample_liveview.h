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
#ifndef __SAMPLE_LIVEVIEW_H__
#define __SAMPLE_LIVEVIEW_H__

#include <chrono>
#include <atomic>
#include "error_code.h"
#include "image_processor.h"
#include "image_processor_thread.h"
#include "liveview.h"
#include "logger.h"
#include "stream_decoder.h"
#include "stream_processor_thread.h"

namespace edge_app {

class LiveviewSample {
   public:
    explicit LiveviewSample(const std::string& name);
    ~LiveviewSample() {}

    edge_sdk::ErrorCode Init(edge_sdk::Liveview::CameraType type,
                             edge_sdk::Liveview::StreamQuality quality,
                             std::shared_ptr<StreamProcessorThread> processor);

    edge_sdk::ErrorCode Start();

    edge_sdk::ErrorCode SetCameraSource(
        edge_sdk::Liveview::CameraSource source);

    uint32_t GetStreamBitrate() const {
        return stream_bitrate_kbps_.load();
    }

   private:
    edge_sdk::ErrorCode StreamCallback(const uint8_t* data, size_t len);

    void LiveviewStatusCallback(
        const edge_sdk::Liveview::LiveviewStatus& status);

    std::shared_ptr<edge_sdk::Liveview> liveview_;
    std::shared_ptr<StreamProcessorThread> stream_processor_thread_;
    edge_sdk::Liveview::LiveviewStatus liveview_status_;
    std::atomic<uint32_t> stream_bitrate_kbps_;
    std::chrono::system_clock::time_point receive_stream_data_time_ = std::chrono::system_clock::now();
    uint32_t receive_stream_data_total_size_;
};

int32_t InitLiveviewSample(std::shared_ptr<LiveviewSample>& liveview_sample, edge_sdk::Liveview::CameraType type,
                           edge_sdk::Liveview::StreamQuality quality,
                           std::shared_ptr<StreamDecoder> stream_decoder,
                           std::shared_ptr<ImageProcessor> image_processor);

}  // namespace edge_app

#endif
