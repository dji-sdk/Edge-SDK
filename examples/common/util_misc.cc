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
#include "util_misc.h"
#include <cstring>

namespace edge_app {

int32_t GetCurrentFileDirPath(const char* file_path,
                                uint32_t path_buffer_size, char* dir_path) {
    uint32_t i = strlen(file_path) - 1;
    uint32_t dir_path_len;

    while (file_path[i] != '/') {
        i--;
    }

    dir_path_len = i + 1;

    if (dir_path_len + 1 > path_buffer_size) {
        return -1;
    }

    memcpy(dir_path, file_path, dir_path_len);
    dir_path[dir_path_len] = 0;

    return 0;
}

}  // namespace edge_app
