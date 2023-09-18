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
#include "ffmpeg_stream_decoder.h"

#include <unistd.h>

#include "logger.h"

using namespace edge_sdk;

namespace edge_app {

FFmpegStreamDecoder::FFmpegStreamDecoder(const std::string &name)
    : StreamDecoder(name) {}

FFmpegStreamDecoder::~FFmpegStreamDecoder() {}

int32_t FFmpegStreamDecoder::Init() {
    avcodec_register_all();
    pCodecCtx = avcodec_alloc_context3(nullptr);
    if (!pCodecCtx) {
        return -1;
    }

    pCodecCtx->thread_count = 4;
    pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!pCodec || avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        return -2;
    }

    pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
    if (!pCodecParserCtx) {
        return -3;
    }

    pFrameYUV = av_frame_alloc();
    if (!pFrameYUV) {
        return -4;
    }

    pFrameRGB = av_frame_alloc();
    if (!pFrameRGB) {
        return -5;
    }

    pSwsCtx = nullptr;

    pCodecCtx->flags2 |= AV_CODEC_FLAG2_SHOW_ALL;
    return 0;
}

int32_t FFmpegStreamDecoder::DeInit() {
    if (nullptr != pSwsCtx) {
        sws_freeContext(pSwsCtx);
        pSwsCtx = nullptr;
    }

    if (nullptr != pFrameYUV) {
        av_free(pFrameYUV);
        pFrameYUV = nullptr;
    }

    if (nullptr != pCodecParserCtx) {
        av_parser_close(pCodecParserCtx);
        pCodecParserCtx = nullptr;
    }

    if (nullptr != pCodec) {
        avcodec_close(pCodecCtx);
        pCodec = nullptr;
    }

    if (nullptr != pCodecCtx) {
        av_free(pCodecCtx);
        pCodecCtx = nullptr;
    }

    if (nullptr != rgbBuf) {
        av_free(rgbBuf);
        rgbBuf = nullptr;
    }

    if (nullptr != pFrameRGB) {
        av_free(pFrameRGB);
        pFrameRGB = nullptr;
    }
    return 0;
}

int32_t FFmpegStreamDecoder::Decode(const uint8_t *data, size_t length,
                                    std::shared_ptr<Image> &result) {
    const uint8_t *pData = data;
    int remainingLen = length;
    int processedLen = 0;

    std::lock_guard<std::mutex> l(decode_mutex);
    AVPacket pkt;
    av_init_packet(&pkt);
    while (remainingLen > 0) {
        if (!pCodecParserCtx || !pCodecCtx) {
            break;
        }
        processedLen = av_parser_parse2(
            pCodecParserCtx, pCodecCtx, &pkt.data, &pkt.size, pData,
            remainingLen, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
        remainingLen -= processedLen;
        pData += processedLen;

        if (pkt.size > 0) {
            int gotPicture = 0;
            avcodec_decode_video2(pCodecCtx, pFrameYUV, &gotPicture, &pkt);

            if (!gotPicture) {
                continue;
            } else {
                if (pFrameYUV->width != decode_width ||
                    pFrameYUV->height != decode_hight) {
                    decode_width = pFrameYUV->width;
                    decode_hight = pFrameYUV->height;
                    if (nullptr != pSwsCtx) {
                        sws_freeContext(pSwsCtx);
                        pSwsCtx = nullptr;
                    }
                    if (nullptr != rgbBuf) {
                        av_free(rgbBuf);
                        rgbBuf = nullptr;
                    }
                    INFO("New H264 Width: %d, Height: %d", decode_width,
                         decode_hight);
                }
                int w = decode_width;
                int h = decode_hight;

                if (nullptr == pSwsCtx) {
                    pSwsCtx = sws_getContext(w, h, pCodecCtx->pix_fmt, w, h,
                                             AV_PIX_FMT_RGB24, 4, nullptr,
                                             nullptr, nullptr);
                }

                if (nullptr == rgbBuf) {
                    bufSize = avpicture_get_size(AV_PIX_FMT_RGB24, w, h);
                    rgbBuf = (uint8_t *)av_malloc(bufSize);
                    avpicture_fill((AVPicture *)pFrameRGB, rgbBuf,
                                   AV_PIX_FMT_RGB24, w, h);
                }

                if (nullptr != pSwsCtx && nullptr != rgbBuf) {
                    sws_scale(pSwsCtx, (uint8_t const *const *)pFrameYUV->data,
                              pFrameYUV->linesize, 0, pFrameYUV->height,
                              pFrameRGB->data, pFrameRGB->linesize);

                    pFrameRGB->height = h;
                    pFrameRGB->width = w;

                    if (pFrameRGB->data[0]) {
                        cv::Mat tmp(h, w, CV_8UC3, pFrameRGB->data[0], w * 3);
                        cv::Mat cvtmp;
                        cvtColor(tmp, cvtmp, cv::COLOR_RGB2BGR);
                        auto mat = std::make_shared<cv::Mat>(cvtmp);
                        result = mat;
                    }
                }
            }
        }
        av_free_packet(&pkt);
    }
    return 0;
}

}  // namespace edge_app
