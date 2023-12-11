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
#include "image_processor_yolovfastest.h"

#include <sys/time.h>

#include <fstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>

#include "logger.h"
#include "util_misc.h"

using namespace cv;
using namespace dnn;
using namespace std;
using namespace edge_sdk;

namespace edge_app {

static const char* class_names[] = {
    "person",        "bicycle",       "car",           "motorbike",
    "aeroplane",     "bus",           "train",         "truck",
    "boat",          "traffic light", "fire hydrant",  "stop sign",
    "parking meter", "bench",         "bird",          "cat",
    "dog",           "horse",         "sheep",         "cow",
    "elephant",      "bear",          "zebra",         "giraffe",
    "backpack",      "umbrella",      "handbag",       "tie",
    "suitcase",      "frisbee",       "skis",          "snowboard",
    "sports ball",   "kite",          "baseball bat",  "baseball glove",
    "skateboard",    "surfboard",     "tennis racket", "bottle",
    "wine glass",    "cup",           "fork",          "knife",
    "spoon",         "bowl",          "banana",        "apple",
    "sandwich",      "orange",        "broccoli",      "carrot",
    "hot dog",       "pizza",         "donut",         "cake",
    "chair",         "sofa",          "pottedplant",   "bed",
    "diningtable",   "toilet",        "tvmonitor",     "laptop",
    "mouse",         "remote",        "keyboard",      "cell phone",
    "microwave",     "oven",          "toaster",       "sink",
    "refrigerator",  "book",          "clock",         "vase",
    "scissors",      "teddy bear",    "hair drier",    "toothbrush",
};

int32_t ImageProcessorYolovFastest::Init() {
    if (GetCurrentFileDirPath(__FILE__, sizeof(cur_file_dir_path_),
                              cur_file_dir_path_) != 0) {
        ERROR("get path failed");
        return -1;
    }
    snprintf(prototxt_file_dir_path_, kFilePathSizeMax,
             "%s/data/yolo-fastest-1.1_coco/yolo-fastest-1.1-xl.cfg",
             cur_file_dir_path_);
    snprintf(weights_file_dir_path_, kFilePathSizeMax,
             "%sdata/yolo-fastest-1.1_coco/yolo-fastest-1.1-xl.weights",
             cur_file_dir_path_);

    DEBUG("%s, %s", prototxt_file_dir_path_, weights_file_dir_path_);
    net_ = readNetFromDarknet(prototxt_file_dir_path_, weights_file_dir_path_);

    cv::namedWindow(show_name_.c_str(), cv::WINDOW_NORMAL);
    cv::resizeWindow(show_name_.c_str(), 960, 540);
    cv::moveWindow(show_name_.c_str(), rand()&0xFF, rand()&0xff);
    return 0;
}

void ImageProcessorYolovFastest::Process(const std::shared_ptr<Image> image) {
    auto detect = [&](cv::Mat& frame, vector<Mat>& outs) {
        Mat blob;
        blobFromImage(frame, blob, 1 / 255.0, Size(320, 320), Scalar(0, 0, 0),
                      true, false);
        net_.setInput(blob);

        net_.forward(outs, net_.getUnconnectedOutLayersNames());
    };

    auto draw_pred = [](int class_id, float conf, int left, int top, int right,
                        int bottom, Mat& frame) {
        rectangle(frame, Point(left, top), Point(right, bottom),
                  Scalar(0, 0, 255), 3);

        string label = format("%.2f", conf);
        if (class_id < 80) {
            label = std::string(class_names[class_id]) + ":" + label;
        }

        int base_line;
        Size label_size =
            getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);
        top = max(top, label_size.height);
        putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75,
                Scalar(0, 255, 0), 1);
    };

    auto post_process = [&](Mat& frame, const vector<Mat>& outs) {
        vector<int> class_ids;
        vector<float> confidences;
        vector<Rect> boxes;

        for (size_t i = 0; i < outs.size(); ++i) {
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classid_point;
                double confidence;

                minMaxLoc(scores, 0, &confidence, 0, &classid_point);
                if (confidence > 0.5) {
                    int cx = (int)(data[0] * frame.cols);
                    int cy = (int)(data[1] * frame.rows);
                    int w = (int)(data[2] * frame.cols);
                    int h = (int)(data[3] * frame.rows);
                    int left = cx - (w >> 1);
                    int top = cy - (h >> 1);

                    class_ids.push_back(classid_point.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, w, h));
                }
            }
        }

        vector<int> indices;
        NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
        for (size_t i = 0; i < indices.size(); ++i) {
            int idx = indices[i];
            Rect box = boxes[idx];
            draw_pred(class_ids[idx], confidences[idx], box.x, box.y,
                      box.x + box.width, box.y + box.height, frame);
        }
    };

    auto draw_fps = [&](cv::Mat& frame) {
        vector<double> layers_timings;
        double freq = getTickFrequency() / 1000;
        double time = net_.getPerfProfile(layers_timings) / freq;
        ostringstream ss;
        ss << "FPS: " << 1000 / time << ", time: " << time << " ms";
        putText(frame, ss.str(), Point(0, 30), FONT_HERSHEY_SIMPLEX, 1,
                Scalar(0, 0, 255), 2);
    };

    auto do_process = [&] {
        Mat& frame = *image;
        vector<Mat> outs;
        detect(frame, outs);
        post_process(frame, outs);
        draw_fps(frame);

        imshow(show_name_.c_str(), frame);
        cv::waitKey(1);
    };

    do_process();
}

}  // namespace edge_app
