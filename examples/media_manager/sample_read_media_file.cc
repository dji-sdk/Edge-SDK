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
#include <unistd.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "logger.h"
#include "media_manager.h"

using namespace edge_sdk;

ErrorCode ESDKInit();

namespace {

std::queue<MediaFile> media_file_queue;
std::mutex media_file_queue_mutex;
std::condition_variable media_file_queue_cv;
std::shared_ptr<MediaFilesReader> media_files_reader;

}  // namespace

static ErrorCode ReadMediaFile(const MediaFile& file,
                               std::vector<uint8_t>& image) {
    char buf[1024 * 1024];
    auto fd = media_files_reader->Open(file.file_path);
    if (fd < 0) {
        return kErrorSystemError;
    }
    do {
        auto nread = media_files_reader->Read(fd, buf, sizeof(buf));
        if (nread > 0) {
            image.insert(image.end(), (uint8_t*)buf, (uint8_t*)(buf + nread));
        } else {
            media_files_reader->Close(fd);
            break;
        }
    } while (1);

    INFO("filesize: %d, image.size: %d", file.file_size, image.size());

    return kOk;
}

static void DumpMediaFile(const std::string& filename,
                          const std::vector<uint8_t>& data) {
    FILE* f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(data.data(), data.size(), 1, f);
        fclose(f);
    }
}

static ErrorCode MediaFilesUpdateCallback(const MediaFile& file) {
    INFO("File update: %s", file.file_name.c_str());
    std::lock_guard<std::mutex> l(media_file_queue_mutex);
    media_file_queue.push(file);
    media_file_queue_cv.notify_one();
    return kOk;
}

static void ProcessMediaFileThread() {
    std::vector<uint8_t> image;
    while (1) {
        std::unique_lock<std::mutex> l(media_file_queue_mutex);

        media_file_queue_cv.wait(l,
                                 [] { return media_file_queue.size() != 0; });

        MediaFile file = media_file_queue.front();
        media_file_queue.pop();
        l.unlock();

        INFO("process media file:%s", file.file_name.c_str());
        auto rc = ReadMediaFile(file, image);
        if (rc == kOk) {
            DumpMediaFile(file.file_name, image);
        } else {
            ERROR("dump media file failed: %s", file.file_name.c_str());
        }

        image.clear();
    }
}

ErrorCode StartReadMediaFileSample() {
    media_files_reader = MediaManager::Instance()->CreateMediaFilesReader();
    auto rc = media_files_reader->Init();
    if (rc != kOk) {
        ERROR("init media files reader error");
        return rc;
    }

    rc = MediaManager::Instance()->RegisterMediaFilesObserver(
        std::bind(MediaFilesUpdateCallback, std::placeholders::_1));
    if (rc != kOk) {
        ERROR("registe media files observer");
        return rc;
    }

    auto t = std::thread(&ProcessMediaFileThread);
    t.detach();

    return kOk;
}

int main(int argc, char** argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    rc = MediaManager::Instance()->SetDroneNestAutoDelete(false);
    if (rc == kOk) {
        if (StartReadMediaFileSample() != kOk) {
            ERROR("start media file sample failed");
        }
    } else {
        ERROR("set media file auto delete failed");
    }

    while (1) sleep(3);

    return 0;
}
