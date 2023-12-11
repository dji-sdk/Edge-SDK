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

#include "stream_decoder.h"

#include "ffmpeg_stream_decoder.h"
#include "logger.h"

namespace edge_app {

class UndefinedStreamDecoder : public StreamDecoder {
   public:
    UndefinedStreamDecoder(const std::string& name)
        : StreamDecoder(name), name_(name) {}

    int32_t Init() override {
        ERROR("undefine stream decoder: %s", name_.c_str());
        return -1;
    }

    int32_t DeInit() override {
        ERROR("undefine stream decoder: %s", name_.c_str());
        return -1;
    }
    int32_t Decode(const uint8_t* data, size_t length,
                   DecodeResultCallback result_callback) override {
        ERROR("undefine stream decoder: %s", name_.c_str());
        return -1;
    }

   private:
    std::string name_;
};

std::shared_ptr<StreamDecoder> CreateStreamDecoder(
    const StreamDecoder::Options& option) {
    if (option.name == std::string("ffmpeg")) {
        return std::make_shared<FFmpegStreamDecoder>(option.name);
    }

    return std::make_shared<UndefinedStreamDecoder>(option.name);
}

}  // namespace edge_app
