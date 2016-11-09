#ifndef LANE_MARKINGS_SCAN_H_
#define LANE_MARKINGS_SCAN_H_

#include <memory>

#include "MatVisitInterface.h"

namespace lmvp {

class LaneMarkingsScanner : public MatVisitor {
public:
    /**
     * @param scanDirection In which direction the scanner shall look for white pixels
     * @param scanRegion The region in which to look for lane markings
     * @param maxMarkingWidth After which maximum scan length per row the scanning shall be interrupted
     * @param maxMarkingRowCount The maximum number of rows a marking shall span
     * @param maxMarkingInclination The maximum inclination a road marking is expected to have,
     *                              given in the amount of pixels to travel to the right/left from
     *                              the first lane marking pixel when going to the next row
     */
    LaneMarkingsScanner(RelativeDirection scanDirection, const ScanRegion & scanRegion,
                        ColumnIndex maxMarkingWidth, ColumnIndex maxMarkingRowCount,
                        double maxMarkingInclination, size_t minPixelCount,
                        size_t maxNumLaneMarkings = 1)

        : MatVisitor(scanDirection)
        , scanRegion_(scanRegion)
        , maxMarkingWidth_(maxMarkingWidth)
        , maxMarkingRowCount_(maxMarkingRowCount)
        , maxMarkingInclination_(static_cast<int>(std::ceil(std::abs(maxMarkingInclination))))
        , minPixelCount_(minPixelCount)
        , maxNumLaneMarkings_(maxNumLaneMarkings)
        , previousRowVisitor_()
        , laneMarkingContours_() {

        if(maxMarkingWidth < 0) {
            throw std::invalid_argument("maxMarkingWidth needs to be a positive value");
        }
    }

    std::shared_ptr<RowVisitor> getRowVisitor(RowIndex rowIndex) {
        if(previousRowVisitor_) {
            if(ScanContinuation::STOP == postProcessRowVisit(previousRowVisitor_, rowIndex - 1)) {
                return std::shared_ptr<RowVisitor>();
            }
        }

        ColumnIndex startIndex = getStartIndex(rowIndex),
                    endIndex = getEndIndex(rowIndex),
                    maxScanLength = getMaxScanLength(startIndex);

        previousRowVisitor_ = std::make_shared<RV>(startIndex, endIndex, maxScanLength, *this);

        return std::static_pointer_cast<RowVisitor>(previousRowVisitor_);
    }

    Contours getContours() const {
        Contours contours;
        contours.reserve(laneMarkingContours_.size());

        std::transform(laneMarkingContours_.cbegin(), laneMarkingContours_.cend(),
                       std::back_inserter(contours),
                       [](const LaneMarkingContour & lmc) { return lmc.toCvContour(); });

        return contours;
    }

private:
    enum class ScanContinuation {
        CONTINUE,
        STOP
    };

    static const PixelValue HIT_VALUE = 255;

    class RV;

    class LaneMarkingContour {
    public:
        LaneMarkingContour(RowIndex startRow) : startRow_(startRow), scanLines_() {};

        std::vector<cv::Point> toCvContour() const {
            std::vector<cv::Point> cvContour;
            cvContour.reserve(scanLines_.size() * 2);

            for(size_t lineIndex = 0; lineIndex < scanLines_.size(); ++lineIndex) {
                const auto line = scanLines_[lineIndex];

                const RowIndex imageRowIndex = startRow_ - static_cast<RowIndex>(lineIndex);

                cvContour.emplace_back(line.first, imageRowIndex);
                cvContour.emplace_back(line.second, imageRowIndex);
            }

            return cvContour;
        }

        RowIndex getRowCount() const {
            return static_cast<RowIndex>(scanLines_.size());
        }

        size_t getPixelCount() const {
            size_t pixelCount = 0;

            for(const auto & line : scanLines_) {
                pixelCount += std::abs(line.second - line.first) + 1;
            }

            return pixelCount;
        }

        void setFinalized() {
            finalized_ = true;
        }

        bool isFinalized() const {
            return finalized_;
        }

        void addLine(ColumnIndex startIndex, ColumnIndex endIndex) {
            scanLines_.emplace_back(startIndex, endIndex);
        }

