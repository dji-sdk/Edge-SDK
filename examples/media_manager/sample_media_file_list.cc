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
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "logger.h"
#include "media_manager.h"

#define DEBUG_DUMP_FILE

using namespace edge_sdk;

ErrorCode ESDKInit();

namespace {

char buffer[1024 * 1024 * 2];

}  // namespace

static int32_t PullMediaFiles(std::shared_ptr<MediaFilesReader> reader,
                              const MediaFilesReader::MediaFileList& list) {

    INFO("File total size: %d\n", list.size());

    uint32_t read_file_bytes_total = 0;
    int32_t read_files_counts = 0;
    for (const auto& entry : list) {

        // open file
        auto filepath = entry->file_path;
        auto fd = reader->Open(filepath);
        if (fd < 0) {
            INFO("Open failed, file path: %s\n", filepath.c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        auto filename = entry->file_name;
#ifdef DEBUG_DUMP_FILE
        FILE* file = NULL;
        file = fopen(filename.c_str(), "wb");
#endif

        read_file_bytes_total = 0;
        auto filesize = entry->file_size;
        auto start = std::chrono::high_resolution_clock::now();

        // read file
        do {
            auto nread = reader->Read(fd, buffer, sizeof(buffer));
            if (nread <= 0) {
                reader->Close(fd);
                break;
            }
            read_file_bytes_total += nread;

#ifdef DEBUG_DUMP_FILE
            if (file) fwrite(buffer, nread, 1, file);
#endif
            auto print_progress = [&]() {
                auto now = std::chrono::high_resolution_clock::now();
                auto time_taken =
                    std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                          start)
                        .count();
                static auto printed_time = 0;
                if (time(NULL) >= printed_time + 1) {
                    printed_time = time(NULL);
                    INFO("read filename: %s, percent: %f%, rate: %fM/s\n",
                         filename.c_str(),
                         (float)read_file_bytes_total * 100. / filesize,
                         (((float)(read_file_bytes_total)*1000) /
                          (1024 * 1024)) /
                             time_taken);
                    fflush(stdout);
                }
            };
            print_progress();
        } while (1);
#ifdef DEBUG_DUMP_FILE
        if (file) {
            fclose(file);
            file = nullptr;
        }
#endif

        auto print_progress_sum = [&]() {
            read_files_counts++;
            auto end = std::chrono::high_resolution_clock::now();
            auto time_taken =
                std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                      start)
                    .count();
            INFO(
                "progress(%d/%d): %s, filesize: %f MB, spend time: %ld ms, "
                "rate: "
                "%fM/s\n",
                read_files_counts, list.size(), filename.c_str(),
                ((float)read_file_bytes_total) / 1024 / 1024, time_taken,
                (((float)(read_file_bytes_total)*1000) / (1024 * 1024)) /
                    time_taken);
        };
        print_progress_sum();
    }
    return 0;
}

int main(int argc, char** argv) {
    auto rc = ESDKInit();
    if (rc != kOk) {
        ERROR("pre init failed");
        return -1;
    }

    auto reader = MediaManager::Instance()->CreateMediaFilesReader();
    reader->Init();
    MediaFilesReader::MediaFileList list;
    auto size_total = reader->FileList(list);
    if (size_total <= 0) {
        INFO("no media files");
        goto EXIT;
    }

    for (const auto& entry : list) {
        INFO("filename: %s, filesize: %ld\n", entry->file_name.c_str(),
             entry->file_size);
    }

    PullMediaFiles(reader, list);

EXIT:
    reader->DeInit();

    while (1) std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}
