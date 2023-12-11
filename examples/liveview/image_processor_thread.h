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
#ifndef __IMAGE_PROCESSOR_THREAD_H__
#define __IMAGE_PROCESSOR_THREAD_H__

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "error_code.h"

namespace cv {
class Mat;
}

namespace edge_app {

class ImageProcessor;

class ImageProcessorThread {
   public:
    explicit ImageProcessorThread(const std::string& name);

    virtual ~ImageProcessorThread();

    using Image = cv::Mat;

    const std::string Name() const { return processor_name_; }

    void InputImage(const std::shared_ptr<Image> image);

    int32_t SetImageProcessor(std::shared_ptr<ImageProcessor> image_processor);

    int32_t Start();

    int32_t Stop();

   protected:
    enum {
        kImageQueueSizeLimit = 10,
    };

    void ImageProcess();

    virtual void DoProcess(const std::shared_ptr<Image> image);

    std::string processor_name_;

    std::mutex image_queue_mutex_;
    std::condition_variable image_queue_cv_;
    std::queue<std::shared_ptr<Image>> image_queue_;

    std::thread image_processor_thread_;
    std::atomic<bool> processor_start_;
    std::shared_ptr<ImageProcessor> image_processor_;
};

}  // namespace edge_app

#endif
