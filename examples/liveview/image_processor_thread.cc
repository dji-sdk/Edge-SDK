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
#include "image_processor_thread.h"

#include <cerrno>
#include <cstring>

#include "image_processor.h"
#include "logger.h"

using namespace edge_sdk;

namespace edge_app {

class NullImageProcessor : public ImageProcessor {
   public:
    void Process(const std::shared_ptr<Image> image) {
        WARN("unset image processor...");
    }
};

ImageProcessorThread::ImageProcessorThread(const std::string& name)
    : processor_name_(name) {
    processor_start_ = false;
    image_processor_ = std::make_shared<NullImageProcessor>();
}

ImageProcessorThread::~ImageProcessorThread() { Stop(); }

int32_t ImageProcessorThread::SetImageProcessor(
    std::shared_ptr<ImageProcessor> image_processor) {
    if (image_processor == nullptr) {
        return -1;
    }
    image_processor_ = image_processor;
    return 0;
}

void ImageProcessorThread::InputImage(const std::shared_ptr<Image> image) {
    std::lock_guard<std::mutex> l(image_queue_mutex_);
    image_queue_.push(image);
    if (image_queue_.size() > kImageQueueSizeLimit) {
        image_queue_.pop();
    }
    image_queue_cv_.notify_one();
}

void ImageProcessorThread::DoProcess(const std::shared_ptr<Image> image) {
    if (image_processor_) image_processor_->Process(image);
}

int32_t ImageProcessorThread::Start() {
    if (processor_start_) {
        WARN("repeat start processor");
        return -1;
    }
    processor_start_ = true;
    if (image_processor_) {
        auto ret = image_processor_->Init();
        if (ret < 0) {
            ERROR("Failed to init image processor");
            processor_start_ = false;
            return -1;
        }
    }
    image_processor_thread_ =
        std::thread(&ImageProcessorThread::ImageProcess, this);

    {
        sched_param sch;
        int policy;
        pthread_getschedparam(image_processor_thread_.native_handle(), &policy,
                              &sch);
        sch.sched_priority = 40;
        if (pthread_setschedparam(image_processor_thread_.native_handle(),
                                  SCHED_FIFO, &sch)) {
            ERROR("Failed to setschedparam: %s", strerror(errno));
        }
    }

    return 0;
}

int32_t ImageProcessorThread::Stop() {
    processor_start_ = false;
    if (image_processor_thread_.joinable()) {
        image_processor_thread_.join();
    }

    return 0;
}

void ImageProcessorThread::ImageProcess() {
    INFO("start image processor: %s", processor_name_.c_str());
    pthread_setname_np(pthread_self(), "opencvimshow");
    while (processor_start_) {
        std::unique_lock<std::mutex> l(image_queue_mutex_);
        image_queue_cv_.wait(l, [&] { return image_queue_.size() != 0; });
        auto img = image_queue_.front();
        image_queue_.pop();
        l.unlock();
        DoProcess(img);
    }
    INFO("stop image processor: %s", processor_name_.c_str());
}

}  // namespace edge_app
