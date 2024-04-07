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
#include "image_processor.h"

#include "image_processor_display.h"
#include "image_processor_yolovfastest.h"
#include "logger.h"

namespace edge_app {

class UndefinedImageProcessor : public ImageProcessor {
   public:
    UndefinedImageProcessor(const std::string& name) : name_(name) {}

    void Process(const std::shared_ptr<Image> image) override {
        ERROR("undefine image processor: %s", name_.c_str());
    }

   private:
    std::string name_;
};

ImageProcessor::ImageProcessor() {}

ImageProcessor::~ImageProcessor() {}

std::shared_ptr<ImageProcessor> CreateImageProcessor(
    const ImageProcessor::Options& option) {
    if (option.name == std::string("display")) {
        return std::make_shared<ImageDisplayProcessor>(option.alias, option.userdata);
    }
    if (option.name == std::string("yolovfastest")) {
        return std::make_shared<ImageProcessorYolovFastest>(option.alias);
    }
    return std::make_shared<UndefinedImageProcessor>(option.alias);
}

}  // namespace edge_app
