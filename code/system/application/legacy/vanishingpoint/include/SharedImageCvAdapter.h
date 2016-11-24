#ifndef OPENDLV_LEGACY_VANISHINGPOINT_SHARED_IMAGE_CV_ADAPTER_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_SHARED_IMAGE_CV_ADAPTER_H_

#include <memory>
#include <stdexcept>
#include <string>

#include <opencv2/core/core.hpp>

#include <opendavinci/generated/odcore/data/image/SharedImage.h>
#include <opendavinci/odcore/wrapper/SharedMemory.h>

namespace lmvp {

class FailedAttachingToSharedMemoryException : public std::runtime_error {
public:
    FailedAttachingToSharedMemoryException(const std::string & name);
};

/**
 * Class linking a SharedImage to a cv::Mat. Attachment to the shared memory happens on construction.
 * Construction only succeeds when attaching to the shared memory was successful.
 */
class SharedImageCvAdapter {
public:
    SharedImageCvAdapter(const odcore::data::image::SharedImage & sharedImage);

    cv::Mat fetchImage();

private:
    void attachToSharedMemory(const odcore::data::image::SharedImage & sharedImage);

    std::shared_ptr<odcore::wrapper::SharedMemory> sharedImageMemory_;
    cv::Mat imageHeader_;
};

}

#endif /* OPENDLV_LEGACY_VANISHINGPOINT_SHARED_IMAGE_CV_ADAPTER_H_ */
