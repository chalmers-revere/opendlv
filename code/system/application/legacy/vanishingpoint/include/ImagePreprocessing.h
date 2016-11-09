#ifndef IMAGE_PREPROCESSING_H_
#define IMAGE_PREPROCESSING_H_

#include <opencv2/imgproc/imgproc.hpp>

namespace lmvp {

class ImagePreprocessing {
public:
    virtual ~ImagePreprocessing() {}

    virtual cv::Mat preprocessImage(cv::Mat &) const = 0;
};

class AstaZeroImagePreprocessing : public ImagePreprocessing {
public:
    AstaZeroImagePreprocessing(int blurSize, uint8_t threshold) : blurSize_(blurSize), threshold_(threshold) {

    }

    cv::Mat preprocessImage(cv::Mat & coloredImage) const {
        cv::Mat image;

        // convert to grayscale
        cv::cvtColor(coloredImage, image, CV_BGR2GRAY);

        // crop car
        cv::Point roiTopLeft(0,169);
        cv::rectangle(image, cv::Point(268,240) - roiTopLeft, cv::Point(527,408) - roiTopLeft, cv::Scalar(0,0,0), CV_FILLED);
//        cv::Mat labImage;
//        cv::cvtColor(coloredImage, labImage, CV_BGR2Lab);
//        cv::Mat labChannels[3];
//        cv::split(labImage, labChannels);
//        image = labChannels[0];

        //cv::blur(image, image, cv::Size(blurSize_, blurSize_));
        //cv::equalizeHist(image, image);

        cv::threshold(image, image, threshold_, 255, cv::THRESH_BINARY);

        //const int dilateRadius = 2;
        //cv::dilate(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE,
                 //                       cv::Size(2 * dilateRadius + 1, 2 * dilateRadius + 1)) );

        return image;
    }

private:
    int blurSize_;

    uint8_t threshold_;
};

}

#endif
