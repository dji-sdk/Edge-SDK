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

#include "liveview/liveview.h"

using namespace edge_sdk;

namespace edge_app {

class ImagePlayer : public ImageProcessor {
   public:
    ImagePlayer(const std::string& name) : name_(name) {
        cv::namedWindow(name.c_str(), cv::WINDOW_NORMAL);
    }

    ~ImagePlayer() override {}

    void Process(const std::shared_ptr<Image> image) override {
        imshow(name_.c_str(), *image);
        cv::waitKey(1);
    }

   private:
    std::string name_;
};

LiveviewSample::LiveviewSample(const std::string& name) {
    liveview_ = edge_sdk::CreateLiveview();
}

ErrorCode LiveviewSample::StreamCallback(const uint8_t* data, size_t len) {
    if (stream_processor_thread_) {
        stream_processor_thread_->InputStream(data, len);
    }
    return kOk;
}

ErrorCode LiveviewSample::Init(Liveview::CameraType type,
                               Liveview::StreamQuality quality) {
    auto callback_main =
        std::bind(&LiveviewSample::StreamCallback, this, std::placeholders::_1,
                  std::placeholders::_2);
    Liveview::Options option = {type, quality, callback_main};

    auto rc = liveview_->Init(option);

    liveview_->SubscribeLiveviewStatus(std::bind(
        &LiveviewSample::LiveviewStatusCallback, this, std::placeholders::_1));

    return rc;
}

ErrorCode LiveviewSample::Start() {
    stream_processor_thread_->Start();
    auto rc = liveview_->StartH264Stream();
    return rc;
}

void LiveviewSample::LiveviewStatusCallback(
    const Liveview::LiveviewStatus& status) {
    auto now = time(NULL);
    if (now >
        kLiveviewStatusTimeOutThreshold + received_liveview_status_time_) {
        try_restart_liveview_ = true;
    }

    if (status != liveview_status_) {
        if (liveview_status_ == 0) {
            try_restart_liveview_ = true;
        }
        liveview_status_ = status;
    }

    if (status != 0 && try_restart_liveview_) {
        INFO("Restart h264 stream...");
        auto rc = liveview_->StartH264Stream();
        if (rc == kOk) {
            try_restart_liveview_ = false;
        } else {
            ERROR("restart failed: %d", rc);
            try_restart_liveview_ = true;
        }
    }
    received_liveview_status_time_ = now;
}

std::shared_ptr<LiveviewSample> LiveviewSample::CreateLiveview(
    const std::string& name, edge_sdk::Liveview::CameraType type,
    edge_sdk::Liveview::StreamQuality quality,
    std::shared_ptr<ImageProcessor> image_processor) {
    auto image_processor_thread = std::make_shared<ImageProcessorThread>(name);
    if (!image_processor) image_processor = std::make_shared<ImagePlayer>(name);
    image_processor_thread->SetImageProcessor(image_processor);

    auto stream_processor_thread =
        std::make_shared<StreamProcessorThread>(name);

    StreamDecoder::Options option = {.name = "ffmpeg"};
    auto decoder = CreateStreamDecoder(option);
    decoder->Init();

    stream_processor_thread->SetStreamDecoder(decoder);
    stream_processor_thread->SetImageProcessorThread(image_processor_thread);

    auto liveview = std::make_shared<LiveviewSample>(name);
    liveview->SetProcessor(stream_processor_thread);
    liveview->Init(type, quality);
    return liveview;
}

void LiveviewSample::SetProcessor(
    std::shared_ptr<StreamProcessorThread> processor) {
    stream_processor_thread_ = processor;
}

ErrorCode LiveviewSample::SetCameraSource(
    edge_sdk::Liveview::CameraSource source) {
    return liveview_->SetCameraSource(source);
}

}  // namespace edge_app

