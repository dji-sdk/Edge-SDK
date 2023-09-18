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

#ifndef __MEDIA_FILES_READER_H__
#define __MEDIA_FILES_READER_H__

#include <cstdint>
#include <list>
#include <memory>

#include "error_code.h"
#include "media_file.h"

namespace edge_sdk {

class MediaFilesReader {
   public:
    /* Exported types
     * ------------------------------------------------------------*/

    using MediaFilePtr = std::shared_ptr<MediaFile>;
    using MediaFileList = std::list<MediaFilePtr>;

    /* Exported functions
     * --------------------------------------------------------*/
    MediaFilesReader() = default;
    virtual ~MediaFilesReader();

    MediaFilesReader(const MediaFilesReader &) = delete;
    MediaFilesReader &operator=(const MediaFilesReader &) = delete;

    /**
     * @brief Initialize media file reading
     * @return Execution result
     */
    virtual ErrorCode Init() = 0;

    /**
     * @brief Media file list
     * @param filelist: Media file list
     * @return Number of media files
     */
    virtual int32_t FileList(MediaFileList &filelist) = 0;

    /**
     * @brief Open media file
     * @param file_path: Media file path
     * @return Handle for the opened media file
     */
    using FdType = int32_t;
    virtual FdType Open(const std::string &file_path) = 0;

    /**
     * @brief Read media file
     * @param fd: Media file handle
     * @param buf: Buffer for storing media file data
     * @param count: Size of the buffer
     * @return Size of the data read
     */
    virtual size_t Read(FdType fd, void *buf, size_t count) = 0;

    /**
     * @brief Close media file
     * @param fd: Media file handle
     * @return Execution result
     */
    virtual ErrorCode Close(FdType fd) = 0;

    /**
     * @brief Deinitialize media file reading
     * @return Execution result
     */
    virtual ErrorCode DeInit() = 0;
};

}  // namespace edge_sdk

#endif
