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

#ifndef __MEDIA_FILES_H__
#define __MEDIA_FILES_H__

#include <cstdint>
#include <string>
#include <time.h>

namespace edge_sdk {

/* Exported types
 * ------------------------------------------------------------*/

/**
 * @brief Media file information
 */
struct MediaFile {

    /*! Type of camera for the media file */
    enum CameraAttr {
        /*! Infrared camera */
        kCameraAttrInfrared = 0,

        /*! Zoom camera */
        kCameraAttrZoom = 1,

        /*! Wide-angle camera */
        kCameraAttrWide = 2,

        /*! visible camera */
        kCameraAttrVisible = 5,
    };

    /*! Type of file */
    enum FileType {

        /*! File type in JPEG photo format */
        kFileTypeJpeg = 0,

        /*! File type in MP4 video format */
        kFileTypeMp4 = 3,
    };

    /*! File name */
    std::string file_name;

    /*! File path, used when reading the file */
    std::string file_path;

    /*! File size */
    size_t file_size;

    /*! Type of file */
    FileType file_type;

    /*! attribute of camera */
    CameraAttr camera_attr;

    /*! Longitude of the photo taken */
    double latitude;

    /*! Latitude of the photo taken */
    double longitude;

    /*! Absolute altitude of the photo taken */
    double absolute_altitude;

    /*! Relative altitude of the photo taken */
    double relative_altitude;

    /*! Gimbal angle when taking the photo */
    double gimbal_yaw_degree;

    /*! Width of the photo */
    int32_t image_width;

    /*! Height of the photo */
    int32_t image_height;

    /*! Duration of the video */
    uint32_t video_duration;

    /*! File creation time */
    time_t create_time;
};

}  // namespace edge_sdk

#endif
