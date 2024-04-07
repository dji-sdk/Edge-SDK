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
#ifndef __IMAGE_PROCESSOR_DIAPLAY_H__
#define __IMAGE_PROCESSOR_DIAPLAY_H__

#include <memory>
#include "opencv2/opencv.hpp"
#include "liveview/sample_liveview.h"

namespace edge_app {

class ImageDisplayProcessor : public ImageProcessor {
   public:
    ImageDisplayProcessor(const std::string& name, std::shared_ptr<void> userdata) : name_(name) {
        cv::namedWindow(name.c_str(), cv::WINDOW_NORMAL);
        cv::resizeWindow(name.c_str(), 960, 540);
        cv::moveWindow(name.c_str(), rand() & 0xFF, rand() & 0xFF);
        if (userdata) {
            liveview_sample_ = std::static_pointer_cast<LiveviewSample>(userdata);
        }
    }

    ~ImageDisplayProcessor() override {}

    void Process(const std::shared_ptr<Image> image) override {
        std::string h = std::to_string(image->size().width);
        std::string w = std::to_string(image->size().height);
        std::string osd = h + "x" + w;
        if (liveview_sample_) {
            auto kbps = liveview_sample_->GetStreamBitrate();
            osd += std::string(",") + std::to_string(kbps) + std::string("kbps");
        }
        putText(*image, osd, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
                cv::Scalar(0, 0, 255), 3);
        imshow(name_.c_str(), *image);
        cv::waitKey(1);
    }

   private:
    std::string name_;
    std::shared_ptr<LiveviewSample> liveview_sample_;
};

}  // namespace edge_app

#endif