    private:
        const RowIndex startRow_;

        bool finalized_ = false;
        std::vector<std::pair<ColumnIndex, ColumnIndex>> scanLines_;
    };

    ScanContinuation postProcessRowVisit(std::shared_ptr<RV> rowVisitor, RowIndex rowIndex);

    void startNewContour(RowIndex rowIndex) {
        laneMarkingContours_.emplace_back(rowIndex);
    }

    bool noCurrentContour() const {
        return laneMarkingContours_.empty() || laneMarkingContours_.rbegin()->isFinalized();
    }

    void addHit(RowIndex rowIndex, ColumnIndex hitStart, ColumnIndex hitEnd) {
        if(noCurrentContour()) {
            startNewContour(rowIndex);
        }

        auto & currentContour = *laneMarkingContours_.rbegin();
        currentContour.addLine(hitStart, hitEnd);

        if(currentContour.getRowCount() >= maxMarkingRowCount_) {
            finalizeCurrentContour();
        }
    }

    void finalizeCurrentContour() {
        if(noCurrentContour()) {
            return;
        }

        LaneMarkingContour & currentContour = *laneMarkingContours_.rbegin();

        if(currentContour.getPixelCount() < minPixelCount_) {
            laneMarkingContours_.pop_back();
        }
        else {
            currentContour.setFinalized();
        }
    }

    ColumnIndex getMaxScanLength(ColumnIndex startIndex);

    ColumnIndex getStartIndex(RowIndex rowIndex);

    ColumnIndex getEndIndex(RowIndex rowIndex) {
        return scanRegion_.getBorder(getScanDirection()).getX(rowIndex);
    }

    const ScanRegion scanRegion_;

    const ColumnIndex maxMarkingWidth_;
    const RowIndex    maxMarkingRowCount_;
    const int         maxMarkingInclination_;
    const size_t      minPixelCount_,
                      maxNumLaneMarkings_;

    std::shared_ptr<RV> previousRowVisitor_;
    std::vector<LaneMarkingContour> laneMarkingContours_;
};

class LaneMarkingsScanner::RV : public RowVisitor {
public:
   static const ColumnIndex INFINITE_SCAN_LENGTH = -1;

   /**
    * @param startIndex The column where the RV will be invoked first
    * @param maxScanLength How many black pixels are acceptible without hitting something
    *                      before the RV gives up. INFINITE_SCAN_LENGTH for letting the scanner
    *                      continue until the end of the line
    */
   RV(ColumnIndex startIndex, ColumnIndex endIndex, ColumnIndex maxScanLength, const LaneMarkingsScanner & parent)
       : RowVisitor(startIndex)
       , endIndex_(endIndex)
       , maxScanLength_(maxScanLength)
       , parent_(parent)
       , hit_() {}

   RowVisitor::Continuation visit(ColumnIndex columnIndex, const PixelValue & pixelValue) {
       ++visitCount_;

       if(reachedEndIndex(columnIndex)) {
           return Continuation::NEXT_ROW;
       }

       if(HIT_VALUE == pixelValue) {
           hit_.push_back(columnIndex);
       }

       if(hit_.size() >= static_cast<size_t>(parent_.maxMarkingWidth_) ||
          endOfHitReached(pixelValue) ||
          givingUp()) {

           return Continuation::NEXT_ROW;
       }

       return Continuation::THIS_ROW;
   }

   bool hasHit() const {
       return hit_.size() > 0;
   }

   ColumnIndex getHitStart() const {
       return *hit_.begin();
   }

   ColumnIndex getHitEnd() const {
       return *hit_.rbegin();
   }

   ColumnIndex getHitLength() const {
       return sgn(parent_.getScanDirection()) * (getHitEnd() - getHitStart());
   }

private:
   bool endOfHitReached(PixelValue pixelValue) const {
       return hasHit() && HIT_VALUE != pixelValue;
   }

   bool reachedEndIndex(ColumnIndex columnIndex) const {
       return sgn(endIndex_ - columnIndex) != sgn(parent_.getScanDirection());
   }

   bool givingUp() const {
       if(INFINITE_SCAN_LENGTH == maxScanLength_) {
           return false;
       }

       bool patienceLimitReached = visitCount_ >= maxScanLength_;

       return !hasHit() && patienceLimitReached;
   }

