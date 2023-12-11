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
#ifndef __FFMPEG_STREAM_DECODER_H__
#define __FFMPEG_STREAM_DECODER_H__

#include <memory>
#include <mutex>

#include "error_code.h"
#include "stream_decoder.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace edge_app {

class FFmpegStreamDecoder : public StreamDecoder {
   public:
    explicit FFmpegStreamDecoder(const std::string &name);
    virtual ~FFmpegStreamDecoder();

    int32_t Init() override;

    int32_t DeInit() override;

    int32_t Decode(const uint8_t *data, size_t length,
                   DecodeResultCallback result_callback) override;

   private:
    std::mutex decode_mutex;
    AVCodecContext *pCodecCtx = nullptr;
    AVCodec *pCodec = nullptr;
    AVCodecParserContext *pCodecParserCtx = nullptr;
    SwsContext *pSwsCtx = nullptr;

    AVFrame *pFrameYUV = nullptr;
    AVFrame *pFrameRGB = nullptr;
    uint8_t *rgbBuf = nullptr;
    size_t bufSize = 0;
    int32_t decode_width;
    int32_t decode_hight;

    std::mutex image_mutex;
};

}  // namespace edge_app

#endif  // __FFMPEG_STREAM_DECODER_H__
