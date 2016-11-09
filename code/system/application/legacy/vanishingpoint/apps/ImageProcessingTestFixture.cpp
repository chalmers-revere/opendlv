#include <iostream>
#include <experimental/filesystem>

#include <opencv2/highgui/highgui.hpp>

#include "../include/VanishingPointDetection.h"

struct Data {
    const std::string WINDOW_NAME = "LaneDetection";
    cv::Mat image;
};

bool lButtonDown = false;

void onMouse(int event, int x, int y, int flags, void * userdata) {
    struct Data & data = *static_cast<struct Data *>(userdata);

    if(event == cv::EVENT_MOUSEMOVE) {
        std::cout << "x: " << x << " y: " << y << std::endl;
    }

    if(event == cv::EVENT_LBUTTONDOWN) {
        lButtonDown = true;
    }

    if(event == cv::EVENT_LBUTTONUP) {
        if(lButtonDown) {
            // clicked
            const cv::Vec3b & color = data.image.at<cv::Vec3b>(cv::Point(x, y));

            std::cout << "blue: " << static_cast<int>(color[0]) <<
                         " green: " << static_cast<int>(color[1]) <<
                         " red: " << static_cast<int>(color[2]) << std::endl;
        }

        lButtonDown = false;
    }
}

int main(int argc, char ** argv) {
    using namespace lmvp;

    if(argc <= 1) {
        return EXIT_FAILURE;
    }

    struct Data data;
    cv::Mat & image = data.image;
    const std::string & WINDOW_NAME = data.WINDOW_NAME;

    ScanRegion scanRegionRight(cv::Point(467,456), cv::Point(393,325),
                               cv::Point(482,326), cv::Point(608,420)),

               scanRegionLeft(cv::Point(55,452), cv::Point(273,329),
                              cv::Point(344,331), cv::Point(240,457));

    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, onMouse, static_cast<void*>(&data));

    namespace fs = std::experimental::filesystem;
    for(const auto & directoryEntry : fs::directory_iterator(argv[1])) {
        fs::path filePath = directoryEntry.path();
        image = cv::imread(filePath.string());
        cv::putText(image, filePath.filename().string(), cv::Point(0,10), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,255,0));

        VanishingPointDetection vanishingPointDetection(
                        RegionOfInterestGeometry(cv::Rect(cv::Point(108,298), cv::Point(595,460))),
                        5, 250,
                        scanRegionLeft, scanRegionRight, 30, 80, 3, 20);

        vanishingPointDetection.detectVanishingPoint(image);

        if(DEBUG_SHOW_SCAN_REGIONS) {
            scanRegionRight.draw(image, cv::Scalar(212,195,0));
            scanRegionLeft.draw(image, cv::Scalar(0,212,89));
        }

        cv::imshow(WINDOW_NAME, image);
        cv::waitKey(0);
    }
}