   const ColumnIndex endIndex_;
   const ColumnIndex maxScanLength_;
   const LaneMarkingsScanner & parent_;

   std::vector<ColumnIndex> hit_;
   ColumnIndex visitCount_ = 0;
};

LaneMarkingsScanner::ScanContinuation
LaneMarkingsScanner::postProcessRowVisit(std::shared_ptr<RV> rowVisitor, RowIndex rowIndex) {
    if(rowVisitor->hasHit()) {
        addHit(rowIndex, rowVisitor->getHitStart(), rowVisitor->getHitEnd());
    }
    else {
        finalizeCurrentContour();
    }

    if(laneMarkingContours_.size() >= maxNumLaneMarkings_ && laneMarkingContours_.rbegin()->isFinalized()) {
        return ScanContinuation::STOP;
    }

    return ScanContinuation::CONTINUE;
}

ColumnIndex LaneMarkingsScanner::getMaxScanLength(ColumnIndex startIndex) {
    if(previousRowVisitor_ && previousRowVisitor_->hasHit()) {
        return sgn(getScanDirection()) * (previousRowVisitor_->getHitEnd() - startIndex);
    }

    return RV::INFINITE_SCAN_LENGTH;
}

ColumnIndex LaneMarkingsScanner::getStartIndex(RowIndex rowIndex) {
    if(previousRowVisitor_ && previousRowVisitor_->hasHit()) {
        int rowOffset = sgn(getScanDirection()) * maxMarkingInclination_;

        return previousRowVisitor_->getHitStart() - rowOffset;
    }

    return scanRegion_.getBorder(oppositeDirection(getScanDirection())).getX(rowIndex);
}

/**
 * Encapsulates all tasks related to scanning the segmented image for lane markings.
 */
class LaneMarkingsScan {
public:
    typedef std::map<RelativeDirection,Contours> LaneMarkings;

    LaneMarkingsScan(const ScanRegion & leftScanRegion, const ScanRegion & rightScanRegion,
                     ColumnIndex maxMarkingWidth, ColumnIndex maxMarkingRowCount,
                     double maxMarkingInclination, size_t minPixelCount)
        : leftScanRegion_(leftScanRegion)
        , rightScanRegion_(rightScanRegion)
        , maxMarkingWidth_(maxMarkingWidth)
        , maxMarkingRowCount_(maxMarkingRowCount)
        , maxMarkingInclination_(maxMarkingInclination)
        , minPixelCount_(minPixelCount) {}

    LaneMarkings scanLaneMarkings(cv::Mat & image) {

        MatVisit visit(image);
#ifndef NDEBUG
        visit.setDebugImage(debugImage_);
#endif

        auto leftScanner = std::make_shared<LaneMarkingsScanner>(
                        RelativeDirection::LEFT, leftScanRegion_, maxMarkingWidth_,
                        maxMarkingRowCount_, maxMarkingInclination_, minPixelCount_, 2);

        auto rightScanner = std::make_shared<LaneMarkingsScanner>(
                        RelativeDirection::RIGHT, rightScanRegion_, maxMarkingWidth_,
                        maxMarkingRowCount_, maxMarkingInclination_, minPixelCount_, 2);

        visit.addVisitor(leftScanner);

        visit.addVisitor(rightScanner);

        visit.accept();

        std::map<RelativeDirection, Contours> contours;
        contours.insert(std::make_pair(RelativeDirection::LEFT, leftScanner->getContours()));
        contours.insert(std::make_pair(RelativeDirection::RIGHT, rightScanner->getContours()));

        return contours;
    }

#ifndef NDEBUG
    void setDebugImage(cv::Mat & debugImage) {
        debugImage_ = &debugImage;
    }
#endif

private:
    const ScanRegion & leftScanRegion_,
                     & rightScanRegion_;

    const ColumnIndex maxMarkingWidth_,
                      maxMarkingRowCount_;

    const double maxMarkingInclination_;

    const size_t minPixelCount_;

#ifndef NDEBUG
    cv::Mat * debugImage_ = NULL;
#endif
};

}

#endif /* LANE_MARKINGS_SCAN_H_ */
