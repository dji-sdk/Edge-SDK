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

#ifndef __LIVEVIEW_H__
#define __LIVEVIEW_H__

#include <functional>
#include <memory>

#include "error_code.h"

namespace edge_sdk {

/*
 * Liveview provides subscribing aircraft stream interface, and the ability of
 * switching camera source
 */
class Liveview {
   public:
    /* Exported types
     * ------------------------------------------------------------*/

    /**
     * @brief Stream source of payload camera
     */
    enum CameraSource {
        /*! Wide-angle lens */
        kCameraSourceWide = 1,

        /*! Zoom lens */
        kCameraSourceZoom = 2,

        /*! Infrared lens */
        kCameraSourceIR = 3,
    };

    /**
     * @brief Type of Stream Camera
     */
    enum CameraType {
        /*! FPV Camera */
        kCameraTypeFpv = 0,

        /*! Payload Camera */
        kCameraTypePayload = 1,
    };

    /**
     * @brief Quality of the Stream
     */
    enum StreamQuality {
        /*! Stream Quality 540p: 30fps, 960*540, bps 512*1024 */
        kStreamQuality540p = 1,

        /*! Stream Quality 720p: 30fps, 1280*720, bps 1024*1024 */
        kStreamQuality720p = 2,

        /*! Stream Quality 720pHigh: 30fps, 1280*720, bps 1024*1024 + 512*1024
         */
        kStreamQuality720pHigh = 3,

        /*! Stream Quality 1080p: 30fps, 1920*1080, bps 3*1024*1024 */
        kStreamQuality1080p = 4,

        /*! Stream Quality 1080p: 30fps, 1920*1080, bps 8*1024*1024 */
        kStreamQuality1080pHigh = 5,
    };

    /**
     * @brief liveview camera h264 stream callback
     * @param buf: Length of the received h264 stream data
     * @param len: Received h264 stream data
     */
    using H264Callback =
        std::function<ErrorCode(const uint8_t* buf, uint32_t len)>;

    /**
     * @brief Initialization Parameters for Live Stream
     */
    struct Options {
        /*! Type of camera for subscribing stream */
        CameraType camera;

        /*! Quality of subscribed stream. Note: The actual quality of the
         * subscribed stream may vary; the dock will return the closest matching
         * stream quality based on current conditions. */
        StreamQuality quality;

        /*! Processing function for received stream */
        H264Callback callback;
    };

    /*! Live stream status. Each bit represents the validity of different stream
     * qualities: bit0 represents the validity of the adaptive stream; bit1
     * represents the 540p stream's validity; bit2 represents the 720p stream's
     * validity; bit3 represents the 720pHigh stream's validity; bit4 represents
     * the 1080p stream's validity. bit5 represents the 1080pHigh stream's validity */
    using LiveviewStatus = uint32_t;
    /**
     * @brief Prototype for stream status processing
     * @param status: Stream's status
     */
    using LiveviewStatusCallback =
        std::function<void(const LiveviewStatus& status)>;

    /* Exported functions
     * --------------------------------------------------------*/

    Liveview();
    virtual ~Liveview();

    Liveview(const Liveview&) = delete;
    Liveview& operator=(const Liveview&) = delete;

    /**
     * @brief Initialize live stream subscription. Note: For a specific camera,
     * you can initialize only once
     * @param option: Stream initialization configuration
     * @return Execution result
     */
    virtual ErrorCode Init(const Options& option) = 0;

    /**
     * @brief Deinitialize stream subscription
     * @return Execution result
     */
    virtual ErrorCode DeInit() = 0;

    /**
     * @brief Set camera stream source
     * @return Execution result
     */
    virtual ErrorCode SetCameraSource(CameraSource source) = 0;

    /**
     * @brief Subscribe to live stream status
     * @param callback: Stream processing callback
     * @return Execution result
     */
    virtual ErrorCode SubscribeLiveviewStatus(
        const LiveviewStatusCallback& callback) = 0;

    /**
     * @brief Start live H264 stream
     * @return Execution result
     */
    virtual ErrorCode StartH264Stream() = 0;

    /**
     * @brief Stop live H264 stream
     * @return Execution result
     */
    virtual ErrorCode StopH264Stream() = 0;
};

/**
 * @brief Get liveview module instance
 */
std::shared_ptr<Liveview> CreateLiveview();

}  // namespace edge_sdk

#endif
