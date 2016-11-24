#include "SharedImageCvAdapter.h"

#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>
#include <opendavinci/odcore/base/Lock.h>

namespace lmvp {

using odcore::base::Lock;
using odcore::data::image::SharedImage;
using odcore::wrapper::SharedMemoryFactory;

FailedAttachingToSharedMemoryException::FailedAttachingToSharedMemoryException(const std::string & name)
    : std::runtime_error("Failed attaching to shared memory " + name) { }

SharedImageCvAdapter::SharedImageCvAdapter(const SharedImage & sharedImage)
    : sharedImageMemory_()
    , imageHeader_() {

    attachToSharedMemory(sharedImage);
}

cv::Mat SharedImageCvAdapter::fetchImage() {
    Lock lock(sharedImageMemory_);

    return imageHeader_.clone();
}

void SharedImageCvAdapter::attachToSharedMemory(const SharedImage & sharedImage) {
    sharedImageMemory_ = SharedMemoryFactory::attachToSharedMemory(sharedImage.getName());

    if(!sharedImageMemory_->isValid()) {
        throw FailedAttachingToSharedMemoryException(sharedImage.getName());
    }

    imageHeader_ = cv::Mat(sharedImage.getHeight(), sharedImage.getWidth(),
                           CV_MAKETYPE(CV_8U, sharedImage.getBytesPerPixel()),
                           sharedImageMemory_->getSharedMemory());
}

}
