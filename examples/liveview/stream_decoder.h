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
#ifndef __STREAM_DECODER_H__
#define __STREAM_DECODER_H__
#include <memory>
#include <string>
#include <functional>

#include "error_code.h"

namespace cv {
class Mat;
}

namespace edge_app {

class StreamDecoder {
   public:
    struct Options {
        std::string name;
    };

    explicit StreamDecoder(const std::string& name) : decoder_name_(name) {}

    virtual ~StreamDecoder() {}

    std::string Name() { return decoder_name_; }

    virtual int32_t Init() = 0;

    virtual int32_t DeInit() = 0;

    using Image = cv::Mat;
    using DecodeResultCallback =
        std::function<void(std::shared_ptr<Image>& result)>;
    virtual int32_t Decode(const uint8_t* data, size_t length,
                           DecodeResultCallback result_callback) = 0;

   private:
    std::string decoder_name_;
};

std::shared_ptr<StreamDecoder> CreateStreamDecoder(
    const StreamDecoder::Options& option);

}  // namespace edge_app

#endif

