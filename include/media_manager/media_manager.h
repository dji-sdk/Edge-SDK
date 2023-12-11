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
#ifndef __MEDIA_MANAGER_H__
#define __MEDIA_MANAGER_H__

#include <functional>
#include <memory>

#include "error_code.h"
namespace edge_sdk {

class MediaFile;
class MediaFilesReader;

class MediaManager final {
   public:
    /* Exported types
     * ------------------------------------------------------------*/

    /**
     * @brief Media file update notification callback
     */
    using MediaFilesObserver = std::function<ErrorCode(const MediaFile &)>;

    /**
     * @brief Media file management handle
     */
    static MediaManager *Instance();

    ~MediaManager();

    MediaManager(const MediaManager &) = delete;
    MediaManager &operator=(const MediaManager &) = delete;

    /**
     * @brief Get media file reading interface handle
     * @return Returns an instance of MediaFilesReader
     */
    std::shared_ptr<MediaFilesReader> CreateMediaFilesReader();

    /**
     * @brief Register media file notification processing callback
     * @param observer: Media file update notification callback
     */
    ErrorCode RegisterMediaFilesObserver(MediaFilesObserver observer);

    /**
     * @brief
     * Set media files for cloud upload. Note: Media files from flight waylines
     * are uploaded to the cloud by  default. If set to not upload, and edge
     * computing goes offline for over 30s, the default method will be restored,
     * restarting to upload media files to the cloud.
     * @param enable: Enable/disable cloud upload
     * @return Execution result
     */
    ErrorCode SetDroneNestUploadCloud(bool enable);

    /**
     * @brief
     * Set whether to delete local media files at the dock after uploading is
     * complete. Note: When edge computing requires media file retrieval, it
     * should be set not to delete.
     * @param enable: Delete after upload completion or not
     * @return Execution result
     */
    ErrorCode SetDroneNestAutoDelete(bool enable);

   private:
    MediaManager();
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace edge_sdk

#endif
