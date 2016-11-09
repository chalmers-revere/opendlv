#ifndef MAT_VISIT_INTERFACE_H_
#define MAT_VISIT_INTERFACE_H_

#include <set>
#include <memory>

#include <opencv2/core/core.hpp>

#include "LmvpTypes.h"
#include "ScanRegion.h"
#include "DebugSettings.h"

namespace lmvp {

#ifndef NDEBUG
std::string type2str(int type);
#endif

class RowVisitor {
public:
    enum class Continuation {
        THIS_ROW,
        NEXT_ROW
    };

    virtual ~RowVisitor() = default;

    ColumnIndex getStartIndex() const {
        return startIndex_;
    }

    virtual Continuation visit(ColumnIndex, const PixelValue &) = 0;

protected:
    RowVisitor(ColumnIndex startIndex) : startIndex_(startIndex) {}

private:
    const ColumnIndex startIndex_;
};

class MatVisitor {
public:
    virtual ~MatVisitor() = default;

    RelativeDirection getScanDirection() const {
        return scanDirection_;
    }

    virtual std::shared_ptr<RowVisitor> getRowVisitor(RowIndex) = 0;

protected:
    MatVisitor(RelativeDirection scanDirection) : scanDirection_(scanDirection) {}

private:
    RelativeDirection scanDirection_;
};

class UnsupportedImageType : std::runtime_error {
public:
    UnsupportedImageType(const std::string & what) : std::runtime_error(what) {}
};

class RowVisit {
public:
    RowVisit(PixelValue * row, ColumnIndex rowLength, RelativeDirection scanDirection)
        : row_(row)
        , rowLength_(rowLength)
        , scanDirection_(scanDirection)
        , rowVisitors_() {}

    RowVisit(const RowVisit &) = delete;
    void operator=(const RowVisit &) = delete;

    void addVisitor(std::shared_ptr<RowVisitor> rowVisitor) {
        rowVisitors_.insert(rowVisitor);
    }

    bool hasVisitors() const {
        return !rowVisitors_.empty();
    }

    void accept() {
        for(ColumnIndex colIndex = getStartIndex(); insideRow(colIndex); forward(colIndex)) {
            auto it = rowVisitors_.begin();
            while(it != rowVisitors_.end()) {
                RowVisitor & rowVisitor = **it;

                if(beforeStartIndex(rowVisitor, colIndex)) {
                    goto forwardRowVisitorIterator;
                }

#ifndef NDEBUG
                if(DEBUG_SHOW_SCAN_COURSE) {
                    debugRow_[colIndex][0] = 0;
                    debugRow_[colIndex][1] = 0;
                    debugRow_[colIndex][2] = 255;
                }
#endif

                if(RowVisitor::Continuation::NEXT_ROW == rowVisitor.visit(colIndex, row_[colIndex])) {
                    it = rowVisitors_.erase(it);
                    continue;
                }

                forwardRowVisitorIterator:
                ++it;
            }
        }
    }

#ifndef NDEBUG
    void setDebugRow(cv::Vec3b * debugRow) {
        debugRow_ = debugRow;
    }
#endif

private:
    /**
     * Used for ordering the rowVisitors along their start index in the visitors set.
     */
    struct RowVisitorComparator {
        bool operator() (std::shared_ptr<RowVisitor> lhs, std::shared_ptr<RowVisitor> rhs) const {
            return lhs->getStartIndex() < rhs->getStartIndex();
        }
    };

    bool beforeStartIndex(const RowVisitor & rowVisitor, ColumnIndex colIndex) {
        if(RelativeDirection::RIGHT == scanDirection_) {
            return colIndex < rowVisitor.getStartIndex();
        }

        return colIndex > rowVisitor.getStartIndex();
    }

    ColumnIndex getStartIndex() {
        if(RelativeDirection::RIGHT == scanDirection_) {
            return (*rowVisitors_.begin())->getStartIndex();
        }

        return (*rowVisitors_.rbegin())->getStartIndex();
    }

