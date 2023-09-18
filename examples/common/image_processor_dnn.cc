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
#include "image_processor.h"

#ifdef OPEN_CV_INSTALLED

#include <sys/time.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "logger.h"
#include "media_manager.h"
#include "opencv2/dnn.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
#endif
using namespace std;
using namespace edge_sdk;
#include "error_code.h"

namespace edge_app {

static const char* classNames[] = {
    "background",   "person",         "bicycle",
    "car",          "motorcycle",     "airplane",
    "bus",          "train",          "truck",
    "boat",         "traffic light",  "fire hydrant",
    "background",   "stop sign",      "parking meter",
    "bench",        "bird",           "cat",
    "dog",          "horse",          "sheep",
    "cow",          "elephant",       "bear",
    "zebra",        "giraffe",        "background",
    "backpack",     "umbrella",       "background",
    "background",   "handbag",        "tie",
    "suitcase",     "frisbee",        "skis",
    "snowboard",    "sports ball",    "kite",
    "baseball bat", "baseball glove", "skateboard",
    "surfboard",    "tennis racket",  "bottle",
    "background",   "wine glass",     "cup",
    "fork",         "knife",          "spoon",
    "bowl",         "banana",         "apple",
    "sandwich",     "orange",         "broccoli",
    "carrot",       "hot dog",        "pizza",
    "donut",        "cake",           "chair",
    "couch",        "potted plant",   "bed",
    "background",   "dining table",   "background",
    "background",   "toilet",         "background",
    "tv",           "laptop",         "mouse",
    "remote",       "keyboard",       "cell phone",
    "microwave",    "oven",           "toaster",
    "sink",         "refrigerator",   "background",
    "book",         "clock",          "vase",
    "scissors",     "teddy bear",     "hair drier",
    "toothbrush"};

static ErrorCode GetCurrentFileDirPath(const char* filePath,
                                       uint32_t pathBufferSize, char* dirPath) {
    uint32_t i = strlen(filePath) - 1;
    uint32_t dirPathLen;

    while (filePath[i] != '/') {
        i--;
    }

    dirPathLen = i + 1;

    if (dirPathLen + 1 > pathBufferSize) {
        return kErrorParamOutOfRange;
    }

    memcpy(dirPath, filePath, dirPathLen);
    dirPath[dirPathLen] = 0;

    return kOk;
}

class TensorflowInception : public ImageProcessor {
   public:
    TensorflowInception(const std::string& option) : show_name_(option) {}
    virtual ~TensorflowInception() {}

    void Process(const std::shared_ptr<Image> image) override {
#ifdef OPEN_CV_INSTALLED
        Mat mat = *image;

        if (GetCurrentFileDirPath(__FILE__, sizeof(curFileDirPath),
                                   curFileDirPath)
                 != kOk) {
            ERROR("get path failed");
            return;
        }
        snprintf(prototxtFileDirPath, kFilePathSizeMax,
                 "%s/data/tensorflow/ssd_inception_v2_coco_2017_11_17.pbtxt",
                 curFileDirPath);
        // Attention: If you want to run the Tensorflow Object detection demo,
        // Please download the tensorflow model. Download Url:
        // http://download.tensorflow.org/models/object_detection/ssd_inception_v2_coco_2017_11_17.tar.gz
        snprintf(weightsFileDirPath, kFilePathSizeMax,
                 "%s/data/tensorflow/frozen_inference_graph.pb",
                 curFileDirPath);

        dnn::Net net = cv::dnn::readNetFromTensorflow(weightsFileDirPath,
                                                      prototxtFileDirPath);
        Size frame_size = mat.size();

        Size cropSize;
        if (frame_size.width / (float)frame_size.height > WHRatio) {
            cropSize = Size(static_cast<int>(frame_size.height * WHRatio),
                            frame_size.height);
        } else {
            cropSize = Size(frame_size.width,
                            static_cast<int>(frame_size.width / WHRatio));
        }

        Rect crop(Point((frame_size.width - cropSize.width) / 2,
                        (frame_size.height - cropSize.height) / 2),
                  cropSize);

        cv::Mat blob = cv::dnn::blobFromImage(mat, 1, Size(300, 320));
        net.setInput(blob);
        Mat output = net.forward();
        Mat detectionMat(output.size[2], output.size[3], CV_32F,
                         output.ptr<float>());

        mat = mat(crop);
        float confidenceThreshold = 0.50;

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > confidenceThreshold) {
                auto objectClass = (size_t)(detectionMat.at<float>(i, 1));

                int xLeftBottom =
                    static_cast<int>(detectionMat.at<float>(i, 3) * mat.cols);
                int yLeftBottom =
                    static_cast<int>(detectionMat.at<float>(i, 4) * mat.rows);
                int xRightTop =
                    static_cast<int>(detectionMat.at<float>(i, 5) * mat.cols);
                int yRightTop =
                    static_cast<int>(detectionMat.at<float>(i, 6) * mat.rows);

                ostringstream ss;
                ss << confidence;
                String conf(ss.str());

                Rect object((int)xLeftBottom, (int)yLeftBottom,
                            (int)(xRightTop - xLeftBottom),
                            (int)(yRightTop - yLeftBottom));

                rectangle(mat, object, Scalar(0, 255, 0), 2);
                String label = String(classNames[objectClass]) + ": " + conf;

                int baseLine = 0;
                Size labelSize =
                    getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
#ifdef OPEN_CV_VERSION_3
                rectangle(
                    mat,
                    Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                         Size(labelSize.width, labelSize.height + baseLine)),
                    Scalar(0, 255, 0), CV_FILLED);
#endif
#ifdef OPEN_CV_VERSION_4
                rectangle(
                    mat,
                    Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                         Size(labelSize.width, labelSize.height + baseLine)),
                    Scalar(0, 255, 0), FILLED);
#endif
                putText(mat, label, Point(xLeftBottom, yLeftBottom),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
            }
        }

        {
            auto get_ms = []() -> time_t {
                struct timeval time_now {};
                gettimeofday(&time_now, nullptr);
                time_t ms_time =
                    (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
                return ms_time;
            };
            if (++frame_counter >= 5) {
                auto delta = (get_ms() - elapsed_time);
                frame_fps = static_cast<int>(5.0f * 1000 / (int)delta);
                elapsed_time = get_ms();
                frame_counter = 0;
            }
            std::string str_h = std::to_string(image->size().width);
            std::string str_w = std::to_string(image->size().height);
            std::string osd =
                str_h + "x" + str_w + " fps: " + std::to_string(frame_fps);
            putText(mat, osd, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5,
                    Scalar(0, 0, 0));
        }

        imshow(show_name_, mat);

        cv::waitKey(1);
#endif
    }

   private:
    enum {
        kWidth = 320,
        kHeight = 300,
        kFilePathSizeMax = 256,
    };
    const size_t inWidth = 320;
    const size_t inHeight = 300;
    std::string show_name_;
    const float WHRatio = (float)kWidth / (float)kHeight;
    char curFileDirPath[128];
    char prototxtFileDirPath[kFilePathSizeMax];
    char weightsFileDirPath[kFilePathSizeMax];
    uint32_t frame_fps;
    uint32_t frame_counter;
    uint32_t elapsed_time;
};

std::shared_ptr<ImageProcessor> CreateImageProcessor(const ImageProcessor::Options& option) {
    auto handle = std::make_shared<TensorflowInception>(option);
    return handle;
}

}  // namespace image_processor