    bool insideRow(ColumnIndex colIndex) {
        if(RelativeDirection::RIGHT == scanDirection_ ) {
            return colIndex < rowLength_;
        }

        return colIndex >= 0;
    }

    ColumnIndex & forward(ColumnIndex & colIndex) {
        if(RelativeDirection::RIGHT == scanDirection_) {
            return ++colIndex;
        }

        return --colIndex;
    }

    PixelValue * row_;
    ColumnIndex rowLength_;
    RelativeDirection scanDirection_;

    std::set<std::shared_ptr<RowVisitor>, RowVisitorComparator> rowVisitors_;

#ifndef NDEBUG
    cv::Vec3b * debugRow_ = NULL;
#endif
};

/**
 * Iterates the rows of the image from bottom to top. Which pixels are visited in each row is guided
 * by the registered visitors.
 */
class MatVisit {
public:
    MatVisit(cv::Mat & image) : image_(image), activeMatVisitors_() {
        if(image_.type() != CV_8UC1) {
            throw UnsupportedImageType("Only 8-bit single-channel images ares supported");
        }

        activeMatVisitors_.emplace(RelativeDirection::LEFT, MatVisitors());
        activeMatVisitors_.emplace(RelativeDirection::RIGHT, MatVisitors());
    }

    MatVisit(const MatVisit &) = delete;
    void operator=(const MatVisit &) = delete;

    void addVisitor(std::shared_ptr<MatVisitor> matVisitor) {
        activeMatVisitors_.at(matVisitor->getScanDirection()).push_back(matVisitor);
    }

    void accept() {
        cv::Size size = image_.size();

        // go through all lines of the image
        for(RowIndex rowIndex = size.height - 1; rowIndex >= 0; --rowIndex) {
            PixelValue * row = image_.ptr<PixelValue>(rowIndex);

#ifndef NDEBUG
            cv::Vec3b * debugRow = debugImage_->ptr<cv::Vec3b>(rowIndex);
#endif

            // check whether all matVisitors are done - if so, we can break
            bool allDone = true;

            // each line is iterated in both directions
            for(RelativeDirection relativeDirection : RELATIVE_DIRECTIONS) {
                // the iteration is guided by the RowVisit object
                RowVisit rowVisit(row, size.width, relativeDirection);

#ifndef NDEBUG
                if(DEBUG_SHOW_SCAN_COURSE) {
                    rowVisit.setDebugRow(debugRow);
                }
#endif

                // now go through the matVisitors of the current scanning direction
                MatVisitors & thisDirectionActiveMatVisitors = activeMatVisitors_.at(relativeDirection);

                auto it = thisDirectionActiveMatVisitors.begin();
                while(it != thisDirectionActiveMatVisitors.end()) {
                    MatVisitor & matVisitor = **it;

                    // get the row visitor from the mat visitor. If it is empty, it means that the
                    // mat visitor is done with its work.
                    auto rowVisitor = matVisitor.getRowVisitor(rowIndex);

                    if(rowVisitor) {
                        rowVisit.addVisitor(rowVisitor);
                        ++it;
                    }

                    else {
                        // remove from the active visitors
                        it = thisDirectionActiveMatVisitors.erase(it);
                    }
                }

                if(rowVisit.hasVisitors()) {
                    allDone = false;

                    rowVisit.accept();
                }
            }

            // if at this row, all matVisitors were done, stop looping through the image
            if(allDone) {
                return;
            }
        }
    }

#ifndef NDEBUG
    void setDebugImage(cv::Mat * debugImage) {
        debugImage_ = debugImage;
    }
#endif

private:
    typedef std::vector<std::shared_ptr<MatVisitor>> MatVisitors;

    static const std::vector<RelativeDirection> SCAN_DIRECTIONS;

    cv::Mat & image_;
    std::map<RelativeDirection, MatVisitors> activeMatVisitors_;

#ifndef NDEBUG
    cv::Mat * debugImage_ = NULL;
#endif
};

}

#endif /* MAT_VISIT_INTERFACE_H_ */
